#include "repositories/RoomRepository.h"

#include "database/DatabaseManager.h"
#include "patterns/RoomFactory.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {
std::unique_ptr<Room> mapRoom(QSqlQuery& q) {
    return RoomFactory::createRoom(Room::typeFromString(q.value(3).toString()), 
                                   q.value(0).toString(),
                                   q.value(1).toDouble(),
                                   Room::statusFromString(q.value(2).toString()),
                                   q.value(4).toInt());
}
}

bool RoomRepository::add(const Room& room, int beds) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("INSERT INTO rooms(id, base_price, status, type, beds) VALUES(?, ?, ?, ?, ?)");
    q.addBindValue(room.getRoomId());
    q.addBindValue(room.getBasePrice());
    q.addBindValue(Room::statusToString(room.getStatus())));
    q.addBindValue(Room::typeToString(room.getRoomType()));
    q.addBindValue(beds);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool RoomRepository::update(const Room& room, int beds) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE rooms SET base_price = ?, status = ?, type = ?, beds = ? WHERE id = ?");
    q.addBindValue(room.getBasePrice());
    q.addBindValue(Room::statusToString(room.getStatus()));
    q.addBindValue(Room::typeToString(room.getRoomType()));
    q.addBindValue(beds);
    q.addBindValue(room.getRoomId());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool RoomRepository::remove(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("DELETE FROM rooms WHERE id = ?");
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool RoomRepository::updateStatus(const QString& id, RoomStatus status) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE rooms SET status = ? WHERE id = ?");
    q.addBindValue(Room::statusToString(status));
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

std::vector<std::unique_ptr<Room>> RoomRepository::findAll() {
    return search("All", "All");
}

std::vector<std::unique_ptr<Room>> RoomRepository::search(const QString& typeFilter, const QString& statusFilter) {
    std::vector<std::unique_ptr<Room>> rows;
    QString sql = "SELECT id, base_price, status, type, beds FROM rooms WHERE 1=1";
    if (!typeFilter.empty() && typeFilter != "All") sql += " AND type = ?";
    if (!statusFilter.empty() && statusFilter != "All") sql += " AND status = ?";
    sql += " ORDER BY id";
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare(sql);
    if (!typeFilter.empty() && typeFilter != "All") q.addBindValue(typeFilter));
    if (!statusFilter.empty() && statusFilter != "All") q.addBindValue(statusFilter));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapRoom(q));
    return rows;
}

std::unique_ptr<Room> RoomRepository::findById(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, base_price, status, type, beds FROM rooms WHERE id = ?");
    q.addBindValue(id));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return nullptr;
    }
    if (q.next()) return mapRoom(q);
    return nullptr;
}

