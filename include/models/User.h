#pragma once

#include <string>

class User
{
private:
    std::string id;
    std::string name;
    std::string email;

public:
    User() = default;
    User(std::string id,
         std::string name,
         std::string email)
        : id(std::move(id)),
          name(std::move(name)),
          email(std::move(email)) {}
    virtual ~User() = default;

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getEmail() const { return email; }

    void setId(const std::string& value) { id = value; }
    void setName(const std::string& value) { name = value; }
    void setEmail(const std::string& value) { email = value; }
    
    virtual std::string role() const
    {
        return "User";
    }
};