#include "DashboardCard.h"
#include "RoomView.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QString>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>

namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }
    QString current(QComboBox* box) { return box->currentText(); }
}

RoomView::RoomView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), typeEdit(new QComboBox(this)), priceEdit(new QDoubleSpinBox(this)),
      statusEdit(new QComboBox(this)), bedsEdit(new QSpinBox(this)),
      filterType(new QComboBox(this)), filterStatus(new QComboBox(this)) {
    typeEdit->addItems({"Standard", "Deluxe", "President"});
    statusEdit->addItems({"Available", "Out of Order", "InUse", "NeedCleaning"});
    filterType->addItems({"All", "Standard", "Deluxe", "President"});
    filterStatus->addItems({"All", "Available", "Booked", "InUse", "NeedCleaning"});
    priceEdit->setRange(1, 100000000);
    priceEdit->setDecimals(0);
    bedsEdit->setRange(1, 6);

    auto* form = new QFormLayout;
    form->addRow("Room ID", idEdit);
    form->addRow("Type", typeEdit);
    form->addRow("Price", priceEdit);
    form->addRow("Status", statusEdit);
    form->addRow("Beds", bedsEdit);

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

    formCard = new DashboardCard("Room", "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(actions);

    auto* filters = new QHBoxLayout;
    auto* filterBtn = new QPushButton("Filter", this);
    filterBtn->setObjectName("btnFilters");
    filters->addWidget(filterType);
    filters->addWidget(filterStatus);
    filters->addWidget(filterBtn);

    // --- Grid phòng 4 cột ---
    auto* gridContainer = new QWidget(this);
    roomGrid = new QGridLayout(gridContainer);
    roomGrid->setSpacing(16);

    tableCard = new DashboardCard("Room List", "purple", this);
    tableCard->addContentLayout(filters);
    tableCard->addContent(gridContainer);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addWidget(formCard);
    layout->addWidget(tableCard, /*stretch=*/1);
}