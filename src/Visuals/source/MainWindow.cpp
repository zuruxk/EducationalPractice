#include "MainWindow.hpp"
#include <iostream>
#include <random>
#include <numeric>

// ======================== AlgorithmWorker ========================

void AlgorithmWorker::runFull() {
    running = true;
    if (!ga) return;

    while (running && !ga->isFinished()) {
        ga->evolveGeneration();

        auto& pop = ga->getCurrentPopulation().getIndividuals();
        int best = ga->getBestSolution().getFitness();
        double avg = 0;
        for (auto& c : pop) avg += c.getFitness();
        avg /= pop.size();

        emit stateUpdated(ga->getGenNumber(), best, avg, pop, ga->getBestFitnessHistory());
        QThread::msleep(50);
    }
    emit finished();
}

void AlgorithmWorker::runStep() {
    if (!ga || ga->isFinished()) {
        emit finished();
        return;
    }
    ga->evolveGeneration();

    auto pop = ga->getCurrentPopulation().getIndividuals();
    int best = ga->getBestSolution().getFitness();
    double avg = 0;
    for (auto& c : pop) avg += c.getFitness();
    avg /= pop.size();

    emit stateUpdated(ga->getGenNumber(), best, avg, pop, ga->getBestFitnessHistory());
    if (ga->isFinished()) emit finished();
}

void AlgorithmWorker::stop() {
    running = false;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    createAlgorithm();

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &MainWindow::stepForward);
}

MainWindow::~MainWindow() {
    if (workerThread) {
        workerThread->quit();
        workerThread->wait();
    }
    delete ga;
}

void MainWindow::setupUI() {
    setWindowTitle("Matrix Chain Optimization");
    setMinimumSize(1200, 800);

    QWidget* central = new QWidget;
    setCentralWidget(central);
    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    // ===== Левая панель =====
    QWidget* leftPanel = new QWidget;
    leftPanel->setFixedWidth(400);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);

    // ---- Ввод данных ----
    QGroupBox* dataGroup = new QGroupBox("Data");
    QVBoxLayout* dataLayout = new QVBoxLayout(dataGroup);

    QHBoxLayout* dimsLayout = new QHBoxLayout;
    dimsLayout->addWidget(new QLabel("Dimensions (p0 p1 ... pN):"));
    dimsEdit = new QLineEdit("10 30 5 60 20");
    dimsLayout->addWidget(dimsEdit);
    dataLayout->addLayout(dimsLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    loadBtn = new QPushButton("Load File");
    generateBtn = new QPushButton("Generate Random");
    btnLayout->addWidget(loadBtn);
    btnLayout->addWidget(generateBtn);
    dataLayout->addLayout(btnLayout);

    leftLayout->addWidget(dataGroup);

    // ---- Параметры алгоритма ----
    QGroupBox* paramGroup = new QGroupBox("Parameters");
    QGridLayout* paramLayout = new QGridLayout(paramGroup);

    paramLayout->addWidget(new QLabel("Population:"), 0, 0);
    popSizeSpin = new QSpinBox;
    popSizeSpin->setRange(10, 500);
    popSizeSpin->setValue(100);
    paramLayout->addWidget(popSizeSpin, 0, 1);

    paramLayout->addWidget(new QLabel("Generations:"), 1, 0);
    maxGenSpin = new QSpinBox;
    maxGenSpin->setRange(10, 10000);
    maxGenSpin->setValue(500);
    paramLayout->addWidget(maxGenSpin, 1, 1);

    paramLayout->addWidget(new QLabel("Crossover prob:"), 2, 0);
    crossProbSpin = new QDoubleSpinBox;
    crossProbSpin->setRange(0.0, 1.0);
    crossProbSpin->setSingleStep(0.05);
    crossProbSpin->setValue(0.85);
    paramLayout->addWidget(crossProbSpin, 2, 1);

    paramLayout->addWidget(new QLabel("Mutation prob:"), 3, 0);
    mutProbSpin = new QDoubleSpinBox;
    mutProbSpin->setRange(0.0, 1.0);
    mutProbSpin->setSingleStep(0.01);
    mutProbSpin->setValue(0.05);
    paramLayout->addWidget(mutProbSpin, 3, 1);

    paramLayout->addWidget(new QLabel("Tournament size:"), 4, 0);
    tourSizeSpin = new QSpinBox;
    tourSizeSpin->setRange(2, 10);
    tourSizeSpin->setValue(3);
    paramLayout->addWidget(tourSizeSpin, 4, 1);

    paramLayout->addWidget(new QLabel("Elitism count:"), 5, 0);
    elitismSpin = new QSpinBox;
    elitismSpin->setRange(0, 20);
    elitismSpin->setValue(2);
    paramLayout->addWidget(elitismSpin, 5, 1);

    paramLayout->addWidget(new QLabel("Selection:"), 6, 0);
    selectionCombo = new QComboBox;
    selectionCombo->addItems({"tournament"});
    paramLayout->addWidget(selectionCombo, 6, 1);

    paramLayout->addWidget(new QLabel("Crossover:"), 7, 0);
    crossoverCombo = new QComboBox;
    crossoverCombo->addItems({"singlePoint", "doublePoint", "uniform"});
    paramLayout->addWidget(crossoverCombo, 7, 1);

    paramLayout->addWidget(new QLabel("Mutation:"), 8, 0);
    mutationCombo = new QComboBox;
    mutationCombo->addItems({"exchange", "conversion", "shuffling"});
    paramLayout->addWidget(mutationCombo, 8, 1);

    leftLayout->addWidget(paramGroup);

    // ---- Управление ----
    QGroupBox* controlGroup = new QGroupBox("Control");
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);

    QHBoxLayout* ctrlBtnLayout = new QHBoxLayout;
    startBtn = new QPushButton("Start");
    stepBtn = new QPushButton("Step");
    finishBtn = new QPushButton("Finish");
    backBtn = new QPushButton("Back");
    ctrlBtnLayout->addWidget(startBtn);
    ctrlBtnLayout->addWidget(stepBtn);
    ctrlBtnLayout->addWidget(finishBtn);
    ctrlBtnLayout->addWidget(backBtn);
    controlLayout->addLayout(ctrlBtnLayout);

    QHBoxLayout* speedLayout = new QHBoxLayout;
    speedLayout->addWidget(new QLabel("Speed:"));
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(1, 20);
    speedSlider->setValue(5);
    speedLayout->addWidget(speedSlider);
    controlLayout->addLayout(speedLayout);

    leftLayout->addWidget(controlGroup);

    // ---- Информация ----
    QGroupBox* infoGroup = new QGroupBox("Info");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);
    genLabel = new QLabel("Generation: 0");
    bestLabel = new QLabel("Best: -");
    avgLabel = new QLabel("Average: -");
    orderLabel = new QLabel("Order: -");
    infoLayout->addWidget(genLabel);
    infoLayout->addWidget(bestLabel);
    infoLayout->addWidget(avgLabel);
    infoLayout->addWidget(orderLabel);
    leftLayout->addWidget(infoGroup);

    leftLayout->addStretch();
    mainLayout->addWidget(leftPanel);

    // ===== Правая панель =====
    QWidget* rightPanel = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);

    // ---- График ----
    chart = new QChart;
    chart->setTitle("Convergence");
    chart->legend()->setVisible(true);

    bestSeries = new QLineSeries;
    bestSeries->setName("Best");
    bestSeries->setColor(Qt::green);
    avgSeries = new QLineSeries;
    avgSeries->setName("Average");
    avgSeries->setColor(Qt::blue);

    chart->addSeries(bestSeries);
    chart->addSeries(avgSeries);

    QValueAxis* axisX = new QValueAxis;
    axisX->setTitleText("Generation");
    axisX->setRange(0, 100);
    chart->addAxis(axisX, Qt::AlignBottom);
    bestSeries->attachAxis(axisX);
    avgSeries->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText("Fitness");
    axisY->setRange(0, 100000);
    chart->addAxis(axisY, Qt::AlignLeft);
    bestSeries->attachAxis(axisY);
    avgSeries->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(chartView);

    // ---- Таблица ----
    table = new QTableWidget;
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"#", "Genes", "Fitness"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    rightLayout->addWidget(table);
    rightLayout->setStretchFactor(chartView, 2);
    rightLayout->setStretchFactor(table, 1);

    mainLayout->addWidget(rightPanel);

    // ---- Связи ----
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadFromFile);
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::generateRandom);
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::startAlgorithm);
    connect(stepBtn, &QPushButton::clicked, this, &MainWindow::stepForward);
    connect(finishBtn, &QPushButton::clicked, this, &MainWindow::finishAll);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::goBack);
}

void MainWindow::createAlgorithm() {
    std::vector<int> dims = {10, 30, 5, 60, 20};
    currentProblem = MatrixMultProblem(dims);
    hasProblem = true;

    FitnessFunction fitness(currentProblem);
    ga = new GeneticAlgorithm(fitness);

    ga->setSelectionOperator(std::make_unique<TournamentSelection>(3));
    ga->setCrossoverOperator(std::make_unique<SinglePointCrossover>());
    ga->setMutationOperator(std::make_unique<ExchangeMutation>());

    AlgorithmParams params;
    params.populationSize = popSizeSpin->value();
    params.maxGenerations = maxGenSpin->value();
    params.crossoverProbability = crossProbSpin->value();
    params.mutationProbability = mutProbSpin->value();
    params.elitismSize = elitismSpin->value();
    ga->setParams(params);
}

void MainWindow::applySelectedOperators() {
    if (!ga) return;

    ga->setSelectionOperator(std::make_unique<TournamentSelection>(tourSizeSpin->value()));

    QString crossoverType = crossoverCombo->currentText();
    if (crossoverType == "singlePoint") {
        ga->setCrossoverOperator(std::make_unique<SinglePointCrossover>());
    } else if (crossoverType == "doublePoint") {
        ga->setCrossoverOperator(std::make_unique<DoublePointCrossover>());
    } else if (crossoverType == "uniform") {
        ga->setCrossoverOperator(std::make_unique<UniformCrossover>());
    }

    QString mutationType = mutationCombo->currentText();
    if (mutationType == "exchange") {
        ga->setMutationOperator(std::make_unique<ExchangeMutation>());
    } else if (mutationType == "conversion") {
        ga->setMutationOperator(std::make_unique<ConversionMutation>());
    } else if (mutationType == "shuffling") {
        ga->setMutationOperator(std::make_unique<ShufflingMutation>());
    }
}

void MainWindow::loadFromFile() {
    QString path = QFileDialog::getOpenFileName(this, "Open Problem", "", "*.txt");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open file");
        return;
    }

    QTextStream stream(&file);
    int N;
    stream >> N;
    std::vector<int> dims(N + 1);
    for (int i = 0; i <= N; ++i) stream >> dims[i];
    file.close();

    QString text;
    for (int d : dims) text += QString::number(d) + " ";
    dimsEdit->setText(text);

    currentProblem = MatrixMultProblem(dims);
    hasProblem = true;

    delete ga;
    FitnessFunction fitness(currentProblem);
    ga = new GeneticAlgorithm(fitness);

    history.clear();
    currentStep = -1;

    chart->removeAllSeries();
    bestSeries = new QLineSeries;
    bestSeries->setName("Best");
    avgSeries = new QLineSeries;
    avgSeries->setName("Average");
    chart->addSeries(bestSeries);
    chart->addSeries(avgSeries);
    updateInfo(0, -1, -1);
}

void MainWindow::generateRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dimDist(2, 50);
    std::uniform_int_distribution<> countDist(3, 8);

    int N = countDist(gen);
    std::vector<int> dims(N + 1);
    for (int i = 0; i <= N; ++i) dims[i] = dimDist(gen);

    QString text;
    for (int d : dims) text += QString::number(d) + " ";
    dimsEdit->setText(text);

    currentProblem = MatrixMultProblem(dims);
    hasProblem = true;

    // Пересоздаем GA с новой задачей
    delete ga;
    FitnessFunction fitness(currentProblem);
    ga = new GeneticAlgorithm(fitness);
    
    // Устанавливаем параметры из GUI
    AlgorithmParams params;
    params.populationSize = popSizeSpin->value();
    params.maxGenerations = maxGenSpin->value();
    params.crossoverProbability = crossProbSpin->value();
    params.mutationProbability = mutProbSpin->value();
    params.elitismSize = elitismSpin->value();
    ga->setParams(params);
    
    // Применяем выбранные операторы
    applySelectedOperators();
    
    // Инициализируем популяцию
    ga->initializePopulation();

    history.clear();
    currentStep = -1;

    chart->removeAllSeries();
    bestSeries = new QLineSeries;
    bestSeries->setName("Best");
    avgSeries = new QLineSeries;
    avgSeries->setName("Average");
    chart->addSeries(bestSeries);
    chart->addSeries(avgSeries);
    
    // Обновляем отображение с новыми данными
    // updateInfo(0, ga->getBestSolution().getFitness(), 0);
    // updateTable(ga->getCurrentPopulation().getIndividuals());
    table->setRowCount(0);
    updateInfo(0, -1, -1);
    orderLabel->setText("Order: -");
}

void MainWindow::startAlgorithm() {
    if (!hasProblem) {
        QMessageBox::warning(this, "Error", "No problem loaded");
        return;
    }

    if (isRunning) {
        isRunning = false;
        timer->stop();
        if (worker) worker->stop();
        startBtn->setText("Start");
        return;
    }

    try{
        delete ga;
        FitnessFunction fitness(currentProblem);
        ga = new GeneticAlgorithm(fitness);

        AlgorithmParams params;
        params.populationSize = popSizeSpin->value();
        params.maxGenerations = maxGenSpin->value();
        params.crossoverProbability = crossProbSpin->value();
        params.mutationProbability = mutProbSpin->value();
        params.elitismSize = elitismSpin->value();
        ga->setParams(params);

        applySelectedOperators();

        ga->initializePopulation();
        history.clear();
        currentStep = -1;
        saveSnapshot();
        updateTable(ga->getCurrentPopulation().getIndividuals());
        updateInfo(0, ga->getBestSolution().getFitness(), 0);

        isRunning = true;
        startBtn->setText("Stop");
        timer->setInterval(1000 / speedSlider->value());
        timer->start();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to start: ") + e.what());
        delete ga;
        ga = nullptr;
        startBtn->setText("Start");
        isRunning = false;
    }
}

void MainWindow::stopAlgorithm() {
    if (isRunning) {
        isRunning = false;
        timer->stop();
        if (worker) {
            worker->stop();
        }
        startBtn->setText("Start");
    }
}

void MainWindow::stepForward() {
    if (!hasProblem || !ga) return;
    if (ga->isFinished()) return;
    if (ga->getGenNumber() == 0) {
        ga->initializePopulation();
        saveSnapshot();
    }

    ga->evolveGeneration();
    saveSnapshot();

    auto pop = ga->getCurrentPopulation().getIndividuals();
    updateTable(pop);
    updateChart(ga->getBestFitnessHistory());
    updateInfo(ga->getGenNumber(), ga->getBestSolution().getFitness(), 0);

    if (ga->isFinished()) {
        isRunning = false;
        startBtn->setText("Start");
        timer->stop();
    }
}

void MainWindow::finishAll() {
    if (!ga) return;
    if (ga->getGenNumber() == 0) {
        ga->initializePopulation();
        saveSnapshot();
    }

    while (!ga->isFinished()) {
        ga->evolveGeneration();
        saveSnapshot();
    }

    updateTable(ga->getCurrentPopulation().getIndividuals());
    updateChart(ga->getBestFitnessHistory());
    updateInfo(ga->getGenNumber(), ga->getBestSolution().getFitness(), 0);
    isRunning = false;
    startBtn->setText("Start");
    timer->stop();
}

void MainWindow::goBack() {
    if (currentStep <= 0) return;
    currentStep--;
    restoreSnapshot(currentStep);
}

void MainWindow::saveSnapshot() {
    Snapshot snap;
    snap.generation = ga->getGenNumber();
    snap.population = ga->getCurrentPopulation().getIndividuals();
    snap.history = ga->getBestFitnessHistory();
    snap.best = ga->getBestSolution();

    if (currentStep < (int)history.size() - 1) {
        history.resize(currentStep + 1);
    }
    history.push_back(snap);
    currentStep = history.size() - 1;
}

void MainWindow::restoreSnapshot(int index) {
    if (index < 0 || index >= (int)history.size()) return;
    const auto& snap = history[index];
    updateTable(snap.population);
    updateChart(snap.history);
    updateInfo(snap.generation, snap.best.getFitness(), 0);
    updateOrder(snap.best);
}

void MainWindow::updateTable(const std::vector<Chromosome>& pop) {
    table->setRowCount((int)pop.size());
    for (size_t i = 0; i < pop.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(i)));

        QString genes;
        for (int g : pop[i].getGenes()) genes += QString::number(g) + " ";
        table->setItem(i, 1, new QTableWidgetItem(genes));

        table->setItem(i, 2, new QTableWidgetItem(QString::number(pop[i].getFitness())));
    }
}

void MainWindow::updateChart(const std::vector<int>& history) {
    bestSeries->clear();
    avgSeries->clear();
    for (size_t i = 0; i < history.size(); ++i) {
        bestSeries->append(i, history[i]);
    }
    if (!history.empty()) {
        chart->axisX()->setRange(0, static_cast<int>(history.size()) + 10);
        chart->axisY()->setRange(0, history[0] * 1.5);
    }
}

void MainWindow::updateInfo(int gen, int best, double avg) {
    genLabel->setText(QString("Generation: %1").arg(gen));
    bestLabel->setText(QString("Best: %1").arg(best));
    avgLabel->setText(QString("Average: %1").arg(avg));
}

void MainWindow::updateOrder(const Chromosome& chrom) {
    auto order = chrom.decodeToOrder();
    QString text = "Order: ";
    for (int o : order) text += QString::number(o) + " -> ";
    orderLabel->setText(text);
}

void MainWindow::onStateUpdated(int gen, int best, double avg,
                                const std::vector<Chromosome>& pop,
                                const std::vector<int>& history) {
    updateTable(pop);
    updateChart(history);
    updateInfo(gen, best, avg);
}

void MainWindow::onFinished() {
    isRunning = false;
    startBtn->setText("Start");
    timer->stop();
}

bool MainWindow::validateInputs() {
    return hasProblem;
}