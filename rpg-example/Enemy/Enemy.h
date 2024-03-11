//
// Created by Victor Navarro on 13/02/24.
//

#ifndef RPG_ENEMY_H
#define RPG_ENEMY_H
#include "../Character/Character.h"
#include "../Player/Player.h"
#include "../Player/ActionResult.h"

class Player;

class Enemy: public Character {
public:
    Enemy(string, int, int, int, int);
    int getMaxHealth();
    bool flee() override;
    void doAttack(Character *target) override;
    void takeDamage(int damage) override;
    Character* getTarget(vector<Player*> teamMembers);
    
    //HECHO Crear el metodo takeAction en la clase Enemy
    //HECHO Mover flee() a Character

    ActionResult takeAction(vector<Player*> teamMembers);

};


#endif //RPG_ENEMY_H
