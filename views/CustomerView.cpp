#include "CustomerView.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QString>
#include <QFrame>
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

CustomerView::CustomerView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), nameEdit(new QLineEdit(this)), emailEdit(new QLineEdit(this)),
      phoneEdit(new QLineEdit(this)), searchEdit(new QLineEdit(this)), table(new QTableWidget(this)) {

    auto* form = new QFormLayout;
    form->addRow("Customer ID", idEdit);
    form->addRow("Name", nameEdit);
    form->addRow("Email", emailEdit);
    form->addRow("Phone Number", phoneEdit);

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Add", this);
    auto* updateBtn = new QPushButton("Update", this);
    auto* deleteBtn = new QPushButton("Delete", this);
    deleteBtn->setObjectName("btnCancel");

    auto* reloadBtn = new QPushButton("Refresh", this);

    actions->addWidget(addBtn);
    actions->addWidget(updateBtn);
    actions->addWidget(deleteBtn);
    actions->addWidget(reloadBtn);

    formCard = new QFrame(this);
    formCard->setObjectName("cardPanel");
    auto* formCardLayout = new QVBoxLayout(formCard);
    formCardLayout->addLayout(form);
    formCardLayout->addLayout(actions);

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchBtn->setObjectName("btnSearch");
    searchEdit->setPlaceholderText("Search customers");
    searching->addWidget(searchEdit);
    searching->addWidget(searchBtn);

    table->setObjectName("tableBookings");
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Name", "Email", "Phone"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new QFrame(this);
    tableCard->setObjectName("cardPanel");
    auto* tableCardLayout = new QVBoxLayout(tableCard);
    tableCardLayout->addLayout(searching);
    tableCardLayout->addWidget(table);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(formCard);
    layout->addWidget(tableCard);

    applyCardShadow(formCard);
    applyCardShadow(tableCard);
}