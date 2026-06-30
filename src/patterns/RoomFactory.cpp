#include "patterns/RoomFactory.h"

#include "models/DeluxeRoom.h"
#include "models/PresidentRoom.h"
#include "models/StandardRoom.h"

std::unique_ptr<Room> RoomFactory::createRoom(RoomType type,
                                              const std::string& roomId,
                                              double basePrice,
                                              RoomStatus status,
                                              int numberOfBeds) {
    switch (type) {
    case RoomType::Standard:
        return std::make_unique<StandardRoom>(roomId, basePrice, status, numberOfBeds);
    case RoomType::Deluxe:
        return std::make_unique<DeluxeRoom>(roomId, basePrice, status);
    case RoomType::President:
        return std::make_unique<PresidentRoom>(roomId, basePrice, status);
    }
    return std::make_unique<StandardRoom>(roomId, basePrice, status, numberOfBeds);
}

std::unique_ptr<Room> RoomFactory::createRoom(const std::string& type,
                                              const std::string& roomId,
                                              double basePrice,
                                              RoomStatus status,
                                              int numberOfBeds) {
    return createRoom(Room::typeFromString(type), roomId, basePrice, status, numberOfBeds);
}

