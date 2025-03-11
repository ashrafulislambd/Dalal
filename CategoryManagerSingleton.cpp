#include "CategoryManagerSingleton.h"

CategoryManagerSingleton* CategoryManagerSingleton::instance = nullptr;
std::mutex CategoryManagerSingleton::mutex;

CategoryManagerSingleton* CategoryManagerSingleton::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new CategoryManagerSingleton();
    }
    return instance;
}

CategoryManagerSingleton::~CategoryManagerSingleton() {
    delete categoryManager;
}

void CategoryManagerSingleton::destroyInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    delete instance;
    instance = nullptr;
}
