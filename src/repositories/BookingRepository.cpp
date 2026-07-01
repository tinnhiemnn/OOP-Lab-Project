#include "repositories/BookingRepository.h"

#include "database/DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {
Booking mapBooking(QSqlQuery& q) {
    Booking booking(q.value(0).toString(),
                    q.value(1).toString(),
                    q.value(2).toString(),
                    q.value(3).toString(),
                    q.value(4).toString(),
                    q.value(5).toDate(),
                    q.value(6).toDate(),
                    Booking::statusFromString(q.value(7).toString()));
    return booking;
}
}

bool BookingRepository::add(const Booking& booking) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("INSERT INTO bookings(id, customer_id, receptionist_id, room_id, group_code, check_in, check_out, status) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
    q.addBindValue(booking.getId());
    q.addBindValue(booking.getCustomerId());
    q.addBindValue(booking.getReceptionistId());
    q.addBindValue(booking.getRoomId());
    q.addBindValue(booking.getGroupCode());
    q.addBindValue(booking.getCheckIn());
    q.addBindValue(booking.getCheckOut());
    q.addBindValue(Booking::statusToString(booking.getStatus()));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool BookingRepository::update(const Booking& booking) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE bookings SET customer_id = ?, receptionist_id = ?, room_id = ?, group_code = ?, check_in = ?, check_out = ?, status = ? WHERE id = ?");
    q.addBindValue(booking.getCustomerId()));
    q.addBindValue(booking.getRoomId()));
    q.addBindValue(booking.getCheckIn()));
    q.addBindValue(booking.getCheckOut()));
    q.addBindValue(Booking::statusToString(booking.getStatus())));
    q.addBindValue(booking.getServiceCharge());
    q.addBindValue(booking.getId()));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool BookingRepository::remove(const std::string& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("DELETE FROM bookings WHERE id = ?");
    q.addBindValue(id));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

std::vector<Booking> BookingRepository::findAll() {
    std::vector<Booking> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    if (!q.exec("SELECT id, customer_id, room_id, check_in, check_out, status, service_charge FROM bookings ORDER BY id")) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapBooking(q));
    return rows;
}

std::vector<Booking> BookingRepository::search(const std::string& keyword) {
    std::vector<Booking> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, customer_id, room_id, check_in, check_out, status, service_charge FROM bookings WHERE id LIKE ? OR customer_id LIKE ? OR room_id LIKE ? OR status LIKE ? ORDER BY id");
    const QString pattern = "%" + keyword) + "%";
    for (int i = 0; i < 4; ++i) q.addBindValue(pattern);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapBooking(q));
    return rows;
}

std::optional<Booking> BookingRepository::findById(const std::string& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, customer_id, room_id, check_in, check_out, status, service_charge FROM bookings WHERE id = ?");
    q.addBindValue(id));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return std::nullopt;
    }
    if (q.next()) return mapBooking(q);
    return std::nullopt;
}

