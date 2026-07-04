#include "MainWindow.h"

#include "BookingView.h"
#include "CustomerView.h"
#include "RoomView.h"

#include <QGraphicsDropShadowEffect>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QTabWidget>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Hotel Management System");

    resize(1280, 800);
    setMinimumSize(1100, 700);   // để người dùng không thu nhỏ xuống mức bị vỡ layout

    auto* tabs = new QTabWidget(this);
    tabs->setObjectName("sidebarNav"); //dat object name cho thanh sidebar
    tabs->setTabPosition(QTabWidget::West); //day thanh chuyen tab sang trai

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 40));
    shadow->setOffset(0, 4);
    tabs->setGraphicsEffect(shadow);

    auto* customerView = new CustomerView(tabs);
    auto* roomView = new RoomView(tabs);
    auto* bookingView = new BookingView(tabs);

    tabs->addTab(customerView, "Customers");
    tabs->addTab(roomView, "Rooms");
    tabs->addTab(bookingView, "Bookings");

    setCentralWidget(tabs);
}