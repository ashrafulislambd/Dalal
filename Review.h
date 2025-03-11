#pragma once

#include <string>
using namespace std;

class Review {
private:
    int time;
    int rating;
    string message;
public:
    Review() {
        rating = 5;
    }
    
    string description;

    class OutOfBoundException {};

    int getTime() const;

    void setTime(int time);

    int getRating() const;

    void setRating(int rating);
    
    string getMessage() const;
    
    void setMessage(string message);
};