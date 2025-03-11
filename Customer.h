#pragma once

#include <vector>
#include <map>
#include "User.h"
#include "Order.h"

using namespace std;

class Customer : public User {
private:
    vector<Order*> history;
    map<Product*, int> wishlist, cart;
    string shippingAddress;
public:
	Customer(string name, string username, string password) :
		User(name, username, password) {};
    void addToCart(Product* product);
    void makeOrder();
    void setShippingAddress(string address);
    map<Product*, int> getCart() const;
    string getShippingAddress() const;
};