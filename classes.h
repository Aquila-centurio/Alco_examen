#ifndef Clas
#define Clas

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;



enum class AlcoType : int { Wine, Beer, Vodka };

class AlcoholDrink {
protected:
    AlcoType Type;
    float alcoholContent;
    float volume;
    float price;

public:
    // Конструктор по умолчанию
    AlcoholDrink(float alcoholContent = 0, float volume = 0, float price = 0)
        : alcoholContent(alcoholContent), volume(volume), price(price) {}

    virtual ~AlcoholDrink() {}
    virtual void displayInfo() const = 0;
    AlcoType getType() const { return Type; }
    float getAlcoholContent() const { return alcoholContent; }
    float getVolume() const { return volume; }
    float getPrice() const { return price; }
};

typedef AlcoType * Alco;

class Wine : public AlcoholDrink {
private:
    string grapeSort;
    string country;
    static const vector<string> grapeSorts;
    static const vector<string> countries;

public:
    Wine(float alcoholContent = rand() % (15 - 11 + 1) + 11, float volume = rand() % (1000 - 300 + 1) + 300, float price = rand() % (20000 - 1000 + 1) + 1000)
        : AlcoholDrink(alcoholContent, volume, price) {
        Type = AlcoType::Wine;
        this->grapeSort = grapeSorts[rand() % grapeSorts.size()];
        this->country = countries[rand() % countries.size()];
        
    }

    void displayInfo() const override {
        cout << "Wine: " << grapeSort << " from " << country << ", " << volume << "ml, "
             << alcoholContent << "%. alcohol, rub " << price << endl;
    }


    string getGrapeSort() const { return grapeSort; }
    string getCountry() const { return country; }
};

const vector<string> Wine::grapeSorts = {"Isabella", "Pino blan", "Pino nuar"};
const vector<string> Wine::countries = {"Spain", "Italy", "France"};


class Beer : public AlcoholDrink {
private:
    string kind;
    string manufacturer;
    static const vector<string> manufactures;
    static const vector<string> kindes;

public:
    Beer(float alcoholContent = rand() % (10 - 4 + 1) + 4, float volume = rand() % (1000 - 300 + 1) + 300, float price = rand() % (500 - 30 + 1) + 30)
        : AlcoholDrink(alcoholContent, volume, price) {
        Type = AlcoType::Beer;
        this->kind = kindes[rand() % kindes.size()];
        this->manufacturer = manufactures[rand() % manufactures.size()];
    }

    void displayInfo() const override {
        cout << "Beer: " << kind << " by " << manufacturer << ", " << volume << "ml, "
             << alcoholContent << "%. alcohol, rub " << price << endl;
    }


    string getType() const { return kind; }
    string getManufacturer() const { return manufacturer; }
};

const vector<string> Beer::manufactures = {"Baltica", "Tuborg", "Volkovskaya"};
const vector<string> Beer::kindes = {"El'","Staut", "Lager"};

class Vodka : public AlcoholDrink {
private:
    string countryOfOrigin;
    string composition;
    static const vector<string> countriesOfOrigin;
    static const vector<string> compositions;

public:
    Vodka(float alcoholContent = 40, float volume = rand() % (1000 - 300 + 1) + 300, float price = rand() % (500 - 30 + 1) + 30)
        : AlcoholDrink(alcoholContent, volume, price) {
        Type = AlcoType::Vodka;
        this->countryOfOrigin = countriesOfOrigin[rand() % countriesOfOrigin.size()];
        this->composition = compositions[rand() % compositions.size()];
    }

    void displayInfo() const override {
        cout << "Vodka: " << composition << " from " << countryOfOrigin << ", " << volume << "ml, "
             << alcoholContent << "%. alcohol, rub " << price << endl;
    }

    string getCountryOfOrigin() const { return countryOfOrigin; }
    string getComposition() const { return composition; }
};
const vector<string> Vodka::countriesOfOrigin = {"Russia", "Belarus", "Ukraine"};
const vector<string> Vodka::compositions = {"Tsarskaya", "Stolichnaya", "Husky"};
#endif // Clas
