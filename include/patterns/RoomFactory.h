#pragma once

#include "models/Room.h"

#include <memory>
#include <string>

class RoomFactory {
public:
    static std::unique_ptr<Room> createRoom(RoomType type,
                                            const std::string& roomId,
                                            double basePrice,
                                            RoomStatus status = RoomStatus::Available,
                                            int numberOfBeds = 1);

    static std::unique_ptr<Room> createRoom(const std::string& type,
                                            const std::string& roomId,
                                            double basePrice,
                                            RoomStatus status = RoomStatus::Available,
                                            int numberOfBeds = 1);
};

