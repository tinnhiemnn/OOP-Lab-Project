#pragma once

#include <QString>

enum class RoomType
{
    Standard,
    Deluxe,
    President
};


enum class RoomStatus
{
    Available,
    Maintenance,
    InUse,
    NeedCleaning
};

class Room
{
private:
    QString roomId;
    double basePrice = 0.0;
    RoomStatus status = RoomStatus::Available;
    RoomType type = RoomType::Standard;

public:
    Room() = default;
    Room(QString roomId, double basePrice, RoomStatus status, RoomType type)
        : roomId(std::move(roomId)),
          basePrice(basePrice),
          status(status),
          type(type) {}
    virtual ~Room() = default;

    const QString& getRoomId() const { return roomId; }
    double getBasePrice() const { return basePrice; }
    RoomStatus getStatus() const { return status; }
    RoomType getRoomType() const { return type; }

    //kiểm tra phòng có sẵn hay không
    bool isAvailable() const
    {
        return status == RoomStatus::Available;
    }

    //thay đổi trạng thái phòng
    void changeStatus(RoomStatus value)
    {
        status = value;
    }

    //tính tiền
    virtual double calculatePrice(int days) const = 0;

    //chuyển RoomType -> String
    static QString typeToString(RoomType value)
    {
        if (value == RoomType::Deluxe) return "Deluxe";
        if (value == RoomType::President) return "President";
        return "Standard";
    }

    //chuyển String -> RoomType
    static RoomType typeFromString(const QString& value)
    {
        if (value == "Deluxe") return RoomType::Deluxe;
        if (value == "President") return RoomType::President;
        return RoomType::Standard;
    }

    //chuyển RoomStatus -> String
    static QString statusToString(RoomStatus value)
    {
        if (value == RoomStatus::Maintenance) return "Maintenance";
        return "Available";
    }

    //chuyển String -> RoomStatus
    static RoomStatus statusFromString(const QString& value)
    {
        if (value == "Maintenance") return RoomStatus::Maintenance;
        return RoomStatus::Available;
    }
};