#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Класс для управления локализацией
class Localization {
private:
    json texts;

public:
    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open localization file: " << filename << std::endl;
            return false;
        }

        try {
            file >> texts;
            return true;
        }
        catch (const json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            return false;
        }
    }

    std::string get(const std::string& key, const std::string& defaultValue = "") const {
        try {
            return texts.at(key).get<std::string>();
        }
        catch (...) {
            std::cerr << "Missing localization key: " << key << std::endl;
            return defaultValue;
        }
    }

    std::string getWithReplace(const std::string& key, const std::vector<std::string>& replacements, const std::string& defaultValue = "") const {
        try {
            std::string result = texts.at(key).get<std::string>();
            for (size_t i = 0; i < replacements.size(); ++i) {
                std::string placeholder = "{" + std::to_string(i) + "}";
                size_t pos = result.find(placeholder);
                while (pos != std::string::npos) {
                    result.replace(pos, placeholder.length(), replacements[i]);
                    pos = result.find(placeholder, pos + replacements[i].length());
                }
            }
            return result;
        }
        catch (...) {
            std::cerr << "Missing localization key: " << key << std::endl;
            return defaultValue;
        }
    }
};

Localization loc;

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
        weapons.push_back(Item(loc.get("ship.default_weapon.name"),
            loc.get("ship.default_weapon.description"), 10, 15));
    }

    void printStatus() {
        std::cout << "\n" << loc.get("ship.status_header") << std::endl;
        std::cout << loc.getWithReplace("ship.hull_status", { std::to_string(hull), std::to_string(maxHull) }) << std::endl;
        std::cout << loc.getWithReplace("ship.shields_status", { std::to_string(shields), std::to_string(maxShields) }) << std::endl;
        std::cout << loc.getWithReplace("ship.engines_status", { std::to_string(engines), std::to_string(maxEngines) }) << std::endl;

        std::cout << loc.get("ship.weapons_header") << std::endl;
        for (auto& weapon : weapons) {
            std::cout << loc.getWithReplace("ship.weapon_info",
                { weapon.name, std::to_string(weapon.power) }) << std::endl;
        }

        std::cout << loc.get("ship.upgrades_header") << std::endl;
        for (auto& upgrade : upgrades) {
            std::cout << "- " << upgrade.name << std::endl;
        }
    }

    void repair(int amount) {
        hull = std::min(hull + amount, maxHull);
        std::cout << loc.getWithReplace("ship.repair_message", { std::to_string(amount) }) << std::endl;
    }

    void rechargeShields(int amount) {
        shields = std::min(shields + amount, maxShields);
        std::cout << loc.getWithReplace("ship.recharge_shields_message", { std::to_string(amount) }) << std::endl;
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
        weapons.push_back(Item(loc.get("character.default_weapon.name"),
            loc.get("character.default_weapon.description"), 5, 10));
        skills.push_back(Skill(loc.get("skills.shooting.name"),
            loc.get("skills.shooting.description"), 5));
        skills.push_back(Skill(loc.get("skills.repair.name"),
            loc.get("skills.repair.description"), 5));
        skills.push_back(Skill(loc.get("skills.negotiation.name"),
            loc.get("skills.negotiation.description"), 5));
    }

    void printStatus() {
        std::cout << "\n" << loc.get("character.status_header") << std::endl;
        std::cout << loc.getWithReplace("character.health_status", { std::to_string(health), std::to_string(maxHealth) }) << std::endl;
        std::cout << loc.getWithReplace("character.armor_status", { std::to_string(armor), std::to_string(maxArmor) }) << std::endl;
        std::cout << loc.getWithReplace("character.credits_status", { std::to_string(credits) }) << std::endl;
        std::cout << loc.getWithReplace("character.crit_bonus_status", { std::to_string(critBonus) }) << std::endl;

        std::cout << loc.get("character.weapons_header") << std::endl;
        for (auto& weapon : weapons) {
            std::cout << loc.getWithReplace("character.weapon_info",
                { weapon.name, std::to_string(weapon.power) }) << std::endl;
        }

        std::cout << loc.get("character.skills_header") << std::endl;
        for (auto& skill : skills) {
            std::cout << loc.getWithReplace("character.skill_info",
                { skill.name, std::to_string(skill.level), std::to_string(skill.maxLevel) }) << std::endl;
        }
    }

    void heal(int amount) {
        int oldHealth = health;
        health = std::min(health + amount, maxHealth);
        std::cout << loc.getWithReplace("character.heal_message",
            { std::to_string(oldHealth), std::to_string(health) }) << std::endl;
    }

    void repairArmor(int amount) {
        int oldArmor = armor;
        armor = std::min(armor + amount, maxArmor);
        std::cout << loc.getWithReplace("character.repair_armor_message",
            { std::to_string(oldArmor), std::to_string(armor) }) << std::endl;
    }
};

// Класс для представления врага
class Enemy {
public:
    std::string name;
    std::string description;
    int health;
    int maxHealth;
    int minPower;
    int maxPower;
    int reward;

    Enemy(std::string n, std::string d, int h, int minP, int maxP, int r)
        : name(n), description(d), health(h), maxHealth(h), minPower(minP), maxPower(maxP), reward(r) {}

    void printStatus() {
        std::cout << loc.getWithReplace("enemy.status",
            { name, std::to_string(health), std::to_string(maxHealth) }) << std::endl;
    }

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
    bool hasClue;
    bool clueCollected;
    bool hasRestSpot;

    Planet(std::string n, std::string d, bool clue) : name(n), description(d), hasClue(clue), clueCollected(false) {
        hasRestSpot = (std::rand() % 100 < 30);
    }

    bool explore(Character& player) {
        std::cout << "\n" << loc.getWithReplace("planet.arrival", { name, description }) << std::endl;
        bool clueFoundHere = false;
        if (hasClue && !clueCollected) {
            std::cout << "\n" << loc.get("planet.clue_found") << std::endl;
            clueCollected = true;
            clueFoundHere = true;
        }

        if (!items.empty()) {
            std::cout << "\n" << loc.get("planet.items_found") << std::endl;
            for (auto& item : items) {
                std::cout << loc.getWithReplace("planet.item_info", { item.name, item.description }) << std::endl;
                player.inventory.push_back(item);
            }
            items.clear();
        }

        if (!enemies.empty()) {
            std::cout << "\n" << loc.get("planet.under_attack") << std::endl;
            combat(player, enemies);
        }

        if (hasRestSpot) {
            int restChoice;
            std::cout << "\n" << loc.get("planet.rest_spot") << " ";
            std::cin >> restChoice;
            if (restChoice == 1) {
                int healAmount = randomInRange(10, 20);
                int armorAmount = randomInRange(5, 15);
                std::cout << loc.get("planet.resting") << std::endl;
                player.heal(healAmount);
                player.repairArmor(armorAmount);
            }
        }
        return clueFoundHere;
    }

    void combat(Character& player, std::vector<Enemy>& enemies) {
        bool playerDodged = false;
        while (!enemies.empty() && player.health > 0) {
            std::cout << "\n" << loc.get("combat.ground_battle") << std::endl;
            std::cout << loc.get("combat.player_status") << std::endl;
            player.printStatus();

            std::cout << "\n" << loc.get("combat.enemies") << std::endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                std::cout << i + 1 << ". ";
                enemies[i].printStatus();
            }

            std::cout << "\n" << loc.get("combat.choose_action") << std::endl;
            std::cout << "1. " << loc.get("combat.attack") << std::endl;
            std::cout << "2. " << loc.get("combat.use_item") << std::endl;
            std::cout << "3. " << loc.get("combat.try_escape") << std::endl;
            std::cout << "4. " << loc.get("combat.dodge") << std::endl;

            int choice;
            std::cin >> choice;
            playerDodged = false;

            if (choice == 1) {
                std::cout << loc.get("combat.choose_weapon") << std::endl;
                for (size_t i = 0; i < player.weapons.size(); i++) {
                    std::cout << i + 1 << ". " << player.weapons[i].name
                        << " (" << loc.getWithReplace("combat.base_damage", { std::to_string(player.weapons[i].power) }) << ")" << std::endl;
                }
                int weaponChoice;
                std::cin >> weaponChoice;
                weaponChoice--;
                if (weaponChoice >= 0 && weaponChoice < player.weapons.size()) {
                    std::cout << loc.get("combat.choose_target") << std::endl;
                    for (size_t i = 0; i < enemies.size(); i++) {
                        std::cout << i + 1 << ". " << enemies[i].name << std::endl;
                    }
                    int target;
                    std::cin >> target;
                    target--;
                    if (target >= 0 && target < enemies.size()) {
                        int baseDamage = player.weapons[weaponChoice].power;
                        int damage = randomInRange(std::max(1, baseDamage - 2), baseDamage + 2);
                        int critChance = 5 + static_cast<int>(player.critBonus);
                        if (std::rand() % 100 < critChance) {
                            damage *= 2;
                            std::cout << loc.get("combat.critical") << " ";
                        }
                        if (std::rand() % 100 < 80) {
                            enemies[target].health -= damage;
                            std::cout << loc.getWithReplace("combat.damage_dealt",
                                { std::to_string(damage), enemies[target].name }) << std::endl;
                            if (enemies[target].health <= 0) {
                                std::cout << loc.getWithReplace("combat.enemy_defeated", { enemies[target].name }) << std::endl;
                                player.credits += enemies[target].reward;
                                enemies.erase(enemies.begin() + target);
                            }
                        }
                        else {
                            std::cout << loc.get("combat.missed") << std::endl;
                        }
                    }
                }
            }
            else if (choice == 2) {
                if (!player.inventory.empty()) {
                    std::cout << loc.get("combat.choose_item") << std::endl;
                    for (size_t i = 0; i < player.inventory.size(); i++) {
                        std::cout << i + 1 << ". " << player.inventory[i].name << std::endl;
                    }
                    int itemChoice;
                    std::cin >> itemChoice;
                    itemChoice--;
                    if (itemChoice >= 0 && itemChoice < player.inventory.size()) {
                        Item usedItem = player.inventory[itemChoice];
                        if (usedItem.name.find(loc.get("items.medkit")) != std::string::npos) {
                            player.heal(usedItem.power);
                        }
                        else if (usedItem.name.find(loc.get("items.armor")) != std::string::npos) {
                            player.repairArmor(usedItem.power);
                        }
                        player.inventory.erase(player.inventory.begin() + itemChoice);
                    }
                }
                else {
                    std::cout << loc.get("combat.no_items") << std::endl;
                }
            }
            else if (choice == 3) {
                if (std::rand() % 100 < 50) {
                    std::cout << loc.get("combat.escape_success") << std::endl;
                    return;
                }
                else {
                    std::cout << loc.get("combat.escape_failed") << std::endl;
                }
            }
            else if (choice == 4) {
                std::cout << loc.get("combat.dodge_attempt") << std::endl;
                if (std::rand() % 100 < 50) {
                    std::cout << loc.get("combat.dodge_success") << std::endl;
                    playerDodged = true;
                }
                else {
                    std::cout << loc.get("combat.dodge_failed") << std::endl;
                }
            }

            if (!playerDodged) {
                for (auto& enemy : enemies) {
                    int enemyDamage = enemy.getRandomDamage();
                    if (player.armor > 0) {
                        int armorDamage = enemyDamage / 2;
                        player.armor -= armorDamage;
                        if (player.armor < 0) player.armor = 0;
                        int healthDamage = enemyDamage - armorDamage;
                        player.health -= healthDamage;
                        std::cout << loc.getWithReplace("combat.enemy_damage_armor",
                            { enemy.name, std::to_string(enemyDamage), std::to_string(armorDamage) }) << std::endl;
                    }
                    else {
                        player.health -= enemyDamage;
                        std::cout << loc.getWithReplace("combat.enemy_damage",
                            { enemy.name, std::to_string(enemyDamage) }) << std::endl;
                    }
                }
            }
            else {
                std::cout << loc.get("combat.dodged_all") << std::endl;
            }

            if (player.health <= 0) {
                std::cout << loc.get("combat.player_dead") << std::endl;
                return;
            }
        }
        if (player.health > 0)
            std::cout << loc.get("combat.all_enemies_defeated") << std::endl;
    }
};

// Класс для представления сектора
class Sector {
public:
    std::string name;
    std::vector<Planet> planets;
    std::vector<Enemy> spaceEnemies;

    Sector(std::string n) : name(n) {}

    void spaceCombat(Ship& ship, Character& player, std::vector<Enemy>& enemies) {
        bool shipDodged = false;
        while (!enemies.empty() && ship.hull > 0) {
            std::cout << "\n" << loc.get("combat.space_battle") << std::endl;
            std::cout << loc.get("combat.ship_status") << std::endl;
            ship.printStatus();

            std::cout << "\n" << loc.get("combat.enemies") << std::endl;
            for (size_t i = 0; i < enemies.size(); i++) {
                std::cout << i + 1 << ". ";
                enemies[i].printStatus();
            }

            std::cout << "\n" << loc.get("combat.choose_action") << std::endl;
            std::cout << "1. " << loc.get("combat.attack") << std::endl;
            std::cout << "2. " << loc.get("combat.use_upgrade") << std::endl;
            std::cout << "3. " << loc.get("combat.try_escape") << std::endl;
            std::cout << "4. " << loc.get("combat.evasive_maneuver") << std::endl;

            int choice;
            std::cin >> choice;
            shipDodged = false;

            if (choice == 1) {
                std::cout << loc.get("combat.choose_weapon") << std::endl;
                for (size_t i = 0; i < ship.weapons.size(); i++) {
                    std::cout << i + 1 << ". " << ship.weapons[i].name << " ("
                        << loc.getWithReplace("combat.base_damage", { std::to_string(ship.weapons[i].power) }) << ")" << std::endl;
                }
                int weaponChoice;
                std::cin >> weaponChoice;
                weaponChoice--;
                if (weaponChoice >= 0 && weaponChoice < ship.weapons.size()) {
                    std::cout << loc.get("combat.choose_target") << std::endl;
                    for (size_t i = 0; i < enemies.size(); i++) {
                        std::cout << i + 1 << ". " << enemies[i].name << std::endl;
                    }
                    int target;
                    std::cin >> target;
                    target--;
                    if (target >= 0 && target < enemies.size()) {
                        int baseDamage = ship.weapons[weaponChoice].power;
                        int damage = randomInRange(std::max(1, baseDamage - 2), baseDamage + 2);
                        int critChance = 5;
                        if (std::rand() % 100 < critChance) {
                            damage *= 2;
                            std::cout << loc.get("combat.critical") << " ";
                        }
                        if (std::rand() % 100 < 70) {
                            enemies[target].health -= damage;
                            std::cout << loc.getWithReplace("combat.damage_dealt",
                                { std::to_string(damage), enemies[target].name }) << std::endl;
                            if (enemies[target].health <= 0) {
                                std::cout << loc.getWithReplace("combat.enemy_destroyed", { enemies[target].name }) << std::endl;
                                player.credits += enemies[target].reward;
                                enemies.erase(enemies.begin() + target);
                            }
                        }
                        else {
                            std::cout << loc.get("combat.missed") << std::endl;
                        }
                    }
                }
            }
            else if (choice == 2) {
                if (!ship.upgrades.empty()) {
                    std::cout << loc.get("combat.choose_upgrade") << std::endl;
                    for (size_t i = 0; i < ship.upgrades.size(); i++) {
                        std::cout << i + 1 << ". " << ship.upgrades[i].name << std::endl;
                    }
                    int upgradeChoice;
                    std::cin >> upgradeChoice;
                    upgradeChoice--;
                    if (upgradeChoice >= 0 && upgradeChoice < ship.upgrades.size()) {
                        Item usedUpgrade = ship.upgrades[upgradeChoice];
                        if (usedUpgrade.name.find(loc.get("items.shield")) != std::string::npos) {
                            ship.rechargeShields(usedUpgrade.power);
                        }
                        else if (usedUpgrade.name.find(loc.get("items.repair")) != std::string::npos) {
                            ship.repair(usedUpgrade.power);
                        }
                        ship.upgrades.erase(ship.upgrades.begin() + upgradeChoice);
                    }
                }
                else {
                    std::cout << loc.get("combat.no_upgrades") << std::endl;
                }
            }
            else if (choice == 3) {
                if (std::rand() % 100 < ship.engines) {
                    std::cout << loc.get("combat.escape_success") << std::endl;
                    return;
                }
                else {
                    std::cout << loc.get("combat.escape_failed") << std::endl;
                }
            }
            else if (choice == 4) {
                std::cout << loc.get("combat.evasive_attempt") << std::endl;
                if (std::rand() % 100 < 50) {
                    std::cout << loc.get("combat.evasive_success") << std::endl;
                    shipDodged = true;
                }
                else {
                    std::cout << loc.get("combat.evasive_failed") << std::endl;
                }
            }

            if (!shipDodged) {
                for (auto& enemy : enemies) {
                    int enemyDamage = randomInRange(enemy.minPower, enemy.maxPower);
                    if (ship.shields > 0) {
                        int shieldDamage = enemyDamage / 2;
                        ship.shields -= shieldDamage;
                        if (ship.shields < 0) ship.shields = 0;
                        int hullDamage = enemyDamage - shieldDamage;
                        ship.hull -= hullDamage;
                        std::cout << loc.getWithReplace("combat.ship_damage_shields",
                            { enemy.name, std::to_string(enemyDamage), std::to_string(shieldDamage) }) << std::endl;
                    }
                    else {
                        ship.hull -= enemyDamage;
                        std::cout << loc.getWithReplace("combat.ship_damage",
                            { enemy.name, std::to_string(enemyDamage) }) << std::endl;
                    }
                }
            }
            else {
                std::cout << loc.get("combat.evasive_all") << std::endl;
            }

            if (ship.hull <= 0) {
                std::cout << loc.get("combat.ship_destroyed") << std::endl;
                player.health = 0;
                return;
            }
        }
        if (ship.hull > 0)
            std::cout << loc.get("combat.all_enemies_destroyed") << std::endl;
    }

    void explore(Ship& ship, Character& player, int& globalClues) {
        std::cout << "\n" << loc.getWithReplace("sector.arrival", { name }) << std::endl;

        if (!spaceEnemies.empty()) {
            std::cout << "\n" << loc.get("sector.space_attack") << std::endl;
            spaceCombat(ship, player, spaceEnemies);
        }

        if (ship.hull <= 0 || player.health <= 0) {
            return;
        }

        if (!planets.empty()) {
            std::cout << "\n" << loc.get("sector.available_planets") << std::endl;
            for (size_t i = 0; i < planets.size(); i++) {
                std::cout << i + 1 << ". " << planets[i].name << std::endl;
            }

            std::cout << loc.get("sector.choose_planet") << " ";
            int choice;
            std::cin >> choice;
            if (choice > 0 && choice <= planets.size()) {
                bool foundClue = planets[choice - 1].explore(player);
                if (foundClue)
                    globalClues++;
            }
        }
        else {
            std::cout << loc.get("sector.no_planets") << std::endl;
        }

        if (std::rand() % 100 < 20) {
            std::cout << "\n" << loc.get("sector.random_event") << " ";
            int bonus = randomInRange(5, 10);
            std::cout << loc.getWithReplace("sector.bonus_credits", { std::to_string(bonus) }) << std::endl;
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
        // Инициализация предметов из локализации
        items.push_back(Item(loc.get("market.items.medkit.name"),
            loc.get("market.items.medkit.description"), 50, 25));
        items.push_back(Item(loc.get("market.items.armor.name"),
            loc.get("market.items.armor.description"), 40, 30));
        items.push_back(Item(loc.get("market.items.blaster.name"),
            loc.get("market.items.blaster.description"), 150, 20));
        items.push_back(Item(loc.get("market.items.shield.name"),
            loc.get("market.items.shield.description"), 60, 20));

        // Улучшения для корабля
        shipUpgrades.push_back(Item(loc.get("market.upgrades.shield.name"),
            loc.get("market.upgrades.shield.description"), 200, 30));
        shipUpgrades.push_back(Item(loc.get("market.upgrades.repair_drone.name"),
            loc.get("market.upgrades.repair_drone.description"), 180, 40));
        shipUpgrades.push_back(Item(loc.get("market.upgrades.turbo_laser.name"),
            loc.get("market.upgrades.turbo_laser.description"), 300, 50));

        // Улучшения для оружия
        weaponUpgrades.push_back(Item(loc.get("market.weapon_upgrades.crit_module.name"),
            loc.get("market.weapon_upgrades.crit_module.description"), 250, 5));
        weaponUpgrades.push_back(Item(loc.get("market.weapon_upgrades.damage_booster.name"),
            loc.get("market.weapon_upgrades.damage_booster.description"), 300, 3));

        // Дополнительные предметы
        items.push_back(Item(loc.get("market.items.repair_kit.name"),
            loc.get("market.items.repair_kit.description"), 100, 15));
        items.push_back(Item(loc.get("market.items.energy_essence.name"),
            loc.get("market.items.energy_essence.description"), 80, 20));
    }

    void visit(Character& player, Ship& ship) {
        while (true) {
            std::cout << "\n" << loc.get("market.header") << std::endl;
            std::cout << loc.getWithReplace("market.credits", { std::to_string(player.credits) }) << std::endl;
            std::cout << "\n1. " << loc.get("market.buy_items") << std::endl;
            std::cout << "2. " << loc.get("market.buy_ship_upgrades") << std::endl;
            std::cout << "3. " << loc.get("market.buy_weapon_upgrades") << std::endl;
            std::cout << "4. " << loc.get("market.exit") << std::endl;

            int choice;
            std::cin >> choice;
            if (choice == 1) {
                std::cout << "\n" << loc.get("market.available_items") << std::endl;
                for (size_t i = 0; i < items.size(); i++) {
                    std::cout << i + 1 << ". " << items[i].name << " (" << items[i].description << ") - "
                        << items[i].value << " " << loc.get("market.credits_short") << std::endl;
                }
                std::cout << loc.get("market.choose_item") << " ";
                int itemChoice;
                std::cin >> itemChoice;
                if (itemChoice > 0 && itemChoice <= items.size()) {
                    Item selected = items[itemChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        player.inventory.push_back(selected);
                        std::cout << loc.getWithReplace("market.item_purchased", { selected.name }) << std::endl;
                    }
                    else {
                        std::cout << loc.get("market.not_enough_credits") << std::endl;
                    }
                }
            }
            else if (choice == 2) {
                std::cout << "\n" << loc.get("market.available_ship_upgrades") << std::endl;
                for (size_t i = 0; i < shipUpgrades.size(); i++) {
                    std::cout << i + 1 << ". " << shipUpgrades[i].name << " (" << shipUpgrades[i].description << ") - "
                        << shipUpgrades[i].value << " " << loc.get("market.credits_short") << std::endl;
                }
                std::cout << loc.get("market.choose_upgrade") << " ";
                int upgradeChoice;
                std::cin >> upgradeChoice;
                if (upgradeChoice > 0 && upgradeChoice <= shipUpgrades.size()) {
                    Item selected = shipUpgrades[upgradeChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        ship.upgrades.push_back(selected);
                        std::cout << loc.getWithReplace("market.upgrade_purchased", { selected.name }) << std::endl;
                        if (selected.name.find(loc.get("items.shield")) != std::string::npos) {
                            ship.maxShields += selected.power;
                            ship.shields = ship.maxShields;
                        }
                        else if (selected.name.find(loc.get("items.engine")) != std::string::npos) {
                            ship.maxEngines += selected.power;
                            ship.engines = ship.maxEngines;
                        }
                    }
                    else {
                        std::cout << loc.get("market.not_enough_credits") << std::endl;
                    }
                }
            }
            else if (choice == 3) {
                std::cout << "\n" << loc.get("market.available_weapon_upgrades") << std::endl;
                for (size_t i = 0; i < weaponUpgrades.size(); i++) {
                    std::cout << i + 1 << ". " << weaponUpgrades[i].name << " (" << weaponUpgrades[i].description
                        << ") - " << weaponUpgrades[i].value << " " << loc.get("market.credits_short") << std::endl;
                }
                std::cout << loc.get("market.choose_weapon_upgrade") << " ";
                int wChoice;
                std::cin >> wChoice;
                if (wChoice > 0 && wChoice <= weaponUpgrades.size()) {
                    Item selected = weaponUpgrades[wChoice - 1];
                    if (player.credits >= selected.value) {
                        player.credits -= selected.value;
                        if (selected.name.find(loc.get("weapon_upgrades.crit")) != std::string::npos) {
                            player.critBonus += selected.power;
                            std::cout << loc.getWithReplace("weapon_upgrades.crit_bonus", { std::to_string(selected.power) }) << std::endl;
                        }
                        else if (selected.name.find(loc.get("weapon_upgrades.damage")) != std::string::npos) {
                            for (auto& weapon : player.weapons) {
                                weapon.power += selected.power;
                            }
                            std::cout << loc.getWithReplace("weapon_upgrades.damage_bonus", { std::to_string(selected.power) }) << std::endl;
                        }
                    }
                    else {
                        std::cout << loc.get("market.not_enough_credits") << std::endl;
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
    int cluesFound;
    int totalClues;

    Game() : player(loc.get("game.player_name")), ship(loc.get("game.ship_name")), currentSector(0), cluesFound(0), totalClues(5) {
        initializeGame();
    }

    void initializeGame() {
        sectors.clear();
        for (int i = 1; i <= 10; i++) {
            Sector sector(loc.getWithReplace("sector.name", { std::to_string(i) }));
            int planetCount = std::rand() % 4 + 1;
            for (int j = 1; j <= planetCount; j++) {
                bool hasClue = (std::rand() % 100 < 20);
                Planet planet(loc.getWithReplace("planet.name", { std::to_string(i), std::to_string(j) }),
                    loc.getWithReplace("planet.description", { std::to_string(i), std::to_string(j) }),
                    hasClue);

                if (std::rand() % 100 < 70) {
                    int itemType = std::rand() % 3;
                    if (itemType == 0) {
                        planet.items.push_back(Item(loc.get("items.medkit"),
                            loc.get("items.medkit_description"),
                            0, 15 + std::rand() % 20));
                    }
                    else if (itemType == 1) {
                        planet.items.push_back(Item(loc.get("items.armor"),
                            loc.get("items.armor_description"),
                            0, 10 + std::rand() % 15));
                    }
                    else {
                        planet.items.push_back(Item(loc.get("items.credits"),
                            loc.get("items.credits_description"),
                            50 + std::rand() % 100, 0));
                    }
                }

                if (std::rand() % 100 < 60) {
                    int enemyCount = std::rand() % 3 + 1;
                    for (int k = 0; k < enemyCount; k++) {
                        int enemyType = std::rand() % 3;
                        if (enemyType == 0) {
                            planet.enemies.push_back(Enemy(loc.get("enemies.pirate.name"),
                                loc.get("enemies.pirate.description"),
                                randomInRange(30, 60), 5, 10, randomInRange(20, 50)));
                        }
                        else if (enemyType == 1) {
                            planet.enemies.push_back(Enemy(loc.get("enemies.drone.name"),
                                loc.get("enemies.drone.description"),
                                randomInRange(20, 40), 8, 16, randomInRange(15, 35)));
                        }
                        else {
                            planet.enemies.push_back(Enemy(loc.get("enemies.predator.name"),
                                loc.get("enemies.predator.description"),
                                randomInRange(40, 80), 3, 10, randomInRange(10, 25)));
                        }
                    }
                }
                sector.planets.push_back(planet);
            }

            if (std::rand() % 100 < 50) {
                int enemyCount = std::rand() % 2 + 1;
                for (int k = 0; k < enemyCount; k++) {
                    int enemyType = std::rand() % 2;
                    if (enemyType == 0) {
                        sector.spaceEnemies.push_back(Enemy(loc.get("enemies.pirate_ship.name"),
                            loc.get("enemies.pirate_ship.description"),
                            randomInRange(50, 100), 10, 15, randomInRange(50, 100)));
                    }
                    else {
                        sector.spaceEnemies.push_back(Enemy(loc.get("enemies.fighter_drone.name"),
                            loc.get("enemies.fighter_drone.description"),
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

    void regenerateHealth() {
        if (player.health < player.maxHealth) {
            int regen = 2;
            player.heal(regen);
        }
    }

    void start() {
        std::cout << loc.get("game.title") << std::endl;
        std::cout << loc.get("game.intro1") << std::endl;
        std::cout << loc.get("game.intro2") << std::endl;
        std::cout << loc.get("game.intro3") << std::endl;
        std::cout << loc.get("game.intro4") << std::endl;

        while (player.health > 0 && ship.hull > 0 && cluesFound < totalClues) {
            std::cout << "\n" << loc.get("game.main_menu") << std::endl;
            std::cout << loc.getWithReplace("game.current_sector", { sectors[currentSector].name }) << std::endl;
            std::cout << loc.getWithReplace("game.clues_found", { std::to_string(cluesFound), std::to_string(totalClues) }) << std::endl;
            std::cout << loc.getWithReplace("game.credits", { std::to_string(player.credits) }) << std::endl;
            std::cout << "\n1. " << loc.get("menu.explore") << std::endl;
            std::cout << "2. " << loc.get("menu.next_sector") << std::endl;
            std::cout << "3. " << loc.get("menu.market") << std::endl;
            std::cout << "4. " << loc.get("menu.status") << std::endl;
            std::cout << "5. " << loc.get("menu.use_item") << std::endl;
            std::cout << loc.get("menu.choose_action") << " ";
            int choice;
            std::cin >> choice;
            switch (choice) {
            case 1:
                sectors[currentSector].explore(ship, player, cluesFound);
                break;
            case 2:
                if (currentSector < sectors.size() - 1) {
                    currentSector++;
                    std::cout << loc.getWithReplace("game.sector_moved", { sectors[currentSector].name }) << std::endl;
                    if (std::rand() % 100 < 20) {
                        std::cout << "\n" << loc.get("game.random_event") << " ";
                        int bonus = randomInRange(5, 10);
                        std::cout << loc.getWithReplace("game.bonus_credits", { std::to_string(bonus) }) << std::endl;
                        player.credits += bonus;
                    }
                }
                else {
                    std::cout << loc.get("game.end_of_space") << std::endl;
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
                    std::cout << loc.get("game.no_items") << std::endl;
                }
                else {
                    std::cout << "\n" << loc.get("game.inventory") << std::endl;
                    for (size_t i = 0; i < player.inventory.size(); i++) {
                        std::cout << i + 1 << ". " << player.inventory[i].name
                            << " (" << player.inventory[i].description << ")" << std::endl;
                    }
                    std::cout << loc.get("game.choose_item_use") << " ";
                    int itemChoice;
                    std::cin >> itemChoice;
                    if (itemChoice > 0 && itemChoice <= player.inventory.size()) {
                        Item usedItem = player.inventory[itemChoice - 1];
                        if (usedItem.name.find(loc.get("items.medkit")) != std::string::npos) {
                            player.heal(usedItem.power);
                        }
                        else if (usedItem.name.find(loc.get("items.armor")) != std::string::npos) {
                            player.repairArmor(usedItem.power);
                        }
                        else {
                            std::cout << loc.get("game.cant_use_item") << std::endl;
                        }
                        player.inventory.erase(player.inventory.begin() + itemChoice - 1);
                    }
                }
                break;
            default:
                std::cout << loc.get("game.invalid_choice") << std::endl;
                break;
            }
            regenerateHealth();
        }

        if (player.health <= 0) {
            std::cout << "\n" << loc.get("game.over.dead") << std::endl;
        }
        else if (ship.hull <= 0) {
            std::cout << "\n" << loc.get("game.over.ship_destroyed") << std::endl;
        }
        else if (cluesFound >= totalClues) {
            std::cout << "\n" << loc.get("game.ending.congrats") << std::endl;
            std::cout << loc.get("game.ending.choice1") << std::endl;
            std::cout << loc.get("game.ending.choice2") << std::endl;
            int finalChoice;
            std::cin >> finalChoice;
            if (finalChoice == 1) {
                std::cout << "\n" << loc.get("game.ending.kill") << std::endl;
                std::cout << loc.get("game.ending.kill_ending") << std::endl;
            }
            else if (finalChoice == 2) {
                std::cout << "\n" << loc.get("game.ending.arrest") << std::endl;
                std::cout << loc.get("game.ending.arrest_ending") << std::endl;
            }
            else {
                std::cout << "\n" << loc.get("game.ending.invalid_choice") << std::endl;
            }
        }
    }
};

int main() {
    std::srand(std::time(0));
    std::setlocale(LC_ALL, "Russian");

    // Загрузка локализации
    if (!loc.load("C:/Users/andre/OneDrive/Документы/GitHub/game/Project16/localization.json")) {
        std::cerr << "Failed to load localization file. Using default texts." << std::endl;
    }

    Game game;
    game.start();
    return 0;
}