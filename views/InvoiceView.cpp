#include "DashboardCard.h"
#include "InvoiceView.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QString>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <algorithm>

namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }
    QString current(QComboBox* box) { return box->currentText(); }
}

InvoiceView::InvoiceView(QWidget* parent)
    : QWidget(parent),
      invoiceRepo(),
      invoiceService(invoiceRepo),
      controller(invoiceService),
      bookingIdEdit(new QLineEdit(this)),
      receptionistIdEdit(new QLineEdit(this)),
      serviceChargeEdit(new QDoubleSpinBox(this)),
      discountEdit(new QComboBox(this)), paymentEdit(new QComboBox(this)),
      searchEdit(new QLineEdit(this)),
      discountFilter(new QComboBox(this)), paymentFilter(new QComboBox(this)),
      table(new QTableWidget(this)) {
    serviceChargeEdit->setRange(0, 100000000);
    serviceChargeEdit->setDecimals(0);
    discountEdit->addItems({"None", "Seasonal", "Member"});
    paymentEdit->addItems({"Cash", "Credit Card", "E-Wallet"});

    bookingIdEdit->setPlaceholderText("Booking ID");
    receptionistIdEdit->setPlaceholderText("Receptionist ID");

    // --- Form nhap lieu tao hoa don ---
    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10);
    form->addWidget(new QLabel("Booking ID", this), 0, 0);
    form->addWidget(bookingIdEdit, 0, 1);
    form->addWidget(new QLabel("Receptionist ID", this), 0, 2);
    form->addWidget(receptionistIdEdit, 0, 3);
    form->addWidget(new QLabel("Service Charge", this), 1, 0);
    form->addWidget(serviceChargeEdit, 1, 1);
    form->addWidget(new QLabel("Discount", this), 1, 2);
    form->addWidget(discountEdit, 1, 3);
    form->addWidget(new QLabel("Payment", this), 2, 0);
    form->addWidget(paymentEdit, 2, 1);
    form->setColumnStretch(1, 1);
    form->setColumnStretch(3, 1);

    // --- Hang nut hanh dong: Create Invoice | Create All Invoices in Group | Refresh ---
    auto* actions = new QHBoxLayout;
    auto* createBtn = new QPushButton("Create Invoice", this);
    createBtn->setProperty("variant", "primary");

    auto* createGroupBtn = new QPushButton("Create All Invoices in Group", this);
    createGroupBtn->setProperty("variant", "secondary");

    auto* reloadBtn = new QPushButton("Refresh", this);
    reloadBtn->setProperty("variant", "ghost");

    actions->addWidget(createBtn);
    actions->addWidget(createGroupBtn);
    actions->addWidget(reloadBtn);

    // --- Hang tim kiem + filter ---
    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchBtn->setProperty("variant", "primary");
    searchEdit->setPlaceholderText("Search invoices (by Invoice ID or Booking ID)");

    discountFilter->addItems({"All Discounts", "None", "Seasonal", "Member"});
    paymentFilter->addItems({"All Payments", "Cash", "Credit Card", "E-Wallet", "Bank Transfer"});

    searching->addWidget(searchEdit, /*stretch=*/1);
    searching->addWidget(searchBtn);
    searching->addWidget(discountFilter);
    searching->addWidget(paymentFilter);

    // --- Card 1: form tao hoa don ---
    formCard = new DashboardCard("Invoice", "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(actions);

    // --- Card 2: tim kiem + danh sach hoa don dang bang ---
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels({
        "ID", "Booking", "Receptionist", "Date",
        "Subtotal", "Discount Name", "Discount Amount", "Total", "Payment"
    });
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableCard = new DashboardCard("Invoice List", "green", this);
    tableCard->addContentLayout(searching);
    tableCard->addContent(table);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addWidget(formCard);
    layout->addWidget(tableCard, /*stretch=*/1);

    connect(createBtn, &QPushButton::clicked, this, [this] { add(); });
    //connect(createGroupBtn, &QPushButton::clicked, this, [this] { addGroup(); });
    connect(reloadBtn, &QPushButton::clicked, this, [this] { reload(); });
    connect(searchBtn, &QPushButton::clicked, this, [this] { search(); });
    connect(discountFilter, &QComboBox::currentTextChanged, this, [this] { applyFilters(); });
    connect(paymentFilter, &QComboBox::currentTextChanged, this, [this] { applyFilters(); });
    connect(table, &QTableWidget::itemSelectionChanged, this, [this] { selected(); });
    reload();
}

void InvoiceView::refresh(const std::vector<Invoice>& rows) {
    table->setRowCount(static_cast<int>(rows.size()));
    for (int row = 0; row < static_cast<int>(rows.size()); ++row) {
        const auto& i = rows[static_cast<size_t>(row)];
        table->setItem(row, 0, new QTableWidgetItem(i.getId()));
        table->setItem(row, 1, new QTableWidgetItem(i.getBookingId()));
        table->setItem(row, 2, new QTableWidgetItem(i.getReceptionistId()));
        table->setItem(row, 3, new QTableWidgetItem(i.getIssuedDate().toString("yyyy-MM-dd")));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(i.getSubtotalAmount(), 'f', 0)));
        table->setItem(row, 5, new QTableWidgetItem(i.getDiscountName().isEmpty() ? "None" : i.getDiscountName()));
        table->setItem(row, 6, new QTableWidgetItem(QString::number(i.getDiscountAmount(), 'f', 0)));
        table->setItem(row, 7, new QTableWidgetItem(QString::number(i.getTotalAmount(), 'f', 0)));
        table->setItem(row, 8, new QTableWidgetItem(Invoice::paymentMethodToString(i.getPaymentMethod())));
    }
}

void InvoiceView::applyFilters() {
    const QString discountKey = current(discountFilter);
    const QString paymentKey = current(paymentFilter);

    std::vector<Invoice> filtered;
    filtered.reserve(currentInvoices.size());

    for (const auto& inv : currentInvoices) {
        const QString discountName = inv.getDiscountName().isEmpty() ? "None" : inv.getDiscountName();
        const QString paymentName = Invoice::paymentMethodToString(inv.getPaymentMethod());

        const bool discountOk = (discountKey == "All Discounts") || (discountKey == discountName);
        const bool paymentOk  = (paymentKey == "All Payments") || (paymentKey == paymentName);

        if (discountOk && paymentOk) {
            filtered.push_back(inv);
        }
    }

    refresh(filtered);
}

void InvoiceView::reload() {
    currentInvoices = controller.handleGetAllInvoices();
    applyFilters();
}

void InvoiceView::selected() {
    const int row = table->currentRow();
    if (row >= 0)
        bookingIdEdit->setText(table->item(row, 1)->text());
}

void InvoiceView::add() {
    QString e;
    // "None" in the discount combo box means "no discount" -> send empty string.
    const QString discountName = current(discountEdit) == "None" ? QString() : current(discountEdit);

    if (!controller.createInvoice(text(bookingIdEdit), text(receptionistIdEdit),
                                   discountName, current(paymentEdit), e)) {
        error(e);
    } else {
        reload();
    }
}

/*void InvoiceView::addGroup() {
    // TODO: requires InvoiceController::handleCreateInvoicesForGroup(...)
    // See backend TODO notes - not implemented yet, pending Booking/Repository details.
    QString e;
    const QString discountName = current(discountEdit) == "None" ? QString() : current(discountEdit);

    if (!controller.handleCreateInvoicesForGroup(text(bookingIdEdit), text(receptionistIdEdit),
                                                  discountName, current(paymentEdit), e)) {
        error(e);
    } else {
        reload();
    }
}*/

void InvoiceView::search() {
    // InvoiceController only exposes lookup-by-id and lookup-by-booking-id,
    // there is no free-text search, so we try both and show whichever matches.
    const QString key = text(searchEdit);
    std::vector<Invoice> results;

    if (auto invoiceById = controller.handleGetInvoiceById(key)) {
        results.push_back(*invoiceById);
    } else if (auto invoiceByBooking = controller.handleGetInvoiceByBookingId(key)) {
        results.push_back(*invoiceByBooking);
    }

    currentInvoices = results;
    applyFilters();
}

void InvoiceView::error(const QString& message) {
    QMessageBox::warning(this, "Invoice Error", message);
}