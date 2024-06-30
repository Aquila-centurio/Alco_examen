#ifndef Decor
#define Decor

#include <iostream>
#include <algorithm>
#include <vector>
#include "classes.h"
#include "containers.h"

using namespace std;

template <typename T>
class AlcoholIteratorDecorator : public AlcoholIterator<T> {
protected:
    std::vector<T> drinks;
    size_t current;
    AlcoholIterator<T>* iterator; // Сохранение указателя на итератор

public:
    AlcoholIteratorDecorator(AlcoholIterator<T>* iterator) : iterator(iterator), current(0) {
        // Заполняем вектор напитков из итератора
        for (iterator->first(); !iterator->isDone(); iterator->next()) {
            drinks.push_back(iterator->currentItem());
        }
    }

    virtual void sort() = 0;

    void first() override {
        current = 0;
    }

    void next() override {
        ++current;
    }

    bool isDone() const override {
        return current >= drinks.size();
    }

    T currentItem() const override {
        if (!isDone()) {
            return drinks[current];
        } else {
            return nullptr; // или другой способ обработки завершения итерации
        }
    }
};


//----------------------------------------------------------------------------------------------------

class PriceSortDecorator : public AlcoholIteratorDecorator<AlcoholDrink*> {
public:
    PriceSortDecorator(AlcoholIterator<AlcoholDrink*>* iterator) : AlcoholIteratorDecorator(iterator) {
        sort();
    }

    void sort() override {
        std::sort(drinks.begin(), drinks.end(), [](AlcoholDrink* a, AlcoholDrink* b) {
            return a->getPrice() < b->getPrice();
        });
    }
};

class AlcoholContentSortDecorator : public AlcoholIteratorDecorator<AlcoholDrink*> {
public:
    AlcoholContentSortDecorator(AlcoholIterator<AlcoholDrink*>* iterator) : AlcoholIteratorDecorator(iterator) {
        sort();
    }

    void sort() override {
        std::sort(drinks.begin(), drinks.end(), [](AlcoholDrink* a, AlcoholDrink* b) {
            if (a->getAlcoholContent() == b->getAlcoholContent()) {
                return a->getVolume() < b->getVolume();
            }
            return a->getAlcoholContent() < b->getAlcoholContent();
        });
    }
};

class AlcoholContentFilterDecorator : public AlcoholIteratorDecorator<AlcoholDrink*> {
private:
    float minAlcoholContent;
    float maxAlcoholContent;

public:
    AlcoholContentFilterDecorator(AlcoholIterator<AlcoholDrink*>* iterator, float minContent, float maxContent)
        : AlcoholIteratorDecorator(iterator), minAlcoholContent(minContent), maxAlcoholContent(maxContent) {
        filter();
    }

    void sort() override {}

    void filter() {
        std::vector<AlcoholDrink*> filteredDrinks;
        for (AlcoholDrink* drink : drinks) {
            if (drink->getAlcoholContent() >= minAlcoholContent && drink->getAlcoholContent() <= maxAlcoholContent) {
                filteredDrinks.push_back(drink);
            }
        }
        drinks = filteredDrinks;
    }
};

#endif // Decor
