#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

// Функция для генерации случайного числа в диапазоне [min, max]
int randomInRange(int minVal, int maxVal) {
    return minVal + rand() % (maxVal - minVal + 1);
}

// Класс для представления предмета
class Item {
public:
    string name;
    string description;
    int value;
    int power;

    Item(string n, string d, int v, int p) : name(n), description(d), value(v), power(p) {}
};

// Класс для представления навыка
class Skill {
public:
    string name;
    string description;
    int level;
    int maxLevel;

    Skill(string n, string d, int ml) : name(n), description(d), level(1), maxLevel(ml) {}
};

// Класс для представления корабля
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
        // Базовое оружие корабля
        weapons.push_back(Item("Лазерная пушка", "Базовая лазерная установка", 10, 15));
    }

    void printStatus() {
        cout << "\n=== Состояние корабля ===" << endl;
        cout << "Корпус: " << hull << "/" << maxHull << endl;
        cout << "Щиты: " << shields << "/" << maxShields << endl;
        cout << "Двигатели: " << engines << "/" << maxEngines << endl;
        cout << "Вооружение:" << endl;
        for (auto& weapon : weapons) {
            cout << "- " << weapon.name << " (Базовый урон: " << weapon.power << ")" << endl;
        }
        cout << "Улучшения:" << endl;
        for (auto& upgrade : upgrades) {
            cout << "- " << upgrade.name << endl;
        }
    }

    void repair(int amount) {
        hull = min(hull + amount, maxHull);
        cout << "Корпус восстановлен на " << amount << " единиц." << endl;
    }

    void rechargeShields(int amount) {
        shields = min(shields + amount, maxShields);
        cout << "Щиты перезаряжены на " << amount << " единиц." << endl;
    }
};

// Класс для представления персонажа
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
    double critBonus; // Дополнительный шанс критического удара в процентах

    Character(string n) : name(n), health(100), maxHealth(100), armor(50), maxArmor(50), credits(500), critBonus(0.0) {
        weapons.push_back(Item("Бластер", "Стандартный бластер", 5, 10));
        skills.push_back(Skill("Стрельба", "Увеличивает точность и урон оружием", 5));
        skills.push_back(Skill("Ремонт", "Позволяет лучше чинить корабль и снаряжение", 5));
        skills.push_back(Skill("Переговоры", "Помогает получать лучшие сделки и информацию", 5));
    }

    void printStatus() {
        cout << "\n=== Состояние персонажа ===" << endl;
        cout << "Здоровье: " << health << "/" << maxHealth << endl;
        cout << "Броня: " << armor << "/" << maxArmor << endl;
        cout << "Кредиты: " << credits << endl;
        cout << "Бонус шанса крит. удара: " << critBonus << "%" << endl;
        cout << "Оружие:" << endl;
        for (auto& weapon : weapons) {
            cout << "- " << weapon.name << " (Базовый урон: " << weapon.power << ")" << endl;
        }
        cout << "Навыки:" << endl;
        for (auto& skill : skills) {
            cout << "- " << skill.name << " (Уровень: " << skill.level << "/" << skill.maxLevel << ")" << endl;
        }
    }

    void heal(int amount) {
        int oldHealth = health;
        health = min(health + amount, maxHealth);
        cout << "Здоровье восстановлено с " << oldHealth << " до " << health << " единиц." << endl;
    }

    void repairArmor(int amount) {
        int oldArmor = armor;
        armor = min(armor + amount, maxArmor);
        cout << "Броня восстановлена с " << oldArmor << " до " << armor << " единиц." << endl;
    }
};

// Класс для представления врага
class Enemy {
public:
    string name;
    string description;
    int health;
    int maxHealth;
    int minPower; // минимальный урон
    int maxPower; // максимальный урон
    int reward;

    Enemy(string n, string d, int h, int minP, int maxP, int r)
        : name(n), description(d), health(h), maxHealth(h), minPower(minP), maxPower(maxP), reward(r) {}

    void printStatus() {
        cout << name << " (Здоровье: " << health << "/" << maxHealth << ")" << endl;
    }

    // Вычисляем случайный урон
    int getRandomDamage() {
        return randomInRange(minPower, maxPower);
    }
};

// Класс для представления планеты
class Planet {
public:
    string name;
    string description;
    vector<Item> items;
    vector<Enemy> enemies;
    bool hasClue;       // есть ли на планете зацепка
    bool clueCollected; // собрана ли зацепка
    bool hasRestSpot;   // возможность отдыха на планете

    Planet(string n, string d, bool clue) : name(n), description(d), hasClue(clue), clueCollected(false) {
        // 30% планета может иметь место для отдыха
        hasRestSpot = (rand() % 100 < 30);
    }

    // При исследовании планеты, если зацепка есть и еще не собрана, она собирается.
    // Также, если планета имеет зону отдыха, игрок может отдохнуть.
    bool explore(Character& player) {
        cout << "\nВы прибыли на " << name << ". " << description << endl;
        bool clueFoundHere = false;
        if (hasClue && !clueCollected) {
            cout << "\nВы обнаружили зацепку о местонахождении преступника!" << endl;
            clueCollected = true;
            clueFoundHere = true;
        }

        if (!items.empty()) {
            cout << "\nВы нашли предметы:" << endl;
            for (auto& item : items) {
                cout << "- " << item.name << " (" << item.description << ")" << endl;
                player.inventory.push_back(item);
            }
            items.clear();
        }

        if (!enemies.empty()) {
            cout << "\nВас атакуют!" << endl;
            combat(player, enemies);
        }

        // Возможность отдохнуть, если зона отдыха присутствует
        if (hasRestSpot) {
            int restChoice;
            cout << "\nВы обнаружили уютное место для отдыха. Желаете отдохнуть? (1 - да, 0 - нет): ";
            cin >> restChoice;
            if (restChoice == 1) {
                // Отдых восстанавливает здоровье и броню на случайную величину
                int healAmount = randomInRange(10, 20);
                int armorAmount = randomInRange(5, 15);
                cout << "Вы отдыхаете и восстанавливаете силы..." << endl;
                player.heal(healAmount);
                player.repairArmor(armorAmount);
            }
        }
        return clueFoundHere;
    }

    // Бой на планете с добавлением варианта "кувырок" для уклонения
    void combat(Character& player, vector<Enemy>& enemies) {
        bool playerDodged = false;
        while (!enemies.empty() && player.health > 0) {
            cout << "\n=== НАЗЕМНЫЙ БОЙ ===" << endl;
            cout << "Ваше состояние:" << endl;
            player.printStatus();

            cout << "\nПротивники:" << endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                cout << i + 1 << ". ";
                enemies[i].printStatus();
            }

            cout << "\nВыберите действие:" << endl;
            cout << "1. Атаковать" << endl;
            cout << "2. Использовать предмет" << endl;
            cout << "3. Попытаться убежать" << endl;
            cout << "4. Увернуться (кувырок)" << endl;

            int choice;
            cin >> choice;
            playerDodged = false; // сброс флага уклонения

            if (choice == 1) {
                cout << "Выберите оружие:" << endl;
                for (size_t i = 0; i < player.weapons.size(); i++) {
                    cout << i + 1 << ". " << player.weapons[i].name
                        << " (Базовый урон: " << player.weapons[i].power << ")" << endl;
                }
                int weaponChoice;
                cin >> weaponChoice;
                weaponChoice--;
                if (weaponChoice >= 0 && weaponChoice < player.weapons.size()) {
                    cout << "Выберите цель:" << endl;
                    for (size_t i = 0; i < enemies.size(); i++) {
                        cout << i + 1 << ". " << enemies[i].name << endl;
                    }
                    int target;
                    cin >> target;
                    target--;
                    if (target >= 0 && target < enemies.size()) {
                        int baseDamage = player.weapons[weaponChoice].power;
                        int damage = randomInRange(max(1, baseDamage - 2), baseDamage + 2);
                        // Критический удар: базовый шанс 5% + бонус игрока
                        int critChance = 5 + static_cast<int>(player.critBonus);
                        if (rand() % 100 < critChance) {
                            damage *= 2;
                            cout << "КРИТ! ";
                        }
                        if (rand() % 100 < 80) { // шанс попадания 80%
                            enemies[target].health -= damage;
                            cout << "Вы нанесли " << damage << " урона " << enemies[target].name << "!" << endl;
                            if (enemies[target].health <= 0) {
                                cout << enemies[target].name << " побежден!" << endl;
                                player.credits += enemies[target].reward;
                                enemies.erase(enemies.begin() + target);
                            }
                        }
                        else {
                            cout << "Вы промахнулись!" << endl;
                        }
                    }
                }
            }
            else if (choice == 2) {
                if (!player.inventory.empty()) {
                    cout << "Выберите предмет:" << endl;
                    for (size_t i = 0; i < player.inventory.size(); i++) {
                        cout << i + 1 << ". " << player.inventory[i].name << endl;
                    }
                    int itemChoice;
                    cin >> itemChoice;
                    itemChoice--;
                    if (itemChoice >= 0 && itemChoice < player.inventory.size()) {
                        Item usedItem = player.inventory[itemChoice];
                        if (usedItem.name.find("Аптечка") != string::npos) {
                            player.heal(usedItem.power);
                        }
                        else if (usedItem.name.find("Броня") != string::npos) {
                            player.repairArmor(usedItem.power);
                        }
                        player.inventory.erase(player.inventory.begin() + itemChoice);
                    }
                }
                else {
                    cout << "У вас нет предметов!" << endl;
                }
            }
            else if (choice == 3) {
                if (rand() % 100 < 50) {
                    cout << "Вам удалось сбежать!" << endl;
                    return;
                }
                else {
                    cout << "Вам не удалось сбежать!" << endl;
                }
            }
            else if (choice == 4) { // Увернуться
                cout << "Вы совершаете кувырок, чтобы уклониться от атак!" << endl;
                if (rand() % 100 < 50) {
                    cout << "Уклонение успешно! Вы избежали урона в этом раунде." << endl;
                    playerDodged = true;
                }
                else {
                    cout << "Уклонение провалилось!" << endl;
                }
            }

            // Ход врагов
            if (!playerDodged) {
                for (auto& enemy : enemies) {
                    int enemyDamage = enemy.getRandomDamage();
                    if (player.armor > 0) {
                        int armorDamage = enemyDamage / 2;
                        player.armor -= armorDamage;
                        if (player.armor < 0) player.armor = 0;
                        int healthDamage = enemyDamage - armorDamage;
                        player.health -= healthDamage;
                        cout << enemy.name << " наносит вам " << enemyDamage
                            << " урона (Броня поглотила " << armorDamage << " урона)!" << endl;
                    }
                    else {
                        player.health -= enemyDamage;
                        cout << enemy.name << " наносит вам " << enemyDamage << " урона!" << endl;
                    }
                }
            }
            else {
                cout << "Все атаки врагов в этом раунде вы увернулись." << endl;
            }

            if (player.health <= 0) {
                cout << "Вы погибли!" << endl;
                return;
            }
        }
        if (player.health > 0)
            cout << "Все враги побеждены!" << endl;
    }
};

// Класс для представления сектора
class Sector {
public:
    string name;
    vector<Planet> planets;
    vector<Enemy> spaceEnemies;

    Sector(string n) : name(n) {}

    // Космический бой с возможностью маневра уклонения
    void spaceCombat(Ship& ship, Character& player, vector<Enemy>& enemies) {
        bool shipDodged = false;
        while (!enemies.empty() && ship.hull > 0) {
            cout << "\n=== КОСМИЧЕСКИЙ БОЙ ===" << endl;
            cout << "Состояние корабля:" << endl;
            ship.printStatus();

            cout << "\nПротивники:" << endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                cout << i + 1 << ". ";
                enemies[i].printStatus();
            }

            cout << "\nВыберите действие:" << endl;
            cout << "1. Атаковать" << endl;
            cout << "2. Использовать улучшение" << endl;
            cout << "3. Попытаться сбежать" << endl;
            cout << "4. Маневр уклонения" << endl;

            int choice;
            cin >> choice;
            shipDodged = false;

            if (choice == 1) {
                cout << "Выберите оружие:" << endl;
                for (size_t i = 0; i < ship.weapons.size(); i++) {
                    cout << i + 1 << ". " << ship.weapons[i].name << " (Базовый урон: " << ship.weapons[i].power << ")" << endl;
                }
                int weaponChoice;
                cin >> weaponChoice;
                weaponChoice--;
                if (weaponChoice >= 0 && weaponChoice < ship.weapons.size()) {
                    cout << "Выберите цель:" << endl;
                    for (size_t i = 0; i < enemies.size(); i++) {
                        cout << i + 1 << ". " << enemies[i].name << endl;
                    }
                    int target;
                    cin >> target;
                    target--;
                    if (target >= 0 && target < enemies.size()) {
                        int baseDamage = ship.weapons[weaponChoice].power;
                        int damage = randomInRange(max(1, baseDamage - 2), baseDamage + 2);
                        // Критический удар с базовым шансом 5% + бонус игрока (поскольку для корабля нет отдельного бонуса, используем общий шанс)
                        int critChance = 5;
                        if (rand() % 100 < critChance) {
                            damage *= 2;
                            cout << "КРИТИЧЕСКИЙ УДАР! ";
                        }
                        if (rand() % 100 < 70) {
                            enemies[target].health -= damage;
                            cout << "Вы нанесли " << damage << " урона " << enemies[target].name << "!" << endl;
                            if (enemies[target].health <= 0) {
                                cout << enemies[target].name << " уничтожен!" << endl;
                                player.credits += enemies[target].reward;
                                enemies.erase(enemies.begin() + target);
                            }
                        }
                        else {
                            cout << "Вы промахнулись!" << endl;
                        }
                    }
                }
            }
            else if (choice == 2) {
                if (!ship.upgrades.empty()) {
                    cout << "Выберите улучшение:" << endl;
                    for (size_t i = 0; i < ship.upgrades.size(); i++) {
                        cout << i + 1 << ". " << ship.upgrades[i].name << endl;
                    }
                    int upgradeChoice;
                    cin >> upgradeChoice;
                    upgradeChoice--;
                    if (upgradeChoice >= 0 && upgradeChoice < ship.upgrades.size()) {
                        Item usedUpgrade = ship.upgrades[upgradeChoice];
                        if (usedUpgrade.name.find("Щит") != string::npos) {
                            ship.rechargeShields(usedUpgrade.power);
                        }
                        else if (usedUpgrade.name.find("Ремонт") != string::npos) {
                            ship.repair(usedUpgrade.power);
                        }
                        ship.upgrades.erase(ship.upgrades.begin() + upgradeChoice);
                    }
                }
                else {
                    cout << "У вас нет улучшений!" << endl;
                }
            }
            else if (choice == 3) {
                if (rand() % 100 < ship.engines) {
                    cout << "Вам удалось сбежать!" << endl;
                    return;
                }
                else {
                    cout << "Вам не удалось сбежать!" << endl;
                }
            }
            else if (choice == 4) {
                cout << "Вы выполняете маневр уклонения!" << endl;
                if (rand() % 100 < 50) {
                    cout << "Маневр успешен! Все атаки врагов в этом раунде нейтрализованы." << endl;
                    shipDodged = true;
                }
                else {
                    cout << "Маневр провалился!" << endl;
                }
            }

            // Ход врагов
            if (!shipDodged) {
                for (auto& enemy : enemies) {
                    int enemyDamage = randomInRange(enemy.minPower, enemy.maxPower);
                    if (ship.shields > 0) {
                        int shieldDamage = enemyDamage / 2;
                        ship.shields -= shieldDamage;
                        if (ship.shields < 0) ship.shields = 0;
                        int hullDamage = enemyDamage - shieldDamage;
                        ship.hull -= hullDamage;
                        cout << enemy.name << " наносит вашему кораблю " << enemyDamage
                            << " урона (Щиты поглотили " << shieldDamage << " урона)!" << endl;
                    }
                    else {
                        ship.hull -= enemyDamage;
                        cout << enemy.name << " наносит вашему кораблю " << enemyDamage << " урона!" << endl;
                    }
                }
            }
            else {
                cout << "Все атаки врагов в этом раунде вы увернулись." << endl;
            }

            if (ship.hull <= 0) {
                cout << "Ваш корабль уничтожен!" << endl;
                player.health = 0;
                return;
            }
        }
        if (ship.hull > 0)
            cout << "Все враги уничтожены!" << endl;
    }

    // При исследовании сектора случайное событие происходит при активном перемещении.
    void explore(Ship& ship, Character& player, int& globalClues) {
        cout << "\nВы вошли в " << name << endl;

        // Если есть космические враги, начинается космический бой
        if (!spaceEnemies.empty()) {
            cout << "\nВас атакуют в космосе!" << endl;
            spaceCombat(ship, player, spaceEnemies);
        }

        if (!planets.empty()) {
            cout << "\nДоступные планеты в секторе:" << endl;
            for (size_t i = 0; i < planets.size(); i++) {
                cout << i + 1 << ". " << planets[i].name << endl;
            }

            cout << "Выберите планету для исследования (0 - уйти из сектора): ";
            int choice;
            cin >> choice;
            if (choice > 0 && choice <= planets.size()) {
                bool foundClue = planets[choice - 1].explore(player);
                if (foundClue)
                    globalClues++;
            }
        }
        else {
            cout << "В этом секторе нет планет." << endl;
        }

        // Дополнительное случайное событие при перемещении между локациями
        if (rand() % 100 < 20) {
            cout << "\nСлучайное событие: вы обнаружили заброшенный космический корабль! ";
            int bonus = randomInRange(5, 10);
            cout << "Вы получили бонус в " << bonus << " кредитов." << endl;
            player.credits += bonus;
        }
    }
};

// Класс для представления рынка
class Market {
public:
    vector<Item> items;
    vector<Item> shipUpgrades;
    vector<Item> weaponUpgrades;

    Market() {
        // Инициализация обычных предметов
        items.push_back(Item("Аптечка", "Восстанавливает здоровье", 50, 25));
        items.push_back(Item("Броня", "Восстанавливает броню", 40, 30));
        items.push_back(Item("Мощный бластер", "Улучшенное оружие", 150, 20));
        items.push_back(Item("Малый энергетический щит", "Небольшое усиление щитов", 60, 20));

        // Улучшения для корабля
        shipUpgrades.push_back(Item("Улучшенные щиты", "Увеличивает мощность щитов", 200, 30));
        shipUpgrades.push_back(Item("Ремонтный дрон", "Восстанавливает корпус", 180, 40));
        shipUpgrades.push_back(Item("Турбо-лазер", "Мощное корабельное оружие", 300, 50));

        // Перманентные улучшения для оружия
        weaponUpgrades.push_back(Item("Модуль критического прицеливания", "Увеличивает шанс крит. удара на 5%", 250, 5));
        weaponUpgrades.push_back(Item("Усилитель урона", "Увеличивает базовый урон оружия на 3", 300, 3));

        // Добавим еще несколько случайных предметов на рынок
        items.push_back(Item("Набор ремонта", "Восстанавливает здоровье и броню", 100, 15));
        items.push_back(Item("Энергетический эссенс", "Мгновенно восстанавливает немного здоровья", 80, 20));
    }

    void visit(Character& player, Ship& ship) {
        while (true) {
            cout << "\n=== РЫНОК ===" << endl;
            cout << "Ваши кредиты: " << player.credits << endl;
            cout << "\n1. Купить предметы" << endl;
            cout << "2. Купить улучшения для корабля" << endl;
            cout << "3. Купить улучшения для оружия" << endl;
            cout << "4. Выйти" << endl;

            int choice;
            cin >> choice;
            if (choice == 1) {
                cout << "\nДоступные предметы:" << endl;
                for (size_t i = 0; i < items.size(); i++) {
                    cout << i + 1 << ". " << items[i].name << " (" << items[i].description << ") - "
                        << items[i].value << " кредитов" << endl;
                }
                cout << "Выберите предмет для покупки (0 - назад): ";
                int itemChoice;
                cin >> itemChoice;
                if (itemChoice > 0 && itemChoice <= items.size()) {
                    Item selected = items[itemChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        player.inventory.push_back(selected);
                        cout << "Вы купили " << selected.name << "!" << endl;
                    }
                    else {
                        cout << "Недостаточно кредитов!" << endl;
                    }
                }
            }
            else if (choice == 2) {
                cout << "\nДоступные улучшения для корабля:" << endl;
                for (size_t i = 0; i < shipUpgrades.size(); i++) {
                    cout << i + 1 << ". " << shipUpgrades[i].name << " (" << shipUpgrades[i].description << ") - "
                        << shipUpgrades[i].value << " кредитов" << endl;
                }
                cout << "Выберите улучшение для покупки (0 - назад): ";
                int upgradeChoice;
                cin >> upgradeChoice;
                if (upgradeChoice > 0 && upgradeChoice <= shipUpgrades.size()) {
                    Item selected = shipUpgrades[upgradeChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        ship.upgrades.push_back(selected);
                        cout << "Вы купили " << selected.name << "!" << endl;
                        if (selected.name.find("Щит") != string::npos) {
                            ship.maxShields += selected.power;
                            ship.shields = ship.maxShields;
                        }
                        else if (selected.name.find("двигатель") != string::npos) {
                            ship.maxEngines += selected.power;
                            ship.engines = ship.maxEngines;
                        }
                    }
                    else {
                        cout << "Недостаточно кредитов!" << endl;
                    }
                }
            }
            else if (choice == 3) {
                cout << "\nДоступные улучшения для оружия:" << endl;
                for (size_t i = 0; i < weaponUpgrades.size(); i++) {
                    cout << i + 1 << ". " << weaponUpgrades[i].name << " (" << weaponUpgrades[i].description
                        << ") - " << weaponUpgrades[i].value << " кредитов" << endl;
                }
                cout << "Выберите улучшение для покупки (0 - назад): ";
                int wChoice;
                cin >> wChoice;
                if (wChoice > 0 && wChoice <= weaponUpgrades.size()) {
                    Item selected = weaponUpgrades[wChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        // Применяем эффект улучшения
                        if (selected.name.find("критического прицеливания") != string::npos) {
                            player.critBonus += selected.power; // увеличиваем шанс критического удара
                            cout << "Ваш шанс критического удара увеличен на " << selected.power << "%!" << endl;
                        }
                        else if (selected.name.find("Усилитель урона") != string::npos) {
                            // Увеличиваем базовый урон всех оружейных предметов игрока
                            for (auto& weapon : player.weapons) {
                                weapon.power += selected.power;
                            }
                            cout << "Базовый урон вашего оружия увеличен на " << selected.power << "!" << endl;
                        }
                    }
                    else {
                        cout << "Недостаточно кредитов!" << endl;
                    }
                }
            }
            else if (choice == 4) {
                break;
            }
        }
    }
};

// Класс для представления игры
class Game {
public:
    Character player;
    Ship ship;
    vector<Sector> sectors;
    Market market;
    int currentSector;
    int cluesFound; // количество собранных зацепок
    int totalClues; // сколько нужно собрать

    Game() : player("Игрок"), ship("Звездный скиталец"), currentSector(0), cluesFound(0), totalClues(5) {
        initializeGame();
    }

    void initializeGame() {
        sectors.clear();
        // Генерация секторов и планет. Зацепки определяются случайно для каждой планеты.
        for (int i = 1; i <= 10; i++) {
            Sector sector("Сектор " + to_string(i));
            int planetCount = rand() % 4 + 1;
            for (int j = 1; j <= planetCount; j++) {
                bool hasClue = (rand() % 100 < 20);
                Planet planet("Планета " + to_string(i) + "-" + to_string(j),
                    "Описание планеты " + to_string(i) + "-" + to_string(j),
                    hasClue);

                // Добавление предметов на планету
                if (rand() % 100 < 70) {
                    int itemType = rand() % 3;
                    if (itemType == 0) {
                        planet.items.push_back(Item("Аптечка", "Восстанавливает здоровье", 0, 15 + rand() % 20));
                    }
                    else if (itemType == 1) {
                        planet.items.push_back(Item("Броня", "Восстанавливает броню", 0, 10 + rand() % 15));
                    }
                    else {
                        planet.items.push_back(Item("Кредиты", "Деньги", 50 + rand() % 100, 0));
                    }
                }

                // Добавление врагов на планету с диапазонами урона
                if (rand() % 100 < 60) {
                    int enemyCount = rand() % 3 + 1;
                    for (int k = 0; k < enemyCount; k++) {
                        int enemyType = rand() % 3;
                        if (enemyType == 0) {
                            planet.enemies.push_back(Enemy("Пират", "Космический пират",
                                randomInRange(30, 60), 5, 10, randomInRange(20, 50)));
                        }
                        else if (enemyType == 1) {
                            planet.enemies.push_back(Enemy("Робот", "Боевой дрон",
                                randomInRange(20, 40), 8, 16, randomInRange(15, 35)));
                        }
                        else {
                            planet.enemies.push_back(Enemy("Хищник", "Местное опасное существо",
                                randomInRange(40, 80), 3, 10, randomInRange(10, 25)));
                        }
                    }
                }
                sector.planets.push_back(planet);
            }

            // Добавляем космических врагов в сектор с диапазонами урона
            if (rand() % 100 < 50) {
                int enemyCount = rand() % 2 + 1;
                for (int k = 0; k < enemyCount; k++) {
                    int enemyType = rand() % 2;
                    if (enemyType == 0) {
                        sector.spaceEnemies.push_back(Enemy("Пиратский корабль", "Корабль космических пиратов",
                            randomInRange(50, 100), 10, 15, randomInRange(50, 100)));
                    }
                    else {
                        sector.spaceEnemies.push_back(Enemy("Дрон-истребитель", "Автономный боевой дрон",
                            randomInRange(30, 60), 15, 20, randomInRange(40, 80)));
                    }
                }
            }
            sectors.push_back(sector);
        }
    }

    // Функция для восстановления здоровья вне боя (без случайных событий)
    void regenerateHealth() {
        if (player.health < player.maxHealth) {
            int regen = 2;
            player.heal(regen);
        }
    }

    // Главное меню игры без возможности сохранения/загрузки.
    // При сборе всех зацепок игроку предлагается выбор двух концовок.
    void start() {
        cout << "=== LAST DEAL ===" << endl;
        cout << "Вы - бывший киллер, вынужденный выполнить последний заказ." << endl;
        cout << "Ваша цель - особо опасный преступник, сбежавший из тюрьмы." << endl;
        cout << "Исследуйте галактику, находите зацепки и выполните миссию." << endl;
        cout << "Но помните - космос таит в себе множество опасностей!" << endl;

        while (player.health > 0 && ship.hull > 0 && cluesFound < totalClues) {
            cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << endl;
            cout << "Текущий сектор: " << sectors[currentSector].name << endl;
            cout << "Собрано зацепок: " << cluesFound << "/" << totalClues << endl;
            cout << "Кредиты: " << player.credits << endl;
            cout << "\n1. Исследовать сектор" << endl;
            cout << "2. Перейти в следующий сектор" << endl;
            cout << "3. Посетить рынок" << endl;
            cout << "4. Показать состояние" << endl;
            cout << "5. Использовать предмет (из инвентаря)" << endl;
            cout << "Введите номер выбранного действия: ";
            int choice;
            cin >> choice;
            switch (choice) {
            case 1:
                sectors[currentSector].explore(ship, player, cluesFound);
                break;
            case 2:
                if (currentSector < sectors.size() - 1) {
                    currentSector++;
                    cout << "Вы переместились в " << sectors[currentSector].name << endl;
                    // Активное действие сопровождается случайным событием
                    if (rand() % 100 < 20) {
                        cout << "\nСлучайное событие: вы обнаружили заброшенный космический корабль! ";
                        int bonus = randomInRange(5, 10);
                        cout << "Бонус: " << bonus << " кредитов." << endl;
                        player.credits += bonus;
                    }
                }
                else {
                    cout << "Вы достигли конца известного пространства!" << endl;
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
                    cout << "У вас нет предметов в инвентаре!" << endl;
                }
                else {
                    cout << "\n=== ИНВЕНТАРЬ ===" << endl;
                    for (size_t i = 0; i < player.inventory.size(); i++) {
                        cout << i + 1 << ". " << player.inventory[i].name
                            << " (" << player.inventory[i].description << ")" << endl;
                    }
                    cout << "Выберите предмет для использования (0 - отмена): ";
                    int itemChoice;
                    cin >> itemChoice;
                    if (itemChoice > 0 && itemChoice <= player.inventory.size()) {
                        Item usedItem = player.inventory[itemChoice - 1];
                        if (usedItem.name.find("Аптечка") != string::npos) {
                            player.heal(usedItem.power);
                        }
                        else if (usedItem.name.find("Броня") != string::npos) {
                            player.repairArmor(usedItem.power);
                        }
                        else {
                            cout << "Этот предмет не может быть использован вне боя." << endl;
                        }
                        player.inventory.erase(player.inventory.begin() + itemChoice - 1);
                    }
                }
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
                break;
            }
            regenerateHealth();
        }

        if (player.health <= 0) {
            cout << "\nИГРА ОКОНЧЕНА. Ваш персонаж погиб." << endl;
        }
        else if (ship.hull <= 0) {
            cout << "\nИГРА ОКОНЧЕНА. Ваш корабль уничтожен." << endl;
        }
        else if (cluesFound >= totalClues) {
            // Финальное событие с выбором двух вариантов концовки
            cout << "\nПОЗДРАВЛЯЕМ! Вы собрали все зацепки." << endl;
            cout << "Перед вами стоит последний выбор:" << endl;
            cout << "1. Убить преступника" << endl;
            cout << "2. Сдать преступника полиции" << endl;
            int finalChoice;
            cin >> finalChoice;
            if (finalChoice == 1) {
                cout << "\nВы решаете убить преступника. В ожесточенном бою вы уничтожаете его, но поступаете жестоко." << endl;
                cout << "Концовка: Вы остались в одиночестве, теряя часть человечности." << endl;
            }
            else if (finalChoice == 2) {
                cout << "\nВы решаете сдать преступника полиции. Ваш поступок приносит вам уважение, и вы вновь обретаете доверие общества." << endl;
                cout << "Концовка: Правосудие восторжествует, а ваша репутация улучшится." << endl;
            }
            else {
                cout << "\nНеверный выбор. Судьба преступника остается неизвестной." << endl;
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