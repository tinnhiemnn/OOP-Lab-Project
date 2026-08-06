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

    idEdit->setReadOnly(true);
    
    idEdit->setPlaceholderText("Customer ID");
    nameEdit->setPlaceholderText("Customer Name");
    phoneEdit->setPlaceholderText("0*********");
    emailEdit->setPlaceholderText("abc@gmail.com");

    auto* form = new QFormLayout;
    form->setVerticalSpacing(10);
    form->addRow("Customer ID", idEdit);
    form->addRow("Name", nameEdit);
    form->addRow("Phone Number", phoneEdit);
    form->addRow("Email", emailEdit);

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

    formCard = new DashboardCard(QString(), this);
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
    table->setHorizontalHeaderLabels({"ID", "Name", "Phone", "Email"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSortingEnabled(true);

    QHeaderView *header = table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new DashboardCard(QString(), this);
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

    connect(searchEdit, &QLineEdit::returnPressed, searchBtn, &QPushButton::click);
    reload();
}

void CustomerView::refresh(const std::vector<Customer>& rows) {
    table->setRowCount(static_cast<int>(rows.size()));
    table->setSortingEnabled(false);
    for (int row = 0; row < static_cast<int>(rows.size()); ++row) {
        const auto& c = rows[static_cast<size_t>(row)];
        table->setItem(row, 0, new QTableWidgetItem(c.getId()));
        table->setItem(row, 1, new QTableWidgetItem(c.getName()));
        table->setItem(row, 2, new QTableWidgetItem(c.getPhone()));
        table->setItem(row, 3, new QTableWidgetItem(c.getEmail()));
    }
    table->setSortingEnabled(true);
}

void CustomerView::reload() { 
    refresh(controller.listCustomers()); 
    table->setCurrentCell(-1, -1);
    idEdit->clear();
    nameEdit->clear();
    emailEdit->clear();
    phoneEdit->clear();
}

void CustomerView::selected() {
    const int row = table->currentRow();
    if (row < 0) return;
    idEdit->setText(table->item(row, 0)->text());
    nameEdit->setText(table->item(row, 1)->text());
    phoneEdit->setText(table->item(row, 2)->text());
    emailEdit->setText(table->item(row, 3)->text());
}

void CustomerView::add() { 
    QString e;
    Customer c(idEdit->text(), 
               nameEdit->text(), 
               emailEdit->text(), 
               phoneEdit->text());

    if (!controller.addCustomer(c, e))
        error(e); 
    else
        reload();
}

void CustomerView::update() { 
    QString e;
    Customer c(idEdit->text(), 
               nameEdit->text(), 
               emailEdit->text(), 
               phoneEdit->text());

    if (!controller.updateCustomer(c, e))
        error(e); 
    else
        reload();
}

void CustomerView::remove() { 
    QString e;
    if (!controller.deleteCustomer(idEdit->text(), e))
        error(e); 
    else
        reload();
}

void CustomerView::search() { 
    refresh(controller.searchCustomers(searchEdit->text())); 
}
void CustomerView::error(const QString& message) { QMessageBox::warning(this, "Customer Error", message); }