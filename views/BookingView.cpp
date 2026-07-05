#include "BookingView.h"

#include <QDate>
#include <QDateEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QString>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }

    void applyCardShadow(QWidget *card)
    {
        auto *shadow = new QGraphicsDropShadowEffect(card);
        shadow->setBlurRadius(24);
        shadow->setOffset(0, 4);
        shadow->setColor(QColor(0, 0, 0, 90));
        card->setGraphicsEffect(shadow);
    }
}

BookingView::BookingView(QWidget* parent)
    : QWidget(parent), bookingIdEdit(new QLineEdit(this)), customerIdEdit(new QLineEdit(this)), roomIdEdit(new QLineEdit(this)),
      searchEdit(new QLineEdit(this)), checkInEdit(new QDateEdit(QDate::currentDate(), this)),
      checkOutEdit(new QDateEdit(QDate::currentDate().addDays(1), this)), table(new QTableWidget(this)) {
    checkInEdit->setCalendarPopup(true);
    checkOutEdit->setCalendarPopup(true);
    checkInEdit->setDisplayFormat("yyyy-MM-dd");
    checkOutEdit->setDisplayFormat("yyyy-MM-dd");

    auto* form = new QFormLayout;
    form->addRow("Booking ID", bookingIdEdit);
    form->addRow("Customer ID", customerIdEdit);
    form->addRow("Room ID", roomIdEdit);
    form->addRow("Check-in", checkInEdit);
    form->addRow("Check-out", checkOutEdit);

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Book", this);
    addBtn->setObjectName("btnBook");

    auto* inBtn = new QPushButton("Check-in", this);
    inBtn->setObjectName("btnCheckin");

    auto* outBtn = new QPushButton("Check-out", this);
    outBtn->setObjectName("btnCheckout");

    auto* cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setObjectName("btnCancel");

    auto* reloadBtn = new QPushButton("Refresh", this);
    reloadBtn->setObjectName("btnRefresh");

    actions->addWidget(addBtn, 1);
    actions->addWidget(inBtn, 1);
    actions->addWidget(outBtn, 1);
    actions->addWidget(cancelBtn, 1);
    actions->addStretch();          // đẩy Refresh sát bên phải, có khoảng trống trước nó
    actions->addWidget(reloadBtn);  // không có stretch, giữ kích thước cố định

    // --- Card 1: form đặt phòng + các nút hành động ---
    formCard = new QFrame(this);
    auto* formCardLayout = new QVBoxLayout(formCard);
    formCardLayout->addLayout(form);
    formCardLayout->addLayout(actions);

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchBtn->setObjectName("btnSearch");
    searchEdit->setPlaceholderText("Search bookings");
    searching->addWidget(searchEdit); searching->addWidget(searchBtn);

    table->setObjectName("tableBookings");
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"ID", "Customer", "Room", "Check-in", "Check-out", "Service", "Status"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // --- Card 2: ô tìm kiếm + bảng ---
    tableCard = new QFrame(this);
    auto* tableCardLayout = new QVBoxLayout(tableCard);
    tableCardLayout->addLayout(searching);
    tableCardLayout->addWidget(table);

    auto* layout = new QVBoxLayout(this);

    layout->addWidget(formCard);
    layout->addWidget(tableCard);

    applyCardShadow(formCard);
    applyCardShadow(tableCard);
}

QLabel* BookingView::createStatusBadge(const QString &status)
{
    auto *label = new QLabel(status, this);
    label->setAlignment(Qt::AlignCenter);

    QString key = status.toLower().remove('-');   // "Checked-in" -> "checkedin"
    label->setProperty("status", key);

    // Bắt buộc — nếu thiếu 2 dòng này, QSS sẽ không nhận property mới:
    label->style()->unpolish(label);
    label->style()->polish(label);

    return label;
}
