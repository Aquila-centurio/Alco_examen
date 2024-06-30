#ifndef Cont
#define Cont
#include <iostream>
#include <sqlite3.h>
#include "classes.h"
#include "iterators.h"
using namespace std;


class AlcoholContainer {
public:
    virtual ~AlcoholContainer() {}
    virtual void addDrink(AlcoholDrink* drink) = 0;
    virtual void removeDrink(const AlcoType& Type) = 0;
    virtual void displayContents() const = 0;
    virtual vector<AlcoholDrink*> findAllDrinks(const AlcoType& Type) const = 0;
    virtual const std::vector<AlcoholDrink*>& getDrinks() const = 0;
};

class VectorAlcoholContainer : public AlcoholContainer {
private:
    std::vector<AlcoholDrink*> drinks; // Вектор для хранения напитков

public:
    
    void addDrink(AlcoholDrink* drink) override {
        drinks.push_back(drink);
    }

    // Удаление напитка из контейнера по типу
    void removeDrink(const AlcoType& type) override {
        for (auto it = drinks.begin(); it != drinks.end(); ++it) {
            if ((*it)->getType() == type) {
                delete *it; 
                it = drinks.erase(it);
            }
        }
    }

    // Вывод содержимого контейнера
    void displayContents() const override {
        for (const auto& drink : drinks) {
            drink->displayInfo();
        }
    }

    const std::vector<AlcoholDrink*>& getDrinks() const {
        return drinks;
    }

    // Поиск напитка в контейнере по типу
    std::vector<AlcoholDrink*> findAllDrinks(const AlcoType& type) const override {
        std::vector<AlcoholDrink*> foundDrinks;
        for (const auto& drink : drinks) {
            if (drink->getType() == type) {
                foundDrinks.push_back(drink);
            }
        }
        return foundDrinks;
    }

    // Деструктор для освобождения памяти от всех напитков
    ~VectorAlcoholContainer() override {
        for (auto& drink : drinks) {
            delete drink;
        }
        drinks.clear(); // Очищаем вектор
    }
};

// -----------------------------------------------------------------------------------------------

class ArrayAlcoholContainer : public AlcoholContainer {
private:
    static const int MAX_SIZE = 100; 
    AlcoholDrink* drinks[MAX_SIZE]; 
    int count; 
public:
    ArrayAlcoholContainer() : count(0) {} 

    void addDrink(AlcoholDrink* drink) override {
        if (count < MAX_SIZE) {
            drinks[count++] = drink;
        } else {
            cout << "Контейнер полон, невозможно добавить новый напиток." << endl;
        }
    }

    void removeDrink(const AlcoType& type) override {
        for (int i = 0; i < count; ++i) {
            if (drinks[i]->getType() == type) {
                delete drinks[i]; 
                for (int j = i; j < count - 1; ++j) {
                    drinks[j] = drinks[j + 1];
                }
                count--; 
                return;
            }
        }
        cout << "Напиток заданного типа не найден в контейнере." << endl;
    }

    // Вывод содержимого контейнера
    void displayContents() const override {
        for (int i = 0; i < count; ++i) {
            drinks[i]->displayInfo();
        }
    }

    // Поиск напитка в контейнере по типу
    vector<AlcoholDrink*> findAllDrinks(const AlcoType& type) const override {
        vector<AlcoholDrink*> foundDrinks;
        for (int i = 0; i < count; ++i) {
            if (drinks[i]->getType() == type) {
                foundDrinks.push_back(drinks[i]);
            }
        }
        return foundDrinks;
    }

    const std::vector<AlcoholDrink*>& getDrinks() const override {
        static std::vector<AlcoholDrink*> drinksVector(drinks, drinks + count);
        return drinksVector;
    }

    // Деструктор для освобождения памяти от всех напитков
    ~ArrayAlcoholContainer() override {
        for (int i = 0; i < count; ++i) {
            delete drinks[i];
        }
    }
};

//------------------------------------------------------------------------------------------------

class SQLiteAlcoholContainer : public AlcoholContainer {
private:
    sqlite3* db;

    void executeQuery(const std::string& sql) const {
        char* errorMessage = nullptr;
        int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);
        if (result != SQLITE_OK) {
            std::cerr << "SQL error: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
    }

    std::string alcoTypeToString(AlcoType type) const {
        switch (type) {
            case AlcoType::Wine: return "Wine";
            case AlcoType::Beer: return "Beer";
            case AlcoType::Vodka: return "Vodka";
            default: return "Unknown";
        }
    }

public:
    SQLiteAlcoholContainer(const std::string& filename) {
        int result = sqlite3_open(filename.c_str(), &db);
        if (result != SQLITE_OK) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }

        executeQuery("CREATE TABLE IF NOT EXISTS drinks (id INTEGER PRIMARY KEY, type INTEGER, alcoholContent REAL, volume REAL, price REAL)");
    }

    void addDrink(AlcoholDrink* drink) override {
        std::string sql = "INSERT INTO drinks (type, alcoholContent, volume, price) VALUES (";
        sql += std::to_string(static_cast<int>(drink->getType())) + ", ";
        sql += std::to_string(drink->getAlcoholContent()) + ", ";
        sql += std::to_string(drink->getVolume()) + ", ";
        sql += std::to_string(drink->getPrice()) + ")";

        executeQuery(sql);
    }

    void removeDrink(const AlcoType& type) override {
        std::string sql = "DELETE FROM drinks WHERE type = " + std::to_string(static_cast<int>(type));
        executeQuery(sql);
    }

    void displayContents() const override {
        std::string sql = "SELECT * FROM drinks";
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr);
        if (result == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                int type = sqlite3_column_int(statement, 1);
                float alcoholContent = sqlite3_column_double(statement, 2);
                float volume = sqlite3_column_double(statement, 3);
                float price = sqlite3_column_double(statement, 4);

                std::cout << "Type: " << alcoTypeToString(static_cast<AlcoType>(type)) << ", Alcohol Content: " << alcoholContent
                          << "%, Volume: " << volume << "ml , Price: rub " << price << std::endl;
            }
            sqlite3_finalize(statement);
        } else {
            std::cerr << "Failed to execute SQL query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    std::vector<AlcoholDrink*> findAllDrinks(const AlcoType& type) const override {
        std::vector<AlcoholDrink*> foundDrinks;
        std::string sql = "SELECT * FROM drinks WHERE type = " + std::to_string(static_cast<int>(type));
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr);
        if (result == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                float alcoholContent = sqlite3_column_double(statement, 2);
                float volume = sqlite3_column_double(statement, 3);
                float price = sqlite3_column_double(statement, 4);

                AlcoholDrink* drink = nullptr;
                switch (type) {
                    case AlcoType::Wine:
                        drink = new Wine(alcoholContent, volume, price);
                        break;
                    case AlcoType::Beer:
                        drink = new Beer(alcoholContent, volume, price);
                        break;
                    case AlcoType::Vodka:
                        drink = new Vodka(alcoholContent, volume, price);
                        break;
                }
                if (drink) {
                    foundDrinks.push_back(drink);
                }
            }
            sqlite3_finalize(statement);
        } else {
            std::cerr << "Failed to execute SQL query: " << sqlite3_errmsg(db) << std::endl;
        }
        return foundDrinks;
    }

    const std::vector<AlcoholDrink*>& getDrinks() const override {
        static std::vector<AlcoholDrink*> allDrinks;
        allDrinks.clear();
        std::string sql = "SELECT * FROM drinks";
        sqlite3_stmt* statement;
        int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr);
        if (result == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                int type = sqlite3_column_int(statement, 1);
                float alcoholContent = sqlite3_column_double(statement, 2);
                float volume = sqlite3_column_double(statement, 3);
                float price = sqlite3_column_double(statement, 4);

                AlcoholDrink* drink = nullptr;
                switch (static_cast<AlcoType>(type)) {
                    case AlcoType::Wine:
                        drink = new Wine(alcoholContent, volume, price);
                        break;
                    case AlcoType::Beer:
                        drink = new Beer(alcoholContent, volume, price);
                        break;
                    case AlcoType::Vodka:
                        drink = new Vodka(alcoholContent, volume, price);
                        break;
                }
                if (drink) {
                    allDrinks.push_back(drink);
                }
            }
            sqlite3_finalize(statement);
        } else {
            std::cerr << "Failed to execute SQL query: " << sqlite3_errmsg(db) << std::endl;
        }
        return allDrinks;
    }

    ~SQLiteAlcoholContainer() override {
        sqlite3_close(db);
    }
};


#endif // Cont