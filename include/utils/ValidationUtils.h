#pragma once
#include "DateUtils.h"
#include <QString>

class ValidationUtils {
public:
    static bool isNonEmpty(const QString& value) {
        return !value.trimmed().isEmpty();
    }

    static bool isValidEmail(const QString& email) {
        const auto at = email.indexOf('@');
        const auto dot = email.lastIndexOf('.');
        return at != -1 && dot != -1 && at > 0 && dot > at + 1 && dot + 1 < email.size();
    }

    static bool isValidPhone(const QString& phone) {
        if (phone.size() < 9 || phone.size() > 12) return false;
        for (const QChar& c : phone) {
            if (c < '0' || c > '9') return false;
        }
        return true;
    }

    static bool isPositiveMoney(double amount) {
        return amount > 0;
    }

    static bool isValidBookingInput(const QString& customerId, const QString& roomId, const QString& checkIn, const QString& checkOut, QString& error) {
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
