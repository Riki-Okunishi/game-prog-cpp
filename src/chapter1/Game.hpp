#pragma once

#include <vector>

#include "Ball.hpp"
#include "Paddle.hpp"
#include "SDL2/SDL.h"
#include "util.hpp"

namespace mygame {

/**
 * @brief Game
 * @details This object is responsible to Initialize, keep looping and shut
 * down.
 */
class Game {
   public:
    static const double WINDOW_WIDTH;   /** constant width of game window */
    static const double WINDOW_HEIGHT;  /** constant height of game window */
    static const double WALL_THICKNESS; /** constant thickness of wall */
    static const RGBA WALL;             /** constant color of wall */
    static RGBA PADDLE;                 /** constant color of paddle */
    static const RGBA BACKGROUND;       /** constant background color */
    static RGBA BALL;                   /** constant color of ball */

    Game();            /** Default constructor */
    bool Initialize(); /** Initialize this Game object to start the game */
    void RunLoop();    /** Keep game loop running */
    void Shutdown();   /** Shut down game and SDL Library */

   private:
    void ProcessInput();   /** Deal with user inputs */
    void UpdateGame();     /* Update game every frame **/
    void GenerateOutput(); /** Generate output and show game content for user */

    SDL_Window* mWindow;     /** SDL Widow to show game for user */
    SDL_Renderer* mRenderer; /** Renderer to draw some shape */

    Uint32
        mTicksCount; /** time in millisecond since SDL library initialization */
    bool mIsRunning; /** true if game is running */

    std::vector<Paddle> mPaddles = {}; /** List of paddle(s) to user operate */
    std::vector<Ball> mBalls = {};     /** List of Ball(s) */
    std::vector<SDL_Rect> mWalls = {}; /** List of walls */
};

}  // namespace mygame