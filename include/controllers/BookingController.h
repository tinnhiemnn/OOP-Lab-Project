#pragma once

#include "services/BookingService.h"
#include <QString>
#include <QDate>

class BookingController {
private:
    BookingService& bookingService;
    BookingRepository bookingRepo;

public:
    // Nhận vào Service thông qua cơ chế Dependency Injection (tham chiếu)
    explicit BookingController(BookingService& service);

    bool createBooking(const QString& customerId, const QString& roomId, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, const QString& groupCode, int buffetQty, bool laundry, bool decoration, const QString& decorationNote, QString& error);

    bool processCheckIn(const QString& bookingId, QString& error);

    bool processCheckOut(const QString& bookingId, QString& error);

    bool processCancelBooking(const QString& bookingId, QString& error);

    std::vector<Booking> getAllBookings() const;

    std::vector<Booking> searchBookings(const QString& keyword);
};