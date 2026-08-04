#include <QCoreApplication>
#include <iostream>

extern void testDatabaseReconnect();
extern void testRoomDeleteConstraint();
extern void testCustomerDeleteConstraint();
extern void testBookingEmptyReceptionistInsert();
extern void runBookingServiceTests();
extern void runControllerTests();
extern void runControllerValidationTests();

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    testDatabaseReconnect();
    testRoomDeleteConstraint();
    testCustomerDeleteConstraint();
    testBookingEmptyReceptionistInsert();
    runBookingServiceTests();
    runControllerTests();
    runControllerValidationTests();

    std::cout << "All program tests passed successfully!" << std::endl;
    return 0;
}
