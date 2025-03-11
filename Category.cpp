#include "Category.h"

set<Category*> CategoryManager::getCategories() {
    return categories;
}

int CategoryManager::getCount() {
    return categories.size();
}

void CategoryManager::addCategory(Category* category) {
    for(Category* icategory : categories) {
     	if(icategory->name == category->name) {
     		throw DuplicateCategoryException();
     	}
    }
    categories.insert(category);
}

bool CategoryManager::removeCategory(Category* category) {
    if(categories.find(category) == categories.end()) {
        return false;
    }

    categories.erase(category);
    return true;
}