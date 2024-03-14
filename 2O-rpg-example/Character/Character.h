//
// Created by Victor Navarro on 13/02/24.
//

#ifndef RPG_CHARACTER_H
#define RPG_CHARACTER_H
#include<string>
#include "../Combat/Action.h"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

using namespace std;

class Character {
protected:
    string name;
    int health;
    int attack;
    int defense;
    int speed;
    bool isPlayer;
    bool fleed;

public:
    Character(string, int, int, int, int, bool);

    virtual void doAttack(Character *target) = 0;
    virtual void takeDamage(int damage) = 0;

    void setName(string);
    string getName();
    void setHealth(int);
    int getHealth();
    void setAttack(int);
    int getAttack();
    void setDefense(int);
    int getDefense();
    void setSpeed(int);
    int getSpeed();
    string toString();
    bool getIsPlayer();
    bool hasFleed();
};


#endif //RPG_CHARACTER_H
