#pragma once
#include <string>
#include <chrono>
#include <utility>

class Player {
public:
    // Constructor accepts a name and an id
    Player() = default;
	Player(std::string name, int id);

    // Basic actions
    void move(float x, float y);
    void attack();
    void update();  // Handles per-frame updates

    // Get player name
	std::string getName() const;

    // Movement prediction support:
    // Returns true if the last position update was recent enough to use interpolation.
    bool hasRecentUpdate() const;

    // Getters for current and previous positions
    std::pair<float, float> getCurrentPosition() const;
    std::pair<float, float> getPreviousPosition() const;

    // Setters for position (if you need to update them externally)
    void setPosition(float newX, float newY);

    // Getters for velocity
    std::pair<float, float> getVelocity() const;
    void setVelocity(float newVelX, float newVelY);
    
	void saveToBinary(std::ofstream& outFile) const;
    void loadFromBinary(std::ifstream& inFile);

	// Getters for player ID and position
    int getID() const;
    float getX() const;
    float getY() const;

private:
    std::string name;
    int playerID;
    float posX, posY;       // Current position
    float prevX, prevY;     // Previous position (used for interpolation)
    float velocityX, velocityY; // Current velocity (used for extrapolation)
    int health;

    // Timestamp of the last movement update using std::chrono for precision timing.
    std::chrono::steady_clock::time_point lastUpdateTime;
};