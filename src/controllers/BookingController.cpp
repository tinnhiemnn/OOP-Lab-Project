#include "controllers/BookingController.h"
#include "utils/ValidationUtils.h"

BookingController::BookingController()
    : bookingRepo(),
      roomRepo(),
      bookingService(bookingRepo, roomRepo) {}

bool BookingController::createMultiBookings(const QString& customerId, const std::vector<QString>& roomIds, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, const std::vector<RoomServiceSelection>& services, QString& error)
{
    if (!ValidationUtils::isNonEmpty(customerId)) {
        error = "Please select or enter the Customer ID!";
        return false;
    }

    auto customer = customerRepo.findById(customerId);
    if (!customer) {
        error = "Customer ID does not exist or is inactive!";
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

    auto receptionist = receptionistRepo.findById(receptionistId);
    if (!receptionist) {
        error = "Receptionist ID does not exist or is inactive!";
        return false;
    }

    if (services.size() != roomIds.size()) {
        error = "Service information does not match the selected rooms!";
        return false;
    }

    for (const auto& service : services) {
        if (service.buffetQty < 0) {
            error = "The number of buffet tickets cannot be negative!";
            return false;
        }
    }

    return bookingService.createMultiBookings(customerId, roomIds, checkIn, checkOut, receptionistId, services, error);
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

std::vector<QString> BookingController::getAvailableRoomIds(const QDate& checkIn, const QDate& checkOut) {
    std::vector<QString> availableIds;
    if (checkIn >= checkOut) return availableIds;
    
    auto rooms = roomRepo.findAvailableInPeriod(checkIn, checkOut); 
    for (const auto& room : rooms) {
        if (room) {
            availableIds.push_back(room->getRoomId());
        }
    }
    
    return availableIds;
}