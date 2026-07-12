#include "DashboardCard.h"
#include "ReportView.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

ReportView::ReportView(QWidget* parent)
    : QWidget(parent), revenueLabel(new QLabel(this)), bookingLabel(new QLabel(this)), roomLabel(new QLabel(this)),
    occupancyLabel(new QLabel(this)), table(new QTableWidget(this)){

    // --- Hero card: doanh thu + occupancy ---
    auto* heroRow = new QHBoxLayout;

    auto* revenueBox = new QFrame(this);
    revenueBox->setProperty("heroCard", true);
    auto* revenueLayout = new QVBoxLayout(revenueBox);
    auto* revenueTitle = new QLabel("Tổng doanh thu", this);
    revenueTitle->setProperty("role", "heroLabel");
    revenueLabel->setProperty("role", "heroValue");
    revenueLayout->addWidget(revenueTitle);
    revenueLayout->addWidget(revenueLabel);

    auto* occupancyBox = new QFrame(this);
    occupancyBox->setProperty("heroCard", true);
    auto* occupancyLayout = new QVBoxLayout(occupancyBox);
    auto* occupancyTitle = new QLabel("Tỉ lệ lấp đầy phòng", this);
    occupancyTitle->setProperty("role", "heroLabel");
    occupancyLabel->setProperty("role", "heroValue");
    occupancyLayout->addWidget(occupancyTitle);
    occupancyLayout->addWidget(occupancyLabel);

    heroRow->addWidget(revenueBox);
    heroRow->addWidget(occupancyBox);

    heroCard = new DashboardCard(QString(), QString(), this); // không cần tiêu đề riêng, hero card tự có tiêu đề bên trong
    heroCard->addContentLayout(heroRow);

    // --- Bảng số liệu phụ: booking / room / hoá đơn ---
    auto* summary = new QGridLayout;
    summary->addWidget(new QLabel("Bookings", this), 0, 0); summary->addWidget(bookingLabel, 0, 1);
    summary->addWidget(new QLabel("Rooms", this), 1, 0); summary->addWidget(roomLabel, 1, 1);

    auto* reloadBtn = new QPushButton("Refresh Report", this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Invoice", "Booking", "Amount", "Payment"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new DashboardCard("Danh sách hoá đơn", "purple", this);
    tableCard->addContentLayout(summary);
    tableCard->addContent(reloadBtn);
    tableCard->addContent(table);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->addWidget(heroCard);
    layout->addWidget(tableCard);
}
