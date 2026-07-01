#pragma once

#include "models/Booking.h"

#include <optional>
#include <vector>

class BookingRepository {
public:
    bool add(const Booking& booking);
    bool update(const Booking& booking);
    bool remove(const QString& id);
    std::vector<Booking> findAll();
    std::vector<Booking> search(const QString& keyword);
    std::optional<Booking> findById(const QString& id);
    QString lastError() const { return lastErrorMessage; }

private:
    QString lastErrorMessage;
};

