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

namespace { std::string text(QLineEdit* edit) { return edit->text().trimmed().toStdString(); } }

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
    auto* inBtn = new QPushButton("Check-in", this);
    auto* outBtn = new QPushButton("Check-out", this);
    auto* cancelBtn = new QPushButton("Cancel", this);
    auto* reloadBtn = new QPushButton("Refresh", this);
    actions->addWidget(addBtn); actions->addWidget(inBtn); actions->addWidget(outBtn); actions->addWidget(cancelBtn); actions->addWidget(reloadBtn);

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchEdit->setPlaceholderText("Search bookings");
    searching->addWidget(searchEdit); searching->addWidget(searchBtn);

    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"ID", "Customer", "Room", "Check-in", "Check-out", "Service", "Status"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form); layout->addLayout(actions); layout->addLayout(searching); layout->addWidget(table);
}