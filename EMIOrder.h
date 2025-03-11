#pragma once

#include <ctime>

class EMIOrder {
private:
    float due, minimumPayment;
    time_t expiryDate;

    class InvalidAmountException {};
    
public:
    EMIOrder(float due, float minimumPayment);
    bool getStatus() const;
    void payInstallment(float amount);
};