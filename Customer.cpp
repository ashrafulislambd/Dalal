#include "Customer.h"
#include <functional>

void Customer::addToCart(Product* product)
{
    cart[product]++;
}

void Customer::makeOrder()
{
    Order* newOrder = new Order();
    for(auto& [product, cnt] : cart) {
        for(int i=0; i<cnt; i++) {
            newOrder->addProduct(product);
        }
    }

    history.push_back(newOrder);
    cart.clear();
}

void Customer::setShippingAddress(string address)
{
    this->shippingAddress = address;
}

string Customer::getShippingAddress() const
{
    return shippingAddress;
}

map<Product*, int> Customer::getCart() const 
{
	return cart;
}