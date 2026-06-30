#include "patterns/PricingContext.h"

#include "patterns/NoDiscount.h"

PricingContext::PricingContext()
    : strategy(std::make_unique<NoDiscount>()) {}

void PricingContext::setStrategy(std::unique_ptr<DiscountStrategy> value) {
    strategy = std::move(value);
}

double PricingContext::calculateFinalAmount(double amount) const {
    return strategy ? strategy->apply(amount) : amount;
}

std::string PricingContext::currentStrategyName() const {
    return strategy ? strategy->name() : "No Discount";
}

