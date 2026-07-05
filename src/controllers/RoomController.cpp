#include "RoomController.h"

RoomController::RoomController() {}

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
    if (repository.remove(id)) {
        return true;
    }
    error = repository.lastError();
    return false;
}