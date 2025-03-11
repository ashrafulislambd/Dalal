#include "Product.h"

int Product::getPrice() const {
    return price;
}

void Product::setPrice(int price) {
    if(price < 0) {
        throw InvalidPriceException();
    }
    
    this->price = price;
}

int Product::getReviewCount() const {
    return reviews.size();
}

void Product::addReview(Review* review) {
    reviews.push_back(review);
}

float Product::getAvgRating() const {
    if(getReviewCount() == 0) {
        throw NoReviewException();
    }

    float totalRating = 0;
    for(auto& review : reviews) {
        totalRating += review->getRating();
    }

    return totalRating /= getReviewCount();
}

vector<Review*> Product::getReviews() {
	return this->reviews;
}