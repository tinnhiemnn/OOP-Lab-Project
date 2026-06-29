#pragma once

#include "models/User.h"

class Receptionist : public User
{
public:
    Receptionist() = default;

    Receptionist(std::string id, std::string name, std::string email)
        : User(std::move(id),
        std::move(name),
        std::move(email)) {}
    
    std::string role() const override
    {
        return "Receptionist";
    }
};