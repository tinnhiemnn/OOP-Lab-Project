#pragma once

#include <QSqlDatabase>
#include <QString>

class DatabaseManager {
private:
    QSqlDatabase db;
    QString lastErrorMessage;
    
    // Private constructor
    DatabaseManager();
    
    // Delete copied
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QString resolveDatabasePath(const QString& databasePath) const;

public:
    static DatabaseManager& getInstance();
    bool openDatabase(const QString& databasePath = QString());
    bool initializeSchema();
    QSqlDatabase database() const;
    QString lastError() const;

};

