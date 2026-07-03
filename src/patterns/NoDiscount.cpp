#include "patterns/NoDiscount.h"

double NoDiscount::apply(double amount) const {
    return amount;
}

QString NoDiscount::name() const {
    return "No Discount";
}

