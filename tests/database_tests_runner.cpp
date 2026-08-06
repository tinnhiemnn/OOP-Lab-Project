#include <QCoreApplication>
#include <iostream>

extern void testDatabaseReconnect();
extern void testRoomDeleteConstraint();
extern void testCustomerDeleteConstraint();
extern void testBookingEmptyReceptionistInsert();

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    
    testDatabaseReconnect();
    testRoomDeleteConstraint();
    testCustomerDeleteConstraint();
    testBookingEmptyReceptionistInsert();
    std::cout << "All database tests passed successfully!" << std::endl;
    return 0;
}
