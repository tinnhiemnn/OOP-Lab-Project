#pragma once

#include "models/Room.h"
#include <QString>

class PresidentRoom : public Room
{
public:
    PresidentRoom() = default;
    
    PresidentRoom(QString id = "", double price = 0, RoomStatus status = RoomStatus::Available)
        : Room(std::move(id), price, status, RoomType::President) {}

    int getBeds() const override { return 1; }

    double calculatePrice(int days) const override
    {
        return std::max(1, days) * getBasePrice() * 1.5;
    }
};