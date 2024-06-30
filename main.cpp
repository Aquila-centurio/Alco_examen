#include <iostream>
#include "classes.h"
#include "containers.h"
#include "decorators.h"
#include <sqlite3.h>

using namespace std;

//Fabric methods
class AlcoholFactory {
public:
    virtual ~AlcoholFactory() {}
    virtual AlcoholDrink* createDrink() = 0;
};

class WineFactory : public AlcoholFactory {
public:
    AlcoholDrink* createDrink() override {
        return new Wine();
    }
};

class BeerFactory : public AlcoholFactory {
public:
    AlcoholDrink* createDrink() override {
        return new Beer();
    }
};

class VodkaFactory : public AlcoholFactory {
public:
    AlcoholDrink* createDrink() override {
        return new Vodka();
    }
};

void fillContainerWithRandomDrinks(AlcoholContainer& container, AlcoholFactory& factory, int count) {
    for (int i = 0; i < count; ++i) {
        AlcoholDrink* drink = factory.createDrink();
        container.addDrink(drink);
    }
}



int main() {
    srand(time(0)); 

    VectorAlcoholContainer container;
    //ArrayAlcoholContainer container;
    //SQLiteAlcoholContainer container("database.db");


    WineFactory wineFactory;
    fillContainerWithRandomDrinks(container, wineFactory, 5);

    BeerFactory beerFactory;
    fillContainerWithRandomDrinks(container, beerFactory, 3); 

    VodkaFactory vodkaFactory;
    fillContainerWithRandomDrinks(container, vodkaFactory, 4); 

    // Выводим содержимое контейнера
    container.displayContents();

    // Создание итератора
    VectorAlcoholIterator baseIterator(container.getDrinks());
    
    // Декоратор сортировки по цене
    //PriceSortDecorator sortedIterator(&baseIterator);
    //AlcoholContentFilterDecorator sortedIterator(&baseIterator);
    AlcoholContentSortDecorator sortedIterator(&baseIterator);


    cout << "Decor \n";
    // Итерация и вывод информации
    for (sortedIterator.first(); !sortedIterator.isDone(); sortedIterator.next()) {
        AlcoholDrink* drink = sortedIterator.currentItem();
        if (drink) {
            drink->displayInfo();
        }
    }

    


    return 0;
}
