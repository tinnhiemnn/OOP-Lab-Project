#pragma once

#include "patterns/DiscountStrategy.h"

class NoDiscount : public DiscountStrategy {
public:
    double apply(double amount) const override;
    std::string name() const override;
};

