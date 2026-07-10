#ifndef TOURNAMENTSELECTION_H
#define TOURNAMENTSELECTION_H

#include <random>
#include <limits>

#include "../../ISelection/include/ISelection.hpp"

class TournamentSelection : public ISelection {
private:
    int tournamentSize;

public:
    explicit TournamentSelection (int size = 3);
    std::vector<Chromosome> select (Population population, int selectionCount);
    std::string getName () const;
    int getTournamentSize () const noexcept;
    void setTournamentSize (int size);
};

#endif