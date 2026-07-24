#include "repositories/InvoiceRepository.h"

#include "database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {
Invoice mapInvoice(QSqlQuery& q) {
    return Invoice(q.value(0).toString(),
                   q.value(1).toString(),
                   q.value(2).toString(),
                   q.value(3).toDate(),
                   q.value(4).toDouble(),
                   q.value(5).toDouble(),
                   q.value(6).toDouble(),
                   Invoice::paymentMethodFromString(q.value(7).toString()),
                   q.value(8).toString());
}
}

bool InvoiceRepository::startTransaction() {
    QSqlDatabase db = DatabaseManager::getInstance().database();
    if (db.isOpen()) {
        return db.transaction(); 
    }
    lastErrorMessage = "Database is not open.";
    return false;
}

bool InvoiceRepository::commitTransaction() {
    QSqlDatabase db = DatabaseManager::getInstance().database();
    if (db.isOpen()) {
        return db.commit(); 
    }
    lastErrorMessage = "Database is not open.";
    return false;
}

bool InvoiceRepository::rollbackTransaction() {
    QSqlDatabase db = DatabaseManager::getInstance().database();
    if (db.isOpen()) {
        return db.rollback(); 
    }
    lastErrorMessage = "Database is not open.";
    return false;
}

bool InvoiceRepository::add(const Invoice& invoice) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("INSERT INTO invoices(id, booking_id, receptionist_id, issued_date, subtotal_amount, total_amount, "
              "discount_amount, payment_method, discount_name) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
    q.addBindValue(invoice.getId());
    q.addBindValue(invoice.getBookingId());
    q.addBindValue(invoice.getReceptionistId());
    q.addBindValue(invoice.getIssuedDate());
    q.addBindValue(invoice.getSubtotalAmount());
    q.addBindValue(invoice.getTotalAmount());
    q.addBindValue(invoice.getDiscountAmount());
    q.addBindValue(Invoice::paymentMethodToString(invoice.getPaymentMethod()));
    q.addBindValue(invoice.getDiscountName());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

std::vector<Invoice> InvoiceRepository::findAll() {
    std::vector<Invoice> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    if (!q.exec("SELECT id, booking_id, receptionist_id, issued_date, subtotal_amount, total_amount, discount_amount, payment_method, discount_name FROM invoices ORDER BY id")) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapInvoice(q));
    return rows;
}

std::vector<Invoice> InvoiceRepository::search(const QString& keyword) {
    std::vector<Invoice> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, booking_id, receptionist_id, issued_date, subtotal_amount, total_amount, discount_amount, "
              "payment_method, discount_name FROM invoices WHERE id LIKE ? OR booking_id LIKE ? ORDER BY id");
    const QString pattern = "%" + keyword + "%";
    q.addBindValue(pattern);
    q.addBindValue(pattern);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapInvoice(q));
    return rows;
}

std::optional<Invoice> InvoiceRepository::findById(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, booking_id, receptionist_id, issued_date, subtotal_amount, total_amount, discount_amount, "
              "payment_method, discount_name FROM invoices WHERE id = ?");
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return std::nullopt;
    }
    if (q.next()) return mapInvoice(q);
    return std::nullopt;
}

double InvoiceRepository::totalRevenue() {
    QSqlQuery q(DatabaseManager::getInstance().database());
    if (!q.exec("SELECT COALESCE(SUM(total_amount), 0) FROM invoices")) {
        lastErrorMessage = q.lastError().text();
        return 0;
    }
    return q.next() ? q.value(0).toDouble() : 0;
}

std::vector<double> InvoiceRepository::getMonthlyRevenue(const QString& year) {
    std::vector<double> monthlyRevenue(12, 0.0);
    QSqlQuery q(DatabaseManager::getInstance().database());
    // Dùng strftime('%m', ...) để trích xuất tháng từ chuỗi ngày yyyy-mm-dd
    q.prepare("SELECT strftime('%m', issued_date) as month, SUM(total_amount) as total "
              "FROM invoices "
              "WHERE strftime('%Y', issued_date) = ? "
              "GROUP BY month");  
    q.addBindValue(year); 
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return monthlyRevenue; 
    }
    while (q.next()) {
        int monthIndex = q.value(0).toInt() - 1; 
        double total = q.value(1).toDouble();
        if (monthIndex >= 0 && monthIndex < 12) {
            monthlyRevenue[monthIndex] = total;
        }
    }
    return monthlyRevenue;
}

double InvoiceRepository::getTotalRevenueByRoomType(const QString& roomType) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT SUM(i.total_amount) "
              "FROM invoices i "
              "JOIN bookings b ON i.booking_id = b.id "
              "JOIN rooms r ON b.room_id = r.id "
              "WHERE r.type = ?"); 
    q.addBindValue(roomType);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return 0.0; 
    }
    if (q.next()) return q.value(0).toDouble();
    return 0.0;
}

std::vector<ReceptionistKPI> InvoiceRepository::getRevenueByReceptionist() {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT r.id, r.name, SUM(i.total_amount) as total_revenue "
              "FROM receptionists r "
              "LEFT JOIN invoices i ON r.id = i.receptionist_id "
              "GROUP BY r.id "
              "ORDER BY r.id"); 
    std::vector<ReceptionistKPI> results;
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return results;
    }
    while (q.next()) {
        ReceptionistKPI rec;
        rec.id = q.value(0).toString();
        rec.name = q.value(1).toString();
        rec.totalRevenue = q.value(2).toDouble();
        results.push_back(rec);
    }
    return results;
}

std::vector<TopCustomer> InvoiceRepository::getTop5Customers() {
    std::vector<TopCustomer> topList;
    QSqlQuery q(DatabaseManager::getInstance().database());
    QString sql = "SELECT "
                  "  c.id as cust_id, "
                  "  c.name as cust_name, "
                  "  SUM(i.total_amount) as total_spending "
                  "FROM customers c "
                  "LEFT JOIN bookings b ON c.id = b.customer_id "
                  "LEFT JOIN invoices i ON b.id = i.booking_id "
                  "GROUP BY cust_id, cust_name "
                  "ORDER BY total_spending DESC "  
                  "LIMIT 5";                       
    if (!q.exec(sql)) {
        lastErrorMessage = q.lastError().text();
        return topList;
    }
    while (q.next()) {
        TopCustomer customer;
        customer.id = q.value(0).toString();
        customer.name = q.value(1).toString();
        customer.totalSpending = q.value(2).toDouble();
        topList.push_back(customer);
    }
    return topList;
}