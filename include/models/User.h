#pragma once

#include <string>

class User
{
private:
    QString id;
    QString name;
    QString email;

public:
    User() = default;
    User(QString id,
         QString name,
         QString email)
        : id(std::move(id)),
          name(std::move(name)),
          email(std::move(email)) {}
    virtual ~User() = default;

    const QString& getId() const { return id; }
    const QString& getName() const { return name; }
    const QString& getEmail() const { return email; }

    void setId(const QString& value) { id = value; }
    void setName(const QString& value) { name = value; }
    void setEmail(const QString& value) { email = value; }
    
    virtual QString role() const
    {
        return "User";
    }
};