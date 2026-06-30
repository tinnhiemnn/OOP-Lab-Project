#pragma once

#include <string>

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
    std::string roomId;
    double basePrice = 0.0;
    RoomStatus status = RoomStatus::Available;
    RoomType type = RoomType::Standard;

public:
    Room() = default;
    Room(std::string roomId, double basePrice, RoomStatus status, RoomType type)
        : roomId(std::move(roomId)),
          basePrice(basePrice),
          status(status),
          type(type) {}
    virtual ~Room() = default;

    const std::string& getRoomId() const { return roomId; }
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
    static std::string typeToString(RoomType value)
    {
        if (value == RoomType::Deluxe) return "Deluxe";
        if (value == RoomType::President) return "President";
        return "Standard";
    }

    //chuyển String -> RoomType
    static RoomType typeFromString(const std::string& value)
    {
        if (value == "Deluxe") return RoomType::Deluxe;
        if (value == "President") return RoomType::President;
        return RoomType::Standard;
    }

    //chuyển RoomStatus -> String
    static std::string statusToString(RoomStatus value) {
        switch (value) {
            case RoomStatus::Maintenance:  return "Maintenance";
            case RoomStatus::InUse:return "InUse";
            case RoomStatus::NeedCleaning: return "NeedCleaning";
            case RoomStatus::Available:
            default: return "Available";
        }
    }
    static RoomStatus statusFromString(const std::string& value) {
        if (value == "Maintenance")  return RoomStatus::Maintenance;
        if (value == "InUse") return RoomStatus::InUse;
        if (value == "NeedCleaning") return RoomStatus::NeedCleaning;
        return RoomStatus::Available;
    }
};