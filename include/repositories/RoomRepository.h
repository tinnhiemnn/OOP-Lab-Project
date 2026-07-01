#pragma once

#include "models/Room.h"

#include <memory>
#include <vector>

class RoomRepository {
public:
    bool add(const Room& room, int beds);
    bool update(const Room& room, int beds);
    bool remove(const QString& id);
    bool updateStatus(const QString& id, RoomStatus status);
    std::vector<std::unique_ptr<Room>> findAll();
    std::vector<std::unique_ptr<Room>> search(const QString& typeFilter, const QString& statusFilter);
    std::unique_ptr<Room> findById(const QString& id);
    QString lastError() const { return lastErrorMessage; }

private:
    QString lastErrorMessage;
};

