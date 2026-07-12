#include "DashboardCard.h"
#include "InvoiceView.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QString>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }
    QString current(QComboBox* box) { return box->currentText(); }
}

    InvoiceView::InvoiceView(QWidget* parent)
        : QWidget(parent), bookingIdEdit(new QLineEdit(this)), serviceChargeEdit(new QDoubleSpinBox(this)),
        discountEdit(new QComboBox(this)), paymentEdit(new QComboBox(this)), searchEdit(new QLineEdit(this)) {
    serviceChargeEdit->setRange(0, 100000000);
    serviceChargeEdit->setDecimals(0);
    discountEdit->addItems({"None", "Seasonal", "Member"});
    paymentEdit->addItems({"Cash", "Credit Card", "E-Wallet"});

    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10 );
    form->addWidget(new QLabel("Booking ID", this), 0, 0);
    form->addWidget(bookingIdEdit, 0, 1);
    form->addWidget(new QLabel("Service Charge", this), 0, 2);
    form->addWidget(serviceChargeEdit, 0, 3);
    form->addWidget(new QLabel("Discount", this), 1, 0);
    form->addWidget(discountEdit, 1, 1);
    form->addWidget(new QLabel("Payment", this), 1, 2);
    form->addWidget(paymentEdit, 1, 3);
    form->setColumnStretch(1, 1);
    form->setColumnStretch(3, 1);

    auto* actions = new QHBoxLayout;
    auto* createBtn = new QPushButton("Create Invoice", this);
    auto* reloadBtn = new QPushButton("Refresh", this);
    actions->addWidget(createBtn); actions->addWidget(reloadBtn);

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchEdit->setPlaceholderText("Search invoices");
    searching->addWidget(searchEdit); searching->addWidget(searchBtn);

    // --- 2 ô thống kê mini phía trên ---
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
    statsRow->addWidget(makeStat("Doanh thu hoá đơn tháng này", "0 ₫"));
    statsRow->addWidget(makeStat("Hoá đơn chờ thanh toán", "0"));

    // --- Card 1: form tạo hoá đơn ---
    formCard = new DashboardCard("Invoice", "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(actions);

    // --- Card 2: tìm kiếm + danh sách hoá đơn dạng card ---
    auto* listContainer = new QWidget(this);
    invoiceList = new QVBoxLayout(listContainer);
    invoiceList->setSpacing(10);
    invoiceList->addStretch();

    tableCard = new DashboardCard("Invoice List", "green", this);
    tableCard->addContentLayout(searching);
    tableCard->addContent(listContainer);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addLayout(statsRow);
    layout->addWidget(formCard);
    layout->addWidget(tableCard, /*stretch=*/1);
}