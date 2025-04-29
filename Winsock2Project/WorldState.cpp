#include "WorldState.h"
#include <algorithm>  // Needed for std::remove_if

void WorldState::addPlayer(const Player& player) {
    players.push_back(player);
}

void WorldState::removePlayer(int playerID) {
    players.erase(std::remove_if(players.begin(), players.end(),
        [playerID](const Player& p) { return p.getID() == playerID; }), players.end());
}


std::vector<Player>& WorldState::getPlayers() {
	return players;
}

std::vector<NPC>& WorldState::getNPCs() {
	return npcs;
}

void WorldState::addNPC(const NPC& npc) {
    npcs.push_back(npc);
}

void WorldState::removeNPC(int npcID) {
    npcs.erase(std::remove_if(npcs.begin(), npcs.end(),
        [npcID](const NPC& n) { return n.getID() == npcID; }), npcs.end());
}

void WorldState::updateWorld() {
    for (auto& player : players)
        player.update();
    for (auto& npc : npcs)
        npc.update();
}