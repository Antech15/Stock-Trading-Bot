// Market.cpp
#include "Market.h"

// Initialize the singleton instance to nullptr
Market* Market::instance = nullptr;

// Singleton method to get the single instance of Market
Market* Market::getInstance() {
    if (!instance) {
        instance = new Market();  // Create the instance if it doesn't exist
    }
    return instance;
}

// Set the market capitalization
void Market::setMarketCapitalization(float newCapitalization) {
    marketCapitalization = newCapitalization;
}

// Get the current market capitalization
float Market::getMarketCapitalization() const {
    return marketCapitalization;
}
