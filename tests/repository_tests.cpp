#include "TestHelpers.h"

#include <cassert>
#include <iostream>
#include <QCoreApplication>

#include "models/Booking.h"
#include "controllers/CustomerController.h"
#include "controllers/RoomController.h"
#include "repositories/BookingRepository.h"
#include "repositories/RoomRepository.h"
#include "repositories/CustomerRepository.h"

void runRepositoryTests()
{
    QString databasePath;
    QString error;
    assert(openTemporaryDatabase(databasePath, error));

    RoomRepository roomRepo;
    BookingRepository bookingRepo;
    CustomerRepository customerRepo;

    auto room = makeRoom("R101", RoomType::Standard, 100.0, RoomStatus::Available, 2);
    assert(room && roomRepo.add(*room));
    auto foundRooms = roomRepo.findAll();
    assert(foundRooms.size() >= 1);
    assert(roomRepo.findById("R101") != nullptr);
    assert(roomRepo.findById("BAD") == nullptr);
    assert(roomRepo.search("Standard", "Available").size() >= 1);

    // Add duplicate room should fail
    assert(!roomRepo.add(*room));
    assert(!roomRepo.lastError().isEmpty());

    Booking booking("B123", "CUS001", "REC001", "R101", "GRP1", QDate::currentDate().addDays(1), QDate::currentDate().addDays(2));
    assert(bookingRepo.add(booking));
    assert(bookingRepo.findById("B123").has_value());
    assert(bookingRepo.countBookings("Standard", "All") >= 1);

    DatabaseManager::getInstance().closeConnection();
    Booking invalidBooking("B999", "C001", "REC1", "R101", "GRP2", QDate::currentDate().addDays(3), QDate::currentDate().addDays(4));
    assert(!bookingRepo.add(invalidBooking));
    assert(!bookingRepo.lastError().isEmpty());

    assert(openTemporaryDatabase(databasePath, error));

    // Deletion constraints are enforced by controllers, not repositories.
    RoomController roomController;
    bool removed = roomController.deleteRoom("P301", error);
    assert(removed == false);
    assert(error == "Cannot delete a room that currently has Booked or Checked In bookings.");

    CustomerController customerController;
    bool custRemoved = customerController.deleteCustomer("CUS003", error);
    assert(custRemoved == false);
    assert(error == "Cannot delete a customer who currently has active Booked or Checked In bookings.");

    // Database reconnect test: close and re-open the same DB path.
    DatabaseManager::getInstance().closeConnection();
    assert(DatabaseManager::getInstance().openDatabase(databasePath));

    BookingRepository reopenedBookingRepo;

    // Empty receptionist is rejected by the database when inserted directly as an invalid FK value.
    Booking nullRecBooking("B_TEST_NULL", "CUS001", "", "P302", "GRP_TEST", QDate::currentDate().addDays(2), QDate::currentDate().addDays(3));
    bool addNullRec = reopenedBookingRepo.add(nullRecBooking);
    assert(addNullRec == false);
    assert(!reopenedBookingRepo.lastError().isEmpty());

    cleanupTemporaryDatabase(databasePath);
    std::cout << "Repository tests passed!" << std::endl;
}

#ifdef REPOSITORY_TESTS_STANDALONE
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    runRepositoryTests();

    std::cout << "All repository tests passed!" << std::endl;
    return 0;
}
#endif
