#include "patterns/MemberDiscount.h"

double MemberDiscount::apply(double amount) const {
    return amount * 0.95;
}

QString MemberDiscount::name() const {
    return "Member 5%";
}

