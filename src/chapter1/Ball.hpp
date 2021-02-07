#pragma once

#include "Paddle.hpp"
#include "SDL2/SDL.h"
#include "util.hpp"

/**
 * @namespace mygame
 * @brief namespace for my game development
 * @details This is a temporary name, therefore it will change to another.
 */
namespace mygame {

/**
 * @brief Ball
 * @details Ball object described as square
 */
class Ball {
   public:
    static int NUM_OF_BALL; /** Default number of balls */
    static double RADUIS; /** Default radius */

    Ball(void); /** Default constructor */
    Ball(Vector2 position, Vector2 velocity); /** Basic constructor */
    Ball(double pos_x, double pos_y, double vel_x, double vel_y); /** Wrapper constructor for one that use Vector2 */
    Vector2 pos; /** Coordinates of the center of the ball */
    Vector2 vel; /** Velocity of ball */
    double radius; /** Radius of ball */

    void UpdatePosition(const double deltaTime);  /** Update ball position based on delta time */
    void CollideWithPaddle(const Paddle &paddle); /** Update ball velocity if collide with the paddle */
    void CollideWithWall(const SDL_Rect &wall); /** Update ball velocity if collide with the wall */
    bool IsOutOfField(void) const; /** Judge that oneself is out of the field */

   private:
};

}  // namespace mygame