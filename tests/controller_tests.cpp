#include "controllers/CustomerController.h"
#include "database/DatabaseManager.h"
#include <QCoreApplication>
#include <iostream>
#include <cassert>

void testControllerValidationBypass() {
    auto& dbMgr = DatabaseManager::getInstance();
    assert(dbMgr.openDatabase("test_hotel.db") == true);
    
    CustomerController ctrl;
    QString error;
    
    // Khởi tạo khách hàng với email lỗi và số điện thoại chứa chữ cái
    Customer badCust("CBAD", "Bad Name", "not-an-email", "abc1234");
    
    // Thử gửi lệnh thêm mới thông qua Controller
    bool result = ctrl.addCustomer(badCust, error);
    
    // Nếu bug chưa được sửa, hàm trả về true (ghi sổ rác thành công). 
    // Unit test lúc này sẽ assert fail vì bản chất ta cần result == false
    assert(result == false);
    
    // Làm sạch
    ctrl.deleteCustomer("CBAD", error);
    std::cout << "Controller Validation Bypass test passed!" << std::endl;
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    
    testControllerValidationBypass();
    
    std::cout << "All controller tests passed!" << std::endl;
    return 0;
}
