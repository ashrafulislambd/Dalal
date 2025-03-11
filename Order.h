#pragma once

#include <vector>
#include <map>

#include "Product.h"
using namespace std;

class Order {
private:
    map<Product*, int> products;
    int time;
    float discount;

    class OutOfBoundException {};
public:
    void addProduct(Product* product);
    vector<Product*> getProducts() const;
    int getProductQuantity(Product* product);
    void deleteProduct(Product* product);
    int getTime() const;
    void setTime(int time);
    float getDiscount() const;
    void setDiscount(float discount);
    float getTotalPrice() const;
};