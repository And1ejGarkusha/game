#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Функция для генерации случайного числа в диапазоне [min, max]
int randomInRange(int minVal, int maxVal) {
    return minVal + std::rand() % (maxVal - minVal + 1);
}

// Класс для представления предмета
class Item {
public:
    std::string name;
    std::string description;
    int value;
    int power;

    Item(std::string n, std::string d, int v, int p) : name(n), description(d), value(v), power(p) {}
};

// Класс для представления навыка
class Skill {
public:
    std::string name;
    std::string description;
    int level;
    int maxLevel;

    Skill(std::string n, std::string d, int ml) : name(n), description(d), level(1), maxLevel(ml) {}
};

// Класс для представления корабля
class Ship {
public:
    std::string name;
    int hull;
    int maxHull;
    int shields;
    int maxShields;
    int engines;
    int maxEngines;
    std::vector<Item> weapons;
    std::vector<Item> upgrades;

    Ship(std::string n) : name(n), hull(100), maxHull(100), shields(50), maxShields(50), engines(30), maxEngines(30) {
        // Базовое оружие корабля
        weapons.push_back(Item("Лазерная пушка", "Базовая лазерная установка", 10, 15));
    }

    void printStatus() {
        std::cout << "\n=== Состояние корабля ===" << std::endl;
        std::cout << "Корпус: " << hull << "/" << maxHull << std::endl;
        std::cout << "Щиты: " << shields << "/" << maxShields << std::endl;
        std::cout << "Двигатели: " << engines << "/" << maxEngines << std::endl;
        std::cout << "Вооружение:" << std::endl;
        for (auto& weapon : weapons) {
            std::cout << "- " << weapon.name << " (Базовый урон: " << weapon.power << ")" << std::endl;
        }
        std::cout << "Улучшения:" << std::endl;
        for (auto& upgrade : upgrades) {
            std::cout << "- " << upgrade.name << std::endl;
        }
    }

    void repair(int amount) {
        hull = std::min(hull + amount, maxHull);
        std::cout << "Корпус восстановлен на " << amount << " единиц." << std::endl;
    }

    void rechargeShields(int amount) {
        shields = std::min(shields + amount, maxShields);
        std::cout << "Щиты перезаряжены на " << amount << " единиц." << std::endl;
    }
};

// Класс для представления персонажа
class Character {
public:
    std::string name;
    int health;
    int maxHealth;
    int armor;
    int maxArmor;
    std::vector<Item> weapons;
    std::vector<Item> inventory;
    std::vector<Skill> skills;
    int credits;
    double critBonus; // Дополнительный шанс критического удара в процентах

    Character(std::string n) : name(n), health(100), maxHealth(100), armor(50), maxArmor(50), credits(500), critBonus(0.0) {
        weapons.push_back(Item("Бластер", "Стандартный бластер", 5, 10));
        skills.push_back(Skill("Стрельба", "Увеличивает точность и урон оружием", 5));
        skills.push_back(Skill("Ремонт", "Позволяет лучше чинить корабль и снаряжение", 5));
        skills.push_back(Skill("Переговоры", "Помогает получать лучшие сделки и информацию", 5));
    }

    void printStatus() {
        std::cout << "\n=== Состояние персонажа ===" << std::endl;
        std::cout << "Здоровье: " << health << "/" << maxHealth << std::endl;
        std::cout << "Броня: " << armor << "/" << maxArmor << std::endl;
        std::cout << "Кредиты: " << credits << std::endl;
        std::cout << "Бонус шанса крит. удара: " << critBonus << "%" << std::endl;
        std::cout << "Оружие:" << std::endl;
        for (auto& weapon : weapons) {
            std::cout << "- " << weapon.name << " (Базовый урон: " << weapon.power << ")" << std::endl;
        }
        std::cout << "Навыки:" << std::endl;
        for (auto& skill : skills) {
            std::cout << "- " << skill.name << " (Уровень: " << skill.level << "/" << skill.maxLevel << ")" << std::endl;
        }
    }

    void heal(int amount) {
        int oldHealth = health;
        health = std::min(health + amount, maxHealth);
        std::cout << "Здоровье восстановлено с " << oldHealth << " до " << health << " единиц." << std::endl;
    }

    void repairArmor(int amount) {
        int oldArmor = armor;
        armor = std::min(armor + amount, maxArmor);
        std::cout << "Броня восстановлена с " << oldArmor << " до " << armor << " единиц." << std::endl;
    }
};

// Класс для представления врага
class Enemy {
public:
    std::string name;
    std::string description;
    int health;
    int maxHealth;
    int minPower; // минимальный урон
    int maxPower; // максимальный урон
    int reward;

    Enemy(std::string n, std::string d, int h, int minP, int maxP, int r)
        : name(n), description(d), health(h), maxHealth(h), minPower(minP), maxPower(maxP), reward(r) {}

    void printStatus() {
        std::cout << name << " (Здоровье: " << health << "/" << maxHealth << ")" << std::endl;
    }

    // Вычисляем случайный урон
    int getRandomDamage() {
        return randomInRange(minPower, maxPower);
    }
};

// Класс для представления планеты
class Planet {
public:
    std::string name;
    std::string description;
    std::vector<Item> items;
    std::vector<Enemy> enemies;
    bool hasClue;       // есть ли на планете зацепка
    bool clueCollected; // собрана ли зацепка
    bool hasRestSpot;   // возможность отдыха на планете

    Planet(std::string n, std::string d, bool clue) : name(n), description(d), hasClue(clue), clueCollected(false) {
        // 30% планета может иметь место для отдыха
        hasRestSpot = (std::rand() % 100 < 30);
    }

    // При исследовании планеты, если зацепка есть и еще не собрана, она собирается.
    // Также, если планета имеет зону отдыха, игрок может отдохнуть.
    bool explore(Character& player) {
        std::cout << "\nВы прибыли на " << name << ". " << description << std::endl;
        bool clueFoundHere = false;
        if (hasClue && !clueCollected) {
            std::cout << "\nВы обнаружили зацепку о местонахождении преступника!" << std::endl;
            clueCollected = true;
            clueFoundHere = true;
        }

        if (!items.empty()) {
            std::cout << "\nВы нашли предметы:" << std::endl;
            for (auto& item : items) {
                std::cout << "- " << item.name << " (" << item.description << ")" << std::endl;
                player.inventory.push_back(item);
            }
            items.clear();
        }

        if (!enemies.empty()) {
            std::cout << "\nВас атакуют!" << std::endl;
            combat(player, enemies);
        }

        // Возможность отдохнуть, если зона отдыха присутствует
        if (hasRestSpot) {
            int restChoice;
            std::cout << "\nВы обнаружили уютное место для отдыха. Желаете отдохнуть? (1 - да, 0 - нет): ";
            std::cin >> restChoice;
            if (restChoice == 1) {
                // Отдых восстанавливает здоровье и броню на случайную величину
                int healAmount = randomInRange(10, 20);
                int armorAmount = randomInRange(5, 15);
                std::cout << "Вы отдыхаете и восстанавливаете силы..." << std::endl;
                player.heal(healAmount);
                player.repairArmor(armorAmount);
            }
        }
        return clueFoundHere;
    }

    // Бой на планете с добавлением варианта "кувырок" для уклонения
    void combat(Character& player, std::vector<Enemy>& enemies) {
        bool playerDodged = false;
        while (!enemies.empty() && player.health > 0) {
            std::cout << "\n=== НАЗЕМНЫЙ БОЙ ===" << std::endl;
            std::cout << "Ваше состояние:" << std::endl;
            player.printStatus();

            std::cout << "\nПротивники:" << std::endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                std::cout << i + 1 << ". ";
                enemies[i].printStatus();
            }

            std::cout << "\nВыберите действие:" << std::endl;
            std::cout << "1. Атаковать" << std::endl;
            std::cout << "2. Использовать предмет" << std::endl;
            std::cout << "3. Попытаться убежать" << std::endl;
            std::cout << "4. Увернуться (кувырок)" << std::endl;

            int choice;
            std::cin >> choice;
            playerDodged = false; // сброс флага уклонения

            if (choice == 1) {
                std::cout << "Выберите оружие:" << std::endl;
                for (size_t i = 0; i < player.weapons.size(); i++) {
                    std::cout << i + 1 << ". " << player.weapons[i].name
                        << " (Базовый урон: " << player.weapons[i].power << ")" << std::endl;
                }
                int weaponChoice;
                std::cin >> weaponChoice;
                weaponChoice--;
                if (weaponChoice >= 0 && weaponChoice < player.weapons.size()) {
                    std::cout << "Выберите цель:" << std::endl;
                    for (size_t i = 0; i < enemies.size(); i++) {
                        std::cout << i + 1 << ". " << enemies[i].name << std::endl;
                    }
                    int target;
                    std::cin >> target;
                    target--;
                    if (target >= 0 && target < enemies.size()) {
                        int baseDamage = player.weapons[weaponChoice].power;
                        int damage = randomInRange(std::max(1, baseDamage - 2), baseDamage + 2);
                        // Критический удар: базовый шанс 5% + бонус игрока
                        int critChance = 5 + static_cast<int>(player.critBonus);
                        if (std::rand() % 100 < critChance) {
                            damage *= 2;
                            std::cout << "КРИТ! ";
                        }
                        if (std::rand() % 100 < 80) { // шанс попадания 80%
                            enemies[target].health -= damage;
                            std::cout << "Вы нанесли " << damage << " урона " << enemies[target].name << "!" << std::endl;
                            if (enemies[target].health <= 0) {
                                std::cout << enemies[target].name << " побежден!" << std::endl;
                                player.credits += enemies[target].reward;
                                enemies.erase(enemies.begin() + target);
                            }
                        }
                        else {
                            std::cout << "Вы промахнулись!" << std::endl;
                        }
                    }
                }
            }
            else if (choice == 2) {
                if (!player.inventory.empty()) {
                    std::cout << "Выберите предмет:" << std::endl;
                    for (size_t i = 0; i < player.inventory.size(); i++) {
                        std::cout << i + 1 << ". " << player.inventory[i].name << std::endl;
                    }
                    int itemChoice;
                    std::cin >> itemChoice;
                    itemChoice--;
                    if (itemChoice >= 0 && itemChoice < player.inventory.size()) {
                        Item usedItem = player.inventory[itemChoice];
                        if (usedItem.name.find("Аптечка") != std::string::npos) {
                            player.heal(usedItem.power);
                        }
                        else if (usedItem.name.find("Броня") != std::string::npos) {
                            player.repairArmor(usedItem.power);
                        }
                        player.inventory.erase(player.inventory.begin() + itemChoice);
                    }
                }
                else {
                    std::cout << "У вас нет предметов!" << std::endl;
                }
            }
            else if (choice == 3) {
                if (std::rand() % 100 < 50) {
                    std::cout << "Вам удалось сбежать!" << std::endl;
                    return;
                }
                else {
                    std::cout << "Вам не удалось сбежать!" << std::endl;
                }
            }
            else if (choice == 4) { // Увернуться
                std::cout << "Вы совершаете кувырок, чтобы уклониться от атак!" << std::endl;
                if (std::rand() % 100 < 50) {
                    std::cout << "Уклонение успешно! Вы избежали урона в этом раунде." << std::endl;
                    playerDodged = true;
                }
                else {
                    std::cout << "Уклонение провалилось!" << std::endl;
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
                        std::cout << enemy.name << " наносит вам " << enemyDamage
                            << " урона (Броня поглотила " << armorDamage << " урона)!" << std::endl;
                    }
                    else {
                        player.health -= enemyDamage;
                        std::cout << enemy.name << " наносит вам " << enemyDamage << " урона!" << std::endl;
                    }
                }
            }
            else {
                std::cout << "От всех атак врагов в этом раунде вы увернулись." << std::endl;
            }

            if (player.health <= 0) {
                std::cout << "Вы погибли!" << std::endl;
                return;
            }
        }
        if (player.health > 0)
            std::cout << "Все враги побеждены!" << std::endl;
    }
};

// Класс для представления сектора
class Sector {
public:
    std::string name;
    std::vector<Planet> planets;
    std::vector<Enemy> spaceEnemies;

    Sector(std::string n) : name(n) {}

    // Космический бой с возможностью маневра уклонения
    void spaceCombat(Ship& ship, Character& player, std::vector<Enemy>& enemies) {
        bool shipDodged = false;
        while (!enemies.empty() && ship.hull > 0) {
            std::cout << "\n=== КОСМИЧЕСКИЙ БОЙ ===" << std::endl;
            std::cout << "Состояние корабля:" << std::endl;
            ship.printStatus();

            std::cout << "\nПротивники:" << std::endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                std::cout << i + 1 << ". ";
                enemies[i].printStatus();
            }

            std::cout << "\nВыберите действие:" << std::endl;
            std::cout << "1. Атаковать" << std::endl;
            std::cout << "2. Использовать улучшение" << std::endl;
            std::cout << "3. Попытаться сбежать" << std::endl;
            std::cout << "4. Маневр уклонения" << std::endl;

            int choice;
            std::cin >> choice;
            shipDodged = false;

            if (choice == 1) {
                std::cout << "Выберите оружие:" << std::endl;
                for (size_t i = 0; i < ship.weapons.size(); i++) {
                    std::cout << i + 1 << ". " << ship.weapons[i].name << " (Базовый урон: " << ship.weapons[i].power << ")" << std::endl;
                }
                int weaponChoice;
                std::cin >> weaponChoice;
                weaponChoice--;
                if (weaponChoice >= 0 && weaponChoice < ship.weapons.size()) {
                    std::cout << "Выберите цель:" << std::endl;
                    for (size_t i = 0; i < enemies.size(); i++) {
                        std::cout << i + 1 << ". " << enemies[i].name << std::endl;
                    }
                    int target;
                    std::cin >> target;
                    target--;
                    if (target >= 0 && target < enemies.size()) {
                        int baseDamage = ship.weapons[weaponChoice].power;
                        int damage = randomInRange(std::max(1, baseDamage - 2), baseDamage + 2);
                        // Критический удар с базовым шансом 5% + бонус игрока (поскольку для корабля нет отдельного бонуса, используем общий шанс)
                        int critChance = 5;
                        if (std::rand() % 100 < critChance) {
                            damage *= 2;
                            std::cout << "КРИТИЧЕСКИЙ УДАР! ";
                        }
                        if (std::rand() % 100 < 70) {
                            enemies[target].health -= damage;
                            std::cout << "Вы нанесли " << damage << " урона " << enemies[target].name << "!" << std::endl;
                            if (enemies[target].health <= 0) {
                                std::cout << enemies[target].name << " уничтожен!" << std::endl;
                                player.credits += enemies[target].reward;
                                enemies.erase(enemies.begin() + target);
                            }
                        }
                        else {
                            std::cout << "Вы промахнулись!" << std::endl;
                        }
                    }
                }
            }
            else if (choice == 2) {
                if (!ship.upgrades.empty()) {
                    std::cout << "Выберите улучшение:" << std::endl;
                    for (size_t i = 0; i < ship.upgrades.size(); i++) {
                        std::cout << i + 1 << ". " << ship.upgrades[i].name << std::endl;
                    }
                    int upgradeChoice;
                    std::cin >> upgradeChoice;
                    upgradeChoice--;
                    if (upgradeChoice >= 0 && upgradeChoice < ship.upgrades.size()) {
                        Item usedUpgrade = ship.upgrades[upgradeChoice];
                        if (usedUpgrade.name.find("Щит") != std::string::npos) {
                            ship.rechargeShields(usedUpgrade.power);
                        }
                        else if (usedUpgrade.name.find("Ремонт") != std::string::npos) {
                            ship.repair(usedUpgrade.power);
                        }
                        ship.upgrades.erase(ship.upgrades.begin() + upgradeChoice);
                    }
                }
                else {
                    std::cout << "У вас нет улучшений!" << std::endl;
                }
            }
            else if (choice == 3) {
                if (std::rand() % 100 < ship.engines) {
                    std::cout << "Вам удалось сбежать!" << std::endl;
                    return;
                }
                else {
                    std::cout << "Вам не удалось сбежать!" << std::endl;
                }
            }
            else if (choice == 4) {
                std::cout << "Вы выполняете маневр уклонения!" << std::endl;
                if (std::rand() % 100 < 50) {
                    std::cout << "Маневр успешен! Все атаки врагов в этом раунде нейтрализованы." << std::endl;
                    shipDodged = true;
                }
                else {
                    std::cout << "Маневр провалился!" << std::endl;
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
                        std::cout << enemy.name << " наносит вашему кораблю " << enemyDamage
                            << " урона (Щиты поглотили " << shieldDamage << " урона)!" << std::endl;
                    }
                    else {
                        ship.hull -= enemyDamage;
                        std::cout << enemy.name << " наносит вашему кораблю " << enemyDamage << " урона!" << std::endl;
                    }
                }
            }
            else {
                std::cout << "От всех атак врагов в этом раунде вы увернулись." << std::endl;
            }

            if (ship.hull <= 0) {
                std::cout << "Ваш корабль уничтожен!" << std::endl;
                player.health = 0;
                return;
            }
        }
        if (ship.hull > 0)
            std::cout << "Все враги уничтожены!" << std::endl;
    }

    // При исследовании сектора случайное событие происходит при активном перемещении.
    void explore(Ship& ship, Character& player, int& globalClues) {
        std::cout << "\nВы вошли в " << name << std::endl;

        // Если есть космические враги, начинается космический бой
        if (!spaceEnemies.empty()) {
            std::cout << "\nВас атакуют в космосе!" << std::endl;
            spaceCombat(ship, player, spaceEnemies);
        }

        if (ship.hull <= 0 || player.health <= 0) {
            return;
        }

        if (!planets.empty()) {
            std::cout << "\nДоступные планеты в секторе:" << std::endl;
            for (size_t i = 0; i < planets.size(); i++) {
                std::cout << i + 1 << ". " << planets[i].name << std::endl;
            }

            std::cout << "Выберите планету для исследования (0 - уйти из сектора): ";
            int choice;
            std::cin >> choice;
            if (choice > 0 && choice <= planets.size()) {
                bool foundClue = planets[choice - 1].explore(player);
                if (foundClue)
                    globalClues++;
            }
        }
        else {
            std::cout << "В этом секторе нет планет." << std::endl;
        }

        // Дополнительное случайное событие при перемещении между локациями
        if (std::rand() % 100 < 20) {
            std::cout << "\nСлучайное событие: вы обнаружили заброшенный космический корабль! ";
            int bonus = randomInRange(5, 10);
            std::cout << "Вы получили бонус в " << bonus << " кредитов." << std::endl;
            player.credits += bonus;
        }
    }
};

// Класс для представления рынка
class Market {
public:
    std::vector<Item> items;
    std::vector<Item> shipUpgrades;
    std::vector<Item> weaponUpgrades;

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
            std::cout << "\n=== РЫНОК ===" << std::endl;
            std::cout << "Ваши кредиты: " << player.credits << std::endl;
            std::cout << "\n1. Купить предметы" << std::endl;
            std::cout << "2. Купить улучшения для корабля" << std::endl;
            std::cout << "3. Купить улучшения для оружия" << std::endl;
            std::cout << "4. Выйти" << std::endl;

            int choice;
            std::cin >> choice;
            if (choice == 1) {
                std::cout << "\nДоступные предметы:" << std::endl;
                for (size_t i = 0; i < items.size(); i++) {
                    std::cout << i + 1 << ". " << items[i].name << " (" << items[i].description << ") - "
                        << items[i].value << " кредитов" << std::endl;
                }
                std::cout << "Выберите предмет для покупки (0 - назад): ";
                int itemChoice;
                std::cin >> itemChoice;
                if (itemChoice > 0 && itemChoice <= items.size()) {
                    Item selected = items[itemChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        player.inventory.push_back(selected);
                        std::cout << "Вы купили " << selected.name << "!" << std::endl;
                    }
                    else {
                        std::cout << "Недостаточно кредитов!" << std::endl;
                    }
                }
            }
            else if (choice == 2) {
                std::cout << "\nДоступные улучшения для корабля:" << std::endl;
                for (size_t i = 0; i < shipUpgrades.size(); i++) {
                    std::cout << i + 1 << ". " << shipUpgrades[i].name << " (" << shipUpgrades[i].description << ") - "
                        << shipUpgrades[i].value << " кредитов" << std::endl;
                }
                std::cout << "Выберите улучшение для покупки (0 - назад): ";
                int upgradeChoice;
                std::cin >> upgradeChoice;
                if (upgradeChoice > 0 && upgradeChoice <= shipUpgrades.size()) {
                    Item selected = shipUpgrades[upgradeChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        ship.upgrades.push_back(selected);
                        std::cout << "Вы купили " << selected.name << "!" << std::endl;
                        if (selected.name.find("Щит") != std::string::npos) {
                            ship.maxShields += selected.power;
                            ship.shields = ship.maxShields;
                        }
                        else if (selected.name.find("двигатель") != std::string::npos) {
                            ship.maxEngines += selected.power;
                            ship.engines = ship.maxEngines;
                        }
                    }
                    else {
                        std::cout << "Недостаточно кредитов!" << std::endl;
                    }
                }
            }
            else if (choice == 3) {
                std::cout << "\nДоступные улучшения для оружия:" << std::endl;
                for (size_t i = 0; i < weaponUpgrades.size(); i++) {
                    std::cout << i + 1 << ". " << weaponUpgrades[i].name << " (" << weaponUpgrades[i].description
                        << ") - " << weaponUpgrades[i].value << " кредитов" << std::endl;
                }
                std::cout << "Выберите улучшение для покупки (0 - назад): ";
                int wChoice;
                std::cin >> wChoice;
                if (wChoice > 0 && wChoice <= weaponUpgrades.size()) {
                    Item selected = weaponUpgrades[wChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        // Применяем эффект улучшения
                        if (selected.name.find("критического прицеливания") != std::string::npos) {
                            player.critBonus += selected.power; // увеличиваем шанс критического удара
                            std::cout << "Ваш шанс критического удара увеличен на " << selected.power << "%!" << std::endl;
                        }
                        else if (selected.name.find("Усилитель урона") != std::string::npos) {
                            // Увеличиваем базовый урон всех оружейных предметов игрока
                            for (auto& weapon : player.weapons) {
                                weapon.power += selected.power;
                            }
                            std::cout << "Базовый урон вашего оружия увеличен на " << selected.power << "!" << std::endl;
                        }
                    }
                    else {
                        std::cout << "Недостаточно кредитов!" << std::endl;
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
    std::vector<Sector> sectors;
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
            Sector sector("Сектор " + std::to_string(i));
            int planetCount = std::rand() % 4 + 1;
            for (int j = 1; j <= planetCount; j++) {
                bool hasClue = (std::rand() % 100 < 20);
                Planet planet("Планета " + std::to_string(i) + "-" + std::to_string(j),
                    "Описание планеты " + std::to_string(i) + "-" + std::to_string(j),
                    hasClue);

                // Добавление предметов на планету
                if (std::rand() % 100 < 70) {
                    int itemType = std::rand() % 3;
                    if (itemType == 0) {
                        planet.items.push_back(Item("Аптечка", "Восстанавливает здоровье", 0, 15 + std::rand() % 20));
                    }
                    else if (itemType == 1) {
                        planet.items.push_back(Item("Броня", "Восстанавливает броню", 0, 10 + std::rand() % 15));
                    }
                    else {
                        planet.items.push_back(Item("Кредиты", "Деньги", 50 + std::rand() % 100, 0));
                    }
                }

                // Добавление врагов на планету с диапазонами урона
                if (std::rand() % 100 < 60) {
                    int enemyCount = std::rand() % 3 + 1;
                    for (int k = 0; k < enemyCount; k++) {
                        int enemyType = std::rand() % 3;
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
            if (std::rand() % 100 < 50) {
                int enemyCount = std::rand() % 2 + 1;
                for (int k = 0; k < enemyCount; k++) {
                    int enemyType = std::rand() % 2;
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
        std::vector<Planet*> allPlanetsWithoutClue;
        int clueCount = 0;
        for (auto& sector : sectors) {
            for (auto& planet : sector.planets) {
                if (planet.hasClue) clueCount++;
                else allPlanetsWithoutClue.push_back(&planet);
            }
        }
        while (clueCount < totalClues && !allPlanetsWithoutClue.empty()) {
            int idx = std::rand() % allPlanetsWithoutClue.size();
            allPlanetsWithoutClue[idx]->hasClue = true;
            clueCount++;
            allPlanetsWithoutClue.erase(allPlanetsWithoutClue.begin() + idx);
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
        std::cout << "=== LAST DEAL ===" << std::endl;
        std::cout << "Вы - бывший киллер, вынужденный выполнить последний заказ." << std::endl;
        std::cout << "Ваша цель - особо опасный преступник, сбежавший из тюрьмы." << std::endl;
        std::cout << "Исследуйте галактику, находите зацепки и выполните миссию." << std::endl;
        std::cout << "Но помните - космос таит в себе множество опасностей!" << std::endl;

        while (player.health > 0 && ship.hull > 0 && cluesFound < totalClues) {
            std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << std::endl;
            std::cout << "Текущий сектор: " << sectors[currentSector].name << std::endl;
            std::cout << "Собрано зацепок: " << cluesFound << "/" << totalClues << std::endl;
            std::cout << "Кредиты: " << player.credits << std::endl;
            std::cout << "\n1. Исследовать сектор" << std::endl;
            std::cout << "2. Перейти в следующий сектор" << std::endl;
            std::cout << "3. Посетить рынок" << std::endl;
            std::cout << "4. Показать состояние" << std::endl;
            std::cout << "5. Использовать предмет (из инвентаря)" << std::endl;
            std::cout << "Введите номер выбранного действия: ";
            int choice;
            std::cin >> choice;
            switch (choice) {
            case 1:
                sectors[currentSector].explore(ship, player, cluesFound);
                break;
            case 2:
                if (currentSector < sectors.size() - 1) {
                    currentSector++;
                    std::cout << "Вы переместились в " << sectors[currentSector].name << std::endl;
                    // Активное действие сопровождается случайным событием
                    if (std::rand() % 100 < 20) {
                        std::cout << "\nСлучайное событие: вы обнаружили заброшенный космический корабль! ";
                        int bonus = randomInRange(5, 10);
                        std::cout << "Бонус: " << bonus << " кредитов." << std::endl;
                        player.credits += bonus;
                    }
                }
                else {
                    std::cout << "Вы достигли конца известного пространства!" << std::endl;
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
                    std::cout << "У вас нет предметов в инвентаре!" << std::endl;
                }
                else {
                    std::cout << "\n=== ИНВЕНТАРЬ ===" << std::endl;
                    for (size_t i = 0; i < player.inventory.size(); i++) {
                        std::cout << i + 1 << ". " << player.inventory[i].name
                            << " (" << player.inventory[i].description << ")" << std::endl;
                    }
                    std::cout << "Выберите предмет для использования (0 - отмена): ";
                    int itemChoice;
                    std::cin >> itemChoice;
                    if (itemChoice > 0 && itemChoice <= player.inventory.size()) {
                        Item usedItem = player.inventory[itemChoice - 1];
                        if (usedItem.name.find("Аптечка") != std::string::npos) {
                            player.heal(usedItem.power);
                        }
                        else if (usedItem.name.find("Броня") != std::string::npos) {
                            player.repairArmor(usedItem.power);
                        }
                        else {
                            std::cout << "Этот предмет не может быть использован вне боя." << std::endl;
                        }
                        player.inventory.erase(player.inventory.begin() + itemChoice - 1);
                    }
                }
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
            }
            regenerateHealth();
        }

        if (player.health <= 0) {
            std::cout << "\nИГРА ОКОНЧЕНА. Ваш персонаж погиб." << std::endl;
        }
        else if (ship.hull <= 0) {
            std::cout << "\nИГРА ОКОНЧЕНА. Ваш корабль уничтожен." << std::endl;
        }
        else if (cluesFound >= totalClues) {
            // Финальное событие с выбором двух вариантов концовки
            std::cout << "\nПОЗДРАВЛЯЕМ! Вы собрали все зацепки." << std::endl;
            std::cout << "Перед вами стоит последний выбор:" << std::endl;
            std::cout << "1. Убить преступника" << std::endl;
            std::cout << "2. Сдать преступника полиции" << std::endl;
            int finalChoice;
            std::cin >> finalChoice;
            if (finalChoice == 1) {
                std::cout << "\nВы решаете убить преступника. В ожесточенном бою вы уничтожаете его, но поступаете жестоко." << std::endl;
                std::cout << "Концовка: Вы остались в одиночестве, теряя часть человечности." << std::endl;
            }
            else if (finalChoice == 2) {
                std::cout << "\nВы решаете сдать преступника полиции. Ваш поступок приносит вам уважение, и вы вновь обретаете доверие общества." << std::endl;
                std::cout << "Концовка: Правосудие восторжествует, а ваша репутация улучшится." << std::endl;
            }
            else {
                std::cout << "\nНеверный выбор. Судьба преступника остается неизвестной." << std::endl;
            }
        }
    }
};

int main() {
    std::srand(std::time(0));
    std::setlocale(LC_ALL, "Russian");
    Game game;
    game.start();
    return 0;
}