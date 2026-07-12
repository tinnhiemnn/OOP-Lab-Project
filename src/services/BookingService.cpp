#include "services/BookingService.h"
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

//Tính toán chi phí đơn đặt phòng (gồm ngày ở và dịch vụ đi kèm)
double BookingService::calculateBookingCost(const Booking& booking, double pricePerNight) const {
    qint64 nights = booking.getCheckIn().daysTo(booking.getCheckOut());
    if (nights <= 0) nights = 1; // Tối thiểu 1 đêm

    double baseRoomCost = nights * pricePerNight;

    // Bảng giá dịch vụ mẫu
    const double PRICE_PER_BUFFET = 120000.0;    
    const double PRICE_LAUNDRY = 50000.0;        
    const double PRICE_DECORATION = 250000.0;    

    double extraServiceCost = 0.0;
    extraServiceCost += booking.getBuffetQuantity() * PRICE_PER_BUFFET;
    if (booking.isUsingLaundry()) extraServiceCost += PRICE_LAUNDRY;
    if (booking.isUsingDecoration()) extraServiceCost += PRICE_DECORATION;

    return baseRoomCost + extraServiceCost;
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
        error = "Ngày Check-out phải sau ngày Check-in!";
        return false;
    }
    if (checkIn < QDate::currentDate()) {
        error = "Không thể tạo đơn đặt phòng cho ngày trong quá khứ!";
        return false;
    }

    // Kiểm tra phòng có tồn tại không
    auto roomPtr = rooms.findById(roomId);
    if (!roomPtr) {
        error = "Không tìm thấy thông tin phòng!";
        return false;
    }
    if (roomPtr->getStatus() == RoomStatus::Maintenance) {
        error = "Phòng đang bảo trì, không thể đặt phòng!";
        return false;
    }

    // Kiểm tra trùng lịch phòng
    if (hasConflict(roomId, checkIn, checkOut)) {
        error = "Phòng đã có người đặt hoặc đang sử dụng trong khoảng thời gian này!";
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
        error = "Lỗi Database khi lưu Booking: " + bookings.lastError();
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

bool BookingService::checkOut(const QString& bookingId, double& finalAmountOut, QString& error) {
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

    // Tính tổng chi phí (Ngày ở * Giá phòng + Tiền dịch vụ phát sinh)
    finalAmountOut = calculateBookingCost(booking, roomPtr->getBasePrice());

    // Đổi trạng thái đơn thành CheckedOut
    booking.markCheckedOut();
    if (!bookings.update(booking)) {
        error = "Lỗi cập nhật trạng thái đơn sang CheckedOut: " + bookings.lastError();
        return false;
    }

    // Đổi trạng thái phòng sang NeedCleaning
    if (!rooms.updateStatus(booking.getRoomId(), RoomStatus::NeedCleaning)) {
        error = "Lỗi cập nhật trạng thái phòng sang NeedCleaning: " + rooms.lastError();
        return false;
    }

    return true;
}

bool BookingService::cancelBooking(const QString& bookingId, QString& error) {
    auto target = bookings.findById(bookingId);
    if (!target.has_value()) {
        error = "Không tìm thấy mã đặt phòng!";
        return false;
    }

    Booking booking = target.value();
    if (booking.getStatus() == BookingStatus::CheckedOut || booking.getStatus() == BookingStatus::Cancelled) {
        error = "Đơn đặt phòng này đã hoàn thành hoặc đã hủy trước đó!";
        return false;
    }

    // Đổi trạng thái đơn thành Cancelled
    booking.cancelBooking();
    if (!bookings.update(booking)) {
        error = "Hủy đơn thất bại trong cơ sở dữ liệu!";
        return false;
    }

    // Nếu phòng hiện tại đang được sử dụng (InUse) bởi chính đơn đặt phòng bị hủy này, trả trạng thái về Available
    auto roomPtr = rooms.findById(booking.getRoomId());
    if (roomPtr && roomPtr->getStatus() == RoomStatus::InUse) {
        if (!rooms.updateStatus(booking.getRoomId(), RoomStatus::Available)) {
            error = "Lỗi cập nhật trạng thái phòng về Available: " + rooms.lastError();
            return false;
        }
    }

    return true;
}