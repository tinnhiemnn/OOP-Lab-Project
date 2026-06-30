#include "patterns/MemberDiscount.h"

double MemberDiscount::apply(double amount) const {
    return amount * 0.95;
}

std::string MemberDiscount::name() const {
    return "Member 5%";
}

