#include "DashboardCard.h"
#include "RoomView.h"

#include "models/Room.h"
#include "patterns/RoomFactory.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QString>
#include <QGraphicsDropShadowEffect>

namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }
    QString current(QComboBox* box) { return box->currentText(); }
}

RoomView::RoomView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), typeEdit(new QComboBox(this)), priceEdit(new QDoubleSpinBox(this)),
      statusEdit(new QComboBox(this)), bedsEdit(new QSpinBox(this)),
      filterType(new QComboBox(this)), filterStatus(new QComboBox(this)),
      checkInEdit(new QDateEdit(QDate::currentDate(), this)),
      checkOutEdit(new QDateEdit(QDate::currentDate().addDays(1), this)),
      table(new QTableWidget(this)) {

    idEdit->setPlaceholderText("Room ID");

    typeEdit->addItems({"Standard", "Deluxe", "President"});
    statusEdit->addItems({"Available", "Maintenance", "InUse", "NeedCleaning"});
    filterType->addItems({"All", "Standard", "Deluxe", "President"});
    filterStatus->addItems({"All", "Available", "Maintenance", "InUse", "NeedCleaning"});
    priceEdit->setRange(1, 100000000);
    priceEdit->setDecimals(0);
    bedsEdit->setRange(1, 6);

    checkInEdit->setCalendarPopup(true);
    checkOutEdit->setCalendarPopup(true);
    checkInEdit->setDisplayFormat("yyyy-MM-dd");
    checkOutEdit->setDisplayFormat("yyyy-MM-dd");

    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10);
    form->addWidget(new QLabel("Room ID", this), 0, 0);
    form->addWidget(idEdit, 0, 1);
    form->addWidget(new QLabel("Type", this), 0, 2);
    form->addWidget(typeEdit, 0, 3);
    form->addWidget(new QLabel("Price", this), 1, 0);
    form->addWidget(priceEdit, 1, 1);
    form->addWidget(new QLabel("Status", this), 1, 2);
    form->addWidget(statusEdit, 1, 3);
    form->addWidget(new QLabel("Beds", this), 2, 0);
    form->addWidget(bedsEdit, 2, 1);
    form->setColumnStretch(1, 1);
    form->setColumnStretch(3, 1);

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Add", this);
    addBtn->setProperty("variant", "primary");

    auto* updateBtn = new QPushButton("Update", this);
    updateBtn->setProperty("variant", "ghost");

    auto* deleteBtn = new QPushButton("Delete", this);
    deleteBtn->setProperty("variant", "danger");

    auto* reloadBtn = new QPushButton("Refresh", this);
    reloadBtn->setProperty("variant", "ghost");

    actions->addWidget(addBtn);
    actions->addWidget(updateBtn);
    actions->addWidget(deleteBtn);
    actions->addWidget(reloadBtn);

    formCard = new DashboardCard(QString(), "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(actions);

    // --- Gop Filter Type/Status + Check Available vao CHUNG 1 hang cho gon ---
    // (truoc day day la 2 hang rieng biet, chiem gan gap doi chieu cao)
    auto* controls = new QHBoxLayout;
    controls->setSpacing(10);

    filterType->setMaximumWidth(150);
    filterStatus->setMaximumWidth(150);
    auto* filterBtn = new QPushButton("Filter", this);
    filterBtn->setProperty("variant", "secondary");

    checkInEdit->setMaximumWidth(120);
    checkOutEdit->setMaximumWidth(120);
    auto* checkAvailBtn = new QPushButton("Check Available", this);
    checkAvailBtn->setProperty("variant", "secondary");

    controls->addWidget(filterType);
    controls->addWidget(filterStatus);
    controls->addWidget(filterBtn);
    controls->addStretch(1);
    controls->addWidget(new QLabel("Check-in", this));
    controls->addWidget(checkInEdit);
    controls->addWidget(new QLabel("Check-out", this));
    controls->addWidget(checkOutEdit);
    controls->addWidget(checkAvailBtn);

    // --- CHANGED: Room List tu grid 4 cot sang table ---
    // Da bo sung cot "Beds" (nam giua Price va Status).
    // YEU CAU: Room.h can co "virtual int getBeds() const" de refresh() lay du lieu.
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"Room ID", "Type", "Price", "Beds", "Status"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new DashboardCard(QString(), "purple", this);
    tableCard->addContentLayout(controls);
    tableCard->addContent(table);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addWidget(formCard);
    layout->addWidget(tableCard, /*stretch=*/1);

    connect(addBtn, &QPushButton::clicked, this, [this] { add(); });
    connect(updateBtn, &QPushButton::clicked, this, [this] { update(); });
    connect(deleteBtn, &QPushButton::clicked, this, [this] { remove(); });
    connect(reloadBtn, &QPushButton::clicked, this, [this] { reload(); });
    connect(filterBtn, &QPushButton::clicked, this, [this] { search(); });
    connect(checkAvailBtn, &QPushButton::clicked, this, [this] { checkAvailability(); });
    connect(table, &QTableWidget::itemSelectionChanged, this, [this] { selected(); });
    reload();
}

void RoomView::refresh(std::vector<std::unique_ptr<Room>> rows) {
    table->setRowCount(static_cast<int>(rows.size()));
    for (int row = 0; row < static_cast<int>(rows.size()); ++row) {
        const auto& room = rows[static_cast<size_t>(row)];
        table->setItem(row, 0, new QTableWidgetItem(room->getRoomId()));
        table->setItem(row, 1, new QTableWidgetItem(Room::typeToString(room->getRoomType())));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(room->getBasePrice(), 'f', 0)));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(room->getBeds())));
        table->setItem(row, 4, new QTableWidgetItem(Room::statusToString(room->getStatus())));
    }
}

void RoomView::reload() {
    refresh(controller.getAllRooms());
    idEdit->setReadOnly(false);
    idEdit->clear();
}

void RoomView::selected() {
    const int row = table->currentRow();
    if (row < 0) return;
    idEdit->setText(table->item(row, 0)->text());
    typeEdit->setCurrentText(table->item(row, 1)->text());
    priceEdit->setValue(table->item(row, 2)->text().toDouble());
    bedsEdit->setValue(table->item(row, 3)->text().toInt());
    statusEdit->setCurrentText(table->item(row, 4)->text());
    idEdit->setReadOnly(true);
}

void RoomView::add() {
    QString e;
    auto room = RoomFactory::createRoom(Room::typeFromString(current(typeEdit)),
                                         text(idEdit),
                                         priceEdit->value(),
                                         Room::statusFromString(current(statusEdit)),
                                         bedsEdit->value());
    if (!room) {
        error("Unable to build room object for the selected type.");
        return;
    }

    if (!controller.addRoom(*room, e)) {
        error(e);
    } else {
        reload();
    }
}

void RoomView::update() {
    QString e;
    auto room = RoomFactory::createRoom(Room::typeFromString(current(typeEdit)),
                                         text(idEdit),
                                         priceEdit->value(),
                                         Room::statusFromString(current(statusEdit)),
                                         bedsEdit->value());
    if (!room) {
        error("Unable to build room object for the selected type.");
        return;
    }

    if (!controller.updateRoom(*room, e)) {
        error(e);
    } else {
        reload();
    }
}

void RoomView::remove() {
    QString e;
    if (!controller.deleteRoom(text(idEdit), e)) {
        error(e);
    } else {
        reload();
    }
}

void RoomView::search() {
    refresh(controller.searchRooms(current(filterType), current(filterStatus)));
}

void RoomView::checkAvailability() {
    const QDate checkIn = checkInEdit->date();
    const QDate checkOut = checkOutEdit->date();
    QString e;

    if (checkOut <= checkIn) {
        error("Check-out date must be after Check-in date.");
        return;
    }

    refresh(controller.checkAvailability(checkIn, checkOut, e));
}

void RoomView::error(const QString& message) {
    QMessageBox::warning(this, "Room Error", message);
}