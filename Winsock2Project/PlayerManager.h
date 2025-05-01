#pragma once
#include <unordered_map>
#include "Player.h"

class PlayerManager {
public:
    // Adds a player to the manager
    void addPlayer(const Player& player);

    // Removes a player by ID
    void removePlayer(int playerID);

    // Retrieves a player reference by ID
    Player* getPlayer(int playerID);

    // Returns all players
    std::unordered_map<int, Player>& getPlayers();

	// Updates all players simultaneously
    void updatePlayers();

private:
    std::unordered_map<int, Player> players;
};