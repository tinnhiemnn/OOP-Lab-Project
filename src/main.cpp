#include <QCoreApplication>
#include <iostream>
#include <memory>
#include <vector>
#include <optional>

// Include các headers quản lý database và repositories của nhóm bạn
#include "database/DatabaseManager.h"
#include "repositories/CustomerRepository.h"
#include "repositories/ReceptionistRepository.h"
#include "repositories/RoomRepository.h"
#include "models/StandardRoom.h"


// Hàm tiện ích để in dòng phân cách dòng test case
void printSeparator(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << " TEST CASE: " << title << "\n";
    std::cout << "========================================\n";
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);


    printSeparator("KHOI TAO KET NOI DATABASE");
    if (!DatabaseManager::getInstance().openDatabase()) {
        std::cerr << "🚨 Loi khoi tao CSDL! Dung chuong trinh.\n";
        return -1;
    }

    // 2. Khởi tạo các đối tượng Repository phục vụ kiểm thử
    CustomerRepository customerRepo;
    ReceptionistRepository receptionistRepo;
    RoomRepository roomRepo;

    // =========================================================================
    // TEST CASE 1: KIỂM THỬ CUSTOMER REPOSITORY (CRUD)
    // =========================================================================
    printSeparator("CUSTOMER REPOSITORY (CRUD)");

    // Giả định hàm khởi tạo Customer(id, name, email, phone) sử dụng QString
    Customer c1("C001", "Nguyen Van A", "a.nguyen@gmail.com", "0901234567");
    
    // Test Add
    if (customerRepo.add(c1)) {
        std::cout << "✅ Them khach hang C001 thanh cong.\n";
    } else {
        std::cerr << "❌ Them khach hang that bai: " << customerRepo.lastError().toStdString() << "\n";
    }

    // Test FindById & Update
    auto foundCustomer = customerRepo.findById("C001");
    if (foundCustomer.has_value()) {
        std::cout << "🔍 Tim thay khach hang: " << foundCustomer->getName().toStdString() << "\n";
        
        // Thay đổi thông tin để test Update
        foundCustomer->setName("Nguyen Van A (Updated)");
        if (customerRepo.update(*foundCustomer)) {
            std::cout << "✅ Cap nhat thong tin khach hang C001 thanh cong.\n";
        }
    } else {
        std::cerr << "❌ Khong tim thay khach hang C001!\n";
    }

    // Test Search
    std::cout << "🔎 Thu nghiem tim kiem Khach hang voi tu khoa 'Nguyen':\n";
    auto searchResults = customerRepo.search("Nguyen");
    for (const auto& customer : searchResults) {
        std::cout << "   - [" << customer.getId().toStdString() << "] " << customer.getName().toStdString() << "\n";
    }


    // =========================================================================
    // TEST CASE 2: KIỂM THỬ RECEPTIONIST REPOSITORY (CRUD)
    // =========================================================================
    printSeparator("RECEPTIONIST REPOSITORY (CRUD)");

    // Giả định hàm khởi tạo Receptionist(id, name, email)
    Receptionist r1("REC01", "Tran Thi Le Tan", "letan@hotel.com");

    // Test Add
    if (receptionistRepo.add(r1)) {
        std::cout << "✅ Them le tan REC01 thanh cong.\n";
    } else {
        std::cerr << "❌ Them le tan that bai: " << receptionistRepo.lastError().toStdString() << "\n";
    }

    // Test FindAll
    std::cout << "📋 Danh sach tat ca le tan hien co:\n";
    auto allReceptionists = receptionistRepo.findAll();
    for (const auto& rec : allReceptionists) {
        std::cout << "   - [" << rec.getId().toStdString() << "] " << rec.getName().toStdString() << "\n";
    }


    // =========================================================================
    // TEST CASE 3: KIỂM THỬ ROOM REPOSITORY (Smart Pointer & Enum)
    // =========================================================================
    printSeparator("ROOM REPOSITORY (UNIQUE_PTR & STATUS)");

    // Giả định hàm khởi tạo Room(id, roomNumber, type, status)
    StandardRoom room1("RM101", 100000, RoomStatus::Available, 2);
    int bedsCount = 2;

    // Test Add Room kèm tham số số giường (beds)
    if (roomRepo.add(room1, bedsCount)) {
        std::cout << "✅ Them phong 101 thanh cong.\n";
    } else {
        std::cerr << "❌ Them phong that bai: " << roomRepo.lastError().toStdString() << "\n";
    }

    // Test Update Status độc lập
    if (roomRepo.updateStatus("RM101", RoomStatus::InUse)) {
        std::cout << "✅ Da chuyen trang thai phong 101 sang [InUse].\n";
    } else {
        std::cerr << "❌ Cap nhat trang thai that bai: " << roomRepo.lastError().toStdString() << "\n";
    }

    // Test FindById xử lý với std::unique_ptr
    std::unique_ptr<Room> foundRoom = roomRepo.findById("RM101");
    if (foundRoom != nullptr) {
        std::cout << "🔍 Tim thay phong tu DB bang Unique_Ptr.\n";
    } else {
        std::cerr << "❌ Khong tim thay phong RM101!\n";
    }

    // Test FindAll xử lý danh sách std::vector<std::unique_ptr<Room>>
    std::cout << "📋 Danh sach tat ca cac phong trong he thong:\n";
    std::vector<std::unique_ptr<Room>> allRooms = roomRepo.findAll();
    for (const auto& r : allRooms) {
        // Vì r là unique_ptr, ta truy cập qua toán tử -> thông thường
        std::cout << "   - Phong ID: " << r->getRoomId().toStdString() 
                  << " | Trang thai: " << Room::statusToString(r->getStatus()).toStdString()
                  << " | Loai: " << Room::typeToString(r->getRoomType()).toStdString() << "\n";
    }


    // =========================================================================
    // TEST CASE 4: DỌN DẸP DỮ LIỆU (REMOVE TESTS)
    // =========================================================================
    printSeparator("DON DEP DU LIEU (REMOVE / DELETE)");

    // Xóa thử nghiệm các bản ghi vừa thêm để dọn rác database (nếu cần)
    // Bạn có thể comment các dòng dưới này lại nếu muốn dữ liệu giữ nguyên dưới SQLite để làm GUI
    if (customerRepo.remove("C001")) {
        std::cout << "🗑️ Da xoa khach hang C001 khoi he thong.\n";
    }
    
    if (roomRepo.remove("RM101")) {
        std::cout << "🗑️ Da xoa phong RM101 khoi he thong.\n";
    }

    std::cout << "\n🎉 === HOAN THANH TAT CA CAC TEST CASE KIEM TRA ===\n";

    // Đóng kết nối an toàn trước khi thoát chương trình
    DatabaseManager::getInstance().closeConnection();
    return 0;
}