#pragma once

#include <string>
#include <QString>

class DiscountStrategy {
public:
    virtual ~DiscountStrategy() = default;
    virtual double apply(double amount) const = 0;
    virtual QString name() const = 0;
};

