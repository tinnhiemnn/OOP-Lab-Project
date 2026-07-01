#include "repositories/CustomerRepository.h"
#include "database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

Customer CustomerRepository::mapCustomer(QSqlQuery& q) {
    return Customer(q.value(0).toString(),
                    q.value(1).toString(),
                    q.value(2).toString(),
                    q.value(3).toString());
}

bool CustomerRepository::add(const Customer& customer) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("INSERT INTO customers(id, name, email, phone) VALUES(?, ?, ?, ?)");
    q.addBindValue(customer.getId());
    q.addBindValue(customer.getName());
    q.addBindValue(customer.getEmail());
    q.addBindValue(customer.getPhone());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool CustomerRepository::update(const Customer& customer) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE customers SET name = ?, email = ?, phone = ? WHERE id = ?");
    q.addBindValue(customer.getName());
    q.addBindValue(customer.getEmail());
    q.addBindValue(customer.getPhone());
    q.addBindValue(customer.getId());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool CustomerRepository::remove(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("DELETE FROM customers WHERE id = ?");
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

std::vector<Customer> CustomerRepository::findAll() {
    std::vector<Customer> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    if (!q.exec("SELECT id, name, email, phone FROM customers ORDER BY id")) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapCustomer(q));
    return rows;
}

std::vector<Customer> CustomerRepository::search(const QString& keyword) {
    std::vector<Customer> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, name, email, phone FROM customers WHERE id LIKE ? OR name LIKE ? OR email LIKE ? OR phone LIKE ? ORDER BY id");
    const QString pattern = "%" + keyword + "%";
    for (int i = 0; i < 4; ++i) q.addBindValue(pattern);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapCustomer(q));
    return rows;
}

std::optional<Customer> CustomerRepository::findById(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, name, email, phone FROM customers WHERE id = ?");
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return std::nullopt;
    }
    if (q.next()) return mapCustomer(q);
    return std::nullopt;
}


