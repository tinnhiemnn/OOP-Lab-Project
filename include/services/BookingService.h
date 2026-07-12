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
    double calculateBookingCost(const Booking& booking, double pricePerNight) const;

    BookingRepository& bookings;
    RoomRepository& rooms;

public:
    BookingService(BookingRepository& bookings, RoomRepository& rooms);

    bool createBooking(const QString& customerId, const QString& roomId, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, const QString& groupCode, int buffetQty, bool laundry, bool decoration, const QString& decorationNote, QString& error);

    bool checkIn(const QString& bookingId, QString& error);

    bool checkOut(const QString& bookingId, double& finalAmountOut, QString& error);                     

    bool cancelBooking(const QString& bookingId, QString& error);
};