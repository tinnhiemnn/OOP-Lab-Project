#pragma once

#include "repositories/BookingRepository.h"
#include "repositories/RoomRepository.h"
#include "models/Booking.h"
#include "models/Room.h" 

#include <QString>
#include <QDate>
#include <optional>

class BookingService {
private:
    bool hasConflict(const QString& roomId, const QDate& checkIn, const QDate& checkOut, const QString& excludeBookingId = "") const;
    QString makeBookingId() const;
    QString makeGroupCode() const;
    double calculateBookingCost(const Booking& booking, double pricePerNight) const;

    BookingRepository& bookings;
    RoomRepository& rooms;

public:
    BookingService(BookingRepository& bookings, RoomRepository& rooms);

    bool createMultiBookings(const QString& customerId, const std::vector<QString>& roomIds, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, int buffetQty, bool laundry, bool decoration, const QString& decorationNote, QString& error);

    bool checkIn(const QString& bookingId, QString& error);

    bool checkOut(const QString& bookingId, QString& error);                     

    bool cancelBooking(const QString& bookingId, QString& error);
};