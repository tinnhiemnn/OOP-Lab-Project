#pragma once

#include "repositories/RoomRepository.h"
#include "repositories/BookingRepository.h"
#include "services/BookingService.h"

class RoomController
{
private:
    RoomRepository roomRepo;
    BookingRepository bookingRepo;
    BookingService bookingService;

public:
    RoomController();

    std::vector<std::unique_ptr<Room>> getAllRooms();
    std::vector<std::unique_ptr<Room>> searchRooms(const QString& type, const QString& status);
    std::unique_ptr<Room> getRoomById(const QString& id);
    
    bool addRoom(const Room& room, int beds, QString& error);
    bool updateRoom(const Room& room, int beds, QString& error);
    bool updateRoomStatus(const QString& id, RoomStatus status, QString& error);
    bool deleteRoom(const QString& id, QString& error);

    std::vector<std::unique_ptr<Room>> checkAvailability(
        const QDate& checkIn,
        const QDate& checkOut,
        RoomType roomType,
        QString& error);
};