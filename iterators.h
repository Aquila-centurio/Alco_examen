#ifndef Iter
#define Iter

#include <iostream>
#include "classes.h"
#include "containers.h"

using namespace std;

template <typename T>
class AlcoholIterator {
public:
    virtual ~AlcoholIterator() {}
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual T currentItem() const = 0;
};

//----------------------------------------------------------------------------------

class VectorAlcoholIterator : public AlcoholIterator<AlcoholDrink*> {
private:
    const std::vector<AlcoholDrink*>& drinks;
    size_t current;

public:
    VectorAlcoholIterator(const std::vector<AlcoholDrink*>& drinks)
        : drinks(drinks), current(0) {}

    void first() override {
        current = 0;
    }

    void next() override {
        if (current < drinks.size()) {
            ++current;
        }
    }

    bool isDone() const override {
        return current >= drinks.size();
    }

    AlcoholDrink* currentItem() const override {
        if (!isDone()) {
            return drinks[current];
        } else {
            return nullptr;
        }
    }
};

//----------------------------------------------------------------------------------

class ArrayAlcoholIterator : public AlcoholIterator<AlcoholDrink*> {
private:
    AlcoholDrink** drinks;
    size_t current;
    size_t size;

public:
    ArrayAlcoholIterator(AlcoholDrink** drinks, size_t size)
        : drinks(drinks), current(0), size(size) {}

    void first() override {
        current = 0;
    }

    void next() override {
        if (current < size) {
            ++current;
        }
    }

    bool isDone() const override {
        return current >= size;
    }

    AlcoholDrink* currentItem() const override {
        if (!isDone()) {
            return drinks[current];
        } else {
            return nullptr;
        }
    }
};

//----------------------------------------------------------------------------------

class SQLiteAlcoholIterator : public AlcoholIterator<AlcoholDrink*> {
private:
    std::vector<AlcoholDrink*> drinks;
    size_t current;

public:
    SQLiteAlcoholIterator(const std::vector<AlcoholDrink*>& drinks)
        : drinks(drinks), current(0) {}

    void first() override {
        current = 0;
    }

    void next() override {
        ++current;
    }

    bool isDone() const override {
        return current >= drinks.size();
    }

    AlcoholDrink* currentItem() const override {
        if (!isDone()) {
            return drinks[current];
        } else {
            return nullptr;
        }
    }
};

#endif // Iter
