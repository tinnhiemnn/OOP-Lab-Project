#pragma once

#include "patterns/DiscountStrategy.h"

class SeasonalDiscount : public DiscountStrategy {
public:
    double apply(double amount) const override;
    QString name() const override;
};

