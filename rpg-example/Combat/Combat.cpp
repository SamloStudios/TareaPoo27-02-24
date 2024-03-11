//
// Created by Victor Navarro on 19/02/24.
//
#include "Combat.h"
#include <iostream>
#include <algorithm>

using namespace std;

bool compareSpeed(Character *a, Character *b) {
    return a->getSpeed() > b->getSpeed();
}

Combat::Combat(vector<Character *> _participants) {
    participants = std::move(_participants);
    for(auto participant: participants) {
        if(participant->getIsPlayer()) {
            teamMembers.push_back((Player*)participant);
        }
        else {
            enemies.push_back((Enemy*)participant);
        }
    }
}

Combat::Combat(vector<Player*> _teamMembers, vector<Enemy*> _enemies) {
    teamMembers = std::move(_teamMembers);
    enemies = std::move(_enemies);
}

Combat::Combat() {
    participants = vector<Character*>();
}

void Combat::addParticipant(Character *participant) {
    participants.push_back(participant);
    if(participant->getIsPlayer()) {
        teamMembers.push_back((Player*)participant);
    }
    else {
        enemies.push_back((Enemy*)participant);
    }
}

void Combat::prepareCombat() {
    sort(participants.begin(), participants.end(), compareSpeed);
}

void Combat::doCombat() {
    prepareCombat();
    bool isFirstRound = true;

    while(enemies.size() != 0 && teamMembers.size() != 0) {
        vector<Character*>::iterator participant = participants.begin();

        //First round sampler
        if (isFirstRound == true) {
            while (participant != participants.end())
            {
                if ((*participant)->getIsPlayer()) break;
                participant++;
            }
            isFirstRound = false;
        }

        while(participant != participants.end()) {
            if((*participant)->getIsPlayer()){
                ActionResult playerAction = ((Player*)*participant)->takeAction(enemies);
                if(playerAction.target && playerAction.target->getHealth() <= 0) {
                    participant = participants.erase(remove(participants.begin(), participants.end(), playerAction.target), participants.end());
                    enemies.erase(remove(enemies.begin(), enemies.end(), playerAction.target), enemies.end());
                } else if (playerAction.fleed) {
                    return;
                } else {
                    participant++;
                }
            }
            else {
                //TAKE ACTION ()
                ActionResult enemyAction = ((Enemy*)*participant)->takeAction(teamMembers);
                if(enemyAction.target && enemyAction.target->getHealth() <= 0) {
                    participant = participants.erase(remove(participants.begin(), participants.end(), enemyAction.target), participants.end());
                    teamMembers.erase(remove(teamMembers.begin(), teamMembers.end(), enemyAction.target), teamMembers.end());
                } else if (enemyAction.fleed == true) {
                    //El orden de los factores altera el producto
                    enemies.erase(remove(enemies.begin(), enemies.end(), *participant), enemies.end());
                    participant = participants.erase(participant);
                    if (enemies.size() == 0) return;
                    cout << enemies.size() << " <- size of the enemies" << endl;
                } else {
                    participant++;
                }
            }

        }
    }

    //No se imprime el nombre del ganador
    if(enemies.size() == 0) {
        cout<<"You have won the combat"<<endl;
    }
    else {
        cout<<"The enemies have won the combat - Game Over"<<endl;
    }
}

string Combat::participantsToString() {
    string result = "";
    for (int i = 0; i < participants.size(); i++) {
        result += participants[i]->toString() + "\n";
    }
    return result;
}