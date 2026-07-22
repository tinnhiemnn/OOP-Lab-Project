#include "controllers/RoomController.h"
#include "utils/ValidationUtils.h"
#include "repositories/BookingRepository.h"
#include "models/Booking.h"

RoomController::RoomController(BookingService& service)
    : bookingService(service) {}

std::vector<std::unique_ptr<Room>> RoomController::getAllRooms() {
    return repository.findAll();
}

std::vector<std::unique_ptr<Room>> RoomController::searchRooms(const QString& type, const QString& status) {
    return repository.search(type, status);
}

std::unique_ptr<Room> RoomController::getRoomById(const QString& id) {
    return repository.findById(id);
}

bool RoomController::addRoom(const Room& room, int beds, QString& error) {
    if (!ValidationUtils::isNonEmpty(Room::typeToString(room.getRoomType()))) {
        error = "Room cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isNonEmpty(room.getRoomId())) {
        error = "Room ID cannot be empty.";
        return false;
    }

    if(!ValidationUtils::isPositiveMoney(room.getBasePrice())) {
        error = "Room price must be greater than 0.";
        return false;
    }

    if (repository.add(room, beds)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

bool RoomController::updateRoom(const Room& room, int beds, QString& error) {
    if (repository.update(room, beds)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

bool RoomController::updateRoomStatus(const QString& id, RoomStatus status, QString& error) {
    if (repository.updateStatus(id, status)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

bool RoomController::deleteRoom(const QString& id, QString& error) {
    //Check co phai RoomID rong hay khong
    if (!ValidationUtils::isNonEmpty(id)) {
        error = "Room ID cannot be empty.";
        return false;
    }

    //Check room co ton tai hay khong
    auto room = repository.findById(id);
    if (!room) {
        error = "Room does not exist.";
        return false;
    }
    
    BookingRepository bookingRepo;
    std::vector<Booking> allBookings = bookingRepo.findAll();

    //Check xem co booking nao dang Booked hoac CheckedIn voi room nay hay khong
    for (const auto& booking : allBookings) {
        if (booking.getRoomId() == id) {
            if (booking.getStatus() == BookingStatus::Booked || 
                booking.getStatus() == BookingStatus::CheckedIn) {
                error = "Cannot delete a room that currently has Booked or Checked In bookings.";
                return false;
            }
        }
    }

    if (repository.remove(id)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

std::vector<std::unique_ptr<Room>> RoomController::checkAvailability(
    const QDate& checkIn,
    const QDate& checkOut,
    RoomType roomType,
    QString& error)
{
    if (checkIn >= checkOut)
    {
        error = "Checkout date must be after checkin date.";
        return {};
    }

    return bookingService.checkAvailability(
        checkIn,
        checkOut,
        roomType,
        error
    );
}