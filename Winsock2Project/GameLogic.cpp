#include "GameLogic.h"
#include "NPC.h"
#include "Logger.h"
#include <cmath>
#include <random>
#include <chrono>
#include <iostream>

// Global world state instance.
WorldState worldState;

void initializeGame() {
    // For this networking demonstration, we are not creating any players.
    // You can add players later on the client side.
    // For demonstration, create a single NPC (or leave worldState empty if desired).
    NPC npc1("NPC1", 101, 2.0f);
    worldState.addNPC(npc1);

    spdlog::info("Game Initialized (no players added); ready for client connections.");
}

void updateGameWorld() {
    // Update the world state (animations, AI, events, etc.)
    worldState.updateWorld();

    // Use C++11 random facilities for generating waypoints.
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 100.0f);

    // Assume a fixed delta time for demonstration (e.g., 1/60th of a second).
    constexpr float deltaTime = 1.0f / 60.0f;

    // Process each NPC for waypoint following.
    for (auto& [id,npc] : worldState.getNPCs()) {
        if (!npc.hasWaypoint()) {
            float targetX = dis(gen);
            float targetY = dis(gen);
            npc.setWaypoint(targetX, targetY);
            spdlog::trace("NPC {} assigned new waypoint: ({:.2f}, {:.2f})", npc.getName(), targetX, targetY);
        }

        // Compute the movement towards the waypoint scaled by deltaTime.
        auto dir = npc.getDirectionToWaypoint();
        npc.move(dir.first * npc.getSpeed() * deltaTime, dir.second * npc.getSpeed() * deltaTime);
        npc.checkWaypointCompletion();
    }
    // Future updates (collisions, physics, etc.) would go here.
}

void processPlayerActions() {
    // In this demo, no players are created.
    // This function remains as a stub for future player input handling.
}

void applyMovementPrediction(Player& player) {
    // Basic linear extrapolation for a player using a fixed deltaTime.
    constexpr float deltaTime = 1.0f / 60.0f;
    auto velocity = player.getVelocity();
    float predictedX = player.getX() + velocity.first * deltaTime;
    float predictedY = player.getY() + velocity.second * deltaTime;
    player.setPosition(predictedX, predictedY);
    spdlog::trace("Predicted position for player {}: ({:.2f}, {:.2f})", player.getName(), predictedX, predictedY);
}

void applyNPCMovementPrediction(NPC& npc) {
    // Apply basic prediction for NPCs, assuming constant velocity in their current waypoint direction.
    constexpr float deltaTime = 1.0f / 60.0f;
    auto dir = npc.getDirectionToWaypoint();
    npc.move(dir.first * npc.getSpeed() * deltaTime, dir.second * npc.getSpeed() * deltaTime);
    spdlog::trace("Applied movement prediction for NPC {}", npc.getName());
}

void synchronizeGameState() {
    // This stub simulates sending the world state to connected clients.
    // In a full implementation, this function would serialize and transmit state.
    spdlog::info("Game State Synced");

    // Log state for each NPC. (There are no players in this demo.)
    for (const auto& [id,npc] : worldState.getNPCs()) {
        spdlog::info("NPC: {} at ({:.2f}, {:.2f})", npc.getName(), npc.getX(), npc.getY());
    }
}
