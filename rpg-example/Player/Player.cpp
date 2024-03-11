//
// Created by Victor Navarro on 13/02/24.
//
#include "Player.h"
#include <iostream>
#include <algorithm>
#include "../Utils.h"

using namespace std;
using namespace combat_utils;

bool compareSpeed(Enemy *a, Enemy *b) {
    return a->getSpeed() > b->getSpeed();
}

Player::Player(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, true) {
    experience = 0;
    level = 1;
}

void Player::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    cout << "> You attacked " << target->getName() << "!" << endl;
    target->takeDamage(trueDamage);
    if (target->getHealth() <= 0)
    {
        emote(target);
    }
}

void Player::takeDamage(int damage) {
    setHealth(health - damage);

    if(health <= 0) {
        cout<<"You have died"<<endl;
    }
    else {
        cout<<"You have taken " << damage << " damage" << endl;
    }
}

bool Player::flee(vector<Enemy*> enemies) {
    std::sort(enemies.begin(), enemies.end(), compareSpeed);
    Enemy* fastestEnemy = enemies[0];
    bool fleed = false;
    if(this->getSpeed() > fastestEnemy->getSpeed()) {
        fleed =  true;
    }
    else {
        srand(time(NULL));
        int chance = rand() % 100;
        cout<< "chance: " << chance << endl;
        fleed = chance > 80;
    }

    return fleed;
}

void Player::emote(Character *target) {
    cout<<"Jokes on you " << target->getName() << endl;
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
    bool doContinue = false;
    do {
        cout << "- Choose a target" << endl;
        for (int i = 0; i < enemies.size(); i++) {
            cout << i + 1 << ". " << enemies[i]->getName() << endl;
        }
        cin >> targetIndex;
        cout << endl;
        
        //input validation
        if (targetIndex-1 >= enemies.size() || targetIndex < 0) {
            cout << "Invalid target" << endl;
        } else {
            doContinue = true;
        }
    } while (doContinue == false);

    return enemies[targetIndex - 1];
}

ActionResult Player::takeAction(vector<Enemy*> enemies) {
    int option = 0;
    Character* target = nullptr;
    bool fleed = false;
    bool doContinue = false;
    do {
        cout << endl;
        cout << "- Choose an action" << endl;
        cout << "1. Attack" << endl;
        cout << "2. Flee" << endl;
        cin >> option;
        cout << endl;

        switch (option) {
        case 1:
            target = getTarget(enemies);
            doAttack(target);
            doContinue = true;
            break;
        case 2:
            fleed = flee(enemies);
            if (fleed) {
                cout << "> You have fled" << endl;
            }
            else {
                cout << "> You couldn't flee" << endl;
            }
            doContinue = true;
            break;
        default:
            cout << "Invalid option" << endl << endl;
            break;
        }
    } while (doContinue == false);

    return ActionResult(target, fleed);
}