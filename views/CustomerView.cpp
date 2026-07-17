#include "DashboardCard.h"
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
}

CustomerView::CustomerView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), nameEdit(new QLineEdit(this)), emailEdit(new QLineEdit(this)),
      phoneEdit(new QLineEdit(this)), searchEdit(new QLineEdit(this)), table(new QTableWidget(this)) {

    idEdit->setReadOnly(true); //idCustomer -> no-edit
    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10 );
    form->addWidget(new QLabel("Customer ID", this), 0, 0);
    form->addWidget(idEdit, 0, 1);
    form->addWidget(new QLabel("Name", this), 0, 2);
    form->addWidget(nameEdit, 0, 3);
    form->addWidget(new QLabel("Email", this), 1, 0);
    form->addWidget(emailEdit, 1, 1);
    form->addWidget(new QLabel("Phone Number", this), 1, 2);
    form->addWidget(phoneEdit, 1, 3);
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

    formCard = new DashboardCard("Customer", "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(actions);

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchBtn->setProperty("variant", "primary");

    searchEdit->setPlaceholderText("Search customers");
    searching->addWidget(searchEdit);
    searching->addWidget(searchBtn);

    table->setObjectName("tableBookings");
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Name", "Email", "Phone"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new DashboardCard("Customer List", "purple", this);
    tableCard->addContentLayout(searching);
    tableCard->addContent(table);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    //layout->addLayout(statsRow);
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

void CustomerView::refresh(const std::vector<Customer>& rows) {
    table->setRowCount(static_cast<int>(rows.size()));
    for (int row = 0; row < static_cast<int>(rows.size()); ++row) {
        const auto& c = rows[static_cast<size_t>(row)];
        table->setItem(row, 0, new QTableWidgetItem(c.getId()));
        table->setItem(row, 1, new QTableWidgetItem(c.getName()));
        table->setItem(row, 2, new QTableWidgetItem(c.getEmail()));
        table->setItem(row, 3, new QTableWidgetItem(c.getPhone()));
    }
}

void CustomerView::reload() { 
    refresh(controller.listCustomers()); 
}

void CustomerView::selected() {
    const int row = table->currentRow();
    if (row < 0) return;
    idEdit->setText(table->item(row, 0)->text());
    nameEdit->setText(table->item(row, 1)->text());
    emailEdit->setText(table->item(row, 2)->text());
    phoneEdit->setText(table->item(row, 3)->text());
}

void CustomerView::add() { 
    QString e;
    const Customer c(idEdit->text(), 
               nameEdit->text(), 
               emailEdit->text(), 
               phoneEdit->text());

    if (!controller.addCustomer(c, e))
        error(e); 
    else {
        reload();
        idEdit->clear();
        nameEdit->clear();
        emailEdit->clear();
        phoneEdit->clear();
    }
}

void CustomerView::update() { 
    QString e;
    const Customer c(idEdit->text(), 
               nameEdit->text(), 
               emailEdit->text(), 
               phoneEdit->text());

    if (!controller.updateCustomer(c, e))
        error(e); 
    else {
        reload();
        idEdit->clear();
        nameEdit->clear();
        emailEdit->clear();
        phoneEdit->clear();
    }
}

void CustomerView::remove() { 
    QString e;
    if (!controller.deleteCustomer(idEdit->text(), e))
        error(e); 
    else {
        reload();
        idEdit->clear();
        nameEdit->clear();
        emailEdit->clear();
        phoneEdit->clear();
    }
}

void CustomerView::search() { 
    refresh(controller.searchCustomers(searchEdit->text())); 
}
void CustomerView::error(const QString& message) { QMessageBox::warning(this, "Customer Error", message); }