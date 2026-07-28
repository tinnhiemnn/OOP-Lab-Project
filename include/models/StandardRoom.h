#pragma once

#include "models/Room.h"
#include <algorithm>
#include <QString>

class StandardRoom : public Room
{
private:
    int beds = 1;

public:
    StandardRoom() = default;
 
    StandardRoom(QString id, double price, RoomStatus status = RoomStatus::Available, int beds = 1)
        : Room(std::move(id), price, status,
               RoomType::Standard, beds) {}

    int maxBeds() const override { return 2; }
    
    double calculatePrice(int days) const override
    {
        return std::max(1, days) * (getBasePrice() + (getBeds() - 1) * 100000.0);
    }
};