#pragma once

#include "patterns/DiscountStrategy.h"

class MemberDiscount : public DiscountStrategy {
public:
    double apply(double amount) const override;
    QString name() const override;
};

