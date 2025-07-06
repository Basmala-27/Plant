#include <iostream>
#include "C:\Users\ITC\Desktop\New folder\Plant\sqlite3.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string Check(string& answer)
{

    if (answer != "y" && answer != "n")
    {
        cout << "Error!! Enter y/n\n";
        cin >> answer;
        clearScreen();
        while (answer != "y" && answer != "n")
        {
            cout << "Error!! Enter y/n\n";
            cin >> answer;
            clearScreen();
        }

        return answer;
    }

    return answer;
}
bool RuternToMain()
{
    cout << "\n\nDo you want to rutern to the main?\n(y/n)\n\n";
    string answer;
    cin >> answer;
    clearScreen();
    Check(answer);
    if (answer == "y")
        return 1;
    return 0;
}




class Plant {
private:
    string name;
    string waterRate;
    string bloomRate;
    int age;
    int purchaseDay;
    int purchaseMonth;
    int purchaseYear;
    string note;

public:
    Plant(string n, string wr, string br, int a, int pd, int pm, int py, string nt)
        : name(n), waterRate(wr), bloomRate(br), age(a),
        purchaseDay(pd), purchaseMonth(pm), purchaseYear(py), note(nt) {}

    void displayInfo() {
        cout << "Plant Name: " << name << endl;
        cout << "Water Rate: " << waterRate << endl;
        cout << "Bloom Rate: " << bloomRate << endl;
        cout << "Age of Plant: " << age << " years" << endl;
        cout << "Date Purchased: " << purchaseDay << "/" << purchaseMonth << "/" << purchaseYear << endl;
        cout << "Note: " << note << endl;
        cout << endl;
    }

    int saveToDatabase(sqlite3* db) {
        string sql = "INSERT INTO plants (name, waterRate, bloomRate, age, purchaseDay, purchaseMonth, purchaseYear, note) VALUES ('"
            + name + "', '" + waterRate + "', '" + bloomRate + "', " + to_string(age) + ", "
            + to_string(purchaseDay) + ", " + to_string(purchaseMonth) + ", " + to_string(purchaseYear) + ", '"
            + note + "');";
        char* errorMessage;
        int exit = sqlite3_exec(db, sql.c_str(), 0, 0, &errorMessage);
        if (exit != SQLITE_OK) {
            cerr << "Error inserting data: " << errorMessage << endl;
            sqlite3_free(errorMessage);
        }
        else {
            cout << "Plant data saved successfully!" << endl;
        }
        return exit;
    }
};

static int displayCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl;
    return 0;
}

int initDatabase(sqlite3*& db) {
    int exit = sqlite3_open("plants.db", &db);
    if (exit) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return exit;
    }
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS plants ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT, waterRate TEXT, bloomRate TEXT, "
        "age INTEGER, purchaseDay INTEGER, "
        "purchaseMonth INTEGER, purchaseYear INTEGER, note TEXT);";
    char* errorMessage;
    exit = sqlite3_exec(db, createTableSQL, 0, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error creating table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    return exit;
}

int updatePlant(sqlite3* db, int plantID, string newName, string newWaterRate, string newBloomRate, int newAge, int newPurchaseDay, int newPurchaseMonth, int newPurchaseYear, string newNote) {
    string sql = "UPDATE plants SET name='" + newName + "', waterRate='" + newWaterRate + "', bloomRate='" + newBloomRate +
        "', age=" + to_string(newAge) + ", purchaseDay=" + to_string(newPurchaseDay) +
        ", purchaseMonth=" + to_string(newPurchaseMonth) + ", purchaseYear=" + to_string(newPurchaseYear) +
        ", note='" + newNote + "' WHERE id=" + to_string(plantID) + ";";

    char* errorMessage;
    int exit = sqlite3_exec(db, sql.c_str(), 0, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error updating data: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    else {
        cout << "Plant data updated successfully!" << endl;
    }
    return exit;
}

int deletePlant(sqlite3* db, int plantID) {
    string sql = "DELETE FROM plants WHERE id=" + to_string(plantID) + ";";
    char* errorMessage;
    int exit = sqlite3_exec(db, sql.c_str(), 0, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error deleting data: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    else {
        cout << "Plant record deleted successfully!" << endl;
    }
    return exit;
}

void resetPlantIDs(sqlite3* db) {
    char* errorMessage;

    const char* createTempTableSQL =
        "CREATE TABLE IF NOT EXISTS temp_plants ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, waterRate TEXT, bloomRate TEXT, "
        "age INTEGER, purchaseDay INTEGER, "
        "purchaseMonth INTEGER, purchaseYear INTEGER, note TEXT);";

    int exit = sqlite3_exec(db, createTempTableSQL, 0, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error creating temporary table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return;
    }


    const char* copyDataSQL =
        "INSERT INTO temp_plants (name, waterRate, bloomRate, age, purchaseDay, purchaseMonth, purchaseYear, note) "
        "SELECT name, waterRate, bloomRate, age, purchaseDay, purchaseMonth, purchaseYear, note FROM plants ORDER BY id;";

    exit = sqlite3_exec(db, copyDataSQL, 0, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error copying data to temporary table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return;
    }


    const char* dropOriginalTableSQL = "DROP TABLE plants;";
    exit = sqlite3_exec(db, dropOriginalTableSQL, 0, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error dropping original table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return;
    }


    const char* renameTableSQL = "ALTER TABLE temp_plants RENAME TO plants;";
    exit = sqlite3_exec(db, renameTableSQL, 0, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error renaming temporary table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    else {
        cout << "Plant IDs have been reset successfully!" << endl;
    }
}


void searchPlantByName(sqlite3* db, const string& plantName) {
    string sql = "SELECT * FROM plants WHERE name LIKE '%" + plantName + "%';";
    char* errorMessage;
    int exit = sqlite3_exec(db, sql.c_str(), displayCallback, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error searching data: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

void listPlantsByPurchaseYear(sqlite3* db, int year) {
    string sql = "SELECT * FROM plants WHERE purchaseYear=" + to_string(year) + ";";
    char* errorMessage;
    int exit = sqlite3_exec(db, sql.c_str(), displayCallback, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error retrieving data: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

void displayUniqueWaterRates(sqlite3* db) {
    const char* sql = "SELECT DISTINCT name, waterRate FROM plants;";
    char* errorMessage;
    int exit = sqlite3_exec(db, sql, displayCallback, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error retrieving water rates: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

void loadAndDisplayPlants(sqlite3* db) {
    const char* sql = "SELECT * FROM plants;";
    char* errorMessage;


    int exit = sqlite3_exec(db, sql, displayCallback, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        cerr << "Error loading data: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}


void deleteAllRecords(sqlite3* db) {
    char confirmation;
    cout << "Are you sure you want to delete all records in the database? This action cannot be undone. (y/n): ";
    cin >> confirmation;

    if (confirmation == 'y' || confirmation == 'Y') {
        char* errorMessage;


        const char* deleteAllSQL = "DELETE FROM plants;";

        int exit = sqlite3_exec(db, deleteAllSQL, 0, 0, &errorMessage);
        if (exit != SQLITE_OK) {
            cerr << "Error deleting all records: " << errorMessage << endl;
            sqlite3_free(errorMessage);
            return;
        }


        const char* resetSQL = "VACUUM;";
        exit = sqlite3_exec(db, resetSQL, 0, 0, &errorMessage);
        if (exit != SQLITE_OK) {
            cerr << "Error resetting database: " << errorMessage << endl;
            sqlite3_free(errorMessage);
            return;
        }

        cout << "All records have been deleted and the database has been reset!" << endl;
    }
    else {
        cout << "Operation canceled. No records were deleted." << endl;
    }
}

void RecommendPlants()
{
    string line;
    ifstream myfile("recommend plant");
    if (myfile.is_open()) {
        bool start = 0;
        bool finsh = 0;

        while (getline(myfile, line)) {
            if (line == "////")
            {
                cout << "\n\n\nThat is all we have \nI hope that you found a nice one\n\n";
                myfile.close();
                continue;
            }
            if (line == "----")
            {
                cout << "Do you want another one?\nEnter y/n\n\n";
                string answer;
                cin >> answer;
                clearScreen();
                Check(answer);
                if (answer == "y") {
                    start = 1; finsh = 0;
                    continue;
                }
                else {
                    cout << "\n\nI hope that you found a nice choice\n";
                    myfile.close();
                    continue;
                }
            }
            if (line == "--")
            {
                start = 1;
                finsh = 0;
                continue;
            }

            if (start && !finsh) {
                cout << line << '\n';
            }


        }

    }

}
int main() {


    cout << "\n\n\t\t\t\tWelcome to You in \"Nabtaty\" Program!\n"
        << "\t\tWe will help you choose the most suitable plants for you and easily track their status\n" <<
        "\tStart by adding your plants and enjoy monitoring their growth & health to create a thriving natural environment"
        << "\n\n\t\t\t\tLets embark on our green journey now!\n\n";




    sqlite3* db;
    if (initDatabase(db) != SQLITE_OK) return -1;

    int choice;
    while (true) {
        cout << "\n1. Add New Plant\n2. Update Plant\n3. Delete Plant\n4. Search Plant by Name\n"
            << "5. List Plants by Purchase Year\n6. Display Unique Water Rates\n7. Display All Plants\n8. Delete All Records\n9.Recommended plants for you to plant\n10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        clearScreen();
        cin.ignore();

        if (choice == 1) {
            string name, waterRate, bloomRate, note;
            int age, purchaseDay, purchaseMonth, purchaseYear;

            cout << "Enter plant name: ";
            getline(cin, name);
            cout << "Enter water rate (ml): ";
            getline(cin, waterRate);
            cout << "Enter bloom rate(in week): ";
            getline(cin, bloomRate);
            cout << "Enter age of plant (in week): ";
            cin >> age;
            cout << "Enter purchase date (day month year): ";
            cin >> purchaseDay >> purchaseMonth >> purchaseYear;
            cin.ignore();
            cout << "Enter any additional note: ";
            getline(cin, note);

            Plant plant(name, waterRate, bloomRate, age, purchaseDay, purchaseMonth, purchaseYear, note);
            plant.saveToDatabase(db);

            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 2) {
            int plantID, age, purchaseDay, purchaseMonth, purchaseYear;
            string name, waterRate, bloomRate, note;

            cout << "Enter plant ID to update: ";
            cin >> plantID;
            cin.ignore();
            cout << "Enter new plant name: ";
            getline(cin, name);
            cout << "Enter new water rate: ";
            getline(cin, waterRate);
            cout << "Enter new bloom rate: ";
            getline(cin, bloomRate);
            cout << "Enter new age of plant (in years): ";
            cin >> age;
            cout << "Enter new purchase date (day month year): ";
            cin >> purchaseDay >> purchaseMonth >> purchaseYear;
            cin.ignore();
            cout << "Enter new note: ";
            getline(cin, note);

            updatePlant(db, plantID, name, waterRate, bloomRate, age, purchaseDay, purchaseMonth, purchaseYear, note);

            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 3) {
            int plantID;
            cout << "Enter plant ID to delete: ";
            cin >> plantID;
            deletePlant(db, plantID);
            resetPlantIDs(db);
            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 4) {
            string name;
            cout << "Enter plant name to search: ";
            getline(cin, name);
            searchPlantByName(db, name);
            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 5) {
            int year;
            cout << "Enter purchase year to filter plants: ";
            cin >> year;
            listPlantsByPurchaseYear(db, year);
            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 6) {
            displayUniqueWaterRates(db);
            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 7) {
            loadAndDisplayPlants(db);
            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 8) {
            deleteAllRecords(db);
            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 9) {
            cout << "\n\nThere is some recommend plant that I suggest you to plant\n\n";
            RecommendPlants();

            if (!RuternToMain()) {
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                break;
            }
        }
        else if (choice == 10) {
            
                cout << "\n\n\t\tThank you for using our program   I hope you choose and track your plants.\t\t\n"
                    << "       \t\tthis app will add a touch of greenery and tranquility to your life.\t\t\t\n"
                    << "\t\tFeel free to come back to update your data and monitor your plants' growth.\t\t\n"
                    << " \t\t\t\t\t\tHappy planting!\t\t\t\n\n";
                return 0;
            
        }
        else {
            cout << "Invalid choice, please try again." << endl;
        }
    }

    sqlite3_close(db);
    return 0;
}