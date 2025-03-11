#ifndef CATEGORY_MANAGER_SINGLETON_H
#define CATEGORY_MANAGER_SINGLETON_H

#include "Category.h"
#include <mutex>

class CategoryManagerSingleton {
private:
    static CategoryManagerSingleton* instance;
    static std::mutex mutex;
    
    CategoryManager* categoryManager;

    CategoryManagerSingleton() : categoryManager(nullptr) {}
    CategoryManagerSingleton(const CategoryManagerSingleton&) = delete;
    CategoryManagerSingleton& operator=(const CategoryManagerSingleton&) = delete;

public:
    static CategoryManagerSingleton* getInstance();

    CategoryManager* getCategoryManager() {
        return categoryManager;
    }

    void setCategoryManager(CategoryManager* cm) {
        if (categoryManager != nullptr) {
            delete categoryManager;
        }
        categoryManager = cm;
    }

    ~CategoryManagerSingleton();

    static void destroyInstance();
};

#endif

