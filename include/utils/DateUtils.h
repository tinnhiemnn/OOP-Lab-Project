#pragma once

#include <QString>
#include <QDate>

class DateUtils {
public:
    static bool isValidDate(const QString& date) {
        QDate d = QDate::fromString(date, "yyyy-MM-dd");
        if (!d.isValid()) return false;
        return d.toString("yyyy-MM-dd") == date;
    }

    static bool isDateRangeValid(const QString& checkIn, const QString& checkOut) {
        if (!isValidDate(checkIn) || !isValidDate(checkOut)) return false;
        QDate in = QDate::fromString(checkIn, "yyyy-MM-dd");
        QDate out = QDate::fromString(checkOut, "yyyy-MM-dd");
        return in < out;
    }

    static int daysBetween(const QString& checkIn, const QString& checkOut) {
        QDate in = QDate::fromString(checkIn, "yyyy-MM-dd");
        QDate out = QDate::fromString(checkOut, "yyyy-MM-dd");
        if (!in.isValid() || !out.isValid()) return 0;
        int diff = in.daysTo(out);
        return diff <= 0 ? 0 : diff;
    }

    static bool datesOverlap(const QString& startA, const QString& endA, const QString& startB, const QString& endB) {
        if (!isDateRangeValid(startA, endA) || !isDateRangeValid(startB, endB)) return false;
        QDate sA = QDate::fromString(startA, "yyyy-MM-dd");
        QDate eA = QDate::fromString(endA, "yyyy-MM-dd");
        QDate sB = QDate::fromString(startB, "yyyy-MM-dd");
        QDate eB = QDate::fromString(endB, "yyyy-MM-dd");
        return sA < eB && sB < eA;
    }

    static QString today() {
        return QDate::currentDate().toString("yyyy-MM-dd");
    }
};