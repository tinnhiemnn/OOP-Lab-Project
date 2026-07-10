#include <QApplication>
#include "../views/MainWindow.h" // Hoặc file view chính của bạn

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w; // Đảm bảo class này tồn tại
    w.show();
    return a.exec();
}