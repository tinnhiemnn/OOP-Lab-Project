#pragma once

#include "models/Room.h"
#include <QString>

class PresidentRoom : public Room
{
public:
    PresidentRoom() = default;
    
    PresidentRoom(QString id = "", double price = 0, RoomStatus status = RoomStatus::Available, int beds = 1)
        : Room(std::move(id), price, status, RoomType::President, beds) {}

    int maxBeds() const override { return 5; }
    
    double calculatePrice(int days) const override
    {
        return std::max(1, days) * (getBasePrice() * 1.5 + (getBeds() - 1) * 200000.0);
    }
};