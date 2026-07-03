#include "patterns/SeasonalDiscount.h"

double SeasonalDiscount::apply(double amount) const {
    return amount * 0.90;
}

QString SeasonalDiscount::name() const {
    return "Seasonal 10%";
}

