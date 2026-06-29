#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

class DateUtils {
public:
    static bool isValidDate(const std::string& date) {
        std::tm parsed = {};
        std::istringstream stream(date);
        stream >> std::get_time(&parsed, "%Y-%m-%d");
        if (stream.fail()) return false;
        std::tm copy = parsed;
        copy.tm_isdst = 0; 
        std::time_t normalized_time = std::mktime(&copy); //tự động sửa logic ngày nếu nhập sai
        if (normalized_time == -1) return false;
        std::ostringstream normalized;
        normalized << std::put_time(&copy, "%Y-%m-%d");
        return normalized.str() == date; //ss chuỗi sửa với chuỗi nhập
    }

    static bool isDateRangeValid(const std::string& checkIn, const std::string& checkOut) {
        return isValidDate(checkIn) && isValidDate(checkOut) && checkIn < checkOut;
    }

    static int daysBetween(const std::string& checkIn, const std::string& checkOut) {
        std::tm a = {};
        std::tm b = {};
        std::istringstream sa(checkIn);
        std::istringstream sb(checkOut);
        sa >> std::get_time(&a, "%Y-%m-%d");
        sb >> std::get_time(&b, "%Y-%m-%d");
        if (sa.fail() || sb.fail()) return 0;
        a.tm_isdst = 0;
        b.tm_isdst = 0;
        const auto diff = std::mktime(&b) - std::mktime(&a); //đổi ra giây
        return diff <= 0 ? 0 : static_cast<int>(diff / (60 * 60 * 24)); //đổi giây ra ngày
    }

    static bool datesOverlap(const std::string& startA, const std::string& endA, const std::string& startB, const std::string& endB) {
        return isDateRangeValid(startA, endA) && isDateRangeValid(startB, endB) && startA < endB && startB < endA;
    }

    static std::string today() {
        const auto now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::tm* local = std::localtime(&time);
        if (!local) return "";
        std::ostringstream output;
        output << std::put_time(local, "%Y-%m-%d");
        return output.str();
    }
};