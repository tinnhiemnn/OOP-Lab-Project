#include "MainWindow.h"

#include "BookingView.h"
#include "CustomerView.h"
#include "RoomView.h"

#include <QTabWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Hotel Management System");
    resize(1200, 720);
    auto* tabs = new QTabWidget(this);
    tabs->addTab(new CustomerView(tabs), "Customers");
    tabs->addTab(new RoomView(tabs), "Rooms");
    tabs->addTab(new BookingView(tabs), "Bookings");
    setCentralWidget(tabs);
}

