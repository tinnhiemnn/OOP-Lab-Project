#pragma once
#include "DateUtils.h"
#include <algorithm>
#include <cctype>
#include <string>

class ValidationUtils {
public:
    static bool isNonEmpty(const std::string& value) {
        return std::any_of(value.begin(), value.end(), [](unsigned char c) { return !std::isspace(c); });
    }

    static bool isValidEmail(const std::string& email) {
        const auto at = email.find('@');
        const auto dot = email.rfind('.');
        return at != std::string::npos && dot != std::string::npos && at > 0 && dot > at + 1 && dot + 1 < email.size();
    }

    static bool isValidPhone(const std::string& phone) {
        return phone.size() >= 9 && phone.size() <= 12 && std::all_of(phone.begin(), phone.end(), [](unsigned char c) { return std::isdigit(c); });
    }

    static bool isPositiveMoney(double amount) {
        return amount > 0;
    }

    static bool isValidBookingInput(const std::string& customerId, const std::string& roomId, const std::string& checkIn, const std::string& checkOut, std::string& error) {
        if (!isNonEmpty(customerId)) {
            error = "Customer ID is required.";
            return false;
        }
        if (!isNonEmpty(roomId)) {
            error = "Room ID is required.";
            return false;
        }
        if (!DateUtils::isDateRangeValid(checkIn, checkOut)) {
            error = "Check-out date must be after check-in date.";
            return false;
        }
        error.clear();
        return true;
    }
};
