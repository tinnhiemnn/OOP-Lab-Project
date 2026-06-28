#pragma once

#include <algorithm>
#include <string>
#include <utility>

enum class BookingStatus
{
    Pending,
    Booked,
    CheckedIn,
    CheckedOut,
    Cancelled
};

class Booking
{
private:
    std::string id;
    std::string customerId;
    std::string roomId;

    std::string checkIn;
    std::string checkOut;

    //dịch vụ thêm
    int buffetQuantity = 0;
    bool useLaundry = false;
    bool useDecoration = false;
    std::string decorationNote;

    BookingStatus status = BookingStatus::Pending;

public:
    Booking() = default;
    Booking(std::string id, std::string customerId, std::string roomId, std::string checkIn, std::string checkOut, BookingStatus status = BookingStatus::Pending)
        : id(std::move(id)),
          customerId(std::move(customerId)),
          roomId(std::move(roomId)),
          checkIn(std::move(checkIn)),
          checkOut(std::move(checkOut)),
          status(status)
    {
    }

    const std::string& getId() const { return id; }
    const std::string& getCustomerId() const { return customerId; }
    const std::string& getRoomId() const { return roomId; }

    const std::string& getCheckIn() const { return checkIn; }
    const std::string& getCheckOut() const { return checkOut; }

    BookingStatus getStatus() const { return status; }

    int getBuffetQuantity() const { return buffetQuantity; }
    bool isUsingLaundry() const { return useLaundry; }
    bool isUsingDecoration() const { return useDecoration; }
    const std::string& getDecorationNote() const { return decorationNote; }

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

    void setDecorationNote(const std::string& value)
    {
        decorationNote = value;
    }

    //booking
    void processBooking() { status = BookingStatus::Booked; }
    void cancelBooking() { status = BookingStatus::Cancelled; }
    void markCheckedIn() { status = BookingStatus::CheckedIn; }
    void markCheckedOut() { status = BookingStatus::CheckedOut; }

    bool isActive() const
    {
        return status == BookingStatus::Pending 
        || status == BookingStatus::Booked 
        || status == BookingStatus::CheckedIn;
    }

    //chuyển booking -> String
    static std::string statusToString(BookingStatus value)
    {
        if (value == BookingStatus::Booked) return "Booked";
        if (value == BookingStatus::CheckedIn) return "CheckedIn";
        if (value == BookingStatus::CheckedOut) return "CheckedOut";
        if (value == BookingStatus::Cancelled) return "Cancelled";
        return "Pending";
    }

    //chuyển String -> booking
    static BookingStatus statusFromString(const std::string& value)
    {
        if (value == "Booked") return BookingStatus::Booked;
        if (value == "CheckedIn") return BookingStatus::CheckedIn;
        if (value == "CheckedOut") return BookingStatus::CheckedOut;
        if (value == "Cancelled") return BookingStatus::Cancelled;
        return BookingStatus::Pending;
    }
};