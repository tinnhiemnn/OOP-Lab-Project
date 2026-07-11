#include "models/Room.h"
#include "models/StandardRoom.h"
#include "models/DeluxeRoom.h"
#include "models/PresidentRoom.h"
#include "patterns/RoomFactory.h"

#include <QCoreApplication>
#include <iostream>
#include <cassert>
#include <cmath>

namespace {
    bool nearlyEqual(double a, double b, double eps = 1e-6) {
        return std::fabs(a - b) < eps;
    }
}

// TC20: Chuyen doi hai chieu RoomType/RoomStatus <-> QString
void testRoomTypeAndStatusConversion() {
    assert(Room::typeToString(RoomType::Standard) == "Standard");
    assert(Room::typeToString(RoomType::Deluxe) == "Deluxe");
    assert(Room::typeToString(RoomType::President) == "President");

    assert(Room::typeFromString("Standard") == RoomType::Standard);
    assert(Room::typeFromString("Deluxe") == RoomType::Deluxe);
    assert(Room::typeFromString("President") == RoomType::President);
    assert(Room::typeFromString("unknown") == RoomType::Standard); // fallback mac dinh

    assert(Room::statusToString(RoomStatus::Available) == "Available");
    assert(Room::statusToString(RoomStatus::Maintenance) == "Maintenance");
    assert(Room::statusToString(RoomStatus::InUse) == "InUse");
    assert(Room::statusToString(RoomStatus::NeedCleaning) == "NeedCleaning");

    assert(Room::statusFromString("Available") == RoomStatus::Available);
    assert(Room::statusFromString("Maintenance") == RoomStatus::Maintenance);
    assert(Room::statusFromString("InUse") == RoomStatus::InUse);
    assert(Room::statusFromString("NeedCleaning") == RoomStatus::NeedCleaning);
    assert(Room::statusFromString("unknown") == RoomStatus::Available); // fallback mac dinh

    std::cout << "Room type/status conversion tests passed!" << std::endl;
}

// TC21: Tao phong qua Factory (Standard), gia 500000, 2 giuong, 3 ngay -> 1.800.000
void testRoomFactoryStandard() {
    auto room = RoomFactory::createRoom(RoomType::Standard, "R101", 500000.0, RoomStatus::Available, 2);
    assert(room != nullptr);
    assert(room->getRoomType() == RoomType::Standard);
    assert(dynamic_cast<StandardRoom*>(room.get()) != nullptr);
    assert(nearlyEqual(room->calculatePrice(3), 1800000.0));

    // Kiem tra overload nhan RoomType dang QString cung cho ket qua tuong tu
    auto roomByString = RoomFactory::createRoom(QString("Standard"), "R102", 500000.0, RoomStatus::Available, 2);
    assert(roomByString != nullptr);
    assert(nearlyEqual(roomByString->calculatePrice(3), 1800000.0));

    std::cout << "RoomFactory Standard room tests passed!" << std::endl;
}

// TC22: Tao phong qua Factory (Deluxe), gia 1000000, he so 1.2, 2 ngay -> 2.400.000
void testRoomFactoryDeluxe() {
    auto room = RoomFactory::createRoom(RoomType::Deluxe, "R201", 1000000.0);
    assert(room != nullptr);
    assert(room->getRoomType() == RoomType::Deluxe);
    assert(dynamic_cast<DeluxeRoom*>(room.get()) != nullptr);
    assert(nearlyEqual(room->calculatePrice(2), 2400000.0));

    auto roomByString = RoomFactory::createRoom(QString("Deluxe"), "R202", 1000000.0);
    assert(roomByString != nullptr);
    assert(nearlyEqual(roomByString->calculatePrice(2), 2400000.0));

    std::cout << "RoomFactory Deluxe room tests passed!" << std::endl;
}

// TC23: Tao phong qua Factory (President), gia 2000000, he so 1.5, 1 ngay -> 3.000.000
void testRoomFactoryPresident() {
    auto room = RoomFactory::createRoom(RoomType::President, "R301", 2000000.0);
    assert(room != nullptr);
    assert(room->getRoomType() == RoomType::President);
    assert(dynamic_cast<PresidentRoom*>(room.get()) != nullptr);
    assert(nearlyEqual(room->calculatePrice(1), 3000000.0));

    auto roomByString = RoomFactory::createRoom(QString("President"), "R302", 2000000.0);
    assert(roomByString != nullptr);
    assert(nearlyEqual(roomByString->calculatePrice(1), 3000000.0));

    std::cout << "RoomFactory President room tests passed!" << std::endl;
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    testRoomTypeAndStatusConversion();
    testRoomFactoryStandard();
    testRoomFactoryDeluxe();
    testRoomFactoryPresident();

    std::cout << "All RoomFactory tests passed successfully!" << std::endl;
    return 0;
}
