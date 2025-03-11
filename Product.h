#pragma once

#include <string>
#include <vector>

#include "Review.h"
#include "Category.h"

#include <plugin/sqlite3/Sqlite3.h>

using namespace std;

class Product {
private:
    int price;
    vector<Review*> reviews;
public:
    string name;
    string description;
    string imagePath;
    Category* category;

    class InvalidPriceException {};
    class NoReviewException {};

    int getPrice() const;
    void setPrice(int price);
    //Vendor* getVendor() const;
    //void setVendor(Vendor* vendor);
    vector<Review*> getReviews();
    int getReviewCount() const;
    void addReview(Review* review);
    float getAvgRating() const;
};