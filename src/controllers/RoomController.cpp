#include "controllers/RoomController.h"

#include "models/Booking.h"

#include "utils/ValidationUtils.h"

RoomController::RoomController()
    : bookingRepo(),
      roomRepo(),
      bookingService(bookingRepo, roomRepo) {}

std::vector<std::unique_ptr<Room>> RoomController::getAllRooms() {
    return roomRepo.findAll();
}

std::vector<std::unique_ptr<Room>> RoomController::searchRooms(const QString& type, const QString& status) {
    return roomRepo.search(type, status);
}

std::unique_ptr<Room> RoomController::getRoomById(const QString& id) {
    return roomRepo.findById(id);
}

bool RoomController::addRoom(const Room& room, QString& error) {
    if (!ValidationUtils::isNonEmpty(Room::typeToString(room.getRoomType()))) {
        error = "Room cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isNonEmpty(room.getRoomId())) {
        error = "Room ID cannot be empty.";
        return false;
    }

    if (roomRepo.findById(room.getRoomId())) {
        error = "Room ID already exists.";
        return false;
    }

    if(!ValidationUtils::isPositiveMoney(room.getBasePrice())) {
        error = "Room price must be greater than 0.";
        return false;
    }

    if (room.getBeds() < 1 || room.getBeds() > room.maxBeds()) {
        error = "Invalid number of beds for this room type.";
        return false;
    }

    if (roomRepo.add(room)) {
        return true;
    }

    error = roomRepo.lastError();
    return false;
}

bool RoomController::updateRoom(const Room& room, QString& error) {
    if (room.getBeds() < 1 || room.getBeds() > room.maxBeds()) {
        error = "Invalid number of beds for this room type.";
        return false;
    }

    if (roomRepo.update(room)) {
        return true;
    }

    error = roomRepo.lastError();
    return false;
}

bool RoomController::updateRoomStatus(const QString& id, RoomStatus status, QString& error) {
    if (roomRepo.updateStatus(id, status)) {
        return true;
    }
    error = roomRepo.lastError();
    return false;
}

bool RoomController::deleteRoom(const QString& id, QString& error) {
    if (!ValidationUtils::isNonEmpty(id)) {
        error = "Room ID cannot be empty.";
        return false;
    }

    auto room = roomRepo.findById(id);
    if (!room) {
        error = "Room does not exist.";
        return false;
    }
    
    std::vector<Booking> allBookings = bookingRepo.search(id);

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

    if (roomRepo.remove(id)) {
        return true;
    }
    error = roomRepo.lastError();
    return false;
}

std::vector<std::unique_ptr<Room>> RoomController::checkAvailability(
    const QDate& checkIn,
    const QDate& checkOut,
    QString& error)
{
    if (checkIn >= checkOut)
    {
        error = "Checkout date must be after checkin date.";
        return {};
    }

    return roomRepo.findAvailableInPeriod(checkIn, checkOut);
}