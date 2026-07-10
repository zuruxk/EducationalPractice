#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QLabel>
#include <QTabWidget>
#include <QSlider>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QGroupBox>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>

#include "GeneticAlgorithm.hpp"
#include "MatrixMultProblem.hpp"
#include "FitnessFunction.hpp"
#include "TournamentSelection.hpp"
#include "SinglePointCrossover.hpp"
#include "DoublePointCrossover.hpp"
#include "UniformCrossover.hpp"
#include "ExchangeMutation.hpp"
#include "ConversionMutation.hpp"
#include "ShufflingMutation.hpp"

class AlgorithmWorker : public QObject {
    Q_OBJECT
public:
    GeneticAlgorithm* ga;
    AlgorithmParams params;
    bool running = false;
    bool stepMode = false;

public slots:
    void runFull();
    void runStep();
    void stop();

signals:
    void stateUpdated(int generation, int bestFitness, double avgFitness,
                     const std::vector<Chromosome>& population,
                     const std::vector<int>& history);
    void finished();
};

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QLineEdit* dimsEdit;

    QSpinBox* popSizeSpin;
    QSpinBox* maxGenSpin;
    QDoubleSpinBox* crossProbSpin;
    QDoubleSpinBox* mutProbSpin;
    QSpinBox* tourSizeSpin;
    QSpinBox* elitismSpin;

    QComboBox* selectionCombo;
    QComboBox* crossoverCombo;
    QComboBox* mutationCombo;

    QPushButton* loadBtn;
    QPushButton* generateBtn;
    QPushButton* startBtn;
    QPushButton* stepBtn;
    QPushButton* finishBtn;
    QPushButton* backBtn;
    QSlider* speedSlider;

    QChartView* chartView;
    QChart* chart;
    QLineSeries* bestSeries;
    QLineSeries* avgSeries;
    QTableWidget* table;
    QLabel* genLabel;
    QLabel* bestLabel;
    QLabel* avgLabel;
    QLabel* orderLabel;

    GeneticAlgorithm* ga;
    AlgorithmWorker* worker;
    QThread* workerThread;
    QTimer* timer;

    struct Snapshot {
        int generation;
        std::vector<Chromosome> population;
        std::vector<int> history;
        Chromosome best;
    };
    std::vector<Snapshot> history;
    int currentStep = -1;

    MatrixMultProblem currentProblem;
    bool hasProblem = false;
    bool isRunning = false;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void loadFromFile();
    void generateRandom();
    void startAlgorithm();
    void stopAlgorithm();
    void stepForward();
    void finishAll();
    void goBack();
    void onStateUpdated(int gen, int best, double avg,
                       const std::vector<Chromosome>& pop,
                       const std::vector<int>& history);
    void onFinished();
    void updateTable(const std::vector<Chromosome>& pop);
    void updateChart(const std::vector<int>& history);
    void updateInfo(int gen, int best, double avg);
    void updateOrder(const Chromosome& chrom);

private:
    void setupUI();
    void createAlgorithm();
    void saveSnapshot();
    void restoreSnapshot(int index);
    bool validateInputs();
    void applySelectedOperators();
};

#endif