#include "EMIOrder.h"

EMIOrder::EMIOrder(float due, float minimumPayment) {
    if(due < 0 || minimumPayment < 0) 
        throw InvalidAmountException();
    this->due = due;
    this->minimumPayment = minimumPayment;
}

bool EMIOrder::getStatus() const {
    return this->due <= 0.00001;
}

void EMIOrder::payInstallment(float amount) {
    if(amount < minimumPayment) 
        throw InvalidAmountException();
    if(amount >= due + 0.00001)
        throw InvalidAmountException();
    this->due -= amount;
}

