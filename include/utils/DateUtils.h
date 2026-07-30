#pragma once

#include <QDate>
#include <QString>

class DateUtils {
public:
  static bool isDateRangeValid(const QDate &checkIn, const QDate &checkOut) {
    return checkIn.isValid() && checkOut.isValid() && checkIn < checkOut;
  }

  static int daysBetween(const QDate &checkIn, const QDate &checkOut) {
    if (!checkIn.isValid() || !checkOut.isValid())
      return 0;
    int diff = checkIn.daysTo(checkOut);
    return diff <= 0 ? 0 : diff;
  }

  static bool datesOverlap(const QDate &startA, const QDate &endA,
                           const QDate &startB, const QDate &endB) {
    if (!isDateRangeValid(startA, endA) || !isDateRangeValid(startB, endB))
      return false;
    return startA < endB && startB < endA;
  }

  static bool isValidDate(const QString &date) {
    QDate d = QDate::fromString(date, "yyyy-MM-dd");
    if (!d.isValid())
      return false;
    return d.toString("yyyy-MM-dd") == date;
  }

  static bool isDateRangeValid(const QString &checkIn,
                               const QString &checkOut) {
    QDate in = QDate::fromString(checkIn, "yyyy-MM-dd");
    QDate out = QDate::fromString(checkOut, "yyyy-MM-dd");
    return isDateRangeValid(in, out);
  }

  static int daysBetween(const QString &checkIn, const QString &checkOut) {
    QDate in = QDate::fromString(checkIn, "yyyy-MM-dd");
    QDate out = QDate::fromString(checkOut, "yyyy-MM-dd");
    return daysBetween(in, out);
  }

  static bool datesOverlap(const QString &startA, const QString &endA,
                           const QString &startB, const QString &endB) {
    QDate sA = QDate::fromString(startA, "yyyy-MM-dd");
    QDate eA = QDate::fromString(endA, "yyyy-MM-dd");
    QDate sB = QDate::fromString(startB, "yyyy-MM-dd");
    QDate eB = QDate::fromString(endB, "yyyy-MM-dd");
    return datesOverlap(sA, eA, sB, eB);
  }

  static QString today() { return QDate::currentDate().toString("yyyy-MM-dd"); }
};
