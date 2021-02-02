#pragma once

#include "SDL2/SDL.h"


struct Vector2 {
    double x;
    double y;
};


/**
 * @brief Gameの基盤になるクラス
 * @author 
 * @since Sun Jan 31 2021
 */
class Game {
    public:
        Game();
        bool Initialize();
        void RunLoop();
        void Shutdown();
    private:
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Uint32 mTicksCount;
        /** ゲームが進行中であることを示すフラグ */
        bool mIsRunning;
        
        int mPaddleDir;
        /** パドルの座標(x:左上, y:パドル中央) */
        Vector2 mPaddlePos;
        Vector2 mBallPos;
        Vector2 mBallVel;
};