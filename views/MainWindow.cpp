#include "MainWindow.h"

#include "BookingView.h"
#include "CustomerView.h"
#include "RoomView.h"

#include <QGraphicsDropShadowEffect>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Hotel Management System");
    resize(1280, 800);
    setMinimumSize(1100, 700);   // để người dùng không thu nhỏ xuống mức bị vỡ layout

    auto *central = new QWidget(this);
    auto *layout  = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* sidebarNav = new QListWidget(this);
    sidebarNav->setObjectName("sidebarNav");   // dat object name cho thanh sidebar
    sidebarNav->setFixedWidth(240);
    sidebarNav->setFrameShape(QFrame::NoFrame);
    sidebarNav->setIconSize(QSize(18, 18));

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 40));
    shadow->setOffset(0, 4);
    sidebarNav->setGraphicsEffect(shadow);

    auto* customerView = new CustomerView(this);
    auto* roomView = new RoomView(this);
    auto* bookingView = new BookingView(this);

    auto* pages = new QStackedWidget(this);
    pages->addWidget(customerView);
    pages->addWidget(roomView);
    pages->addWidget(bookingView);

    QListWidgetItem *itemCustomers = new QListWidgetItem("Customers");
    itemCustomers->setTextAlignment(Qt::AlignCenter);
    sidebarNav->addItem(itemCustomers);

    QListWidgetItem *itemRooms = new QListWidgetItem("Rooms");
    itemRooms->setTextAlignment(Qt::AlignCenter);
    sidebarNav->addItem(itemRooms);

    QListWidgetItem *itemBookings = new QListWidgetItem("Bookings");
    itemBookings->setTextAlignment(Qt::AlignCenter);
    sidebarNav->addItem(itemBookings);

    sidebarNav->setCurrentRow(2);      // "Bookings" active mặc định
    pages->setCurrentIndex(2);

    connect(sidebarNav, &QListWidget::currentRowChanged,
            pages, &QStackedWidget::setCurrentIndex);

    layout->addWidget(sidebarNav);
    layout->addWidget(pages, /*stretch=*/1);
    setCentralWidget(central);
}