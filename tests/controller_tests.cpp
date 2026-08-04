#include <cassert>
#include <iostream>
#include <vector>
#include <QCoreApplication>

#include "controllers/BookingController.h"
#include "TestHelpers.h"
#include "services/BookingService.h"
#include "repositories/BookingRepository.h"
#include "repositories/RoomRepository.h"
#include "utils/ValidationUtils.h"

void runControllerTests()
{
    BookingController controller;
    QString error;

    assert(!controller.createMultiBookings("", {"R101"}, QDate::currentDate().addDays(10), QDate::currentDate().addDays(12), "REC1", {{1, false, false, ""}}, error));
    assert(error == "Please select or enter the Customer ID!");

    QString databasePath;
    assert(openTemporaryDatabase(databasePath, error));

    assert(!controller.createMultiBookings("CUS001", {"P101"}, QDate::currentDate().addDays(10), QDate::currentDate().addDays(12), "", {{1, false, false, ""}}, error));
    assert(error == "Receptionist ID cannot be empty!");

    cleanupTemporaryDatabase(databasePath);

    assert(!controller.processCheckIn("", error));
    assert(error == "Booking ID cannot be empty!");

    assert(!controller.processCheckOut("", error));
    assert(error == "Booking ID cannot be empty!");

    assert(!controller.processCancelBooking("   ", error));
    assert(error == "Invalid Booking ID!");

    std::cout << "Controller tests passed!" << std::endl;
}

// Additional controller-level test to reproduce validation bypass bug described in docs
// Uses CustomerController to check that invalid customer is rejected by controller validation
#include "controllers/CustomerController.h"

void runControllerValidationTests()
{
    CustomerController custCtrl;
    QString error;

    Customer badCust("CBAD", "Bad Name", "not-an-email", "abc1234");
    bool result = custCtrl.addCustomer(badCust, error);
    // Expectation: controller should reject invalid customer input
    assert(result == false);

    std::cout << "Controller validation tests passed!" << std::endl;
}

#ifdef CONTROLLER_TESTS_STANDALONE
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    runControllerTests();
    runControllerValidationTests();

    std::cout << "All controller tests passed!" << std::endl;
    return 0;
}
#endif
