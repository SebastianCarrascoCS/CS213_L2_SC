// Sebastian_Carrasco
// Lab #2


#include <iostream>
#include <string>
#include <random>
#include <iomanip>
#include <limits>

using namespace std;


int main() {
    enum class MenuOptions {
        INVALID = 0,
        BATTLE = 1,
        RESET = 2,
        QUIT = 3
    };

    // Creature class definition
    class Creature {
    private:
        string name;
        string type;
        int strength;
        int health;
        int maxHealth;

    public:
        // Default constructor
        Creature() {
            setCreature("Unknown", "Unknown", 0, 0);
        }

        // 4-argument constructor
        Creature(const string& creatureName, const string& creatureType, int str, int hp) {
            setCreature(creatureName, creatureType, str, hp);
        }

        // Setter function for all member variables
        void setCreature(const string& creatureName, const string& creatureType, int str, int hp) {
            name = creatureName;
            type = creatureType;
            strength = (str < 0) ? 0 : str;
            health = (hp < 0) ? 0 : hp;
            maxHealth = health;
        }

        // Accessor functions
        int getHealth() const {
            return health;
        }

        int getStrength() const {
            return strength;
        }

        string getNameAndType() const {
            return name + " the " + type;
        }

        // Damage calculation - random number up to strength
        int getDamage() const {
            if (strength <= 0) {
                return 0;
            }
            return (rand() % strength) + 1;
        }

        // Take damage function
        void takeDamage(int damage) {
            health -= damage;
            if (health < 0) {
                health = 0;
            }
        }

        // Check if creature is alive
        bool isAlive() const {
            return health > 0;
        }

        // Reset creature to original state
        void reset() {
            health = maxHealth;
        }

        // toString function for table output
        string toString() const {
            return getNameAndType() + "   " + to_string(strength) + "      " + to_string(health);
        }
    };

    // Game variables
    Creature creature1;
    Creature creature2;

    // Helper functions
    auto clearInputBuffer = []() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    };

    auto getValidInteger = [&clearInputBuffer](const string& prompt) {
        int value;
        bool validInput = false;

        while (!validInput) {
            cout << prompt;
            if (cin >> value) {
                clearInputBuffer();
                validInput = true;
            } else {
                cout << "Invalid input. Please enter a number\n";
                clearInputBuffer();
            }
        }
        return value;
    };

    auto getValidIntegerInRange = [&getValidInteger](const string& prompt, int minVal, int maxVal) {
        int value;
        bool validInput = false;

        while (!validInput) {
            value = getValidInteger(prompt);
            if (value >= minVal && value <= maxVal) {
                validInput = true;
            } else {
                cout << "Value must be between " << minVal << " and " << maxVal << ". Please try again.\n";
            }
        }
        return value;
    };

    auto getValidString = [&clearInputBuffer](const string& prompt) {
        string value;
        cout << prompt;
        getline(cin, value);
        return value;
    };

    auto setupCreatures = [&]() {
        cout << "\n=== Setting up Creature 1 ===\n";
        string name1 = getValidString("Enter creature 1 name: ");
        string type1 = getValidString("Enter creature 1 type: ");
        int strength1 = getValidIntegerInRange("Enter creature 1 strength (1-100): ", 1, 100);
        int health1 = getValidIntegerInRange("Enter creature 1 health (1-200): ", 1, 200);

        cout << "\n=== Setting up Creature 2 ===\n";
        string name2 = getValidString("Enter creature 2 name: ");
        string type2 = getValidString("Enter creature 2 type: ");
        int strength2 = getValidIntegerInRange("Enter creature 2 strength (1-100): ", 1, 100);
        int health2 = getValidIntegerInRange("Enter creature 2 health (1-200): ", 1, 200);

        creature1.setCreature(name1, type1, strength1, health1);
        creature2.setCreature(name2, type2, strength2, health2);
    };

    auto displayCreatureStats = [&]() {
        cout << "\n=== Creature Statistics ===\n";
        cout << left << setw(25) << "Name"
             << setw(10) << "Strength"
             << setw(10) << "Health" << "\n";
        cout << string(45, '-') << "\n";

        cout << left << setw(25) << creature1.getNameAndType()
             << setw(10) << creature1.getStrength()
             << setw(10) << creature1.getHealth() << "\n";

        cout << left << setw(25) << creature2.getNameAndType()
             << setw(10) << creature2.getStrength()
             << setw(10) << creature2.getHealth() << "\n";
    };

    auto battle = [&]() {
        setupCreatures();

        cout << "\n=== BEFORE BATTLE ===";
        displayCreatureStats();

        // Randomly select who attacks first
        srand(time(nullptr));
        bool creature1Attacks = (rand() % 2 == 0);
        int round = 1;

        cout << "\n=== BATTLE BEGINS ===\n";
        cout << left << setw(8) << "Round"
             << setw(20) << "Attacker"
             << setw(10) << "Damage"
             << setw(20) << "Defender"
             << setw(15) << "Defender Health" << "\n";
        cout << string(73, '-') << "\n";

        bool battleContinues = true;
        while (battleContinues) {
            Creature* attacker;
            Creature* defender;

            if (creature1Attacks) {
                attacker = &creature1;
                defender = &creature2;
            } else {
                attacker = &creature2;
                defender = &creature1;
            }

            int damage = attacker->getDamage();
            defender->takeDamage(damage);

            cout << left << setw(8) << round
                 << setw(20) << attacker->getNameAndType()
                 << setw(10) << damage
                 << setw(20) << defender->getNameAndType()
                 << setw(15) << defender->getHealth() << "\n";

            // Check if battle should continue
            battleContinues = (creature1.isAlive() && creature2.isAlive());

            creature1Attacks = !creature1Attacks; // Switch attacker
            round++;
        }

        // Determine winner
        if (creature1.isAlive()) {
            cout << "\n*** " << creature1.getNameAndType()
                 << " defeated " << creature2.getNameAndType()
                 << " in " << (round - 1) << " rounds! ***\n";
        } else {
            cout << "\n*** " << creature2.getNameAndType()
                 << " defeated " << creature1.getNameAndType()
                 << " in " << (round - 1) << " rounds! ***\n";
        }

        cout << "\n=== AFTER BATTLE ===";
        displayCreatureStats();
    };

    auto resetCreatures = [&]() {
        creature1.reset();
        creature2.reset();
        cout << "\nCreatures have been reset to their original health\n";
    };

    auto displayMenu = []() {
        cout << "\n\nMenu:\n"
             << "1. Battle\n"
             << "2. Reset Creatures\n"
             << "3. Quit\n"
             << "Enter your choice: ";
    };

    auto getMenuChoice = [&getValidInteger]() {
        int choice = getValidInteger("");

        if (choice >= 1 && choice <= 3) {
            return static_cast<MenuOptions>(choice);
        }
        return MenuOptions::INVALID;
    };

    auto handleInvalidChoice = []() {
        cout << "\nInvalid menu choice. Please select a valid option (1-3)\n";
    };

    // Main program execution
    cout << "Welcome to the Creature Battle Arena!\n";

    MenuOptions choice = MenuOptions::INVALID;
    bool continueGame = true;

    while (continueGame) {
        displayMenu();
        choice = getMenuChoice();

        switch (choice) {
            case MenuOptions::BATTLE:
                battle();
                break;
            case MenuOptions::RESET:
                resetCreatures();
                break;
            case MenuOptions::QUIT:
                cout << "\nThank you for playing! Goodbye!\n";
                continueGame = false;
                break;
            case MenuOptions::INVALID:
            default:
                handleInvalidChoice();
                break;
        }
    }

    return 0;
}