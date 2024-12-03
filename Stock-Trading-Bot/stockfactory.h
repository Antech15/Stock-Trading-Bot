#ifndef STOCKFACTORY_H
#define STOCKFACTORY_H

#include "stock.h"
#include <cstdlib>
#include <memory>



/*
 * Static factory implementation
 *
 *
 */
class StockFactory {
public:
    static std::shared_ptr<Stock> createStock(const QString &type) {
        if (type == "REGAL") {
            return std::make_shared<Stock>("REGAL", std::rand() % 101 + 100);
        } else if (type == "GAMESTOP") {
            return std::make_shared<Stock>("GAMESTOP", std::rand() % 101 + 100);
        } else if (type == "NVIDIA") {
            return std::make_shared<Stock>("NVIDIA", std::rand() % 101 + 100);
        } else if (type == "TESLA") {
            return std::make_shared<Stock>("TESLA", std::rand() % 101 + 100);
        } else if (type == "NOKIA"){
            return std::make_shared<Stock>("NOKIA", std::rand() % 101 + 100);
        } else if (type == "MICROSOFT"){
            return std::make_shared<Stock>("MICROSOFT", std::rand() % 101 + 100);
        } else if (type == "AMAZON"){
            return std::make_shared<Stock>("AMAZON", std::rand() % 101 + 100);
        } else if (type == "APPLE"){
            return std::make_shared<Stock>("APPLE", std::rand() % 101 + 100);
        }
    }
};
#endif
