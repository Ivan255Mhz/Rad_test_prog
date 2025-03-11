#ifndef BASKET_H
#define BASKET_H

#include <QtWidgets>

class Basket {
    int Red;
    int Blue;

public:
    Basket(int r = 0, int b = 0) : Red(r), Blue(b) {}

    int GetRed() const { return Red; }
    int GetBlue() const { return Blue; }
    int total() const { return Red + Blue; }

    double getRedProbability() const {
        return total() > 0 ? static_cast<double>(Red) / total() : 0.0;
    }

    double getBlueProbability() const {
        return total() > 0 ? static_cast<double>(Blue) / total() : 0.0;
    }

    void addRed(int count) {
        if (count > 0) Red += count;
    }

    void removeRed(int count) {
        if (count > 0) {
            Red = qMax(Red - count, 0);
        }
    }

    void addBlue(int count) {
        if (count > 0) Blue += count;
    }

    void removeBlue(int count) {
        if (count > 0) {
            Blue = qMax(Blue - count, 0);
        }
    }
};

#endif // BASKET_H
