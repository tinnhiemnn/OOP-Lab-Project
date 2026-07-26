#include "DashboardCard.h"
#include "ReceptionistView.h"

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
#include <QLabel>
#include <QGraphicsDropShadowEffect>


namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }
}

ReceptionistView::ReceptionistView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), nameEdit(new QLineEdit(this)), emailEdit(new QLineEdit(this)),
    searchEdit(new QLineEdit(this)), table(new QTableWidget(this)) {

    idEdit->setReadOnly(true); //idReceptionist -> no-edit
    idEdit->setPlaceholderText("Receptionist ID");
    emailEdit->setPlaceholderText("abc@gmail.com");

    auto* form = new QFormLayout;
    form->addRow("Receptionist ID",idEdit);
    form->addRow("Name",nameEdit);
    form->addRow("Email",emailEdit);

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

    formCard = new DashboardCard("Receptionist", "orange", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(actions);

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchBtn->setProperty("variant", "primary");

    searchEdit->setPlaceholderText("Search receptionists");
    searching->addWidget(searchEdit);
    searching->addWidget(searchBtn);

    table->setObjectName("tableReceptionists");
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"ID", "Name", "Email"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new DashboardCard("Receptionist List", "purple", this);
    tableCard->addContentLayout(searching);
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
    connect(searchBtn, &QPushButton::clicked, this, [this] { search(); });
    connect(table, &QTableWidget::itemSelectionChanged, this, [this] { selected(); });
    reload();
}


void ReceptionistView::refresh(const std::vector<Receptionist>& rows) {
    table->setRowCount(static_cast<int>(rows.size()));
    for (int row = 0; row < static_cast<int>(rows.size()); ++row) {
        const auto& r = rows[static_cast<size_t>(row)];
        table->setItem(row, 0, new QTableWidgetItem(r.getId()));
        table->setItem(row, 1, new QTableWidgetItem(r.getName()));
        table->setItem(row, 2, new QTableWidgetItem(r.getEmail()));
    }
}

void ReceptionistView::reload() {
    refresh(controller.getAllReceptionists()); 
    //idEdit->setReadOnly(false);
    idEdit->clear();
}

void ReceptionistView::selected() {
    const int row = table->currentRow();
    if (row < 0) return;
    idEdit->setText(table->item(row, 0)->text());
    nameEdit->setText(table->item(row, 1)->text());
    emailEdit->setText(table->item(row, 2)->text());

}

void ReceptionistView::add() { 
    QString e;
    Receptionist r(idEdit->text(), 
               nameEdit->text(), 
               emailEdit->text());

    if (!controller.addReceptionist(r, e))
        error(e); 
    else {
        reload();
        idEdit->clear();
        nameEdit->clear();
        emailEdit->clear();
    }
}

void ReceptionistView::update() { 
    QString e;
    const Receptionist r(idEdit->text(), 
               nameEdit->text(), 
               emailEdit->text());

    if (!controller.updateReceptionist(r, e))
        error(e); 
    else {
        reload();
        idEdit->clear();
        nameEdit->clear();
        emailEdit->clear();
    }
}

void ReceptionistView::remove() { 
    QString e;
    if (!controller.deleteReceptionist(idEdit->text(), e))
        error(e); 
    else {
        reload();
        idEdit->clear();
        nameEdit->clear();
        emailEdit->clear();
    }
}

void ReceptionistView::search() { refresh(controller.searchReceptionists(searchEdit->text())); }
void ReceptionistView::error(const QString& message) { QMessageBox::warning(this, "Receptionist Error", message); }