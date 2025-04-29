#include "NPC.h"

// Constructor – Initializes the NPC with a name, an id, and a speed.
// Positions default to 0 and no waypoint is set.
NPC::NPC(const std::string& npcName, int npcID, float speed)
    : npcName(npcName),
    npcID(npcID),
    posX(0.0f),
    posY(0.0f),
    speed(speed),
    waypointSet(false),
    waypointX(0.0f),
    waypointY(0.0f)
{
}

// Basic movement: Move the NPC by adding the (x, y) delta.
void NPC::move(float x, float y) {
    posX += x;
    posY += y;
}

// Stub for specialized NPC behavior or interactions.
void NPC::interact() {
    // Implement specific NPC behavior here.
}

// Update can integrate per-frame logic (e.g., animations, AI behaviors).
void NPC::update() {
    // For now, just a placeholder.
}

// --- Waypoint and Movement Prediction Functions ---

// Sets a new waypoint target. The NPC will try to move toward this position.
void NPC::setWaypoint(float targetX, float targetY) {
    waypointX = targetX;
    waypointY = targetY;
    waypointSet = true;
}

// Returns true if a waypoint has been set.
bool NPC::hasWaypoint() const {
    return waypointSet;
}

// Returns a normalized direction vector (dx, dy) pointing from the current position to the waypoint.
// If the NPC is already at or extremely close to the waypoint, returns (0,0).
std::pair<float, float> NPC::getDirectionToWaypoint() const {
    if (!waypointSet)
        return { 0.0f, 0.0f };

    float dx = waypointX - posX;
    float dy = waypointY - posY;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance > 0.0f) {
        return { dx / distance, dy / distance };
    }
    return { 0.0f, 0.0f };
}

// Checks if the NPC has reached its waypoint. If within a small threshold, the waypoint is cleared.
void NPC::checkWaypointCompletion() {
    if (!waypointSet)
        return;
    float dx = waypointX - posX;
    float dy = waypointY - posY;
    const float thresholdSquared = 0.01f; // Adjust as needed
    if ((dx * dx + dy * dy) < thresholdSquared) {
        waypointSet = false;
    }
}

// --- Accessors and Mutators ---

std::string NPC::getName() const {
    return npcName;
}

int NPC::getID() const {
    return npcID;
}

// Returns the NPC's current position as a pair (posX, posY)
std::pair<float, float> NPC::getPosition() const {
    return { posX, posY };
}

float NPC::getX() const {
    return posX;
}

float NPC::getY() const {
    return posY;
}

float NPC::getSpeed() const {
    return speed;
}

// Sets the NPC's position to newX and newY.
void NPC::setPosition(float newX, float newY) {
    posX = newX;
    posY = newY;
}

// Sets a new value for NPC speed.
void NPC::setSpeed(float newSpeed) {
    speed = newSpeed;
}