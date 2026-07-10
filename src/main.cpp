#include <QApplication>
#include <QFile>
#include <iostream>
#include "database/DatabaseManager.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    // 1. Khởi tạo ứng dụng GUI với QApplication (bắt buộc đối với ứng dụng có giao diện Qt)
    QApplication a(argc, argv);

    // Tải và áp dụng stylesheet (file style.qss đã được nhúng trong resources)
    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        a.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    // 2. Khởi tạo kết nối database SQLite
    if (!DatabaseManager::getInstance().openDatabase()) {
        std::cerr << "🚨 Loi khoi tao CSDL! Dung chuong trinh.\n";
        return -1;
    }

    // 3. Khởi tạo và hiển thị cửa sổ giao diện chính (MainWindow)
    MainWindow w;
    w.show();

    // 4. Chạy vòng lặp sự kiện (event loop) của Qt
    int result = a.exec();

    // 5. Đóng kết nối database an toàn trước khi thoát chương trình
    DatabaseManager::getInstance().closeConnection();

    return result;
}