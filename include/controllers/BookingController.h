#pragma once

#include "services/BookingService.h"

#include "repositories/BookingRepository.h"
#include "repositories/RoomRepository.h"
#include "repositories/CustomerRepository.h"
#include "repositories/ReceptionistRepository.h"

#include <QString>
#include <QDate>

class BookingController {
private:
    BookingService bookingService;

    BookingRepository bookingRepo;
    RoomRepository roomRepo;
    CustomerRepository customerRepo;
    ReceptionistRepository receptionistRepo;

public:
    BookingController();

    bool createMultiBookings(const QString& customerId, const std::vector<QString>& roomIds, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, const std::vector<RoomServiceSelection>& services, QString& error);

    bool processCheckIn(const QString& bookingId, QString& error);

    bool processCheckOut(const QString& bookingId, QString& error);

    bool processCancelBooking(const QString& bookingId, QString& error);

    std::vector<Booking> getAllBookings();

    std::vector<Booking> searchBookings(const QString& keyword);
    std::vector<QString> getAvailableRoomIds(const QDate& checkIn, const QDate& checkOut);
};