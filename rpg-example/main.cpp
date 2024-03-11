#include <iostream>
#include "Enemy/Enemy.h"
#include "Player/Player.h"
#include "Combat/Combat.h"

int main() {

    Player *player = new Player("Victor", 20, 10, 5, 1);
    Enemy *enemy = new Enemy("Goblin", 20, 5, 3, 7);
    Enemy *enemy2 = new Enemy("Orc", 30, 8, 5, 2);

    vector<Character*> participants;
    participants.push_back(player);
    participants.push_back(enemy);
    participants.push_back(enemy2);

    Combat* combat = new Combat(participants);
    cout << " --- A wild combat starts ---" << endl;
    combat->doCombat();

    delete player;
    delete enemy;
    delete enemy2;
    delete combat;
    return 0;
}
