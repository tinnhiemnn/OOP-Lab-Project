#pragma once

#include "models/User.h"
#include <string>

class Receptionist : public User
{
private:
    std::string employeeId;

public:
    Receptionist() = default;

    Receptionist(std::string id, std::string name, std::string email, std::string password, std::string employeeId)
        : User(std::move(id),
        std::move(name),
        std::move(email),
        std::move(password)),
        employeeId(std::move(employeeId)) {}

    const std::string& getEmployeeId() const { return employeeId; }

    void setEmployeeId(const std::string& value)
    {
        employeeId = value;
    }
};