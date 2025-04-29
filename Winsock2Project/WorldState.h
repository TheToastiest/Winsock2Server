#pragma once
#include <vector>
#include "Player.h"
#include "NPC.h"

class WorldState {
public:
    // Adds a player to the world.
    void addPlayer(const Player& player);

    // Removes a player from the world by player ID.
    void removePlayer(int playerID);

    // Adds an NPC to the world.
    void addNPC(const NPC& npc);

    // Removes an NPC from the world by NPC ID.
    void removeNPC(int npcID);

    // Updates the world (e.g., updates all players and NPCs).
    void updateWorld();

    // Returns a reference to the container of players.
    std::vector<Player>& getPlayers();

    // Returns a reference to the container of NPCs.
    std::vector<NPC>& getNPCs();

private:
    std::vector<Player> players;
    std::vector<NPC> npcs;
};