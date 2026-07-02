#pragma once

#include "models/User.h"
#include <QString>

class Customer : public User
{
private:
    QString phone;

public:
    Customer() = default;
    Customer(QString id, QString name, QString email, QString phone)
        : User(std::move(id),
               std::move(name),
               std::move(email)),
          phone(std::move(phone)) {}

    const QString& getPhone() const { return phone; }

    void setPhone(const QString& value)
    {
        phone = value;
    }
};