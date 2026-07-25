#include "controllers/BookingController.h"
#include "utils/ValidationUtils.h"

BookingController::BookingController()
    : bookingRepo(),
      roomRepo(),
      bookingService(bookingRepo, roomRepo) {}

bool BookingController::createMultiBookings(const QString& customerId, const std::vector<QString>& roomIds, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, int buffetQty, bool laundry, bool decoration, const QString& decorationNote, QString& error)
{
    if (!ValidationUtils::isNonEmpty(customerId)) {
        error = "Please select or enter the Customer ID!";
        return false;
    }
    
    if (roomIds.empty()) {
        error = "Please select at least one room!";
        return false;
    }

    if (!ValidationUtils::isNonEmpty(receptionistId)) {
        error = "Receptionist ID cannot be empty!";
        return false;
    }
    if (buffetQty < 0) {
        error = "The number of buffet tickets cannot be negative!";
        return false;
    }

    return bookingService.createMultiBookings(customerId, roomIds, checkIn, checkOut, receptionistId, buffetQty, laundry, decoration, decorationNote, error);
}

bool BookingController::processCheckIn(const QString& bookingId, QString& error) {
    if (!ValidationUtils::isNonEmpty(bookingId)){
        error ="Booking ID cannot be empty!";
        return false;
    }

    return bookingService.checkIn(bookingId, error);
}

bool BookingController::processCheckOut(const QString& bookingId, QString& error) {
    if(!ValidationUtils::isNonEmpty(bookingId)) {
        error = "Booking ID cannot be empty!";
        return false;
    }

    return bookingService.checkOut(bookingId, error);
}

bool BookingController::processCancelBooking(const QString& bookingId, QString& error) {
    if (bookingId.trimmed().isEmpty()) {
        error = "Invalid Booking ID!";
        return false;
    }

    return bookingService.cancelBooking(bookingId, error);
}

std::vector<Booking> BookingController::getAllBookings()
{
    return bookingRepo.findAll();
}

std::vector<Booking> BookingController::searchBookings (const QString& keyword)
{
    return bookingRepo.search(keyword);
}