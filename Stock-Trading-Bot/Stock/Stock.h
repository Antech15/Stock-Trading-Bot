#ifndef STOCK_H
#define STOCK_H

#include <string>

class Stock {
protected:
    std::string name;
    float price;

public:
    Stock(std::string name, float price) : name(name), price(price) {}

    virtual float getPrice() const { return price; }
    virtual void setPrice(float newPrice) { price = newPrice; }

    virtual ~Stock() {}
};

#endif
