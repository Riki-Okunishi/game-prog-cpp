#pragma once

#include "SDL2/SDL.h"
#include "util.hpp"

namespace mygame {

class Paddle {
   public:
    static int NUM_OF_PADDLE; /** Number of paddle (MAX 2) */
    static const double WIDTH; /** Constant width of paddle */
    static double HEIGHT; /** Default height of paddle */
    static double SPEED; /** Default speed of paddle */

    Paddle(void); /** Default constructor */
    Paddle(Vector2 position); /** Basic constructor */

    void SetKeyBind(int up, int down); /** Set key bind which key lifts up or gets down the paddle */

    void UpdatePosition(const double deltaTime); /** Update paddle position based on delta time and this paddle direction */
    void UpdateDirection(const Uint8 *state); /** Update paddle direction based on user input */

    Vector2 pos; /** Coordinates of the center of the paddle */

   private:
    int direction; /** Direction of the paddle, based on enum Direction */
    int upKey; /** Key bind what key lifts up this paddle */
    int downKey; /** Key bind what key gets down this paddle */

    /**
     * @brief Enum type that maps the upward and downward directions to actual values, respectively.
     * 
     */
    enum Direction { UP = -1, DOWN = 1 };
};

}  // namespace mygame