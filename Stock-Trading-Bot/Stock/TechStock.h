#ifndef TECHSTOCK_H
#define TECHSTOCK_H

#include "Stock.h"

class TechStock : public Stock {
public:
    TechStock(std::string name, float price) : Stock(name, price) {}

    // Custom methods specific to TechStock could be added here
};

#endif
