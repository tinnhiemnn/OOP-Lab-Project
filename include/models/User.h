#pragma once

#include <string>

class User
{
private:
    std::string id;
    std::string name;
    std::string email;
    std::string password;

public:
    User() = default;
    User(std::string id,
         std::string name,
         std::string email,
         std::string password)
        : id(std::move(id)),
          name(std::move(name)),
          email(std::move(email)),
          password(std::move(password)) {}
    virtual ~User() = default;

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getEmail() const { return email; }
    const std::string& getPassword() const { return password; }

    void setId(const std::string& value) { id = value; }
    void setName(const std::string& value) { name = value; }
    void setEmail(const std::string& value) { email = value; }
    void setPassword(const std::string& value) { password = value; }

    virtual bool login(const std::string& inputEmail,const std::string& inputPassword) const
    {
        return (email == inputEmail && password == inputPassword);
    }

    virtual void logout() const{}

    virtual std::string role() const
    {
        return "User";
    }
};