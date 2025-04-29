#pragma once
#include <vector>
#include "Player.h"
#include "NPC.h"	
#include "WorldState.h"

// Define NPC


// Initializes game logic
void initializeGame();

// Updates the game world per tick
void updateGameWorld();

// Handles player interactions (movement, actions)
void processPlayerActions();

// Handles movement prediction
void applyMovementPrediction(Player& player);
void applyNPCMovementPrediction(NPC& npc);

// Synchronizes game state across clients
void synchronizeGameState();