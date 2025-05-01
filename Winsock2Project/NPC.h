#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <utility> // For std::pair

struct Position {
    float x, y;
};


class NPC {
public:
    // Constructor now accepts an optional speed parameter.
    NPC() = default;
	NPC(const std::string& npcName, int npcID, float speed = 1.0f);

    // Basic functionalities
    void move(float x, float y);
    void interact();  // NPC behavior logic
    void update();    // Handles per-frame updates

    // Movement prediction / waypoint functions 
   

    // Sets a new waypoint target for the NPC.
    void setWaypoint(float targetX, float targetY);
    // Returns whether a waypoint has been set.
    bool hasWaypoint() const;
    // Calculates and returns the normalized direction vector (dx, dy) toward the waypoint.
    //std::pair<float, float> getDirectionToWaypoint() const;
    // Checks if the NPC has reached its waypoint (within a small threshold) and clears it if so.
    void checkWaypointCompletion();

    // Accessors for identification and movement data
    std::string getName() const;
    int getID() const;

    // Getters for position and speed
    std::pair<float, float> getPosition() const;
    float getX() const;
    float getY() const;
    float getSpeed() const;

    // Mutators for position and speed
    Position getPositiom() const;
    void setPosition(float newX, float newY);
    void setSpeed(float newSpeed);
    void saveToBinary(std::ofstream& outFile) const;
    void loadFromBinary(std::ifstream& inFile);
    std::pair <float, float> getDirectionToWaypoint() const;

private:
    std::string npcName;
    int npcID;
    float posX, posY;   // Current position
    float speed;        // Movement speed
    
    // Waypoint data
    bool waypointSet;   // Indicates if a waypoint is active
    float waypointX, waypointY;  // Target waypoint coordinates
};




