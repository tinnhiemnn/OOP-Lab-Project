#pragma once

#include <algorithm>
#include <QString>
#include <QDate>

enum class BookingStatus
{
    Booked,
    CheckedIn,
    CheckedOut,
    Cancelled
};

class Booking
{
private:
    QString id;
    QString customerId;
    QString receptionistId;
    QString roomId;
    QString groupCode;

    QDate checkIn;
    QDate checkOut;

    //dịch vụ thêm
    int buffetQuantity = 0;
    bool useLaundry = false;
    bool useDecoration = false;
    QString decorationNote;

    BookingStatus status = BookingStatus::Booked;

public:
    Booking() = default;
    Booking(QString id, QString customerId, QString receptionistId, QString roomId, QString groupCode, QDate checkIn, QDate checkOut, BookingStatus status = BookingStatus::Booked)
        : id(std::move(id)),
          customerId(std::move(customerId)),
          receptionistId(std::move(receptionistId)),
          roomId(std::move(roomId)),
          groupCode(std::move(groupCode)),
          checkIn(std::move(checkIn)),
          checkOut(std::move(checkOut)),
          status(status) {}

    const QString& getId() const { return id; }
    const QString& getCustomerId() const { return customerId; }
    const QString& getReceptionistId() const { return receptionistId; }
    const QString& getRoomId() const { return roomId; }
    const QString& getGroupCode() const { return groupCode; }

    const QDate& getCheckIn() const { return checkIn; }
    const QDate& getCheckOut() const { return checkOut; }

    BookingStatus getStatus() const { return status; }

    int getBuffetQuantity() const { return buffetQuantity; }
    bool isUsingLaundry() const { return useLaundry; }
    bool isUsingDecoration() const { return useDecoration; }
    const QString& getDecorationNote() const { return decorationNote; }

    void setStatus(BookingStatus value) { status = value; }

    void setBuffetQuantity(int value)
    {
        buffetQuantity = std::max(0, value);
    }

    void setLaundry(bool value)
    {
        useLaundry = value;
    }

    void setDecoration(bool value)
    {
        useDecoration = value;
    }

    void setDecorationNote(const QString& value)
    {
        decorationNote = value;
    }

    //booking
    void cancelBooking() { status = BookingStatus::Cancelled; }
    void markCheckedIn() { status = BookingStatus::CheckedIn; }
    void markCheckedOut() { status = BookingStatus::CheckedOut; }

    bool isActive() const
    {
        return status == BookingStatus::Booked 
            || status == BookingStatus::CheckedIn;
    }

    //chuyển booking -> String
    static QString statusToString(BookingStatus value)
    {
        if (value == BookingStatus::Booked) return "Booked";
        if (value == BookingStatus::CheckedIn) return "CheckedIn";
        if (value == BookingStatus::CheckedOut) return "CheckedOut";
        if (value == BookingStatus::Cancelled) return "Cancelled";
        return "Cancelled";
    }

    //chuyển String -> booking
    static BookingStatus statusFromString(const QString& value)
    {
        if (value == "Booked") return BookingStatus::Booked;
        if (value == "CheckedIn") return BookingStatus::CheckedIn;
        if (value == "CheckedOut") return BookingStatus::CheckedOut;
        if (value == "Cancelled") return BookingStatus::Cancelled;
        return BookingStatus::Cancelled;
    }
};