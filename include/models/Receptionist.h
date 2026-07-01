#pragma once

#include "models/User.h"

class Receptionist : public User
{
public:
    Receptionist() = default;

    Receptionist(QString id, QString name, QString email)
        : User(std::move(id),
        std::move(name),
        std::move(email)) {}
    
    QString role() const override
    {
        return "Receptionist";
    }
};