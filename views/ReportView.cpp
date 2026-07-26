#include "DashboardCard.h"
#include "ReportView.h"
#include "models/Invoice.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

ReportView::ReportView(QWidget* parent)
    : QWidget(parent),
      controller(),
      bookingController(),
      roomController(),
      invoiceController(),
      revenueLabel(new QLabel(this)),
      bookingLabel(new QLabel(this)),
      roomLabel(new QLabel(this)),
      occupancyLabel(new QLabel(this)),
      table(new QTableWidget(this)) {

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
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addWidget(heroCard);
    layout->addWidget(tableCard);

    connect(reloadBtn, &QPushButton::clicked, this, [this] { reload(); });
    reload();
}

void ReportView::reload() {
    OverallReport report = controller.getOverallReport();
    revenueLabel->setText(report.totalRevenue);
    occupancyLabel->setText(report.roomOccupancyRate);

    int bookingCount = static_cast<int>(bookingController.getAllBookings().size());
    bookingLabel->setText(QString::number(bookingCount));

    int roomCount = static_cast<int>(roomController.getAllRooms().size());
    roomLabel->setText(QString::number(roomCount));

    auto invoices = invoiceController.handleGetAllInvoices();
    table->setRowCount(static_cast<int>(invoices.size()));
    for (int row = 0; row < static_cast<int>(invoices.size()); ++row) {
        const auto& i = invoices[static_cast<size_t>(row)];
        table->setItem(row, 0, new QTableWidgetItem(i.getId()));
        table->setItem(row, 1, new QTableWidgetItem(i.getBookingId()));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(i.getTotalAmount(), 'f', 0) + " VND"));
        table->setItem(row, 3, new QTableWidgetItem(Invoice::paymentMethodToString(i.getPaymentMethod())));
    }
}