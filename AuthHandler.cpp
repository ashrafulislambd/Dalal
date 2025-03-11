#include "AuthHandler.h"
#include "Customer.h"
#include "Vendor.h"

#include <CtrlLib/CtrlLib.h>
#include <plugin/sqlite3/Sqlite3.h>
using namespace Upp;
using namespace std;

Customer* AuthHandler::registerCustomer(string username, string name, string password)
{
    for(User* user : users) {
        if(user->getUsername() == username) {
            throw UserAlreadyExistsException();
        }
    }

    Customer* newUser = new Customer(name, username, password);
    users.push_back(newUser);
    return newUser;
}

Vendor* AuthHandler::registerVendor(string username, string name, string password)
{
    for(User* user : users) {
        if(user->getUsername() == username) {
            throw UserAlreadyExistsException();
        }
    }

    Vendor* newUser = new Vendor(name, username, password);
    users.push_back(newUser);
    return newUser;
}

User* AuthHandler::loginUser(string username, string password)
{
    for(User* user : users) {
        if(user->getUsername() == username) {
            if(user->getPasswordHash() == password) {
                return user;
            } else {
                throw InvalidCredentialsException();
            }
        }
    }

    throw InvalidCredentialsException();
}

vector<User*> AuthHandler::getUsers() const {
	return this->users;	
}

void AuthHandler::saveUsers(Sqlite3Session* conn) {
	for(User* user : this->users) {
		try {
			Sql sql(*conn);
			Customer* customer = dynamic_cast<Customer*>(user);
			Vendor* vendor = dynamic_cast<Vendor*>(user);
			string type;
			if(vendor)
				type = "vendor";
			else if(customer)
				type = "customer";
			bool success = sql.Execute("INSERT INTO user (id, username, name, password, type) VALUES (NULL, ?, ?, ?, ?)"
				, user->getUsername(), user->getName(), user->getPasswordHash(), type);
			int id = sql.GetInsertedId();
			if(!success) {
				sql.Execute("SELECT id FROM user WHERE username=?", user->getUsername());
				sql.Fetch();
				id = sql[0];	
			}
			
			if(vendor) {
				vendor->saveData(conn, id);
			}
		} catch (...) {}
	}
}

void AuthHandler::loadUsers(Sqlite3Session* conn) {
	try {
		Sql sql(*conn);
		sql.Execute("SELECT * FROM user");
		while(sql.Fetch()) {
			string username = sql[1].ToString().ToStd();
			string name = sql[2].ToString().ToStd();
			string password = sql[3].ToString().ToStd();
			string type = sql[4].ToString().ToStd();
			
			try {
				loginUser(username, password);	
			} catch (AuthHandler::InvalidCredentialsException) {
				if(type == "customer") {
					this->registerCustomer(username, name, password);	
				} else if(type == "vendor") {
					Vendor* vendor = this->registerVendor(username, name, password);
					vendor->loadData(conn, sql[0]);	
				}
			}
		}
	} catch(...) {}
}