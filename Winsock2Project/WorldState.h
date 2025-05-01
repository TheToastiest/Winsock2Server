#pragma once
#include <vector>
#include <unordered_map>
#include "PlayerManager.h"
#include "Player.h"
#include "NPC.h"
#include "Logger.h"


class WorldState {
public:

	// add player to the world
	void addPlayer(const Player& player);

	// Removes a player from the world by player ID.
	void removePlayer(int playerID);

	// Updates all players in the world.    
	void updatePlayers();

	// Retreives a player by ID
	void getPlayer(int playerID);

	// Returns all players in the world. 
	const std::unordered_map<int, Player>& getPlayers() const;

    // Adds an NPC to the world.
    void addNPC(const NPC& npc);

    // Removes an NPC from the world by NPC ID.
    void removeNPC(int npcID);

    // Updates the world (e.g., updates all players and NPCs).
    void updateWorld();
    
    // Access Player Management
    PlayerManager& getPlayerManager();

    // Saves to a binary serialization
    void saveToBinary(const std::string& filename);

	// Loads from a binary serialization
	void loadFromBinary(const std::string& filename);

    // Get npcs
	std::unordered_map<int, NPC>& getNPCs();


private:
    PlayerManager playerManager;  // Handles all player-related operations
    std::unordered_map<int, NPC> npcs;
};
