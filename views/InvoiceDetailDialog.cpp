#include "InvoiceDetailDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QScrollArea>
#include <QLocale>
namespace {
QLabel* createSectionHeader(const QString& title, QWidget* parent) {
    auto* lbl = new QLabel(title, parent);
    lbl->setProperty("role", "sectionHeader");
    return lbl;
}

void addRow(QGridLayout* grid, int& row, const QString& labelText, const QString& valueText, bool highlight = false) {
    auto* keyLbl = new QLabel(labelText + ":");
    keyLbl->setProperty("role", "detailKey");

    auto* valLbl = new QLabel(valueText);
    valLbl->setProperty("role", highlight ? "detailTotal" : "detailValue");

    grid->addWidget(keyLbl, row, 0);
    grid->addWidget(valLbl, row, 1);
    row++;
}
}

InvoiceDetailDialog::InvoiceDetailDialog(const Invoice& invoice, QWidget* parent): QDialog(parent) {
    setObjectName("InvoiceDetailDialog");
    setWindowTitle("Invoice Details - " + invoice.getId());
    setMinimumWidth(520);
    setMinimumHeight(600);

    auto booking = bookingRepo.findById(invoice.getBookingId());
    std::optional<Customer> customer;
    std::unique_ptr<Room> room;
    if (booking.has_value()) {
        customer = customerRepo.findById(booking->getCustomerId());
        room = roomRepo.findById(booking->getRoomId());
    }
    auto receptionist = receptionistRepo.findById(invoice.getReceptionistId());

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(14);

    // Header
    auto* headerFrame = new QFrame(this);
    headerFrame->setProperty("cardStyle", true);
    auto* headerLayout = new QVBoxLayout(headerFrame);
    headerLayout->setContentsMargins(12, 10, 12, 10);
    headerLayout->setSpacing(4);

    auto* hotelLbl = new QLabel("🏨  Pink Home", headerFrame);
    hotelLbl->setProperty("role", "cardTitle");

    auto* subTitleLbl = new QLabel("Official Payment Receipt", headerFrame);
    subTitleLbl->setProperty("role", "cardSubtle");

    headerLayout->addWidget(hotelLbl, 0, Qt::AlignCenter);
    headerLayout->addWidget(subTitleLbl, 0, Qt::AlignCenter);
    mainLayout->addWidget(headerFrame);

    // Body
    auto* bodyFrame = new QFrame(this);
    bodyFrame->setProperty("cardStyle", true);
    auto* bodyLayout = new QVBoxLayout(bodyFrame);
    bodyLayout->setContentsMargins(20, 16, 20, 16);
    bodyLayout->setSpacing(10);

    auto* grid = new QGridLayout();
    grid->setHorizontalSpacing(24);
    grid->setVerticalSpacing(8);

    int row = 0;
    QLocale locale(QLocale::English);

    // 1. Invoice Info
    grid->addWidget(createSectionHeader("Invoice Information", bodyFrame), row, 0, 1, 2);
    row++;
    addRow(grid, row, "Invoice ID", invoice.getId());
    addRow(grid, row, "Issued Date", invoice.getIssuedDate().toString("yyyy-MM-dd"));
    addRow(grid, row, "Payment Method", Invoice::paymentMethodToString(invoice.getPaymentMethod()));

    // 2. Customer & Receptionist
    grid->addWidget(createSectionHeader("Customer & Receptionist", bodyFrame), row, 0, 1, 2);
    row++;
    addRow(grid, row, "Customer ID", booking.has_value() ? booking->getCustomerId() : "N/A");
    addRow(grid, row, "Customer Name", customer.has_value() ? customer->getName() : "N/A");
    addRow(grid, row, "Receptionist ID", invoice.getReceptionistId());
    addRow(grid, row, "Receptionist Name", receptionist.has_value() ? receptionist->getName() : "N/A");

    // 3. Booking & Room
    grid->addWidget(createSectionHeader("Booking & Room Information", bodyFrame), row, 0, 1, 2);
    row++;
    addRow(grid, row, "Booking ID", invoice.getBookingId());
    addRow(grid, row, "Room ID", booking.has_value() ? booking->getRoomId() : "N/A");
    addRow(grid, row, "Room Type", room != nullptr ? Room::typeToString(room->getRoomType()) : "N/A");
    addRow(grid, row, "Check-in Date", booking.has_value() ? booking->getCheckIn().toString("yyyy-MM-dd") : "N/A");
    addRow(grid, row, "Check-out Date", booking.has_value() ? booking->getCheckOut().toString("yyyy-MM-dd") : "N/A");

    // 4. Selected Services
    grid->addWidget(createSectionHeader("Selected Services", bodyFrame), row, 0, 1, 2);
    row++;
    addRow(grid, row, "Buffet Quantity", booking.has_value() ? QString::number(booking->getBuffetQuantity()) : "0");
    addRow(grid, row, "Laundry Service", booking.has_value() ? (booking->isUsingLaundry() ? "Yes" : "No") : "No");
    addRow(grid, row, "Decoration Service", booking.has_value() ? (booking->isUsingDecoration() ? "Yes" : "No") : "No");
    addRow(grid, row, "Decoration Note", (booking.has_value() && booking->isUsingDecoration() && !booking->getDecorationNote().isEmpty()) ? booking->getDecorationNote() : "None");

    // 5. Payment Details
    grid->addWidget(createSectionHeader("Payment Breakdown", bodyFrame), row, 0, 1, 2);
    row++;
    addRow(grid, row, "Subtotal", locale.toString(invoice.getSubtotalAmount(), 'f', 0));
    addRow(grid, row, "Discount Name", invoice.getDiscountName().isEmpty() ? "None" : invoice.getDiscountName());
    addRow(grid, row, "Discount Amount", locale.toString(invoice.getDiscountAmount(), 'f', 0));
    addRow(grid, row, "Total Amount", locale.toString(invoice.getTotalAmount(), 'f', 0), true);
    bodyLayout->addLayout(grid);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(bodyFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    mainLayout->addWidget(scrollArea, 1);

    // Footer
    auto* closeBtn = new QPushButton("Close", this);
    closeBtn->setProperty("variant", "primary");
    closeBtn->setCursor(Qt::PointingHandCursor);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    auto* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);

    mainLayout->addLayout(btnLayout);
}
