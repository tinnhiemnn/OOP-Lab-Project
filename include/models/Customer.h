#pragma once

#include "models/User.h"
#include <string>

class Customer : public User
{
private:
    std::string phone;

public:
    Customer() = default;
    Customer(std::string id, std::string name, std::string email, std::string password, std::string phone)
        : User(std::move(id),
        std::move(name),
        std::move(email),
        std::move(password)),
        phone(std::move(phone)) {}

    const std::string& getPhone() const { return phone; }

    void setPhone(const std::string& value)
    {
        phone = value;
    }
};