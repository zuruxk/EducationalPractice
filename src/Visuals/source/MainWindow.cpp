#include "MainWindow.hpp"
#include <iostream>
#include <random>
#include <numeric>

void AlgorithmWorker::runFull() {
    running = true;
    if (!ga) return;

    while (running && !ga->isFinished()) {
        ga->evolveGeneration();

        auto& pop = ga->getCurrentPopulation().getIndividuals();
        int best = ga->getBestSolution().getFitness();

        emit stateUpdated(ga->getGenNumber(), best, pop, ga->getBestFitnessHistory());
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

    emit stateUpdated(ga->getGenNumber(), best, pop, ga->getBestFitnessHistory());
    if (ga->isFinished()) emit finished();
}

void AlgorithmWorker::stop() {
    running = false;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ga(nullptr), worker(nullptr), workerThread(nullptr) {
    setupUI();
    createAlgorithm();

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &MainWindow::stepForward);
}

MainWindow::~MainWindow() {
    if (timer) {
        timer->stop();
    }
    
    if (worker) {
        worker->stop();
        worker->deleteLater();
        worker = nullptr;
    }
    
    if (workerThread) {
        workerThread->quit();
        workerThread->wait();
        delete workerThread;
        workerThread = nullptr;
    }
    
    delete ga;
    ga = nullptr;
}

void MainWindow::setupUI() {
    setWindowTitle("Matrix Chain Optimization");
    setMinimumSize(1200, 800);

    QWidget* central = new QWidget;
    setCentralWidget(central);
    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    QWidget* leftPanel = new QWidget;
    leftPanel->setFixedWidth(400);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);

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

    QGroupBox* infoGroup = new QGroupBox("Info");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoGroup);
    genLabel = new QLabel("Generation: 0");
    bestLabel = new QLabel("Best: -");
    orderLabel = new QLabel("Order:");

    orderEdit = new QLineEdit;
    orderEdit->setReadOnly(true);
    orderEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; color: #333; }");
    orderEdit->setPlaceholderText("Order will appear here after algorithm starts");

    infoLayout->addWidget(genLabel);
    infoLayout->addWidget(bestLabel);
    infoLayout->addWidget(orderLabel);
    infoLayout->addWidget(orderEdit);
    leftLayout->addWidget(infoGroup);

    leftLayout->addStretch();
    mainLayout->addWidget(leftPanel);

    QWidget* rightPanel = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);

    chart = new QChart;
    chart->setTitle("Convergence");
    chart->legend()->setVisible(true);

    bestSeries = new QLineSeries;
    bestSeries->setName("Best");
    bestSeries->setColor(Qt::green);

    chart->addSeries(bestSeries);

    axisX = new QValueAxis;
    axisX->setTitleText("Generation");
    axisX->setRange(0, 100);
    chart->addAxis(axisX, Qt::AlignBottom);
    bestSeries->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setTitleText("Fitness");
    axisY->setRange(0, 100000);
    chart->addAxis(axisY, Qt::AlignLeft);
    bestSeries->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(chartView);

    table = new QTableWidget;
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"#", "Genes", "Fitness"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    rightLayout->addWidget(table);
    rightLayout->setStretchFactor(chartView, 2);
    rightLayout->setStretchFactor(table, 1);

    mainLayout->addWidget(rightPanel);

    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadFromFile);
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::generateRandom);
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::startAlgorithm);
    connect(stepBtn, &QPushButton::clicked, this, &MainWindow::stepForward);
    connect(finishBtn, &QPushButton::clicked, this, &MainWindow::finishAll);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::goBack);
}

void MainWindow::setupChart() {
    chart->removeAllSeries();
    
    bestSeries = new QLineSeries;
    bestSeries->setName("Best");
    bestSeries->setColor(Qt::green);
    chart->addSeries(bestSeries);

    chart->removeAxis(axisX);
    chart->removeAxis(axisY);
    
    axisX = new QValueAxis;
    axisX->setTitleText("Generation");
    axisX->setRange(0, 100);
    chart->addAxis(axisX, Qt::AlignBottom);
    bestSeries->attachAxis(axisX);
    
    axisY = new QValueAxis;
    axisY->setTitleText("Fitness");
    axisY->setRange(0, 100000);
    chart->addAxis(axisY, Qt::AlignLeft);
    bestSeries->attachAxis(axisY);
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

bool MainWindow::parseDimensions(const QString& text, std::vector<int>& dims) {
    QStringList parts = text.split(' ', Qt::SkipEmptyParts);
    dims.clear();
    for (const QString& part : parts) {
        bool ok;
        int val = part.toInt(&ok);
        if (!ok || val <= 0) return false;
        dims.push_back(val);
    }
    return dims.size() >= 3;
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

    if (N <= 0) {
        QMessageBox::warning(this, "Error", "Invalid N in file");
        return;
    }
    
    std::vector<int> dims;
    dims.reserve(N + 1);

    for (int i = 0; i < N; i++) {
        int val;
        if (!(stream >> val)) {
            QMessageBox::warning(this, "Error", 
                QString("Failed to read dimension %1 (expected %2 dimensions)")
                .arg(i).arg(N));
            return;
        }
        if (val <= 0) {
            QMessageBox::warning(this, "Error", 
                QString("Dimension %1 is not positive: %2")
                .arg(i).arg(val));
            return;
        }
        dims.push_back(val);
    }
    
    file.close();

    if (static_cast<int>(dims.size()) != N) {
        QMessageBox::warning(this, "Error", 
            QString("Expected %1 dimensions, got %2")
            .arg(N).arg(dims.size()));
        return;
    }

    QString text;
    for (int d : dims) text += QString::number(d) + " ";
    dimsEdit->setText(text);

    currentProblem = MatrixMultProblem(dims);
    hasProblem = true;

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

    setupChart();
    table->setRowCount(0);
    updateInfo(0, -1);
    orderEdit->setPlaceholderText("Order will appear here after algorithm starts");
    orderEdit->clear();
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

    setupChart();

    table->setRowCount(0);
    updateInfo(0, -1);
    orderLabel->setText("Order:");
    orderEdit->setPlaceholderText("Order will appear here after algorithm starts");
    orderEdit->clear();
}

void MainWindow::startAlgorithm() {
    if (!hasProblem) {
        QMessageBox::warning(this, "Error", "No problem loaded");
        return;
    }

    std::vector<int> dims;
    if (!parseDimensions(dimsEdit->text(), dims)) {
        QMessageBox::warning(this, "Error", "Invalid dimensions format");
        return;
    }

    currentProblem = MatrixMultProblem(dims);
    hasProblem = true;

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
        updateInfo(0, ga->getBestSolution().getFitness());
        updateOrder(ga->getBestSolution());

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
    std::vector<int> dims;
    if (!parseDimensions(dimsEdit->text(), dims)) {
        QMessageBox::warning(this, "Error", "Invalid dimensions format");
        return;
    }

    currentProblem = MatrixMultProblem(dims);
    hasProblem = true;
    
    if (!ga || ga->getGenNumber() == 0) {
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
        updateInfo(0, ga->getBestSolution().getFitness());
        updateOrder(ga->getBestSolution());
    }
    
    if (ga->isFinished()) return;

    ga->evolveGeneration();
    saveSnapshot();

    auto pop = ga->getCurrentPopulation().getIndividuals();
    updateTable(pop);
    updateChart(ga->getBestFitnessHistory());
    updateInfo(ga->getGenNumber(), ga->getBestSolution().getFitness());
    updateOrder(ga->getBestSolution());

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
    updateInfo(ga->getGenNumber(), ga->getBestSolution().getFitness());
    updateOrder(ga->getBestSolution());
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
    updateInfo(snap.generation, snap.best.getFitness());
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
    for (size_t i = 0; i < history.size(); ++i) {
        bestSeries->append(i, history[i]);
    }
    if (!history.empty()) {
        axisX->setRange(0, static_cast<int>(history.size()) + 10);
        axisY->setRange(0, history[0] * 1.5);
    }
}

void MainWindow::updateInfo(int gen, int best) {
    genLabel->setText(QString("Generation: %1").arg(gen));
    bestLabel->setText(QString("Best: %1").arg(best));

    if (ga && !ga->getCurrentPopulation().isEmpty()) {
        updateOrder(ga->getBestSolution());
    }
}

void MainWindow::updateOrder(const Chromosome& chrom) {
    auto order = chrom.decodeToOrder();
    QString text = "";
    for (int o : order) text += QString::number(o) + " -> ";

    if (text.endsWith(" -> ")) {
        text.chop(4);
    }

    orderEdit->setText(text);
}

void MainWindow::onStateUpdated(int gen, int best,
                                const std::vector<Chromosome>& pop,
                                const std::vector<int>& history) {
    updateTable(pop);
    updateChart(history);
    updateInfo(gen, best);
}

void MainWindow::onFinished() {
    isRunning = false;
    startBtn->setText("Start");
    timer->stop();
}

bool MainWindow::validateInputs() {
    return hasProblem;
}