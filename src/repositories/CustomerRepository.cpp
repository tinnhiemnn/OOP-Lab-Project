#include "repositories/CustomerRepository.h"
#include "database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

Customer CustomerRepository::mapCustomer(QSqlQuery& q) {
    return Customer(q.value(0).toString().toStdString(),
                    q.value(1).toString().toStdString(),
                    q.value(2).toString().toStdString(),
                    q.value(3).toString().toStdString());
}

bool CustomerRepository::add(const Customer& customer) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("INSERT INTO customers(id, name, email, phone) VALUES(?, ?, ?, ?)");
    q.addBindValue(QString::fromStdString(customer.getId()));
    q.addBindValue(QString::fromStdString(customer.getName()));
    q.addBindValue(QString::fromStdString(customer.getEmail()));
    q.addBindValue(QString::fromStdString(customer.getPhone()));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text().toStdString();
        return false;
    }
    return true;
}

bool CustomerRepository::update(const Customer& customer) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE customers SET name = ?, email = ?, phone = ? WHERE id = ?");
    q.addBindValue(QString::fromStdString(customer.getName()));
    q.addBindValue(QString::fromStdString(customer.getEmail()));
    q.addBindValue(QString::fromStdString(customer.getPhone()));
    q.addBindValue(QString::fromStdString(customer.getId()));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text().toStdString();
        return false;
    }
    return true;
}

bool CustomerRepository::remove(const std::string& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("DELETE FROM customers WHERE id = ?");
    q.addBindValue(QString::fromStdString(id));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text().toStdString();
        return false;
    }
    return true;
}

std::vector<Customer> CustomerRepository::findAll() {
    std::vector<Customer> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    if (!q.exec("SELECT id, name, email, phone FROM customers ORDER BY id")) {
        lastErrorMessage = q.lastError().text().toStdString();
        return rows;
    }
    while (q.next()) rows.push_back(mapCustomer(q));
    return rows;
}

std::vector<Customer> CustomerRepository::search(const std::string& keyword) {
    std::vector<Customer> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, name, email, phone FROM customers WHERE id LIKE ? OR name LIKE ? OR email LIKE ? OR phone LIKE ? ORDER BY id");
    const QString pattern = "%" + QString::fromStdString(keyword) + "%";
    for (int i = 0; i < 4; ++i) q.addBindValue(pattern);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text().toStdString();
        return rows;
    }
    while (q.next()) rows.push_back(mapCustomer(q));
    return rows;
}

std::optional<Customer> CustomerRepository::findById(const std::string& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, name, email, phone FROM customers WHERE id = ?");
    q.addBindValue(QString::fromStdString(id));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text().toStdString();
        return std::nullopt;
    }
    if (q.next()) return mapCustomer(q);
    return std::nullopt;
}


