#include <QCoreApplication>
#include <iostream>
#include <memory>
#include <vector>
#include <optional>

// Include Database Manager và các Controllers mới của nhóm bạn
#include "database/DatabaseManager.h"
#include "controllers/CustomerController.h"
#include "controllers/ReceptionistController.h"
#include "controllers/RoomController.h"
#include "models/StandardRoom.h"



void printSeparator(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << " TEST CONTROLLER: " << title << "\n";
    std::cout << "========================================\n";
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);


    printSeparator("KET NOI DATABASE");
    if (!DatabaseManager::getInstance().openDatabase()) {
        std::cerr << "🚨 Huỷ bỏ test do loi khoi tao CSDL.\n";
        return -1;
    }

    // 2. Khởi tạo các Controller độc lập
    CustomerController customerController;
    ReceptionistController receptionistController;
    RoomController roomController;

    // Biến dùng chung để hứng lỗi từ các hàm của Controller
    QString errorMsg; 

    // =========================================================================
    // TEST CASE 1: CUSTOMER CONTROLLER
    // =========================================================================
    printSeparator("CUSTOMER CONTROLLER");

    Customer c2("C002", "Tran Van B", "b.tran@gmail.com", "0911223344");
    
    // Test Thêm Khách Hàng qua Controller
    if (customerController.addCustomer(c2, errorMsg)) {
        std::cout << "✅ [Controller] Them khach hang C002 thanh cong.\n";
    } else {
        std::cerr << "❌ [Controller] Them that bai. Loi: " << errorMsg.toStdString() << "\n";
    }

    // Test lấy danh sách khách hàng
    std::cout << "📋 Danh sach khach hang tu Controller:\n";
    auto customers = customerController.listCustomers();
    for (const auto& c : customers) {
        std::cout << "   - " << c.getId().toStdString() << " | " << c.getName().toStdString() << "\n";
    }

    // Test trường hợp lỗi (Ví dụ thêm trùng ID để xem Controller bắt lỗi thế nào)
    printSeparator("CUSTOMER CONTROLLER - TEST BAT LOI");
    if (!customerController.addCustomer(c2, errorMsg)) {
        std::cout << "🎯 Thử nghiem bat loi trung ID thanh cong! Thong bao loi nhan duoc:\n";
        std::cout << "   👉 \"" << errorMsg.toStdString() << "\"\n";
    }


    // =========================================================================
    // TEST CASE 2: RECEPTIONIST CONTROLLER
    // =========================================================================
    printSeparator("RECEPTIONIST CONTROLLER");

    Receptionist r2("REC02", "Nguyen Van Le Tan", "letan2@hotel.com");

    // Test Add
    if (receptionistController.addReceptionist(r2, errorMsg)) {
        std::cout << "✅ [Controller] Them le tan REC02 thanh cong.\n";
    } else {
        std::cerr << "❌ [Controller] Them le tan that bai: " << errorMsg.toStdString() << "\n";
    }

    // Test GetById
    auto optRec = receptionistController.getReceptionistById("REC02");
    if (optRec.has_value()) {
        std::cout << "🔍 [Controller] GetById tim thay: " << optRec->getName().toStdString() << "\n";
    }


    // =========================================================================
    // TEST CASE 3: ROOM CONTROLLER (Xử lý Unique_Ptr và Enum)
    // =========================================================================
    printSeparator("ROOM CONTROLLER");

    StandardRoom rm202("RM202", 100000, RoomStatus::Available, 2);
    int numBeds = 2;

    // Test Add Room
    if (roomController.addRoom(rm202, numBeds, errorMsg)) {
        std::cout << "✅ [Controller] Them phong 202 thanh cong.\n";
    } else {
        std::cerr << "❌ [Controller] Them phong that bai: " << errorMsg.toStdString() << "\n";
    }

    // Test Cập nhật trạng thái phòng qua Controller
    if (roomController.updateRoomStatus("RM202", RoomStatus::Maintenance, errorMsg)) {
        std::cout << "✅ [Controller] Da chuyen trang thai phong 202 sang [Maintenance].\n";
    } else {
        std::cerr << "❌ [Controller] Cap nhat trang thai that bai: " << errorMsg.toStdString() << "\n";
    }

    // Test hiển thị danh sách dạng unique_ptr từ Controller
    std::cout << "📋 Danh sach tat ca cac phong hien tai:\n";
    auto allRooms = roomController.getAllRooms();
    for (const auto& r : allRooms) {
        std::cout << "   - Phong: " << r->getRoomId().toStdString() 
                  << " | Loai: " << Room::typeToString(r->getRoomType()).toStdString() << "\n";
    }


    // =========================================================================
    // TEST CASE 4: XÓA DỮ LIỆU QUA CONTROLLER
    // =========================================================================
    printSeparator("DON DEP DU LIEU QUA CONTROLLER");

    // Thực hiện xoá thông qua hàm delete của các Controller
    // Lưu ý: Bạn có thể comment các dòng này lại nếu muốn giữ data trong file .db để làm GUI
    if (customerController.deleteCustomer("C002", errorMsg)) {
        std::cout << "🗑️ [Controller] Da xoa khach hang C002.\n";
    }
    else std::cout<<errorMsg.toStdString()<<'\n';
    
    if (roomController.deleteRoom("RM202", errorMsg)) {
        std::cout << "🗑️ [Controller] Da xoa phong RM202.\n";
    }
    else std::cout<<errorMsg.toStdString()<<'\n';

    std::cout << "\n🎉 === HOAN THANH KIEM THU TANG CONTROLLER ===\n";
    
    // Đóng kết nối an toàn
    DatabaseManager::getInstance().closeConnection();
    return 0;
}