#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

// ������� ��� ��������� ���������� ����� � ��������� [min, max]
int randomInRange(int minVal, int maxVal) {
    return minVal + rand() % (maxVal - minVal + 1);
}

// ����� ��� ������������� ��������
class Item {
public:
    string name;
    string description;
    int value;
    int power;

    Item(string n, string d, int v, int p) : name(n), description(d), value(v), power(p) {}
};

// ����� ��� ������������� ������
class Skill {
public:
    string name;
    string description;
    int level;
    int maxLevel;

    Skill(string n, string d, int ml) : name(n), description(d), level(1), maxLevel(ml) {}
};

// ����� ��� ������������� �������
class Ship {
public:
    string name;
    int hull;
    int maxHull;
    int shields;
    int maxShields;
    int engines;
    int maxEngines;
    vector<Item> weapons;
    vector<Item> upgrades;

    Ship(string n) : name(n), hull(100), maxHull(100), shields(50), maxShields(50), engines(30), maxEngines(30) {
        // ������� ������ �������
        weapons.push_back(Item("�������� �����", "������� �������� ���������", 10, 15));
    }

    void printStatus() {
        cout << "\n=== ��������� ������� ===" << endl;
        cout << "������: " << hull << "/" << maxHull << endl;
        cout << "����: " << shields << "/" << maxShields << endl;
        cout << "���������: " << engines << "/" << maxEngines << endl;
        cout << "����������:" << endl;
        for (auto& weapon : weapons) {
            cout << "- " << weapon.name << " (������� ����: " << weapon.power << ")" << endl;
        }
        cout << "���������:" << endl;
        for (auto& upgrade : upgrades) {
            cout << "- " << upgrade.name << endl;
        }
    }

    void repair(int amount) {
        hull = min(hull + amount, maxHull);
        cout << "������ ������������ �� " << amount << " ������." << endl;
    }

    void rechargeShields(int amount) {
        shields = min(shields + amount, maxShields);
        cout << "���� ������������ �� " << amount << " ������." << endl;
    }
};

// ����� ��� ������������� ���������
class Character {
public:
    string name;
    int health;
    int maxHealth;
    int armor;
    int maxArmor;
    vector<Item> weapons;
    vector<Item> inventory;
    vector<Skill> skills;
    int credits;
    double critBonus; // �������������� ���� ������������ ����� � ���������

    Character(string n) : name(n), health(100), maxHealth(100), armor(50), maxArmor(50), credits(500), critBonus(0.0) {
        weapons.push_back(Item("�������", "����������� �������", 5, 10));
        skills.push_back(Skill("��������", "����������� �������� � ���� �������", 5));
        skills.push_back(Skill("������", "��������� ����� ������ ������� � ����������", 5));
        skills.push_back(Skill("����������", "�������� �������� ������ ������ � ����������", 5));
    }

    void printStatus() {
        cout << "\n=== ��������� ��������� ===" << endl;
        cout << "��������: " << health << "/" << maxHealth << endl;
        cout << "�����: " << armor << "/" << maxArmor << endl;
        cout << "�������: " << credits << endl;
        cout << "����� ����� ����. �����: " << critBonus << "%" << endl;
        cout << "������:" << endl;
        for (auto& weapon : weapons) {
            cout << "- " << weapon.name << " (������� ����: " << weapon.power << ")" << endl;
        }
        cout << "������:" << endl;
        for (auto& skill : skills) {
            cout << "- " << skill.name << " (�������: " << skill.level << "/" << skill.maxLevel << ")" << endl;
        }
    }

    void heal(int amount) {
        int oldHealth = health;
        health = min(health + amount, maxHealth);
        cout << "�������� ������������� � " << oldHealth << " �� " << health << " ������." << endl;
    }

    void repairArmor(int amount) {
        int oldArmor = armor;
        armor = min(armor + amount, maxArmor);
        cout << "����� ������������� � " << oldArmor << " �� " << armor << " ������." << endl;
    }
};

// ����� ��� ������������� �����
class Enemy {
public:
    string name;
    string description;
    int health;
    int maxHealth;
    int minPower; // ����������� ����
    int maxPower; // ������������ ����
    int reward;

    Enemy(string n, string d, int h, int minP, int maxP, int r)
        : name(n), description(d), health(h), maxHealth(h), minPower(minP), maxPower(maxP), reward(r) {}

    void printStatus() {
        cout << name << " (��������: " << health << "/" << maxHealth << ")" << endl;
    }

    // ��������� ��������� ����
    int getRandomDamage() {
        return randomInRange(minPower, maxPower);
    }
};

// ����� ��� ������������� �������
class Planet {
public:
    string name;
    string description;
    vector<Item> items;
    vector<Enemy> enemies;
    bool hasClue;       // ���� �� �� ������� �������
    bool clueCollected; // ������� �� �������
    bool hasRestSpot;   // ����������� ������ �� �������

    Planet(string n, string d, bool clue) : name(n), description(d), hasClue(clue), clueCollected(false) {
        // 30% ������� ����� ����� ����� ��� ������
        hasRestSpot = (rand() % 100 < 30);
    }

    // ��� ������������ �������, ���� ������� ���� � ��� �� �������, ��� ����������.
    // �����, ���� ������� ����� ���� ������, ����� ����� ���������.
    bool explore(Character& player) {
        cout << "\n�� ������� �� " << name << ". " << description << endl;
        bool clueFoundHere = false;
        if (hasClue && !clueCollected) {
            cout << "\n�� ���������� ������� � ��������������� �����������!" << endl;
            clueCollected = true;
            clueFoundHere = true;
        }

        if (!items.empty()) {
            cout << "\n�� ����� ��������:" << endl;
            for (auto& item : items) {
                cout << "- " << item.name << " (" << item.description << ")" << endl;
                player.inventory.push_back(item);
            }
            items.clear();
        }

        if (!enemies.empty()) {
            cout << "\n��� �������!" << endl;
            combat(player, enemies);
        }

        // ����������� ���������, ���� ���� ������ ������������
        if (hasRestSpot) {
            int restChoice;
            cout << "\n�� ���������� ������ ����� ��� ������. ������� ���������? (1 - ��, 0 - ���): ";
            cin >> restChoice;
            if (restChoice == 1) {
                // ����� ��������������� �������� � ����� �� ��������� ��������
                int healAmount = randomInRange(10, 20);
                int armorAmount = randomInRange(5, 15);
                cout << "�� ��������� � ���������������� ����..." << endl;
                player.heal(healAmount);
                player.repairArmor(armorAmount);
            }
        }
        return clueFoundHere;
    }

    // ��� �� ������� � ����������� �������� "�������" ��� ���������
    void combat(Character& player, vector<Enemy>& enemies) {
        bool playerDodged = false;
        while (!enemies.empty() && player.health > 0) {
            cout << "\n=== �������� ��� ===" << endl;
            cout << "���� ���������:" << endl;
            player.printStatus();

            cout << "\n����������:" << endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                cout << i + 1 << ". ";
                enemies[i].printStatus();
            }

            cout << "\n�������� ��������:" << endl;
            cout << "1. ���������" << endl;
            cout << "2. ������������ �������" << endl;
            cout << "3. ���������� �������" << endl;
            cout << "4. ���������� (�������)" << endl;

            int choice;
            cin >> choice;
            playerDodged = false; // ����� ����� ���������

            if (choice == 1) {
                cout << "�������� ������:" << endl;
                for (size_t i = 0; i < player.weapons.size(); i++) {
                    cout << i + 1 << ". " << player.weapons[i].name
                        << " (������� ����: " << player.weapons[i].power << ")" << endl;
                }
                int weaponChoice;
                cin >> weaponChoice;
                weaponChoice--;
                if (weaponChoice >= 0 && weaponChoice < player.weapons.size()) {
                    cout << "�������� ����:" << endl;
                    for (size_t i = 0; i < enemies.size(); i++) {
                        cout << i + 1 << ". " << enemies[i].name << endl;
                    }
                    int target;
                    cin >> target;
                    target--;
                    if (target >= 0 && target < enemies.size()) {
                        int baseDamage = player.weapons[weaponChoice].power;
                        int damage = randomInRange(max(1, baseDamage - 2), baseDamage + 2);
                        // ����������� ����: ������� ���� 5% + ����� ������
                        int critChance = 5 + static_cast<int>(player.critBonus);
                        if (rand() % 100 < critChance) {
                            damage *= 2;
                            cout << "����! ";
                        }
                        if (rand() % 100 < 80) { // ���� ��������� 80%
                            enemies[target].health -= damage;
                            cout << "�� ������� " << damage << " ����� " << enemies[target].name << "!" << endl;
                            if (enemies[target].health <= 0) {
                                cout << enemies[target].name << " ��������!" << endl;
                                player.credits += enemies[target].reward;
                                enemies.erase(enemies.begin() + target);
                            }
                        }
                        else {
                            cout << "�� ������������!" << endl;
                        }
                    }
                }
            }
            else if (choice == 2) {
                if (!player.inventory.empty()) {
                    cout << "�������� �������:" << endl;
                    for (size_t i = 0; i < player.inventory.size(); i++) {
                        cout << i + 1 << ". " << player.inventory[i].name << endl;
                    }
                    int itemChoice;
                    cin >> itemChoice;
                    itemChoice--;
                    if (itemChoice >= 0 && itemChoice < player.inventory.size()) {
                        Item usedItem = player.inventory[itemChoice];
                        if (usedItem.name.find("�������") != string::npos) {
                            player.heal(usedItem.power);
                        }
                        else if (usedItem.name.find("�����") != string::npos) {
                            player.repairArmor(usedItem.power);
                        }
                        player.inventory.erase(player.inventory.begin() + itemChoice);
                    }
                }
                else {
                    cout << "� ��� ��� ���������!" << endl;
                }
            }
            else if (choice == 3) {
                if (rand() % 100 < 50) {
                    cout << "��� ������� �������!" << endl;
                    return;
                }
                else {
                    cout << "��� �� ������� �������!" << endl;
                }
            }
            else if (choice == 4) { // ����������
                cout << "�� ���������� �������, ����� ���������� �� ����!" << endl;
                if (rand() % 100 < 50) {
                    cout << "��������� �������! �� �������� ����� � ���� ������." << endl;
                    playerDodged = true;
                }
                else {
                    cout << "��������� �����������!" << endl;
                }
            }

            // ��� ������
            if (!playerDodged) {
                for (auto& enemy : enemies) {
                    int enemyDamage = enemy.getRandomDamage();
                    if (player.armor > 0) {
                        int armorDamage = enemyDamage / 2;
                        player.armor -= armorDamage;
                        if (player.armor < 0) player.armor = 0;
                        int healthDamage = enemyDamage - armorDamage;
                        player.health -= healthDamage;
                        cout << enemy.name << " ������� ��� " << enemyDamage
                            << " ����� (����� ��������� " << armorDamage << " �����)!" << endl;
                    }
                    else {
                        player.health -= enemyDamage;
                        cout << enemy.name << " ������� ��� " << enemyDamage << " �����!" << endl;
                    }
                }
            }
            else {
                cout << "��� ����� ������ � ���� ������ �� ����������." << endl;
            }

            if (player.health <= 0) {
                cout << "�� �������!" << endl;
                return;
            }
        }
        if (player.health > 0)
            cout << "��� ����� ���������!" << endl;
    }
};

// ����� ��� ������������� �������
class Sector {
public:
    string name;
    vector<Planet> planets;
    vector<Enemy> spaceEnemies;

    Sector(string n) : name(n) {}

    // ����������� ��� � ������������ ������� ���������
    void spaceCombat(Ship& ship, Character& player, vector<Enemy>& enemies) {
        bool shipDodged = false;
        while (!enemies.empty() && ship.hull > 0) {
            cout << "\n=== ����������� ��� ===" << endl;
            cout << "��������� �������:" << endl;
            ship.printStatus();

            cout << "\n����������:" << endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                cout << i + 1 << ". ";
                enemies[i].printStatus();
            }

            cout << "\n�������� ��������:" << endl;
            cout << "1. ���������" << endl;
            cout << "2. ������������ ���������" << endl;
            cout << "3. ���������� �������" << endl;
            cout << "4. ������ ���������" << endl;

            int choice;
            cin >> choice;
            shipDodged = false;

            if (choice == 1) {
                cout << "�������� ������:" << endl;
                for (size_t i = 0; i < ship.weapons.size(); i++) {
                    cout << i + 1 << ". " << ship.weapons[i].name << " (������� ����: " << ship.weapons[i].power << ")" << endl;
                }
                int weaponChoice;
                cin >> weaponChoice;
                weaponChoice--;
                if (weaponChoice >= 0 && weaponChoice < ship.weapons.size()) {
                    cout << "�������� ����:" << endl;
                    for (size_t i = 0; i < enemies.size(); i++) {
                        cout << i + 1 << ". " << enemies[i].name << endl;
                    }
                    int target;
                    cin >> target;
                    target--;
                    if (target >= 0 && target < enemies.size()) {
                        int baseDamage = ship.weapons[weaponChoice].power;
                        int damage = randomInRange(max(1, baseDamage - 2), baseDamage + 2);
                        // ����������� ���� � ������� ������ 5% + ����� ������ (��������� ��� ������� ��� ���������� ������, ���������� ����� ����)
                        int critChance = 5;
                        if (rand() % 100 < critChance) {
                            damage *= 2;
                            cout << "����������� ����! ";
                        }
                        if (rand() % 100 < 70) {
                            enemies[target].health -= damage;
                            cout << "�� ������� " << damage << " ����� " << enemies[target].name << "!" << endl;
                            if (enemies[target].health <= 0) {
                                cout << enemies[target].name << " ���������!" << endl;
                                player.credits += enemies[target].reward;
                                enemies.erase(enemies.begin() + target);
                            }
                        }
                        else {
                            cout << "�� ������������!" << endl;
                        }
                    }
                }
            }
            else if (choice == 2) {
                if (!ship.upgrades.empty()) {
                    cout << "�������� ���������:" << endl;
                    for (size_t i = 0; i < ship.upgrades.size(); i++) {
                        cout << i + 1 << ". " << ship.upgrades[i].name << endl;
                    }
                    int upgradeChoice;
                    cin >> upgradeChoice;
                    upgradeChoice--;
                    if (upgradeChoice >= 0 && upgradeChoice < ship.upgrades.size()) {
                        Item usedUpgrade = ship.upgrades[upgradeChoice];
                        if (usedUpgrade.name.find("���") != string::npos) {
                            ship.rechargeShields(usedUpgrade.power);
                        }
                        else if (usedUpgrade.name.find("������") != string::npos) {
                            ship.repair(usedUpgrade.power);
                        }
                        ship.upgrades.erase(ship.upgrades.begin() + upgradeChoice);
                    }
                }
                else {
                    cout << "� ��� ��� ���������!" << endl;
                }
            }
            else if (choice == 3) {
                if (rand() % 100 < ship.engines) {
                    cout << "��� ������� �������!" << endl;
                    return;
                }
                else {
                    cout << "��� �� ������� �������!" << endl;
                }
            }
            else if (choice == 4) {
                cout << "�� ���������� ������ ���������!" << endl;
                if (rand() % 100 < 50) {
                    cout << "������ �������! ��� ����� ������ � ���� ������ ��������������." << endl;
                    shipDodged = true;
                }
                else {
                    cout << "������ ����������!" << endl;
                }
            }

            // ��� ������
            if (!shipDodged) {
                for (auto& enemy : enemies) {
                    int enemyDamage = randomInRange(enemy.minPower, enemy.maxPower);
                    if (ship.shields > 0) {
                        int shieldDamage = enemyDamage / 2;
                        ship.shields -= shieldDamage;
                        if (ship.shields < 0) ship.shields = 0;
                        int hullDamage = enemyDamage - shieldDamage;
                        ship.hull -= hullDamage;
                        cout << enemy.name << " ������� ������ ������� " << enemyDamage
                            << " ����� (���� ��������� " << shieldDamage << " �����)!" << endl;
                    }
                    else {
                        ship.hull -= enemyDamage;
                        cout << enemy.name << " ������� ������ ������� " << enemyDamage << " �����!" << endl;
                    }
                }
            }
            else {
                cout << "��� ����� ������ � ���� ������ �� ����������." << endl;
            }

            if (ship.hull <= 0) {
                cout << "��� ������� ���������!" << endl;
                player.health = 0;
                return;
            }
        }
        if (ship.hull > 0)
            cout << "��� ����� ����������!" << endl;
    }

    // ��� ������������ ������� ��������� ������� ���������� ��� �������� �����������.
    void explore(Ship& ship, Character& player, int& globalClues) {
        cout << "\n�� ����� � " << name << endl;

        // ���� ���� ����������� �����, ���������� ����������� ���
        if (!spaceEnemies.empty()) {
            cout << "\n��� ������� � �������!" << endl;
            spaceCombat(ship, player, spaceEnemies);
        }

        if (!planets.empty()) {
            cout << "\n��������� ������� � �������:" << endl;
            for (size_t i = 0; i < planets.size(); i++) {
                cout << i + 1 << ". " << planets[i].name << endl;
            }

            cout << "�������� ������� ��� ������������ (0 - ���� �� �������): ";
            int choice;
            cin >> choice;
            if (choice > 0 && choice <= planets.size()) {
                bool foundClue = planets[choice - 1].explore(player);
                if (foundClue)
                    globalClues++;
            }
        }
        else {
            cout << "� ���� ������� ��� ������." << endl;
        }

        // �������������� ��������� ������� ��� ����������� ����� ���������
        if (rand() % 100 < 20) {
            cout << "\n��������� �������: �� ���������� ����������� ����������� �������! ";
            int bonus = randomInRange(5, 10);
            cout << "�� �������� ����� � " << bonus << " ��������." << endl;
            player.credits += bonus;
        }
    }
};

// ����� ��� ������������� �����
class Market {
public:
    vector<Item> items;
    vector<Item> shipUpgrades;
    vector<Item> weaponUpgrades;

    Market() {
        // ������������� ������� ���������
        items.push_back(Item("�������", "��������������� ��������", 50, 25));
        items.push_back(Item("�����", "��������������� �����", 40, 30));
        items.push_back(Item("������ �������", "���������� ������", 150, 20));
        items.push_back(Item("����� �������������� ���", "��������� �������� �����", 60, 20));

        // ��������� ��� �������
        shipUpgrades.push_back(Item("���������� ����", "����������� �������� �����", 200, 30));
        shipUpgrades.push_back(Item("��������� ����", "��������������� ������", 180, 40));
        shipUpgrades.push_back(Item("�����-�����", "������ ����������� ������", 300, 50));

        // ������������ ��������� ��� ������
        weaponUpgrades.push_back(Item("������ ������������ ������������", "����������� ���� ����. ����� �� 5%", 250, 5));
        weaponUpgrades.push_back(Item("��������� �����", "����������� ������� ���� ������ �� 3", 300, 3));

        // ������� ��� ��������� ��������� ��������� �� �����
        items.push_back(Item("����� �������", "��������������� �������� � �����", 100, 15));
        items.push_back(Item("�������������� ������", "��������� ��������������� ������� ��������", 80, 20));
    }

    void visit(Character& player, Ship& ship) {
        while (true) {
            cout << "\n=== ����� ===" << endl;
            cout << "���� �������: " << player.credits << endl;
            cout << "\n1. ������ ��������" << endl;
            cout << "2. ������ ��������� ��� �������" << endl;
            cout << "3. ������ ��������� ��� ������" << endl;
            cout << "4. �����" << endl;

            int choice;
            cin >> choice;
            if (choice == 1) {
                cout << "\n��������� ��������:" << endl;
                for (size_t i = 0; i < items.size(); i++) {
                    cout << i + 1 << ". " << items[i].name << " (" << items[i].description << ") - "
                        << items[i].value << " ��������" << endl;
                }
                cout << "�������� ������� ��� ������� (0 - �����): ";
                int itemChoice;
                cin >> itemChoice;
                if (itemChoice > 0 && itemChoice <= items.size()) {
                    Item selected = items[itemChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        player.inventory.push_back(selected);
                        cout << "�� ������ " << selected.name << "!" << endl;
                    }
                    else {
                        cout << "������������ ��������!" << endl;
                    }
                }
            }
            else if (choice == 2) {
                cout << "\n��������� ��������� ��� �������:" << endl;
                for (size_t i = 0; i < shipUpgrades.size(); i++) {
                    cout << i + 1 << ". " << shipUpgrades[i].name << " (" << shipUpgrades[i].description << ") - "
                        << shipUpgrades[i].value << " ��������" << endl;
                }
                cout << "�������� ��������� ��� ������� (0 - �����): ";
                int upgradeChoice;
                cin >> upgradeChoice;
                if (upgradeChoice > 0 && upgradeChoice <= shipUpgrades.size()) {
                    Item selected = shipUpgrades[upgradeChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        ship.upgrades.push_back(selected);
                        cout << "�� ������ " << selected.name << "!" << endl;
                        if (selected.name.find("���") != string::npos) {
                            ship.maxShields += selected.power;
                            ship.shields = ship.maxShields;
                        }
                        else if (selected.name.find("���������") != string::npos) {
                            ship.maxEngines += selected.power;
                            ship.engines = ship.maxEngines;
                        }
                    }
                    else {
                        cout << "������������ ��������!" << endl;
                    }
                }
            }
            else if (choice == 3) {
                cout << "\n��������� ��������� ��� ������:" << endl;
                for (size_t i = 0; i < weaponUpgrades.size(); i++) {
                    cout << i + 1 << ". " << weaponUpgrades[i].name << " (" << weaponUpgrades[i].description
                        << ") - " << weaponUpgrades[i].value << " ��������" << endl;
                }
                cout << "�������� ��������� ��� ������� (0 - �����): ";
                int wChoice;
                cin >> wChoice;
                if (wChoice > 0 && wChoice <= weaponUpgrades.size()) {
                    Item selected = weaponUpgrades[wChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        // ��������� ������ ���������
                        if (selected.name.find("������������ ������������") != string::npos) {
                            player.critBonus += selected.power; // ����������� ���� ������������ �����
                            cout << "��� ���� ������������ ����� �������� �� " << selected.power << "%!" << endl;
                        }
                        else if (selected.name.find("��������� �����") != string::npos) {
                            // ����������� ������� ���� ���� ��������� ��������� ������
                            for (auto& weapon : player.weapons) {
                                weapon.power += selected.power;
                            }
                            cout << "������� ���� ������ ������ �������� �� " << selected.power << "!" << endl;
                        }
                    }
                    else {
                        cout << "������������ ��������!" << endl;
                    }
                }
            }
            else if (choice == 4) {
                break;
            }
        }
    }
};

// ����� ��� ������������� ����
class Game {
public:
    Character player;
    Ship ship;
    vector<Sector> sectors;
    Market market;
    int currentSector;
    int cluesFound; // ���������� ��������� �������
    int totalClues; // ������� ����� �������

    Game() : player("�����"), ship("�������� ��������"), currentSector(0), cluesFound(0), totalClues(5) {
        initializeGame();
    }

    void initializeGame() {
        sectors.clear();
        // ��������� �������� � ������. ������� ������������ �������� ��� ������ �������.
        for (int i = 1; i <= 10; i++) {
            Sector sector("������ " + to_string(i));
            int planetCount = rand() % 4 + 1;
            for (int j = 1; j <= planetCount; j++) {
                bool hasClue = (rand() % 100 < 20);
                Planet planet("������� " + to_string(i) + "-" + to_string(j),
                    "�������� ������� " + to_string(i) + "-" + to_string(j),
                    hasClue);

                // ���������� ��������� �� �������
                if (rand() % 100 < 70) {
                    int itemType = rand() % 3;
                    if (itemType == 0) {
                        planet.items.push_back(Item("�������", "��������������� ��������", 0, 15 + rand() % 20));
                    }
                    else if (itemType == 1) {
                        planet.items.push_back(Item("�����", "��������������� �����", 0, 10 + rand() % 15));
                    }
                    else {
                        planet.items.push_back(Item("�������", "������", 50 + rand() % 100, 0));
                    }
                }

                // ���������� ������ �� ������� � ����������� �����
                if (rand() % 100 < 60) {
                    int enemyCount = rand() % 3 + 1;
                    for (int k = 0; k < enemyCount; k++) {
                        int enemyType = rand() % 3;
                        if (enemyType == 0) {
                            planet.enemies.push_back(Enemy("�����", "����������� �����",
                                randomInRange(30, 60), 5, 10, randomInRange(20, 50)));
                        }
                        else if (enemyType == 1) {
                            planet.enemies.push_back(Enemy("�����", "������ ����",
                                randomInRange(20, 40), 8, 16, randomInRange(15, 35)));
                        }
                        else {
                            planet.enemies.push_back(Enemy("������", "������� ������� ��������",
                                randomInRange(40, 80), 3, 10, randomInRange(10, 25)));
                        }
                    }
                }
                sector.planets.push_back(planet);
            }

            // ��������� ����������� ������ � ������ � ����������� �����
            if (rand() % 100 < 50) {
                int enemyCount = rand() % 2 + 1;
                for (int k = 0; k < enemyCount; k++) {
                    int enemyType = rand() % 2;
                    if (enemyType == 0) {
                        sector.spaceEnemies.push_back(Enemy("��������� �������", "������� ����������� �������",
                            randomInRange(50, 100), 10, 15, randomInRange(50, 100)));
                    }
                    else {
                        sector.spaceEnemies.push_back(Enemy("����-�����������", "���������� ������ ����",
                            randomInRange(30, 60), 15, 20, randomInRange(40, 80)));
                    }
                }
            }
            sectors.push_back(sector);
        }
    }

    // ������� ��� �������������� �������� ��� ��� (��� ��������� �������)
    void regenerateHealth() {
        if (player.health < player.maxHealth) {
            int regen = 2;
            player.heal(regen);
        }
    }

    // ������� ���� ���� ��� ����������� ����������/��������.
    // ��� ����� ���� ������� ������ ������������ ����� ���� ��������.
    void start() {
        cout << "=== LAST DEAL ===" << endl;
        cout << "�� - ������ ������, ����������� ��������� ��������� �����." << endl;
        cout << "���� ���� - ����� ������� ����������, ��������� �� ������." << endl;
        cout << "���������� ���������, �������� ������� � ��������� ������." << endl;
        cout << "�� ������� - ������ ���� � ���� ��������� ����������!" << endl;

        while (player.health > 0 && ship.hull > 0 && cluesFound < totalClues) {
            cout << "\n=== ������� ���� ===" << endl;
            cout << "������� ������: " << sectors[currentSector].name << endl;
            cout << "������� �������: " << cluesFound << "/" << totalClues << endl;
            cout << "�������: " << player.credits << endl;
            cout << "\n1. ����������� ������" << endl;
            cout << "2. ������� � ��������� ������" << endl;
            cout << "3. �������� �����" << endl;
            cout << "4. �������� ���������" << endl;
            cout << "5. ������������ ������� (�� ���������)" << endl;
            cout << "������� ����� ���������� ��������: ";
            int choice;
            cin >> choice;
            switch (choice) {
            case 1:
                sectors[currentSector].explore(ship, player, cluesFound);
                break;
            case 2:
                if (currentSector < sectors.size() - 1) {
                    currentSector++;
                    cout << "�� ������������� � " << sectors[currentSector].name << endl;
                    // �������� �������� �������������� ��������� ��������
                    if (rand() % 100 < 20) {
                        cout << "\n��������� �������: �� ���������� ����������� ����������� �������! ";
                        int bonus = randomInRange(5, 10);
                        cout << "�����: " << bonus << " ��������." << endl;
                        player.credits += bonus;
                    }
                }
                else {
                    cout << "�� �������� ����� ���������� ������������!" << endl;
                }
                break;
            case 3:
                market.visit(player, ship);
                break;
            case 4:
                player.printStatus();
                ship.printStatus();
                break;
            case 5:
                if (player.inventory.empty()) {
                    cout << "� ��� ��� ��������� � ���������!" << endl;
                }
                else {
                    cout << "\n=== ��������� ===" << endl;
                    for (size_t i = 0; i < player.inventory.size(); i++) {
                        cout << i + 1 << ". " << player.inventory[i].name
                            << " (" << player.inventory[i].description << ")" << endl;
                    }
                    cout << "�������� ������� ��� ������������� (0 - ������): ";
                    int itemChoice;
                    cin >> itemChoice;
                    if (itemChoice > 0 && itemChoice <= player.inventory.size()) {
                        Item usedItem = player.inventory[itemChoice - 1];
                        if (usedItem.name.find("�������") != string::npos) {
                            player.heal(usedItem.power);
                        }
                        else if (usedItem.name.find("�����") != string::npos) {
                            player.repairArmor(usedItem.power);
                        }
                        else {
                            cout << "���� ������� �� ����� ���� ����������� ��� ���." << endl;
                        }
                        player.inventory.erase(player.inventory.begin() + itemChoice - 1);
                    }
                }
                break;
            default:
                cout << "�������� �����. ���������� �����." << endl;
                break;
            }
            regenerateHealth();
        }

        if (player.health <= 0) {
            cout << "\n���� ��������. ��� �������� �����." << endl;
        }
        else if (ship.hull <= 0) {
            cout << "\n���� ��������. ��� ������� ���������." << endl;
        }
        else if (cluesFound >= totalClues) {
            // ��������� ������� � ������� ���� ��������� ��������
            cout << "\n�����������! �� ������� ��� �������." << endl;
            cout << "����� ���� ����� ��������� �����:" << endl;
            cout << "1. ����� �����������" << endl;
            cout << "2. ����� ����������� �������" << endl;
            int finalChoice;
            cin >> finalChoice;
            if (finalChoice == 1) {
                cout << "\n�� ������� ����� �����������. � ������������ ��� �� ����������� ���, �� ���������� �������." << endl;
                cout << "��������: �� �������� � �����������, ����� ����� ������������." << endl;
            }
            else if (finalChoice == 2) {
                cout << "\n�� ������� ����� ����������� �������. ��� �������� �������� ��� ��������, � �� ����� ��������� ������� ��������." << endl;
                cout << "��������: ���������� ��������������, � ���� ��������� ���������." << endl;
            }
            else {
                cout << "\n�������� �����. ������ ����������� �������� �����������." << endl;
            }
        }
    }
};

int main() {
    srand(time(0));
    setlocale(LC_ALL, "Russian");
    Game game;
    game.start();
    return 0;
}