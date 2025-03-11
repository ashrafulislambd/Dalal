#pragma once

#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#include <vector>
#include "User.h"
#include "Customer.h"
#include "Vendor.h"

class AuthHandler {
private:
    vector<User*> users;
public:
	class UserAlreadyExistsException {};
    class InvalidCredentialsException {};
    Customer* registerCustomer(string username, string name, string password);
    Vendor* registerVendor(string username, string name, string password);
    User* loginUser(string username, string password); 
    vector<User*> getUsers() const;
    void saveUsers(Sqlite3Session* conn);
    void loadUsers(Sqlite3Session* conn);
};