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
    auto* updateBtn = new QPushButton("Update", this);
    auto* deleteBtn = new QPushButton("Delete", this);
    deleteBtn->setObjectName("btnCancel");

    auto* reloadBtn = new QPushButton("Refresh", this);

    actions->addWidget(addBtn);
    actions->addWidget(updateBtn);
    actions->addWidget(deleteBtn);
    actions->addWidget(reloadBtn);

    // --- 3 ô thống kê mini phía trên ---
    auto* statsRow = new QHBoxLayout;

    auto makeStat = [this](const QString& label, const QString& value) {
        auto* box = new QFrame(this);
        box->setProperty("statMini", true);
        auto* boxLayout = new QVBoxLayout(box);
        auto* lbl = new QLabel(label, this);
        lbl->setProperty("role", "statLabel");
        auto* val = new QLabel(value, this);
        val->setProperty("role", "statValue");
        boxLayout->addWidget(lbl);
        boxLayout->addWidget(val);
        return box;
    };

    statsRow->addWidget(makeStat("Tổng khách hàng", "128"));
    statsRow->addWidget(makeStat("Khách thân thiết", "42"));
    statsRow->addWidget(makeStat("Chi tiêu TB", "1.250.000 ₫"));

    formCard = new DashboardCard("Customer", "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(actions);

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

    tableCard = new DashboardCard("Customer List", "purple", this);
    tableCard->addContentLayout(searching);
    tableCard->addContent(table);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addLayout(statsRow);
    layout->addWidget(formCard);
    layout->addWidget(tableCard, /*stretch=*/1);
}
