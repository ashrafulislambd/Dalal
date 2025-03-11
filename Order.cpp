#include "Order.h"

void Order::addProduct(Product* product) {
    products[product]++;
}

vector<Product*> Order::getProducts() const {
    vector<Product*>* productList = new vector<Product*>();
    for(auto& [product, count] : products) {
        if(count > 0)
            productList->push_back(product);
    }
    return *productList;
}

int Order::getProductQuantity(Product* product) {
    return products[product];
}

void Order::deleteProduct(Product* product) {
    products[product] = 0;
}

int Order::getTime() const {
    return time;
}

void Order::setTime(int time) {
    if(time < 0) {
        throw OutOfBoundException();
    }

    this->time = time;
}

float Order::getDiscount() const {
    return discount;
}

void Order::setDiscount(float discount) {
    if(discount < 0 || discount > 100) {
        throw OutOfBoundException();
    }

    this->discount = discount;
}

float Order::getTotalPrice() const {
    float totalPrice = 0;
    for(auto [product, count] : products) {
        totalPrice += product->getPrice() * count;
    }

    totalPrice = ((100.0 - discount)/100) * totalPrice;
    return totalPrice;
}