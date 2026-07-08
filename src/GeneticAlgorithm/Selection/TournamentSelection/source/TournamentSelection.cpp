#include "../include/TournamentSelection.hpp"

TournamentSelection::TournamentSelection (int size) : tournamentSize(size) {
    if (size < 1) {
        throw std::invalid_argument("Tournament size must be positive");
    }
}

std::vector<Chromosome>& TournamentSelection::select (Population population, int selectionCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, population.getSize() - 1);
    std::vector<Chromosome> selected;
    for (int i = 0; i < selectionCount; i++) {
        std::vector<Chromosome> tournaments;
        for (int i = 0; i < tournamentSize; i++) {
            int index = distr(gen);
            tournaments.push_back(population.getIndividual(index));
        }
        int minFitness = std::numeric_limits<int>::max();
        int winnerIdx;
        for (int i = 0; i < tournamentSize; i++) {
            int fitness = tournaments[i].getFitness();
            if (fitness < minFitness) {
                minFitness = fitness;
                winnerIdx = i;
            }
        }
        selected.push_back(tournaments[winnerIdx]);
    }
    return selected;
}

int TournamentSelection::getTournamentSize () const {
    return tournamentSize;
}

void TournamentSelection::setTournamentSize (int size) {
    if (size < 1) {
        throw std::invalid_argument("Tournament size must be positive");
    }
    tournamentSize = size;
}