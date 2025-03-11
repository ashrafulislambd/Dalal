#include "User.h"
#include <functional>

User::User(string name, string username, string password)
{
    this->name = name;
    this->username = username;
    this->passwordHash = password;
}

string User::getUsername() const
{
    return username;
}

string User::getName() const
{
    return name;
}

string User::getPasswordHash() const
{
    return passwordHash;
}
