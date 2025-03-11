#pragma once

#include "Order.h"

class OneTimeOrder : public Order {
private:
    bool status;

public:
    OneTimeOrder();
    bool getStatus() const;
    void markAsCompleted();
};