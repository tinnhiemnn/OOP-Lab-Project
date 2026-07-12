#include "controllers/BookingController.h"

BookingController::BookingController(BookingService& service)
    : bookingService(service) {}

bool BookingController::processCreateBooking(const QString& customerId, const QString& roomId, const QDate& checkIn, const QDate& checkOut, const QString& receptionistId, const QString& groupCode, int buffetQty, bool laundry, bool decoration, const QString& decorationNote, QString& error)
{
    if (customerId.trimmed().isEmpty()) {
        error = "Vui lòng chọn hoặc nhập thông tin Khách hàng!";
        return false;
    }
    if (roomId.trimmed().isEmpty()) {
        error = "Vui lòng chọn phòng cần đặt!";
        return false;
    }
    if (receptionistId.trimmed().isEmpty()) {
        error = "Không xác định được ID Lễ tân đang thực hiện!";
        return false;
    }
    if (buffetQty < 0) {
        error = "Số lượng suất Buffet không được là số âm!";
        return false;
    }

    return bookingService.createBooking(customerId, roomId, checkIn, checkOut,
                                        receptionistId, groupCode, buffetQty,
                                        laundry, decoration, decorationNote, error);
}

bool BookingController::processCheckIn(const QString& bookingId, QString& error) {
    if (bookingId.trimmed().isEmpty()) {
        error = "Mã đặt phòng trống, không thể thực hiện Check-in!";
        return false;
    }

    return bookingService.checkIn(bookingId, error);
}

bool BookingController::processCheckOut(const QString& bookingId, double& finalAmountOut, QString& error) {
    if (bookingId.trimmed().isEmpty()) {
        error = "Mã đặt phòng trống, không thể thực hiện Check-out!";
        return false;
    }

    return bookingService.checkOut(bookingId, finalAmountOut, error);
}

bool BookingController::processCancelBooking(const QString& bookingId, QString& error) {
    if (bookingId.trimmed().isEmpty()) {
        error = "Mã đặt phòng không hợp lệ để hủy đơn!";
        return false;
    }

    return bookingService.cancelBooking(bookingId, error);
}