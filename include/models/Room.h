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
    int beds = 1; 
    double basePrice = 0.0;
    RoomStatus status = RoomStatus::Available;
    RoomType type = RoomType::Standard;

public:
    Room() = default;
    Room(QString roomId, double basePrice, RoomStatus status, RoomType type, int beds)
        : roomId(std::move(roomId)), beds(beds), basePrice(basePrice), status(status), type(type) {}
    virtual ~Room() = default;

    virtual int maxBeds() const = 0;
    
    const QString& getRoomId() const { return roomId; }
    double getBasePrice() const { return basePrice; }
    RoomStatus getStatus() const { return status; }
    RoomType getRoomType() const { return type; }
    int getBeds() const { return beds; }

    void setBeds(int beds) 
    {
        this -> beds = beds;
    }

    //kiem tra phong
    bool isAvailable() const
    {
        return status == RoomStatus::Available;
    }

    void changeStatus(RoomStatus value)
    {
        status = value;
    }

    //tinh tien phong
    virtual double calculatePrice(int days) const = 0;

    //chuyen RoomType -> QString
    static QString typeToString(RoomType value)
    {
        switch (value) {
            case RoomType::Deluxe:    return "Deluxe";
            case RoomType::President: return "President";
            default:                  return "Standard";
        }
    }

    //chuyen QString -> RoomType
    static RoomType typeFromString(const QString& value)
    {
        if (value == "Deluxe")    return RoomType::Deluxe;
        if (value == "President") return RoomType::President;
        return RoomType::Standard;
    }

    //chuyen RoomStatus -> QString
    static QString statusToString(RoomStatus value) {
        switch (value) {
            case RoomStatus::Maintenance:  return "Maintenance";
            case RoomStatus::InUse:        return "InUse";
            case RoomStatus::NeedCleaning: return "NeedCleaning";
            default:                       return "Available";
        }
    }

    //chuyen QString -> RoomStatus
    static RoomStatus statusFromString(const QString& value) {
        if (value == "Maintenance")  return RoomStatus::Maintenance;
        if (value == "InUse")        return RoomStatus::InUse;
        if (value == "NeedCleaning") return RoomStatus::NeedCleaning;
        return RoomStatus::Available;
    }
};