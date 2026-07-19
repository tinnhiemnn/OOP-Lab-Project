#include "services/BookingService.h"
#include "utils/DateUtils.h"
#include <QDateTime>
#include <algorithm>

BookingService::BookingService(BookingRepository& bookings, RoomRepository& rooms)
    : bookings(bookings), rooms(rooms) {}

//Kiểm tra đè lịch phòng
bool BookingService::hasConflict(const QString& roomId, const QDate& checkIn, const QDate& checkOut, const QString& excludeBookingId) const {
    auto allBookings = bookings.findAll();
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

bool BookingService::createBooking(const QString& customerId,
                                   const QString& roomId,
                                   const QDate& checkIn,
                                   const QDate& checkOut,
                                   const QString& receptionistId,
                                   const QString& groupCode,
                                   int buffetQty,
                                   bool laundry,
                                   bool decoration,
                                   const QString& decorationNote,
                                   QString& error) 
{
    if (checkIn >= checkOut) {
        error = "The check-out date must be after the check-in date!";
        return false;
    }
    if (checkIn < QDate::currentDate()) {
        error = "It is not possible to create a booking for a past date!";
        return false;
    }

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

    return true;
}

bool BookingService::checkIn(const QString& bookingId, QString& error) {
    auto target = bookings.findById(bookingId);
    if (!target.has_value()) {
        error = "Không tìm thấy mã đặt phòng!";
        return false;
    }

    Booking booking = target.value();
    if (booking.getStatus() != BookingStatus::Booked) {
        error = "Đơn đặt phòng này không ở trạng thái có thể Check-in!";
        return false;
    }

    // Kiểm tra trạng thái phòng hiện tại có Available
    auto roomPtr = rooms.findById(booking.getRoomId());
    if (!roomPtr) {
        error = "Không tìm thấy thông tin phòng liên kết với đơn này!";
        return false;
    }
    if (roomPtr->getStatus() != RoomStatus::Available) {
        error = "Phòng hiện tại không sẵn sàng để Check-in (Trạng thái: " + Room::statusToString(roomPtr->getStatus()) + ")!";
        return false;
    }

    // Cập nhật trạng thái đơn đặt phòng sang CheckedIn
    booking.markCheckedIn();
    if (!bookings.update(booking)) {
        error = "Lỗi cập nhật trạng thái đơn đặt phòng: " + bookings.lastError();
        return false;
    }

    // Đồng bộ trạng thái phòng sang "InUse" (Đang sử dụng)
    if (!rooms.updateStatus(booking.getRoomId(), RoomStatus::InUse)) {
        error = "Lỗi cập nhật trạng thái phòng: " + rooms.lastError();
        return false;
    }

    return true;
}

bool BookingService::checkOut(const QString& bookingId, QString& error) {
    auto target = bookings.findById(bookingId);
    if (!target.has_value()) {
        error = "Không tìm thấy mã đặt phòng!";
        return false;
    }

    Booking booking = target.value();
    if (booking.getStatus() != BookingStatus::CheckedIn) {
        error = "Phòng này chưa được Check-in, không thể Check-out!";
        return false;
    }

    // Lấy thông tin phòng để lấy đơn giá phòng/đêm
    auto roomPtr = rooms.findById(booking.getRoomId());
    if (!roomPtr) {
        error = "Không tìm thấy thông tin phòng liên kết với đơn này!";
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
    if (!target.has_value()) {
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

std::vector<std::unique_ptr<Room>> BookingService::checkAvailability(
    const QDate& checkIn,
    const QDate& checkOut,
    RoomType roomType,
    QString& error)
{
    std::vector<std::unique_ptr<Room>> availableRooms;

    auto rooms = this->rooms.findAll();
    auto bookings = this->bookings.findAll();

    for (auto& room : rooms)
    {
        if (room->getRoomType() != roomType) continue;

        bool occupied = false;

        for (const auto& booking : bookings)
        {
            if (booking.getRoomId() != room->getRoomId()) continue;

            // Chỉ xét các booking còn hiệu lực
            if (booking.getStatus() != BookingStatus::Booked && booking.getStatus() != BookingStatus::CheckedIn) continue;

            // Kiểm tra có trùng khoảng ngày
            if (DateUtils::datesOverlap(
                    checkIn.toString("yyyy-MM-dd"),
                    checkOut.toString("yyyy-MM-dd"),
                    booking.getCheckIn().toString("yyyy-MM-dd"),
                    booking.getCheckOut().toString("yyyy-MM-dd")))
            {
                occupied = true;
                break;
            }
        }

        if (!occupied)
        {
            availableRooms.push_back(std::move(room));
        }
    }

    return availableRooms;
}