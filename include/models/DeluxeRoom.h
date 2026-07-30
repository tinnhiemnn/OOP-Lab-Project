#pragma once

#include "models/Room.h"
#include <QString>

class DeluxeRoom : public Room
{
public:
    DeluxeRoom() = default;
    
    DeluxeRoom(QString id = "", double price = 0, RoomStatus status = RoomStatus::Available, int beds = 1)
        : Room(std::move(id), price, status, RoomType::Deluxe, beds) {}

    int maxBeds() const override { return 3; }
    
    double calculatePrice(int days) const override
    {
        return std::max(1, days) * (getBasePrice() * 1.2 + (getBeds() - 1) * 150000.0);
    }
};