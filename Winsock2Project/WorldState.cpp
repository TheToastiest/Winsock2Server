#include "WorldState.h"
#include "Logger.h"
#include "Player.h" // Add this line
#include "NPC.h"    // Add this line
#include <algorithm>  // Needed for std::remove_if

PlayerManager& WorldState::getPlayerManager() {
    return playerManager;  //  Directly return PlayerManager
}

void WorldState::addPlayer(const Player& player) {
    playerManager.addPlayer(player);  //  Directly call PlayerManager
    spdlog::info("Player {} added to the world via PlayerManager.", player.getID());
}

void WorldState::removePlayer(int playerID) {
    playerManager.removePlayer(playerID);  // Delegate to PlayerManager
    spdlog::info("Player {} removed via PlayerManager.", playerID);
}
void WorldState::updatePlayers() {
    playerManager.updatePlayers();  // Delegate to PlayerManager
    spdlog::trace("Updated players in the world.");
}

/*const std::unordered_map<int, Player>& PlayerManager::getPlayers() const {
    return players();
}*/

std::unordered_map<int, NPC>& WorldState::getNPCs() {
    return npcs;
}

void WorldState::addNPC(const NPC& npc) {
    npcs[npc.getID()] = npc;
}

void WorldState::removeNPC(int npcID) {
    npcs.erase(npcID);
}

void WorldState::updateWorld() {
    playerManager.updatePlayers();  // ✔ Efficient player update

    for (auto& [id, npc] : npcs) {  // ✔ Direct NPC iteration without copying
        npc.update();
    }
}

void WorldState::saveToBinary(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile) {
        spdlog::error("Failed to open file for binary saving: {}", filename);
        return;
    }

    size_t playerCount = playerManager.getPlayers().size();  // ✔ Pull from PlayerManager

    outFile.write(reinterpret_cast<const char*>(&playerCount), sizeof(playerCount));

    for (const auto& [id, player] : playerManager.getPlayers()) {
        outFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
        player.saveToBinary(outFile); // Assume Player has binary serialization
    }

    size_t npcCount = npcs.size();
    outFile.write(reinterpret_cast<const char*>(&npcCount), sizeof(npcCount));

    for (const auto& [id, npc] : npcs) {
        outFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
        npc.saveToBinary(outFile); // Assume NPC has binary serialization
    }

    spdlog::info("World state saved to binary: {}", filename);
}

void WorldState::loadFromBinary(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);

    if (!inFile) {
        spdlog::error("Failed to open file for binary loading: {}", filename);
        return;
    }

    size_t playerCount;
    inFile.read(reinterpret_cast<char*>(&playerCount), sizeof(playerCount));

    playerManager.getPlayers().clear();  //  OK: getPlayers() returns const ref, but clear() is called on the map itself
    for (size_t i = 0; i < playerCount; ++i) {
        int id;
        inFile.read(reinterpret_cast<char*>(&id), sizeof(id));

        Player player;
        player.loadFromBinary(inFile);
        playerManager.getPlayers()[id] = player;  //  OK: [] modifies the map, but it's allowed here
    }

    size_t npcCount;
    inFile.read(reinterpret_cast<char*>(&npcCount), sizeof(npcCount));

    npcs.clear();  //  OK: npcs is a non-const member variable
    for (size_t i = 0; i < npcCount; ++i) {
        int id;
        inFile.read(reinterpret_cast<char*>(&id), sizeof(id));

        NPC npc;
        npc.loadFromBinary(inFile);
        npcs[id] = npc;
    }

    spdlog::info("World state loaded from binary: {}", filename);
}