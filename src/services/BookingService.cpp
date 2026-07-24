#include "services/BookingService.h"
#include "utils/DateUtils.h"

#include <QDateTime>

BookingService::BookingService(BookingRepository& bookings, RoomRepository& rooms)
    : bookings(bookings), rooms(rooms) {}

//Kiểm tra đè lịch phòng
bool BookingService::hasConflict(const QString& roomId, const QDate& checkIn, const QDate& checkOut, const QString& excludeBookingId) const {
    auto allBookings = bookings.search(roomId);

    for (const auto& existing : allBookings) {
        if (!excludeBookingId.isEmpty() && existing.getId() == excludeBookingId) continue;
        if (existing.getRoomId() != roomId) continue;

        //kiểm tra xung đột với các đơn đang hoạt động (Booked hoặc CheckedIn)
        if (existing.isActive()) {
            if (checkIn < existing.getCheckOut() && checkOut > existing.getCheckIn()) {
                return true; 
            }
        }
    }
    return false;
}

//Tạo Booking ID ngẫu nhiên dựa trên timestamp
QString BookingService::makeBookingId() const {
    return "BK_" + QString::number(QDateTime::currentMSecsSinceEpoch());
}

QString BookingService::makeGroupCode() const {
    return "GRP_" + QString::number(QDateTime::currentMSecsSinceEpoch());
}

bool BookingService::createMultiBookings(const QString& customerId, const std::vector<QString>& roomIds, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, int buffetQty, bool laundry, bool decoration, const QString& decorationNote, QString& error)
{
    if (roomIds.empty()) {
        error = "No rooms selected!";
        return false;
    }

    if (checkIn >= checkOut) {
        error = "Check-out date must be after check-in date.";
        return false;
    }

    if (checkIn < QDate::currentDate()) {
        error = "It is not possible to create a booking for a past date!";
        return false;
    }

    QString groupCode = makeGroupCode();

    for (const auto& roomId : roomIds)
    {
        // Kiểm tra phòng có tồn tại không
        auto roomPtr = rooms.findById(roomId);
        if (!roomPtr) {
            error = "Room information not found!";
            return false;
        }

        if (roomPtr->getStatus() == RoomStatus::Maintenance) {
            error = "The room is under maintenance and cannot be booked!";
            return false;
        }

        // Kiểm tra trùng lịch phòng
        if (hasConflict(roomId, checkIn, checkOut)) {
            error = "The room has already been booked or is in use during this period!";
            return false;
        }

        // Sinh mã đơn đặt phòng
        QString bookingId = makeBookingId();

        // Khởi tạo đối tượng Booking
        Booking newBooking(bookingId, customerId, receptionistId, roomId, groupCode, checkIn, checkOut, BookingStatus::Booked);
        
        // Thiết lập dịch vụ đi kèm
        newBooking.setBuffetQuantity(buffetQty);
        newBooking.setLaundry(laundry);
        newBooking.setDecoration(decoration);
        newBooking.setDecorationNote(decorationNote);

        // Lưu vào database
        if (!bookings.add(newBooking)) {
            error = "Database error when saving the booking: " + bookings.lastError();
            return false;
        }
    }
    return true;
}

bool BookingService::checkIn(const QString& bookingId, QString& error) {
    auto target = bookings.findById(bookingId);
    if (!target) {
        error = "Booking id not found!";
        return false;
    }

    Booking booking = target.value();
    if (booking.getStatus() != BookingStatus::Booked) {
        error = "This reservation is not in a check-in ready status!";
        return false;
    }

    // Kiểm tra trạng thái phòng hiện tại có Available
    auto roomPtr = rooms.findById(booking.getRoomId());
    if (!roomPtr) {
        error = "Room not found.";
        return false;
    }
    if (roomPtr->getStatus() != RoomStatus::Available) {
        error = "The current room is not ready for check-in (Status: " + Room::statusToString(roomPtr->getStatus()) + ")!";
        return false;
    }

    // Cập nhật trạng thái đơn đặt phòng sang CheckedIn
    booking.markCheckedIn();
    if (!bookings.update(booking)) {
        error = "Error updating reservation status: " + bookings.lastError();
        return false;
    }

    // Đồng bộ trạng thái phòng sang "InUse" (Đang sử dụng)
    if (!rooms.updateStatus(booking.getRoomId(), RoomStatus::InUse)) {
        error = "Room status update error: " + rooms.lastError();
        return false;
    }

    return true;
}

bool BookingService::checkOut(const QString& bookingId, QString& error) {
    auto target = bookings.findById(bookingId);
    if (!target) {
        error = "Booking id not found!";
        return false;
    }

    Booking booking = target.value();
    if (booking.getStatus() != BookingStatus::CheckedIn) {
        error = "This room has not been checked in, so it cannot be checked out!";
        return false;
    }

    auto roomPtr = rooms.findById(booking.getRoomId());
    if (!roomPtr) {
        error = "Room not found.";
        return false;
    }

    // Đổi trạng thái đơn thành CheckedOut
    booking.markCheckedOut();
    if (!bookings.update(booking)) {
        error = "Error updating order status to CheckedOut: " + bookings.lastError();
        return false;
    }

    // Đổi trạng thái phòng sang NeedCleaning
    if (!rooms.updateStatus(booking.getRoomId(), RoomStatus::NeedCleaning)) {
        error = "Error updating order status to NeedCleaning: " + rooms.lastError();
        return false;
    }

    return true;
}

bool BookingService::cancelBooking(const QString& bookingId, QString& error) {
    auto target = bookings.findById(bookingId);
    if (!target) {
        error = "Booking ID not found!";
        return false;
    }

    Booking booking = target.value();
    if (booking.getStatus() == BookingStatus::CheckedOut || booking.getStatus() == BookingStatus::Cancelled) {
        error = "This reservation has already been completed or canceled previously!";
        return false;
    }

    // Đổi trạng thái đơn thành Cancelled
    booking.cancelBooking();
    if (!bookings.update(booking)) {
        error = "Cancellation of the order failed in the database!";
        return false;
    }

    // Nếu phòng hiện tại đang được sử dụng bởi chính đơn đặt phòng bị hủy này, trả trạng thái về Available
    auto roomPtr = rooms.findById(booking.getRoomId());
    if (roomPtr && roomPtr->getStatus() == RoomStatus::InUse) {
        if (!rooms.updateStatus(booking.getRoomId(), RoomStatus::Available)) {
            error = "Error updating order status to Available: " + rooms.lastError();
            return false;
        }
    }

    return true;
}