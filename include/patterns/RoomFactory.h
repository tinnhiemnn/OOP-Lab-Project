#pragma once

#include "models/Room.h"

#include <memory>
#include <string>
#include <QString>

class RoomFactory {
public:
    static std::unique_ptr<Room> createRoom(RoomType type,
                                            const QString& roomId,
                                            double basePrice,
                                            RoomStatus status = RoomStatus::Available,
                                            int numberOfBeds = 1);

    static std::unique_ptr<Room> createRoom(const QString& type,
                                            const QString& roomId,
                                            double basePrice,
                                            RoomStatus status = RoomStatus::Available,
                                            int numberOfBeds = 1);
};

