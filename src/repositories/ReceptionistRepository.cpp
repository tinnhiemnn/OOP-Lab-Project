#include "repositories/ReceptionistRepository.h"
#include "database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

Receptionist ReceptionistRepository::mapReceptionist(QSqlQuery& q) {
    return Receptionist(q.value(0).toString(),
                    q.value(1).toString(),
                    q.value(2).toString());
}

bool ReceptionistRepository::add(const Receptionist& Receptionist) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("INSERT INTO Receptionists(id, name, email) VALUES(?, ?, ?)");
    q.addBindValue(Receptionist.getId());
    q.addBindValue(Receptionist.getName());
    q.addBindValue(Receptionist.getEmail());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool ReceptionistRepository::update(const Receptionist& Receptionist) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE Receptionists SET name = ?, email = ? WHERE id = ?");
    q.addBindValue(Receptionist.getName());
    q.addBindValue(Receptionist.getEmail());
    q.addBindValue(Receptionist.getId());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool ReceptionistRepository::remove(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("DELETE FROM Receptionists WHERE id = ?");
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
    if (!q.exec("SELECT id, name, email FROM Receptionists ORDER BY id")) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapReceptionist(q));
    return rows;
}

std::vector<Receptionist> ReceptionistRepository::search(const QString& keyword) {
    std::vector<Receptionist> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, name, email FROM Receptionists WHERE id LIKE ? OR name LIKE ? OR email LIKE ? ORDER BY id");
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
    q.prepare("SELECT id, name, email FROM Receptionists WHERE id = ?");
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return std::nullopt;
    }
    if (q.next()) return mapReceptionist(q);
    return std::nullopt;
}


