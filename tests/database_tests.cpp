#include "database/DatabaseManager.h"
#include "repositories/RoomRepository.h"
#include "repositories/CustomerRepository.h"
#include "repositories/BookingRepository.h"
#include "models/StandardRoom.h"
#include "models/Customer.h"
#include "models/Booking.h"

#include <QCoreApplication>
#include <QDate>
#include <iostream>
#include <cassert>

namespace {
    const QString TEST_DB_PATH = "test_hotel.db";
}

// TC26: DatabaseManager phai mo lai duoc CSDL binh thuong sau khi da dong ket noi truoc do
void testDatabaseReconnect() {
    auto& dbMgr = DatabaseManager::getInstance();

    bool openFirst = dbMgr.openDatabase(TEST_DB_PATH);
    assert(openFirst == true);

    dbMgr.closeConnection();

    bool openSecond = dbMgr.openDatabase(TEST_DB_PATH);
    assert(openSecond == true); // se that bai (crash) neu bug "invalid database connection" chua duoc sua

    std::cout << "Database reconnect test passed!" << std::endl;
}

// TC24: Khong duoc phep xoa phong khi con booking dang hoat dong (Booked/CheckedIn)
void testDeleteRoomConstraint() {
    RoomRepository roomRepo;
    CustomerRepository custRepo;
    BookingRepository bookRepo;

    // Don dep du lieu rac truoc khi chay, tranh xung dot khoa chinh giua cac lan chay test
    bookRepo.remove("B999");
    roomRepo.remove("R999");
    custRepo.remove("C999");

    StandardRoom room("R999", 500000.0, RoomStatus::Available, 2);
    assert(roomRepo.add(room, 2) == true);

    Customer customer("C999", "Nguyen Van A", "a@test.com", "0987654321");
    assert(custRepo.add(customer) == true);

    Booking booking("B999", "C999", "REC001", "R999", "",
                     QDate::currentDate(), QDate::currentDate().addDays(2),
                     BookingStatus::Booked);
    assert(bookRepo.add(booking) == true);

    bool result = roomRepo.remove("R999");
    // Neu bug chua sua: result == true (xoa duoc, booking bi mo coi vi room_id -> NULL)
    // Neu bug da sua: result == false vi phong dang co booking hoat dong
    assert(result == false);

    // Don dep sau khi test de khong anh huong cac test khac
    bookRepo.remove("B999");
    custRepo.remove("C999");

    std::cout << "Room delete constraint test passed!" << std::endl;
}

// TC25: Khong duoc phep xoa khach hang khi con booking dang hoat dong
void testDeleteCustomerConstraint() {
    RoomRepository roomRepo;
    CustomerRepository custRepo;
    BookingRepository bookRepo;

    bookRepo.remove("B998");
    roomRepo.remove("R998");
    custRepo.remove("C998");

    StandardRoom room("R998", 500000.0, RoomStatus::Available, 2);
    assert(roomRepo.add(room, 2) == true);

    Customer customer("C998", "Nguyen Van B", "b@test.com", "0987654322");
    assert(custRepo.add(customer) == true);

    Booking booking("B998", "C998", "REC001", "R998", "",
                     QDate::currentDate(), QDate::currentDate().addDays(2),
                     BookingStatus::Booked);
    assert(bookRepo.add(booking) == true);

    bool result = custRepo.remove("C998");
    // Neu bug chua sua: result == true (xoa duoc, booking mat lien ket customer_id)
    // Neu bug da sua: result == false vi khach hang dang co booking hoat dong
    assert(result == false);

    bookRepo.remove("B998");
    roomRepo.remove("R998");

    std::cout << "Customer delete constraint test passed!" << std::endl;
}

// TC27: Duoc phep them Booking khi chua phan cong Receptionist (receptionist_id rong)
void testBookingEmptyReceptionist() {
    RoomRepository roomRepo;
    CustomerRepository custRepo;
    BookingRepository bookRepo;

    bookRepo.remove("B997");
    roomRepo.remove("R997");
    custRepo.remove("C997");

    StandardRoom room("R997", 500000.0, RoomStatus::Available, 2);
    assert(roomRepo.add(room, 2) == true);

    Customer customer("C997", "Nguyen Van C", "c@test.com", "0987654323");
    assert(custRepo.add(customer) == true);

    // receptionistId de rong "" vi khach tu dat online, chua co le tan phan cong
    Booking booking("B997", "C997", "", "R997", "",
                     QDate::currentDate(), QDate::currentDate().addDays(2),
                     BookingStatus::Booked);

    bool result = bookRepo.add(booking);
    // Neu bug chua sua: SQLite tu choi vi "" != NULL nhung khong ton tai receptionist_id = ""
    //                   -> insert that bai, result == false
    // Neu bug da sua: chuoi rong duoc chuyen thanh QVariant NULL truoc khi bind -> result == true
    assert(result == true);

    // Don dep sau khi test
    bookRepo.remove("B997");
    custRepo.remove("C997");
    roomRepo.remove("R997");

    std::cout << "Booking empty receptionist test passed!" << std::endl;
}

int main(int argc, char* argv[]) {
    // Bat buoc phai co QCoreApplication de Qt SQL plugin (SQLite) nap duoc dung cach
    QCoreApplication app(argc, argv);

    testDatabaseReconnect();
    testDeleteRoomConstraint();
    testDeleteCustomerConstraint();
    testBookingEmptyReceptionist();

    std::cout << "All database & repository constraint tests passed successfully!" << std::endl;
    return 0;
}