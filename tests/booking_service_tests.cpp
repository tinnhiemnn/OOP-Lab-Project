#include "TestHelpers.h"

#include <cassert>
#include <iostream>
#include <vector>

#include "repositories/BookingRepository.h"
#include "repositories/RoomRepository.h"
#include "services/BookingService.h"

void runBookingServiceTests()
{
    QString databasePath;
    QString error;
    assert(openTemporaryDatabase(databasePath, error));

    BookingRepository bookingRepo;
    RoomRepository roomRepo;
    BookingService service(bookingRepo, roomRepo);

    QString message;
    assert(!service.createMultiBookings("C001", {}, QDate::currentDate().addDays(10), QDate::currentDate().addDays(12), "REC1", {}, message));
    assert(message == "No rooms selected!");

    assert(!service.createMultiBookings("C001", {"R101"}, QDate::currentDate().addDays(10), QDate::currentDate().addDays(9), "REC1", {{}}, message));
    assert(message == "Check-out date must be after check-in date.");

    assert(!service.createMultiBookings("C001", {"R101"}, QDate::currentDate().addDays(-2), QDate::currentDate().addDays(2), "REC1", {{}}, message));
    assert(message == "It is not possible to create a booking for a past date!");

    assert(!service.createMultiBookings("C001", {"R101"}, QDate::currentDate().addDays(10), QDate::currentDate().addDays(12), "REC1", {}, message));
    assert(message == "Service information does not match the selected rooms.");

    auto maintenanceRoom = makeRoom("R101", RoomType::Standard, 120.0, RoomStatus::Maintenance, 2);
    assert(maintenanceRoom && roomRepo.add(*maintenanceRoom));
    assert(!service.createMultiBookings("C001", {"R101"}, QDate::currentDate().addDays(10), QDate::currentDate().addDays(12), "REC1", {{}}, message));
    assert(message == "The room is under maintenance and cannot be booked!");

    auto room = makeRoom("R102", RoomType::Standard, 90.0, RoomStatus::Available, 2);
    assert(room && roomRepo.add(*room));
    std::vector<RoomServiceSelection> services = {{1, false, false, ""}};
    assert(service.createMultiBookings("CUS001", {"R102"}, QDate::currentDate().addDays(10), QDate::currentDate().addDays(12), "REC001", services, message));

    auto bookings = bookingRepo.search("R102");
    assert(bookings.size() == 1);
    const QString bookingId = bookings[0].getId();

    assert(!service.checkIn("BAD_ID", message));
    assert(message == "Booking id not found!");

    assert(!service.checkOut(bookingId, message));
    assert(message == "This room has not been checked in, so it cannot be checked out!");

    assert(service.checkIn(bookingId, message));
    assert(!service.checkIn(bookingId, message));
    assert(message == "This reservation is not in a check-in ready status!");

    assert(service.checkOut(bookingId, message));
    assert(!service.cancelBooking(bookingId, message));
    assert(message == "This reservation has already been completed or canceled previously!");

    cleanupTemporaryDatabase(databasePath);
    std::cout << "Booking service tests passed!" << std::endl;
}
