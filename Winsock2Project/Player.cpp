#include "Player.h"


// Constructor: initializes member variables and sets initial positions and timestamp.
Player::Player(const std::string& name, int id)
    : name(name),
    id(id),
    posX(0.0f),
    posY(0.0f),
    prevX(0.0f),
    prevY(0.0f),
    velocityX(0.0f),
    velocityY(0.0f),
    health(100),
    lastUpdateTime(std::chrono::steady_clock::now())
{
}

// Moves the player by (x, y). Updates previous position and recalculates velocity based on elapsed time.
void Player::move(float x, float y)
{
    // Save the current position as the previous position.
    prevX = posX;
    prevY = posY;

    // Update current position with the delta values.
    posX += x;
    posY += y;

    // Compute the time elapsed since the last update.
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastUpdateTime;

    // Calculate velocity if a significant time has elapsed.
    if (deltaTime.count() > 0.0f) {
        velocityX = (posX - prevX) / deltaTime.count();
        velocityY = (posY - prevY) / deltaTime.count();
    }
    lastUpdateTime = currentTime;
}

// Stub for attack action.
void Player::attack()
{
    // Implement attack logic (e.g., damage calculation) as needed.
}

// Per-frame update: could handle animations, input, or other state updates.
void Player::update()
{
    // For now, this is a stub.
    // In a real application, you might recalculate velocity or perform input processing here.
}

// Determines if the last update was recent enough to favor interpolation.
// Here we use a threshold of 0.1 seconds (100 ms) as an example.
bool Player::hasRecentUpdate() const
{
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastUpdateTime;
    return (elapsed.count() < 0.1f);
}

// Returns the current position as a pair (posX, posY).
std::pair<float, float> Player::getCurrentPosition() const
{
    return { posX, posY };
}

// Returns the previous position as a pair (prevX, prevY).
std::pair<float, float> Player::getPreviousPosition() const
{
    return { prevX, prevY };
}

// Updates the player's position. Also updates the previous position and timestamp.
void Player::setPosition(float newX, float newY)
{
    prevX = posX;
    prevY = posY;
    posX = newX;
    posY = newY;
    lastUpdateTime = std::chrono::steady_clock::now();
}

// Returns the current velocity as a pair (velocityX, velocityY).
std::pair<float, float> Player::getVelocity() const
{
    return { velocityX, velocityY };
}

// Sets the player's velocity.
void Player::setVelocity(float newVelX, float newVelY)
{
    velocityX = newVelX;
    velocityY = newVelY;
}

// Returns the player's name.
std::string Player::getName() const
{
    return name;
}

// Returns the player's ID.
int Player::getID() const
{
    return id;
}

// Convenience accessor for the current X position.
float Player::getX() const
{
    return posX;
}

// Convenience accessor for the current Y position.
float Player::getY() const
{
    return posY;
}