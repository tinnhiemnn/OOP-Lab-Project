
#pragma once

#include <QDir>
#include <QFile>
#include <QUuid>
#include <QString>

#include "database/DatabaseManager.h"
#include "models/Room.h"
#include "patterns/RoomFactory.h"
#include "repositories/BookingRepository.h"
#include "repositories/RoomRepository.h"

inline QString makeTemporaryDatabasePath()
{
    const QString fileName = QStringLiteral("hotel_test_%1.db").arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
    return QDir::temp().absoluteFilePath(fileName);
}

inline bool openTemporaryDatabase(QString& outPath, QString& error)
{
    outPath = makeTemporaryDatabasePath();
    DatabaseManager::getInstance().closeConnection();
    if (!DatabaseManager::getInstance().openDatabase(outPath)) {
        error = DatabaseManager::getInstance().lastError();
        return false;
    }
    return true;
}

inline void cleanupTemporaryDatabase(const QString& path)
{
    if (path.isEmpty()) return;
    QFile::remove(path);
}

inline std::unique_ptr<Room> makeRoom(const QString& roomId, RoomType type, double price, RoomStatus status, int beds)
{
    return RoomFactory::createRoom(type, roomId, price, status, beds);
}
