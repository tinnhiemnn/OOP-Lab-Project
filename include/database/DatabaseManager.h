#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class DatabaseManager {
private:
    QSqlDatabase db;
    // Private constructor
    DatabaseManager() {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("../data/hotel.db"); 
    }

public:
    // Delete copied
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager& getInstance() {
        static DatabaseManager instance;
        return instance;
    }

    bool openConnection() {
        if (!db.isOpen()) {
            if (!db.open()) {
                return false;
            }
        }
        return true;
    }

    void closeConnection() {
        if (db.isOpen()) {
            db.close();
        }
    }

};

#endif // DATABASEMANAGER_H