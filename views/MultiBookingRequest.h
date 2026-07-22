#pragma once

#include <QString>
#include <QDate>
#include <vector>

struct SingleRoomRequest {
    QString roomId;
    int buffetQty = 0;
    bool laundryService = false;
    bool decorService = false;
    QString decorNote;
};

// View gom dữ liệu từ form + roomRows thành 1 request duy nhất rồi gửi
struct MultiBookingRequest {
    QString customerId;
    QString receptionistId;
    QDate checkIn;
    QDate checkOut;
    QString groupCode;

    std::vector<SingleRoomRequest> rooms;
};