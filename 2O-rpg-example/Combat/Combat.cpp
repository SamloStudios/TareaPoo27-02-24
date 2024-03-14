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

    //Este while es 1 iteracion por ronda
    while(enemies.size() != 0 && teamMembers.size() != 0) {
        registerActions();
        executeActions();
    }

    //No se imprime el nombre del ganador
    if(enemies.size() == 0) {
        cout<<"You have won the combat"<<endl;
    }
    else {
        cout<<"The enemies have won the combat - Game Over"<<endl;
    }
}

void Combat::registerActions() {
    vector<Character*>::iterator participant = participants.begin();
    //Una iteracion por turno de cada participante (player y enemigo)
    while(participant != participants.end()) {
        Character* target = nullptr;
        Action currentAction;
        if((*participant)->getIsPlayer()) {
            currentAction = ((Player*)*participant)->takeAction(enemies);
        }
        else {
            currentAction = ((Enemy*)*participant)->takeAction(teamMembers);
        }
        actions.push(currentAction);
        participant++;
    }
}

//Aqui se ejecutan las acciones
void Combat::executeActions() {
    while(!actions.empty()) {
        Action currentAction = actions.top(); //currentAction set
        //If there is a target
        if (currentAction.target != nullptr) {
            // If both target and actor are alive 
            if (currentAction.target->getHealth() > 0 && currentAction.subscriber->getHealth() > 0) {
                currentAction.action();
                actions.pop(); //remove action from queue
                //cout << "Action executed" << endl; DEBUG

                //delete defeated enemy
                checkParticipantStatus(currentAction.subscriber);
                checkParticipantStatus(currentAction.target);
            }
            else {
            // If target or the actor are ded, though both are still on currentAction
                //cout << " was unable to attack" << endl; --deprecated
                actions.pop(); 
            }
        }
        else { //If there is not a target
            if (currentAction.subscriber->getHealth() > 0) {
                currentAction.action();
            }
            //cout << "Action executed" << endl; DEBUG
            checkForFlee(currentAction.subscriber);
            if (currentAction.subscriber->hasFleed()) {
                //si huyo
                actions.pop();
                //si el jugador huyo saltarse todos los turnos
                if (currentAction.subscriber->getIsPlayer()) {
                    while (!actions.empty()) {
                        actions.pop();
                    }
                }
            }
            else actions.pop(); //si no huyo
        }
    } //end and return to loop
    cout << endl;
}

void Combat::checkParticipantStatus(Character* participant) {
    if (participant == nullptr) return;
    if(participant->getHealth() <= 0) {
        if(participant->getIsPlayer()) {
            //cout << "Removed " << participant->getName() << endl;
            teamMembers.erase(remove(teamMembers.begin(), teamMembers.end(), participant), teamMembers.end());
        }
        else {
            enemies.erase(remove(enemies.begin(), enemies.end(), participant), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }
}

void Combat::checkForFlee(Character *character) {
    bool fleed = character->hasFleed();
    if(character->getHealth() > 0 && fleed) {
        if(character->getIsPlayer()) {
            cout<<"You have fled the combat"<<endl;
            teamMembers.erase(remove(teamMembers.begin(), teamMembers.end(), character), teamMembers.end());
        }
        else {
            cout<<character->getName()<<" has fled the combat"<<endl;
            enemies.erase(remove(enemies.begin(), enemies.end(), character), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), character), participants.end());
    }
}

string Combat::participantsToString() {
    string result = "";
    for (int i = 0; i < participants.size(); i++) {
        result += participants[i]->toString() + "\n";
    }
    return result;
}