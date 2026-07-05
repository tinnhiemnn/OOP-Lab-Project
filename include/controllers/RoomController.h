#pragma once

#include "repositories/RoomRepository.h"

class RoomController
{
private:
    RoomRepository repository;
    
public:
    RoomController();

    std::vector<std::unique_ptr<Room>> getAllRooms();
    std::vector<std::unique_ptr<Room>> searchRooms(const QString& type, const QString& status);
    std::unique_ptr<Room> getRoomById(const QString& id);
    
    bool addRoom(const Room& room, int beds, QString& error);
    bool updateRoom(const Room& room, int beds, QString& error);
    bool updateRoomStatus(const QString& id, RoomStatus status, QString& error);
    bool deleteRoom(const QString& id, QString& error);
};