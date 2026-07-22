#include "controllers/BookingController.h"
#include "utils/ValidationUtils.h"

BookingController::BookingController(BookingService& service)
    : bookingService(service) {}

bool BookingController::createBooking(const QString& customerId, const QString& roomId, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, const QString& groupCode, int buffetQty, bool laundry, bool decoration, const QString& decorationNote, QString& error)
{
    if (!ValidationUtils::isNonEmpty(customerId)) {
        error = "Please select or enter the Customer ID!";
        return false;
    }
    
    if (!ValidationUtils::isNonEmpty(roomId)) {
        error = "Please select the room!";
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

    return bookingService.createBooking(customerId, roomId, checkIn, checkOut, receptionistId, groupCode, buffetQty, laundry, decoration, decorationNote, error);
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

std::vector<Booking> BookingController::getAllBookings() const
{
    BookingRepository repo;
    return repo.findAll();
}

std::vector<Booking> BookingController::searchBookings (const QString& keyword){
    std::vector<Booking> result;

    auto bookings = bookingRepo.findAll();

    for (const auto& booking : bookings)
    {
        if (booking.getId().contains(keyword, Qt::CaseInsensitive) ||
            booking.getCustomerId().contains(keyword, Qt::CaseInsensitive) ||
            booking.getRoomId().contains(keyword, Qt::CaseInsensitive) ||
            booking.getGroupCode().contains(keyword, Qt::CaseInsensitive))
        {
            result.push_back(booking);
        }
    }
    return result;
}