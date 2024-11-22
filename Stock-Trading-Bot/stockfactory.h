#ifndef STOCKFACTORY_H
#define STOCKFACTORY_H

#include "stock.h"
#include <memory>

class StockFactory {
public:
    static std::shared_ptr<Stock> createStock(const QString &type) {
        if (type == "Tech") {
            return std::make_shared<Stock>("Tech", 100.0);
        } else if (type == "Health") {
            return std::make_shared<Stock>("Health", 150.0);
        } else if (type == "Finance") {
            return std::make_shared<Stock>("Finance", 200.0);
        } else {
            return std::make_shared<Stock>("Energy", 120.0);
        }
    }
};

#endif // STOCKFACTORY_H
