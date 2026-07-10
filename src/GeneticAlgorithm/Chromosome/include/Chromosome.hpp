#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

class Chromosome {
private:
    std::vector<int> genes;
    int fitness;

public:
    Chromosome () = default;
    explicit Chromosome (const std::vector<int>& genes);

    int getGeneAt (int index) const;
    int getLength () const noexcept;
    const std::vector<int>& getGenes () const noexcept;
    int getFitness () const noexcept;

    void setGene (int index, int gene);
    void setGenes (const std::vector<int>& genes);
    void setFitness (int value) noexcept;

    bool isFitnessCalculated () const;
    void resetFitness () noexcept;
    const std::vector<int> decodeToOrder () const noexcept;
    std::string toOrderString () const noexcept;
    std::string toString () const noexcept;
};

#endif