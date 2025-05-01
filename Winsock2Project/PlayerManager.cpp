#include "PlayerManager.h"
#include "Player.h"  // Add this line
#include "Logger.h"

void PlayerManager::addPlayer(const Player& player) {
    players[player.getID()] = player;
    spdlog::info("Player {} added to the manager.", player.getID());
}

void PlayerManager::removePlayer(int playerID) {
    if (players.erase(playerID) > 0) {
        spdlog::info("Player {} removed.", playerID);
    }
    else {
        spdlog::warn("Failed to remove Player {} (not found).", playerID);
    }
}

Player* PlayerManager::getPlayer(int playerID) {
    auto it = players.find(playerID);
    return (it != players.end()) ? &it->second : nullptr;
}

std::unordered_map<int, Player>& PlayerManager::getPlayers() {
    return players;
}

PlayerManager playerManager;

void PlayerManager::updatePlayers() {
    for (auto& [id, player] : players) {
        player.update();
    }
    spdlog::trace("Updated {} players.", players.size());
}