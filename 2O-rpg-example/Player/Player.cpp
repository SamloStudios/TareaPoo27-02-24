//
// Created by Victor Navarro on 13/02/24.
//
#include "Player.h"
#include <iostream>
#include "../Utils.h"
#include <algorithm>

using namespace std;
using namespace combat_utils;


bool compareSpeed(Enemy *a, Enemy *b) {
    return a->getSpeed() > b->getSpeed();
}

Player::Player(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense,
                                                                                        speed, true) {
    experience = 0;
    level = 1;
}

void Player::doAttack(Character* target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    cout << "> You have atacked " << target->getName() << endl;
    target->takeDamage(trueDamage);
    if (target->getHealth() <= 0) emote(target);
}

void Player::takeDamage(int damage) {
    setHealth(health - damage);
    cout << "You have taken " << damage << " damage" << endl;
    if (health <= 0) {
        cout << "You have died" << endl;
    }
}

void Player::flee(vector<Enemy *> enemies) {
    std::sort(enemies.begin(), enemies.end(), compareSpeed);
    Enemy *fastestEnemy = enemies[0];
    bool fleed = false;
    cout << "> You have attempted to flee" << endl;
    if (this->getSpeed() > fastestEnemy->getSpeed()) {
        fleed = true;
    } else {
        srand(time(NULL));
        int chance = rand() % 100;
        cout << "your chance is: " << chance << endl;
        fleed = chance > 80;
    }
    if (!fleed)
    {
        cout << "> You have failed to flee!"<< endl;
        srand(time(NULL));
        int emote_index = rand() % 4 + 1;
        switch (emote_index)
        {
        case 1:
            cout << "Guess you didn't train enough..." << endl;
            break;
        case 2:
            cout << "Seems like you are too slow..." << endl;
            break;
        case 3:
            cout << "You could use some potions to increase your speed..." << endl;
            break;
        default:
            cout << "Looks like " << fastestEnemy->getName() << " is faster than you..." << endl;
            break;
        }
    }

    this->fleed = fleed;
}

void Player::emote(Character* target) {
    srand(time(NULL));
    int emote_index = rand() % 4 + 1;
    switch (emote_index)
    {
    case 1:
        cout << "Guess " << target->getName() << " was just a joke..." << endl;
        break;
    case 2:
        cout << "My sister could have beaten " << target->getName() << endl;
        break;
    case 3:
        cout << "FATALITY" << endl;
        break;
    default:
        cout << "Guess " << target->getName() << " was just a noob lol" << endl;
        break;
    }
}

void Player::levelUp() {
    level++;
    setHealth(getHealth() + 10);
    setAttack(getAttack() + 5);
    setDefense(getDefense() + 5);
    setSpeed(getSpeed() + 5);
}

void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {
        levelUp();
        experience = 0;
    }
}

Character* Player::getTarget(vector<Enemy*> enemies) {
    int targetIndex = 0;
    while (true)
    {
        cout << "Choose a target" << endl;
        for (int i = 0; i < enemies.size(); i++) {
            cout << i + 1 << ". " << enemies[i]->getName() << endl;
        }
        cin >> targetIndex;

        if ((targetIndex-1) >= enemies.size() || (targetIndex-1) < 0) {
            cout << "Invalid target\n" << endl;
        } else break;
        //Added input validation (SAMLO)
    }
    return enemies[targetIndex-1];
}

Action Player::takeAction(vector<Enemy *> enemies) {
    int option = 0;
    Character *target = nullptr;

    //Esta variable guarda accion, velocidad, actor, y objetivo
    Action myAction;

    myAction.speed = this->getSpeed();
    myAction.subscriber = this;
    string hp = GREEN;
    if (this->getHealth() < 8)
    {
        hp = RED;
    }

    bool ok = false;
    do {
        cout << CYAN <<" - ["<<this->getName()<<"]"<< YELLOW <<" ----|+|----(" << hp << "HP: " <<this->getHealth() << YELLOW << ")----|#|----";
        cout <<"(DEF: "<<this->getDefense()<<")----|%|----(SP: " << this->getSpeed() << ")----" << RESET << endl;
        cout << "Choose an action" << endl;
        cout << "1. Attack" << endl;
        cout << "2. Flee" << endl;
        cin >> option;

        switch (option) {
            case 1:
                target = getTarget(enemies);
                cout << endl; //ESPACIO ///////
                myAction.target = target;
                //1.
                myAction.action = [this, target]() {
                    doAttack(target);
                };
                ok = true;
                break;
            case 2:
                myAction.action = [this, enemies]() {
                    flee(enemies);
                };
                ok = true;
                break;
            default:
                cout << "Invalid option" << endl;
                break;
        }
    } while (!ok);
    

    return myAction;
}