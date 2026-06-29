#pragma once

#include "models/Room.h"
#include <algorithm>

class DeluxeRoom : public Room
{
public:
    DeluxeRoom() = default;
    DeluxeRoom(std::string id = "", double price = 0, RoomStatus status = RoomStatus::Available)
        : Room(std::move(id), price, status, RoomType::Deluxe) {}

    double calculatePrice(int days) const override
    {
        return std::max(1, days) * getBasePrice() * 1.2;
    }
};