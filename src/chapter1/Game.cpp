#include "Game.hpp"

#include <random>

namespace mygame {

const double Game::WINDOW_WIDTH = 1024.0;
const double Game::WINDOW_HEIGHT = 768.0;
const double Game::WALL_THICKNESS = 15.0;
const RGBA Game::WALL = {255, 255, 255, 255};
RGBA Game::PADDLE = {255, 255, 255, 255};
const RGBA Game::BACKGROUND = {0, 0, 240, 255};
RGBA Game::BALL = {240, 0, 0, 255};

/**
 * @brief Construct a new Game:: Game object
 *
 */
Game::Game()
    : mWindow(nullptr),
      mRenderer(nullptr),
      mTicksCount(0),
      mIsRunning(true) {}

/**
 * @brief Initialize this Game object to start the game
 *
 * @return true if succeeded to initialize
 * @return false if failed to initialize SDL Init, Window or Renderer
 */
bool Game::Initialize() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO); /** result of initializing SDL */
    if (sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)",  // Window title
        100,   // Top left x-coordinate of window
        50,    // Top left y-coordinate of window
        1024,  // width
        768,   // height
        0      // option flag (using OpenGL, window resizable, etc.)
    );

    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,  // SDL window
        -1,       // tipically -1 (to use when game has some windows)
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  // use vsync

    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    // Create paddle(s)
    std::vector<std::vector<int>> keyBinds = {{SDL_SCANCODE_W, SDL_SCANCODE_S},
                                              {SDL_SCANCODE_I, SDL_SCANCODE_K}};
    /** @TODO have to check whether key binds conflict or not */

    // NUM_OF_PADDLE must be 1 and over and under 3.
    if(Paddle::NUM_OF_PADDLE <0){
      Paddle::NUM_OF_PADDLE = 1;
    }else if(Paddle::NUM_OF_PADDLE > 2){
      Paddle::NUM_OF_PADDLE = 2;
    }
    for (int i = 0; i < Paddle::NUM_OF_PADDLE; i++) {
        Paddle p = Paddle{Vector2{10.0 + Paddle::WIDTH / 2.0 + 994.0 * i,
                                  Game::WINDOW_HEIGHT / 2.0}};
        p.SetKeyBind(keyBinds[i][0], keyBinds[i][1]);
        this->mPaddles.push_back(p);
        SDL_Log("Paddle %d: (%f, %f)\n", i, p.pos.x, p.pos.y);
    }

    // Create Ball(s)
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<double> pos_x(WINDOW_WIDTH / 2.0 - 100.0,
                                                 WINDOW_WIDTH / 2.0 - 100.0);
    std::uniform_real_distribution<double> pos_y(100.0, WINDOW_HEIGHT - 100.0);
    std::uniform_real_distribution<double> vel(100, 250.0);
    std::uniform_int_distribution<> dirc(0, 1);

    for (int i = 0; i < Ball::NUM_OF_BALL; i++) {
        this->mBalls.emplace_back(Vector2{pos_x(eng), pos_y(eng)},
                                  Vector2{(dirc(eng) * -2 + 1) * vel(eng),
                                          (dirc(eng) * -2 + 1) * vel(eng)});
    }

    // Create Walls
    SDL_Rect top = {0, 0, static_cast<int>(WINDOW_WIDTH),
                    static_cast<int>(WALL_THICKNESS)};
    SDL_Rect bottom = {0, static_cast<int>(WINDOW_HEIGHT - WALL_THICKNESS),
                       static_cast<int>(WINDOW_WIDTH),
                       static_cast<int>(WALL_THICKNESS)};
    mWalls.push_back(top);     // top wall
    mWalls.push_back(bottom);  // bottom wall
    if (mPaddles.size() == 1) {
        SDL_Rect right = {static_cast<int>(WINDOW_WIDTH - WALL_THICKNESS), 0,
                          static_cast<int>(WALL_THICKNESS),
                          static_cast<int>(WINDOW_HEIGHT)};
        mWalls.push_back(right);  // right wall
    }

    SDL_Log("Paddle A: (%f, %f), Paddle B: (%f, %f)\n", mPaddles[0].pos.x,
            mPaddles[0].pos.y, mPaddles[1].pos.x, mPaddles[1].pos.y);
    return true;
}


/**
 * @brief Keep game loop running
 * 
 */
void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

/**
 * @brief Deal with user inputs
 * 
 */
void Game::ProcessInput() {
    /** SDLが管理するOSから受け取ったイベント．ウインドウ関連のイベント(無視すればいい)も取得される */
    SDL_Event event;
    // イベントがSDLの管理するキューにあればtrueを返してeventにはポインタが入る
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            // ウインドウが閉じられたイベント
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    // Get キーボードの状態を取得
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // Escが押されていればゲーム終了
    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    // Update direction of paddle(s)
    for (auto it = mPaddles.begin(); it != mPaddles.end(); ++it) {
        (*it).UpdateDirection(state);
    }
}

/**
 * @brief Update game every frame
 * 
 */
void Game::UpdateGame() {
    // 前のフレームから16msが経過するまで待つ(60FPS)
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // deltaTimeはミリ秒なので秒に変換
    /** 前フレームからの経過時間[ms] */
    double deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // deltaTimeが大きくなりすぎないよう制限する
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    // 次のフレームのために時刻を更新
    mTicksCount = SDL_GetTicks();

    // パドルの位置を更新
    for (auto p = mPaddles.begin(); p != mPaddles.end(); ++p) {
        (*p).UpdatePosition(deltaTime);
    }

    // ボールに対しての処理
    for (auto b = mBalls.begin(); b != mBalls.end();
         ++b) {  // 拡張forだと変数のスコープがfor内になるので注意
        /** @todo ボールの処理は衝突判定など，排反なのか独立なのか確認 */
        // ボールを速度に従って移動させる
        (*b).UpdatePosition(deltaTime);

        /* ボールが画面外に行けば終了 */
        if ((*b).IsOutOfField()) {
            mIsRunning = false;
        }

        /* パドルとボールの衝突判定 */
        for (auto p = mPaddles.begin(); p != mPaddles.end(); ++p) {
            (*b).CollideWithPaddle(*p);
        }

        /* ボールと壁の衝突判定 */
        for (auto w = mWalls.begin(); w != mWalls.end(); ++w) {
            (*b).CollideWithWall(*w);
        }
    }
}

/**
 * @brief Generate output and show game content for user
 * 
 */
void Game::GenerateOutput() {
    // 青で塗りつぶす
    SDL_SetRenderDrawColor(mRenderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);

    // バックバッファを(青で)クリア
    SDL_RenderClear(mRenderer);

    // 壁
    SDL_SetRenderDrawColor(mRenderer, WALL.r, WALL.g, WALL.b, WALL.a);
    for (auto w = mWalls.begin(); w != mWalls.end(); ++w) {
        SDL_RenderFillRect(mRenderer, &(*w));
    }

    // パドル
    SDL_SetRenderDrawColor(mRenderer, PADDLE.r, PADDLE.g, PADDLE.b, PADDLE.a);
    for (auto p = mPaddles.begin(); p != mPaddles.end(); ++p) {
        SDL_Rect paddle{static_cast<int>((*p).pos.x - Paddle::WIDTH / 2.0),
                        static_cast<int>((*p).pos.y - Paddle::HEIGHT / 2.0),
                        static_cast<int>(Paddle::WIDTH), static_cast<int>(Paddle::HEIGHT)};
        SDL_RenderFillRect(mRenderer, &paddle);
    }

    // ボール
    SDL_SetRenderDrawColor(mRenderer, BALL.r, BALL.g, BALL.b,
                           BALL.a);
    for (auto b = mBalls.begin(); b != mBalls.end(); ++b) {
        SDL_Rect ball{static_cast<int>((*b).pos.x - Ball::RADUIS),
                      static_cast<int>((*b).pos.y - Ball::RADUIS),
                      static_cast<int>(Ball::RADUIS * 2.0),
                      static_cast<int>(Ball::RADUIS * 2.0)};
        SDL_RenderFillRect(mRenderer, &ball);
    }

    // バックバッファとフロントバッファを交換
    SDL_RenderPresent(mRenderer);
}

/**
 * @brief Shut down game and SDL Library
 * 
 */
void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

}  // namespace mygame