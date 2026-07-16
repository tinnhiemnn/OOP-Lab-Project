#pragma once

#include "services/BookingService.h"
#include <QString>
#include <QDate>

class BookingController {
private:
    BookingService& bookingService;

public:
    // Nhận vào Service thông qua cơ chế Dependency Injection (tham chiếu)
    explicit BookingController(BookingService& service);

    // Xử lý yêu cầu đặt phòng từ giao diện
    bool processCreateBooking(const QString& customerId,
                              const QString& roomId,
                              const QDate& checkIn,
                              const QDate& checkOut,
                              const QString& receptionistId,
                              const QString& groupCode,
                              int buffetQty,
                              bool laundry,
                              bool decoration,
                              const QString& decorationNote,
                              QString& error);

    // Xử lý yêu cầu nhận phòng từ giao diện
    bool processCheckIn(const QString& bookingId, QString& error);

    // Xử lý yêu cầu trả phòng & thanh toán từ giao diện
    bool processCheckOut(const QString& bookingId, double& finalAmountOut, QString& error);

    // Xử lý yêu cầu hủy đơn đặt phòng từ giao diện
    bool processCancelBooking(const QString& bookingId, QString& error);
};