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
            if (DateUtils::datesOverlap(checkIn, checkOut, existing.getCheckIn(), existing.getCheckOut())) {
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

bool BookingService::createMultiBookings(const QString& customerId, const std::vector<QString>& roomIds, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, const std::vector<RoomServiceSelection>& services, QString& error)
{
    if (roomIds.empty()) {
        error = "No rooms selected!";
        return false;
    }

    if (services.size() != roomIds.size()) {
        error = "Service information does not match the selected rooms.";
        return false;
    }

    if (!DateUtils::isDateRangeValid(checkIn, checkOut)) {
        error = "Check-out date must be after check-in date.";
        return false;
    }

    if (checkIn < QDate::currentDate()) {
        error = "It is not possible to create a booking for a past date!";
        return false;
    }

    if (!bookings.startTransaction())
    {
        error = bookings.lastError();
        return false;
    }

    QString groupCode = makeGroupCode();
    QString baseBookingId = makeBookingId();
    int index = 1;

    for (size_t i = 0; i < roomIds.size(); ++i)
    {
        const auto& roomId = roomIds[i];
        const auto& service = services[i];

        // Kiểm tra phòng có tồn tại không
        auto roomPtr = rooms.findById(roomId);
        if (!roomPtr) {
            bookings.rollbackTransaction();
            error = "Room information not found!";
            return false;
        }

        if (roomPtr->getStatus() == RoomStatus::Maintenance) {
            bookings.rollbackTransaction();
            error = "The room is under maintenance and cannot be booked!";
            return false;
        }

        // Kiểm tra trùng lịch phòng
        if (hasConflict(roomId, checkIn, checkOut)) {
            bookings.rollbackTransaction();
            error = "The room has already been booked or is in use during this period!";
            return false;
        }

        //Sinh ma don dat phong
        QString bookingId = baseBookingId + QString::number(index++);

        Booking newBooking(bookingId, customerId, receptionistId, roomId, groupCode, checkIn, checkOut, BookingStatus::Booked);
        
        // Thiết lập dịch vụ đi kèm
        newBooking.setBuffetQuantity(service.buffetQty);
        newBooking.setLaundry(service.laundry);
        newBooking.setDecoration(service.decoration);
        newBooking.setDecorationNote(service.decorationNote);

        // Lưu vào database
        if (!bookings.add(newBooking)) {
            bookings.rollbackTransaction();
            error = "Database error when saving the booking: " + bookings.lastError();
            return false;
        }
    }

    if (!bookings.commitTransaction()) {
        bookings.rollbackTransaction();
        error = bookings.lastError();
        return false;
    }

    return true;
}

bool BookingService::checkIn(const QString& bookingId, QString& error) {
    auto target = bookings.findById(bookingId);
    if (!target) {
        error = "Booking ID not found!";
        return false;
    }

    Booking selectedBooking = target.value();
    QString groupCode = selectedBooking.getGroupCode();

    std::vector<Booking> groupBookings = bookings.search(groupCode);
    if (groupBookings.empty()) {
        groupBookings.push_back(selectedBooking);
    }

    std::vector<Booking> bookingsToCheckIn;
    for (const auto& b : groupBookings) {
        if (b.getGroupCode() == groupCode && b.getStatus() == BookingStatus::Booked) {
            bookingsToCheckIn.push_back(b);
        }
    }

    if (bookingsToCheckIn.empty()) {
        error = "No rooms in this group are ready for check-in!";
        return false;
    }

    for (const auto& b : bookingsToCheckIn) {
        auto roomPtr = rooms.findById(b.getRoomId());
        if (!roomPtr) {
            error = "Room " + b.getRoomId() + " not found!";
            return false;
        }
        if (roomPtr->getStatus() != RoomStatus::Available) {
            error = "Room " + b.getRoomId() + " is not ready for check-in (Status: " + Room::statusToString(roomPtr->getStatus()) + ")!";
            return false;
        }
    }

    if (!bookings.startTransaction()) {
        error = "Failed to start transaction: " + bookings.lastError();
        return false;
    }
    for (auto& b : bookingsToCheckIn) {
        b.markCheckedIn();
        if (!bookings.update(b)) {
            bookings.rollbackTransaction();
            error = "Error updating reservation status for " + b.getId() + ": " + bookings.lastError();
            return false;
        }

        if (!rooms.updateStatus(b.getRoomId(), RoomStatus::InUse)) {
            bookings.rollbackTransaction();
            error = "Error updating room status for " + b.getRoomId() + ": " + rooms.lastError();
            return false;
        }
    }
    if (!bookings.commitTransaction()) {
        bookings.rollbackTransaction();
        error = "Failed to commit check-in transaction: " + bookings.lastError();
        return false;
    }

    return true;
}

bool BookingService::checkOut(const QString& bookingId, QString& error) {
    auto target = bookings.findById(bookingId);
    if (!target) {
        error = "Booking ID not found!";
        return false;
    }

    Booking selectedBooking = target.value();
    QString groupCode = selectedBooking.getGroupCode();
    
    std::vector<Booking> groupBookings = bookings.search(groupCode);
    if (groupBookings.empty()) {
        groupBookings.push_back(selectedBooking);
    }
    
    std::vector<Booking> bookingsToCheckOut;
    for (const auto& b : groupBookings) {
        if (b.getGroupCode() == groupCode && b.getStatus() == BookingStatus::CheckedIn) {
            bookingsToCheckOut.push_back(b);
        }
    }
    if (bookingsToCheckOut.empty()) {
        error = "No rooms in this group are ready for check-out!";
        return false;
    }

    for (const auto& b : bookingsToCheckOut) {
        auto roomPtr = rooms.findById(b.getRoomId());
        if (!roomPtr) {
            error = "Room " + b.getRoomId() + " not found!";
            return false;
        }
    }

    if (!bookings.startTransaction()) {
        error = "Failed to start transaction: " + bookings.lastError();
        return false;
    }

    for (auto& b : bookingsToCheckOut) {    
        b.markCheckedOut();
        if (!bookings.update(b)) {
            bookings.rollbackTransaction();
            error = "Error updating booking status for " + b.getId() + ": " + bookings.lastError();
            return false;
        }

        if (!rooms.updateStatus(b.getRoomId(), RoomStatus::NeedCleaning)) {
            bookings.rollbackTransaction();
            error = "Error updating room status for " + b.getRoomId() + ": " + rooms.lastError();
            return false;
        }
    }

    if (!bookings.commitTransaction()) {
        bookings.rollbackTransaction();
        error = "Failed to commit checkout transaction: " + bookings.lastError();
        return false;
    }

    return true;

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