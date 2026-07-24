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
                    Booking::statusFromString(q.value(11).toString()));
    booking.setBuffetQuantity(q.value(7).toInt());
    booking.setLaundry(q.value(8).toBool());
    booking.setDecoration(q.value(9).toBool());
    booking.setDecorationNote(q.value(10).toString());
    return booking;
}
}

bool BookingRepository::startTransaction() {
    QSqlDatabase db = DatabaseManager::getInstance().database();
    if (db.isOpen()) {
        return db.transaction(); 
    }
    lastErrorMessage = "Database is not open.";
    return false;
}

bool BookingRepository::commitTransaction() {
    QSqlDatabase db = DatabaseManager::getInstance().database();
    if (db.isOpen()) {
        return db.commit(); 
    }
    lastErrorMessage = "Database is not open.";
    return false;
}

bool BookingRepository::rollbackTransaction() {
    QSqlDatabase db = DatabaseManager::getInstance().database();
    if (db.isOpen()) {
        return db.rollback(); 
    }
    lastErrorMessage = "Database is not open.";
    return false;
}

bool BookingRepository::add(const Booking& booking) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("INSERT INTO bookings(id, customer_id, receptionist_id, room_id, group_code, check_in, check_out, num_buffet, " 
              "laundry_service, decor_service, decor_note, status) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    q.addBindValue(booking.getId());
    q.addBindValue(booking.getCustomerId());
    q.addBindValue(booking.getReceptionistId());
    q.addBindValue(booking.getRoomId());
    q.addBindValue(booking.getGroupCode());
    q.addBindValue(booking.getCheckIn());
    q.addBindValue(booking.getCheckOut());
    q.addBindValue(booking.getBuffetQuantity());
    q.addBindValue(booking.isUsingLaundry());
    q.addBindValue(booking.isUsingDecoration());
    q.addBindValue(booking.getDecorationNote());
    q.addBindValue(Booking::statusToString(booking.getStatus()));
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

bool BookingRepository::update(const Booking& booking) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("UPDATE bookings SET customer_id = ?, receptionist_id = ?, room_id = ?, group_code = ?, check_in = ?, check_out = ?, "
              "num_buffet = ?, laundry_service = ?, decor_service = ?, decor_note = ?, status = ? WHERE id = ?");
    q.addBindValue(booking.getCustomerId());
    q.addBindValue(booking.getReceptionistId());
    q.addBindValue(booking.getRoomId());
    q.addBindValue(booking.getGroupCode());
    q.addBindValue(booking.getCheckIn());
    q.addBindValue(booking.getCheckOut());
    q.addBindValue(booking.getBuffetQuantity());
    q.addBindValue(booking.isUsingLaundry());
    q.addBindValue(booking.isUsingDecoration());
    q.addBindValue(booking.getDecorationNote());
    q.addBindValue(Booking::statusToString(booking.getStatus()));
    q.addBindValue(booking.getId());
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return false;
    }
    return true;
}

std::vector<Booking> BookingRepository::findAll() {
    std::vector<Booking> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    if (!q.exec("SELECT id, customer_id, receptionist_id, room_id, group_code, check_in, check_out, " 
                "num_buffet, laundry_service, decor_service, decor_note, status FROM bookings ORDER BY id")) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapBooking(q));
    return rows;
}

std::vector<Booking> BookingRepository::search(const QString& keyword) {
    std::vector<Booking> rows;
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, customer_id, receptionist_id, room_id, group_code, check_in, check_out, "
              "num_buffet, laundry_service, decor_service, decor_note, status FROM bookings "
              "WHERE id LIKE ? OR customer_id LIKE ? OR receptionist_id LIKE ? OR room_id LIKE ? "
              "OR group_code LIKE ? OR decor_note LIKE ? OR status LIKE ? ORDER BY id");
    const QString pattern = "%" + keyword + "%";
    for (int i = 0; i < 7; ++i) q.addBindValue(pattern);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return rows;
    }
    while (q.next()) rows.push_back(mapBooking(q));
    return rows;
}

std::optional<Booking> BookingRepository::findById(const QString& id) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    q.prepare("SELECT id, customer_id, receptionist_id, room_id, group_code, check_in, check_out, " 
              "num_buffet, laundry_service, decor_service, decor_note, status FROM bookings WHERE id = ?");
    q.addBindValue(id);
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return std::nullopt;
    }
    if (q.next()) return mapBooking(q);
    return std::nullopt;
}

int BookingRepository::countBookings(const QString& roomType, const QString& bookingStatus) {
    QSqlQuery q(DatabaseManager::getInstance().database());
    
    QString sql = "SELECT COUNT(*) FROM bookings b "
                  "JOIN rooms r ON b.room_id = r.id "
                  "WHERE r.type = ?";
                  
    if (!bookingStatus.isEmpty() && bookingStatus != "All") sql += " AND b.status = ?";
    q.prepare(sql);
    q.addBindValue(roomType);
    if (!bookingStatus.isEmpty() && bookingStatus != "All") q.addBindValue(bookingStatus);
    
    if (!q.exec()) {
        lastErrorMessage = q.lastError().text();
        return 0; 
    }
    
    if (q.next()) return q.value(0).toInt(); 
    return 0;
}
