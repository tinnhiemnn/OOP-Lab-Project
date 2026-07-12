#include "MainWindow.h"

#include "BookingView.h"
#include "CustomerView.h"
#include "RoomView.h"

#include <QGraphicsDropShadowEffect>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

    // Cấu hình cửa số chính
    setWindowTitle("Hotel Management System");
    resize(1280, 720);
    setMinimumSize(1100, 700);   // để người dùng không thu nhỏ xuống mức bị vỡ layout

    // Thiết lập layout tổng thể
    auto *central = new QWidget(this);
    auto *layout  = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // ------------------------ 11/07/2026 ------------------------------

    // Bọc sidebar trong 1 widget cha để gắn thêm brand header phía trên
    auto* sidebarWrap = new QWidget(this);
    sidebarWrap->setObjectName("sidebarWrap");
    sidebarWrap->setFixedWidth(220);

    auto* sidebarLayout = new QVBoxLayout(sidebarWrap);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // Brand header: logo "LS" + tên app
    auto* brand = new QWidget(this);
    brand->setObjectName("brandBlock");
    auto* brandLayout = new QHBoxLayout(brand);
    brandLayout->setContentsMargins(18, 20, 12, 16);
    brandLayout->setSpacing(10);

    auto* brandMark = new QLabel("H", this);
    brandMark->setObjectName("brandMark");
    brandMark->setFixedSize(36, 36);
    brandMark->setAlignment(Qt::AlignCenter);
    brandLayout->addWidget(brandMark);

    auto* brandTextCol = new QVBoxLayout();
    brandTextCol->setSpacing(0);
    auto* brandName = new QLabel("HomeStay", this);
    brandName->setObjectName("brandName");
    auto* brandSub = new QLabel("Hotel Management", this);
    brandSub->setObjectName("brandSub");
    brandTextCol->addWidget(brandName);
    brandTextCol->addWidget(brandSub);
    brandLayout->addLayout(brandTextCol);
    brandLayout->addStretch();

    // ------------------------ 11/07/2026 ------------------------------

    // Tạo thanh điều hướng
    auto* sidebarNav = new QListWidget(this);
    sidebarNav->setObjectName("sidebarNav");   // dat object name cho thanh sidebar
    sidebarNav->setFrameShape(QFrame::NoFrame);
    sidebarNav->setIconSize(QSize(18, 18));
    sidebarLayout->addWidget(brand);
    sidebarLayout->addWidget(sidebarNav, /*stretch=*/1);

    // Tạo hiệu ứng đổ bóng cho thanh sidebar
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 40));
    shadow->setOffset(0, 4);
    sidebarWrap->setGraphicsEffect(shadow);

    // Khởi tạo các trang giao diện con
    auto* customerView = new CustomerView(this);
    auto* roomView = new RoomView(this);
    auto* bookingView = new BookingView(this);


    auto* mainCol = new QWidget(this);
    auto* mainColLayout = new QVBoxLayout(mainCol);
    mainColLayout->setContentsMargins(0, 0, 0, 0);
    mainColLayout->setSpacing(0);

    auto* topbar = new QWidget(this);
    topbar->setObjectName("topbar");
    topbar->setFixedHeight(64);
    auto* topbarLayout = new QVBoxLayout(topbar);
    topbarLayout->setContentsMargins(28, 0, 28, 0);

    m_pageTitle = new QLabel("Bookings", this);   // trùng với setCurrentRow(2) bên dưới
    m_pageTitle->setProperty("role", "pageTitle");
    topbarLayout->addWidget(m_pageTitle);
    topbarLayout->setAlignment(m_pageTitle, Qt::AlignVCenter);


    auto* pages = new QStackedWidget(this);
    pages->addWidget(customerView);
    pages->addWidget(roomView);
    pages->addWidget(bookingView);

    mainColLayout->addWidget(topbar);
    mainColLayout->addWidget(pages, /*stretch=*/1);

    QStringList menuTitles = {"Customers", "Rooms", "Bookings"};
    for (const QString& title : menuTitles) {
        auto* item = new QListWidgetItem(title);
        item->setTextAlignment(Qt::AlignCenter); // Tất cả các tab đều được căn giữa đồng đều
        sidebarNav->addItem(item);
    }

    sidebarNav->setCurrentRow(2);      // "Bookings" active mặc định
    pages->setCurrentIndex(2);

    connect(sidebarNav, &QListWidget::currentRowChanged, this, [this, sidebarNav, pages](int row) {
        pages->setCurrentIndex(row);
        m_pageTitle->setText(sidebarNav->item(row)->text());
    });

    layout->addWidget(sidebarWrap);
    layout->addWidget(mainCol, /*stretch=*/1);
    setCentralWidget(central);
}