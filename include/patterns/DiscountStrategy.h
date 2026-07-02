#pragma once

#include <string>

class DiscountStrategy {
public:
    virtual ~DiscountStrategy() = default;
    virtual double apply(double amount) const = 0;
    virtual std::string name() const = 0;
};

