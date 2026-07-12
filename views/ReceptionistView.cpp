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

QLabel* ReceptionistView::makeKpiBadge(const QString& tier) const {
    auto* label = new QLabel();
    label->setAlignment(Qt::AlignCenter);
    label->setProperty("role", "kpiBadge");
    if (tier == "excellent") {
        label->setText("Xuat sac");
        label->setProperty("tier", "excellent");
    } else if (tier == "good") {
        label->setText("Tot");
        label->setProperty("tier", "good");
    } else {
        label->setText("Trung binh");
        label->setProperty("tier", "average");
    }
    return label;
}

ReceptionistView::ReceptionistView(QWidget* parent)
    : QWidget(parent), idEdit(new QLineEdit(this)), nameEdit(new QLineEdit(this)), emailEdit(new QLineEdit(this)),
    phoneEdit(new QLineEdit(this)), searchEdit(new QLineEdit(this)), table(new QTableWidget(this)) {

    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10 );
    form->addWidget(new QLabel("Staff ID", this), 0, 0);
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

    // --- 3 o thong ke mini phia tren, khop voi tab Receptionists trong mockup ---
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

    statsRow->addWidget(makeStat("Tổng nhân viên", "5"));
    statsRow->addWidget(makeStat("Doanh thu TB / nhân viên", "41.2tr VND"));
    statsRow->addWidget(makeStat("Nhân viên xuất sắc tháng", "Hoai Thu"));

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
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID", "Name", "Email", "Bookings handled", "Revenue", "KPI"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new DashboardCard("Receptionist List", "purple", this);
    tableCard->addContentLayout(searching);
    tableCard->addContent(table);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addLayout(statsRow);
    layout->addWidget(formCard);
    layout->addWidget(tableCard, /*stretch=*/1);

    // --- du lieu mau tam thoi, khop voi tab Receptionists trong mockup,
    //     xoa khoi day khi da noi controller/repository that ---
    /*struct SeedRow { QString id, name, email, bookings, revenue, tier; };
    const QVector<SeedRow> seed = {
                                   {"E03", "Hoai Thu",   "thu.hoai@luxestay.vn",   "34", "52.000.000 VND", "excellent"},
                                   {"E01", "Mai Anh",    "anh.mai@luxestay.vn",    "29", "45.000.000 VND", "excellent"},
                                   {"E05", "Ngoc Ha",    "ha.ngoc@luxestay.vn",    "26", "41.000.000 VND", "good"},
                                   {"E02", "Thanh Tung", "tung.thanh@luxestay.vn", "22", "38.000.000 VND", "good"},
                                   {"E04", "Quoc Bao",   "bao.quoc@luxestay.vn",   "18", "30.000.000 VND", "average"},
                                   };
    table->setRowCount(seed.size());
    for (int row = 0; row < seed.size(); ++row) {
        const auto& r = seed[row];
        table->setItem(row, 0, new QTableWidgetItem(r.id));
        table->setItem(row, 1, new QTableWidgetItem(r.name));
        table->setItem(row, 2, new QTableWidgetItem(r.email));
        table->setItem(row, 3, new QTableWidgetItem(r.bookings));
        table->setItem(row, 4, new QTableWidgetItem(r.revenue));
        table->setCellWidget(row, 5, makeKpiBadge(r.tier));
    }*/
}