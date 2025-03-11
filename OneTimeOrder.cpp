#include "OneTimeOrder.h"

OneTimeOrder::OneTimeOrder() {
    status = false;
}

void OneTimeOrder::markAsCompleted() {
    status = true;
}

bool OneTimeOrder::getStatus() const
{
    return status;
}
