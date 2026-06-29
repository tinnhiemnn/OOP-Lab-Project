#pragma once

#include "models/User.h"

class Customer : public User
{
private:
    std::string phone;

public:
    Customer() = default;
    Customer(std::string id, std::string name, std::string email, std::string phone)
        : User(std::move(id),
               std::move(name),
               std::move(email)),
          phone(std::move(phone)) {}

    const std::string& getPhone() const { return phone; }

    void setPhone(const std::string& value)
    {
        phone = value;
    }
};