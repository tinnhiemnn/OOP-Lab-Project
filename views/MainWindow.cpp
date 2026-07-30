#include "MainWindow.h"

#include "BookingView.h"
#include "CustomerView.h"
#include "InvoiceView.h"
#include "ReportView.h"
#include "RoomView.h"
#include "ReceptionistView.h"

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

    // Bọc sidebar trong 1 widget cha để gắn thêm brand header phía trên
    auto* sidebarWrap = new QWidget(this);
    sidebarWrap->setObjectName("sidebarWrap");
    sidebarWrap->setFixedWidth(220);
    sidebarWrap->setAttribute(Qt::WA_StyledBackground, true);

    auto* sidebarLayout = new QVBoxLayout(sidebarWrap);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // Brand header: logo "LS" + tên app
    auto* brand = new QWidget(this);
    brand->setObjectName("brandBlock");
    auto* brandLayout = new QHBoxLayout(brand);
    brandLayout->setContentsMargins(18, 20, 12, 16);
    brandLayout->setSpacing(10);

    auto* brandMark = new QLabel("P", this);
    brandMark->setObjectName("brandMark");
    brandMark->setFixedSize(36, 36);
    brandMark->setAlignment(Qt::AlignCenter);
    brandLayout->addWidget(brandMark);

    auto* brandTextCol = new QVBoxLayout();
    brandTextCol->setSpacing(0);
    auto* brandName = new QLabel("Pink Home", this);
    brandName->setObjectName("brandName");
    auto* brandSub = new QLabel("Hotel Management", this);
    brandSub->setObjectName("brandSub");
    brandTextCol->addWidget(brandName);
    brandTextCol->addWidget(brandSub);
    brandLayout->addLayout(brandTextCol);
    brandLayout->addStretch();

    // Tạo thanh điều hướng
    auto* sidebarNav = new QListWidget(this);
    sidebarNav->setObjectName("sidebarNav");   // dat object name cho thanh sidebar
    sidebarNav->setFrameShape(QFrame::NoFrame);
    sidebarNav->setIconSize(QSize(18, 18));
    sidebarLayout->addWidget(brand);
    sidebarLayout->addWidget(sidebarNav, /*stretch=*/1);

    // Khởi tạo các trang giao diện con
    auto* bookingView = new BookingView(this);
    auto* customerView = new CustomerView(this);
    auto* receptionistView = new ReceptionistView(this);
    auto* roomView = new RoomView(this);
    auto* invoiceView = new InvoiceView(this);
    auto* reportView = new ReportView(this);

    auto* mainCol = new QWidget(this);
    auto* mainColLayout = new QVBoxLayout(mainCol);
    mainColLayout->setContentsMargins(0, 0, 0, 0);
    mainColLayout->setSpacing(0);

    // Boc topbar trong 1 wrapper de co padding dong bo voi le trai/phai cua noi dung ben duoi.
    auto* topbarWrap = new QWidget(this);
    topbarWrap->setObjectName("topbarWrap");
    topbarWrap->setAttribute(Qt::WA_StyledBackground, true);
    auto* topbarWrapLayout = new QVBoxLayout(topbarWrap);
    topbarWrapLayout->setContentsMargins(28, 20, 28, 12);
    topbarWrapLayout->setSpacing(0);

    auto* topbar = new QWidget(this);
    topbar->setObjectName("topbar");
    topbar->setAttribute(Qt::WA_StyledBackground, true);

    auto* topbarLayout = new QVBoxLayout(topbar);
    topbarLayout->setContentsMargins(0, 0, 0, 0);
    topbarLayout->setSpacing(0); // khoảng cách nhỏ giữa tiêu đề và dòng mô tả bên dưới

    m_pageTitle = new QLabel("Customers", this);   // trùng với setCurrentRow(0) bên dưới
    m_pageTitle->setProperty("role", "pageTitle");
    topbarLayout->addWidget(m_pageTitle);

    m_pageSubtitle = new QLabel("Manage customer information", this);
    m_pageSubtitle->setProperty("role", "pageSub");
    topbarLayout->addWidget(m_pageSubtitle);

    topbarWrapLayout->addWidget(topbar);


    auto* pages = new QStackedWidget(this);
    pages->addWidget(customerView);
    pages->addWidget(receptionistView);
    pages->addWidget(roomView);
    pages->addWidget(bookingView);
    pages->addWidget(invoiceView);
    pages->addWidget(reportView);

    mainColLayout->addWidget(topbarWrap);
    mainColLayout->addWidget(pages, /*stretch=*/1);

    QStringList menuTitles = {"Customers", "Receptionists", "Rooms", "Bookings", "Invoices", "Reports"};
    // Dong mo ta ngan duoi tieu de, tuong ung 1-1 voi menuTitles theo thu tu index.
    QStringList menuSubtitles = {
        "Manage customer information",
        "Manage reception staff",
        "Manage room inventory",
        "Track bookings workflow",
        "Manage billing & invoices",
        "Overview statistics and reports"
    };
    for (const QString& title : menuTitles) {
        auto* item = new QListWidgetItem(title);
        item->setTextAlignment(Qt::AlignCenter); // Tất cả các tab đều được căn giữa đồng đều
        sidebarNav->addItem(item);
    }

    sidebarNav->setCurrentRow(0);      // "Bookings" active mặc định
    pages->setCurrentIndex(0);

    connect(sidebarNav, &QListWidget::currentRowChanged, this, [this, sidebarNav, pages, menuSubtitles](int row) {
        pages->setCurrentIndex(row);
        m_pageTitle->setText(sidebarNav->item(row)->text());
        m_pageSubtitle->setText(menuSubtitles.value(row));
    });

    layout->addWidget(sidebarWrap);
    layout->addWidget(mainCol, /*stretch=*/1);
    setCentralWidget(central);
}