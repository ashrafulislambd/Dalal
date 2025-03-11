#pragma once

#include <string>

using namespace std;

class User {
private:
    string name;
    string username;
    string passwordHash;
public:
    string getUsername() const;
    string getName() const;
    string getPasswordHash() const;
    User(string name, string username, string password);
    virtual ~User() {};
};