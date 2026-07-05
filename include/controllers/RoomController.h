#pragma once

#include "repositories/RoomRepository.h"

class RoomController
{
private:
    RoomRepository repository;
    
public:
    RoomController() {}

    std::vector<std::unique_ptr<Room>> getAllRooms()
    {
        return repository.findAll();
    }

    std::vector<std::unique_ptr<Room>> searchRooms(const QString& type, const QString& status)
    {
        return repository.search(type, status);
    }

    std::unique_ptr<Room> getRoomById(const QString& id)
    {
        return repository.findById(id);
    }

    bool addRoom(const Room& room, int beds, QString& error)
    {
        if (repository.add(room, beds)) {
            return true;
        }
        error = repository.lastError();
        return false;
    }

    bool updateRoom(const Room& room, int beds, QString& error)
    {
        if (repository.update(room, beds)) {
            return true;
        }
        error = repository.lastError();
        return false;
    }

    bool updateRoomStatus(const QString& id, RoomStatus status, QString& error)
    {
        if (repository.updateStatus(id, status)) {
            return true;
        }
        error = repository.lastError();
        return false;
    }

    bool deleteRoom(const QString& id, QString& error)
    {
        if (repository.remove(id)) {
            return true;
        }
        error = repository.lastError();
        return false;
    }
};