#ifndef ISELECTION_H
#define ISELECTION_H

#include "Population.hpp"

class ISelection {
public:
    virtual ~ISelection () = default;
    
    virtual std::vector<Chromosome>& select (Population population, int selectionCount) = 0;
};

#endif