// Market.h
#ifndef MARKET_H
#define MARKET_H

class Market {
private:
    static Market* instance;
    float marketCapitalization;

    Market() : marketCapitalization(1000000.0) {}  // Default market cap

public:
    static Market* getInstance() {
        if (!instance) {
            instance = new Market();
        }
        return instance;
    }

    void setMarketCapitalization(float newCapitalization) {
        marketCapitalization = newCapitalization;
    }

    float getMarketCapitalization() const {
        return marketCapitalization;
    }
};

#endif // MARKET_H
