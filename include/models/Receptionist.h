#pragma once

#include "models/User.h"
#include <QString>

class Receptionist : public User
{
private:
    QString password;

public:
    Receptionist() = default;

    Receptionist(QString id, QString name, QString email, QString password)
        : User(std::move(id),
               std::move(name),
               std::move(email)),
          password(std::move(password)) {}
    
    const QString& getPassword() const { return password; }

    void setPassword(const QString& value)
    {
        password = value;
    }
};