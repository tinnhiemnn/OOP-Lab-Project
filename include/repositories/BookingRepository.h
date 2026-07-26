#pragma once

#include "models/Booking.h"

#include <optional>
#include <vector>

class BookingRepository {
public:
    bool startTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
    bool add(const Booking& booking);
    bool update(const Booking& booking);
    std::vector<Booking> findAll();
    std::vector<Booking> search(const QString& keyword);
    std::optional<Booking> findById(const QString& id);
    int countBookings(const QString& roomType, const QString& bookingStatus);
    QString lastError() const { return lastErrorMessage; }

private:
    QString lastErrorMessage;
};

