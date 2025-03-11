#pragma once

#include <plugin/sqlite3/Sqlite3.h>
using namespace Upp;

#include <vector>
#include "User.h"
#include "Product.h"
#include "Order.h"

class Vendor : public User {
private:
    vector<Product*> products;
    vector<Order*> orders;
public:
	Vendor(string name, string username, string password)
	 : User(name, username, password) {};
    class OrderNotFoundException {};
    class NoReviewException {};

    int getProductCount() const;
    vector<Product*> getProducts() const;
    void addProduct(Product* product);
    int getOrderCount() const;
    vector<Order*> getOrders() const;
    float getAvgRating() const;
    void saveData(Sqlite3Session* conn, int vendor_id);
    void loadData(Sqlite3Session* conn, int vendor_id);
};