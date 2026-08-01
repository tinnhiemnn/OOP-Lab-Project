#include "repositories/ReceptionistRepository.h"
#include "database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {
    Receptionist mapReceptionist(QSqlQuery& q) {
        return Receptionist(q.value(0).toString(),
                        q.value(1).toString(),
                        q.value(2).toString(),
                        q.value(3).toString());
    }
}

QString ReceptionistRepository::verifyLogin(const QString& username, const QString& password) {
    if (username == "admin" && password == "admin123") {
        return "ADMIN";
    }
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id FROM receptionists WHERE id = ? AND password = ? AND status = 'Active'");
    q.addBindValue(username);
    q.addBindValue(password);
    if (q.exec() && q.next()) {
        return "RECEPTIONIST";
    }
    return "NONE"; 
}

bool ReceptionistRepository::add(const Receptionist& Receptionist) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("INSERT INTO receptionists(id, name, email, password) VALUES(?, ?, ?, ?)");
    q.addBindValue(Receptionist.getId());
    q.addBindValue(Receptionist.getName());
    q.addBindValue(Receptionist.getEmail());
    q.addBindValue(Receptionist.getPassword());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool ReceptionistRepository::update(const Receptionist& Receptionist) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE receptionists SET name = ?, email = ?, password = ? WHERE id = ?");
    q.addBindValue(Receptionist.getName());
    q.addBindValue(Receptionist.getEmail());
    q.addBindValue(Receptionist.getPassword());
    q.addBindValue(Receptionist.getId());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool ReceptionistRepository::remove(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE receptionists SET status = 'Inactive' WHERE id = ?");
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

std::vector<Receptionist> ReceptionistRepository::findAll() {
    std::vector<Receptionist> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    if (!q.exec("SELECT id, name, email, password FROM receptionists WHERE status = 'Active' ORDER BY id")) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapReceptionist(q));
    return rows;
}

std::vector<Receptionist> ReceptionistRepository::search(const QString& keyword) {
    std::vector<Receptionist> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, name, email, password FROM receptionists "
              "WHERE (id LIKE ? OR name LIKE ? OR email LIKE ?) AND status = 'Active' "
              "ORDER BY id");
    const QString pattern = "%" + keyword + "%";
    for (int i = 0; i < 3; ++i) q.addBindValue(pattern);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapReceptionist(q));
    return rows;
}

std::optional<Receptionist> ReceptionistRepository::findById(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, name, email, password FROM receptionists WHERE id = ? AND status = 'Active'");
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return std::nullopt;
    }
    if (q.next()) return mapReceptionist(q);
    return std::nullopt;
}

QString ReceptionistRepository::generateNextId() {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT MAX(CAST(SUBSTR(id, 4) AS INTEGER)) "
              "FROM receptionists");
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return "";
    }
    if (q.next()) {
        int maxId = q.value(0).toInt();
        return QString("REC%1").arg(maxId + 1, 3, 10, QChar('0'));
    }
    return "REC001";
}

