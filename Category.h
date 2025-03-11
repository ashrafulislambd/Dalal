#pragma once

#include <string>
#include <set>
using namespace std;

class Category {
public:
    string name;
};

class CategoryManager {
private:
    set<Category*> categories;

    class DuplicateCategoryException {};
public:
    set<Category*> getCategories();
    int getCount();
    void addCategory(Category* category);
    bool removeCategory(Category* category);
};