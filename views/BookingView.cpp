#include "BookingView.h"

#include <QDate>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox> // chưa dùng đến
#include <QPushButton>
#include <QTableWidget>
#include <QString>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }
}

BookingView::BookingView(QWidget* parent)
    : QWidget(parent), bookingIdEdit(new QLineEdit(this)), customerIdEdit(new QLineEdit(this)), roomIdEdit(new QLineEdit(this)),
      searchEdit(new QLineEdit(this)), checkInEdit(new QDateEdit(QDate::currentDate(), this)),
      checkOutEdit(new QDateEdit(QDate::currentDate().addDays(1), this)), servicesEdit(new QComboBox(this)){
    checkInEdit->setCalendarPopup(true);
    checkOutEdit->setCalendarPopup(true);
    checkInEdit->setDisplayFormat("yyyy-MM-dd");
    checkOutEdit->setDisplayFormat("yyyy-MM-dd");
    servicesEdit->addItems({"None", "Bike Rental", "Decoration service", "Laundry service", "Express Room Cleaning", "Extra Bed"});

    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10 );
    form->addWidget(new QLabel("Booking ID", this), 0, 0);
    form->addWidget(bookingIdEdit, 0, 1);
    form->addWidget(new QLabel("Customer ID", this), 1, 0);
    form->addWidget(customerIdEdit, 1, 1);
    form->addWidget(new QLabel("Room ID", this), 2, 0);
    form->addWidget(roomIdEdit, 2, 1);
    form->addWidget(new QLabel("Check-in", this), 0, 2);
    form->addWidget(checkInEdit, 0, 3);
    form->addWidget(new QLabel("Check-out", this), 1, 2);
    form->addWidget(checkOutEdit, 1, 3);
    form->addWidget(new QLabel("Services", this), 2, 2);
    form->addWidget(servicesEdit, 2, 3);
    form->setColumnStretch(1, 1);
    form->setColumnStretch(3, 1);

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Book", this);
    addBtn->setProperty("variant", "primary");

    auto* inBtn = new QPushButton("Check-in", this);
    inBtn->setProperty("variant","ghost");

    auto* outBtn = new QPushButton("Check-out", this);
    outBtn->setProperty("variant", "ghost");

    auto* cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setProperty("variant", "danger");

    auto* reloadBtn = new QPushButton("Refresh", this);
    reloadBtn->setProperty("variant", "ghost");

    actions->addWidget(addBtn);
    actions->addWidget(inBtn);
    actions->addWidget(outBtn);
    actions->addWidget(cancelBtn);
    actions->addWidget(reloadBtn);

    // --- Card 1: form đặt phòng + các nút hành động ---
    formCard = new DashboardCard("Booking", "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(actions);

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchBtn->setProperty("variant", "primary");

    searchEdit->setPlaceholderText("Search bookings");
    searching->addWidget(searchEdit);
    searching->addWidget(searchBtn);


    // --- Kanban board: 4 cột trạng thái ---
    auto buildColumn = [this](const QString& title, QVBoxLayout*& colRef) {
        auto* colFrame = new QFrame(this);
        colFrame->setObjectName("kanbanColumn");
        auto* outer = new QVBoxLayout(colFrame);

        auto* colTitle = new QLabel(title, this);
        colTitle->setProperty("role", "columnTitle");
        outer->addWidget(colTitle);

        colRef = new QVBoxLayout();
        colRef->setSpacing(10);
        colRef->addStretch(); // giữ thẻ dồn lên trên
        outer->addLayout(colRef);

        return colFrame;
    };

    auto* kanbanRow = new QHBoxLayout();
    kanbanRow->addWidget(buildColumn("Booked", colBooked));
    kanbanRow->addWidget(buildColumn("Checked-in", colCheckedIn));
    kanbanRow->addWidget(buildColumn("Checked-out", colCheckedOut));
    kanbanRow->addWidget(buildColumn("Cancelled", colCancelled));

    // --- Card 2: ô tìm kiếm + kanban board ---
    tableCard = new DashboardCard("Booking List", "purple", this);
    tableCard->addContentLayout(searching);
    tableCard->addContentLayout(kanbanRow);

    // --- Layout tổng thể card 1 + card 2 ---
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addWidget(formCard);
    layout->addWidget(tableCard, /*stretch=*/1);
}

QFrame* BookingView::createBookingCard(const QString& id, const QString& customerId,
                                       const QString& roomId, const QString& checkIn,
                                       const QString& checkOut, const QString& status) {
    auto* card = new QFrame(this);
    card->setObjectName("bookingCard");

    auto* layout = new QVBoxLayout(card);
    layout->setSpacing(4);

    auto* idLbl = new QLabel("#" + id, this);
    idLbl->setProperty("role", "cardId");

    auto* customerLbl = new QLabel("Khách: " + customerId, this);
    auto* roomLbl = new QLabel("Phòng: " + roomId, this);
    auto* dateLbl = new QLabel(checkIn + " → " + checkOut, this);

    auto* statusLbl = new QLabel(status, this);
    QString key = status.toLower().remove('-');
    statusLbl->setProperty("status", key);
    statusLbl->style()->unpolish(statusLbl);
    statusLbl->style()->polish(statusLbl);

    layout->addWidget(idLbl);
    layout->addWidget(customerLbl);
    layout->addWidget(roomLbl);
    layout->addWidget(dateLbl);
    layout->addWidget(statusLbl);

    return card;
}

void BookingView::clearColumn(QVBoxLayout* col) {
    // Xoá hết widget cũ trong cột, chừa lại addStretch() ở cuối
    while (col->count() > 1) {
        QLayoutItem* item = col->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }
}