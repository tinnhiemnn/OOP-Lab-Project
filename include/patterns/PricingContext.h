#pragma once

#include "patterns/DiscountStrategy.h"

#include <memory>
#include <string>

class PricingContext {
public:
    PricingContext();

    void setStrategy(std::unique_ptr<DiscountStrategy> value);
    double calculateFinalAmount(double amount) const;
    std::string currentStrategyName() const;

private:
    std::unique_ptr<DiscountStrategy> strategy;
};

