#include "patterns/NoDiscount.h"

double NoDiscount::apply(double amount) const {
    return amount;
}

std::string NoDiscount::name() const {
    return "No Discount";
}

