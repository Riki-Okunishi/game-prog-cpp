#include "Game.hpp"

/** 壁の厚さ */
const int thickness = 15;
/** パドルの高さ */
const float paddleH = 100.0f;

Game::Game()
    : mWindow(nullptr),
      mRenderer(nullptr),
      mTicksCount(0),
      mIsRunning(true),
      mPaddleDir(0) {}

bool Game::Initialize() {
    // SDLの初期化
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        // SDL_Init()の結果がエラーなら続行不可
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // SDLウインドウの作成
    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)",  // Windowのタイトル
        100,                                    // 左上隅のx座標
        100,                                    // 左上隅のy座標
        1024,                                   // ウインドウの幅
        768,                                    // ウインドウの高さ
        0  // フラグ(オプション．OpenGLを使う，ウインドウサイズ可変，とか)
    );

    if (!mWindow) {
        // ウインドウIDが0ならウインドウの作成が失敗した
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // SDL rendererの作成
    mRenderer = SDL_CreateRenderer(
        mWindow,  // SDLのウインドウ
        -1,  // 基本は-1(ゲームが複数のウインドウを持つ場合に使用)
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    // 各種変数の初期化
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f / 2.0f;
    mBallPos.x = 1024.0f / 2.0f;
    mBallPos.y = 768.0f / 2.0f;
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;
    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

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

    // 上を1，下を+1で移動方向を表現
    // if文が分かれているので，同時押しの場合は相殺
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W]) {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        mPaddleDir += 1;
    }
}
void Game::UpdateGame() {
    // 前のフレームから16msが経過するまで待つ(60FPS)
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // deltaTimeはミリ秒なので秒に変換
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // deltaTimeが大きくなりすぎないよう制限する
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    // 次のフレームのために時刻を更新
    mTicksCount = SDL_GetTicks();

    // パドルの位置を更新
    if (mPaddleDir != 0) {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        // 画面(パドルの中心が動ける範囲)外に行く場合は境界値で更新
        if (mPaddlePos.y < (paddleH / 2.0f + thickness)) {
            mPaddlePos.y = paddleH / 2.0f + thickness;
        } else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness)) {
            mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    // ボールを速度に従って移動させる
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    // 必要に応じてボールをバウンド
    // ボールがバウンドできるのは「壁へ触れに行くとき」=>速度が反転していれば，触れていても無視

    /* パドルとボールの衝突判定 */
    /** パドルとボールのy座標の差の絶対値 */
    float diff = (mPaddlePos.y > mBallPos.y) ? (mPaddlePos.y - mBallPos.y) : (mBallPos.y - mPaddlePos.y);
    if (
        // パドルの高さの半分よりも小さい(=パドルに当たる高さ)
        diff <= paddleH / 2.0f &&
        // ボールが正しいx座標にいる
        mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
        // ボールが左に向かっている
        mBallVel.x < 0.0f) {
        mBallVel.x *= -1.0f;
    }
    // ボールが画面外に行けば終了
    else if (mBallPos.x <= 0.0f) {
        mIsRunning = false;
    }

    /* 壁との衝突判定 */
    // 右の壁に触れ，かつ右方向へ動いているか？
    else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f) {
        mBallVel.x *= -1.0f;
    }
    // 上の壁に触れ，かつ上方向へ動いているか？
    if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
        mBallVel.y *= -1;
    }
    // 下の壁に触れ，かつ下方向へ動いているか？
    else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f) {
        mBallVel.y *= -1;
    }
}

void Game::GenerateOutput() {
    // 青で塗りつぶすよう設定
    SDL_SetRenderDrawColor(mRenderer,
                           0,    // R
                           0,    // G
                           255,  // B
                           255   // A
    );

    // バックバッファをクリア
    SDL_RenderClear(mRenderer);

    // 壁を塗る用の白
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    // 上の壁
    SDL_Rect wall{
        0,         // 左上x座標
        0,         // 左上y座標
        1024,      // 長方形の幅　左右いっぱい
        thickness  // 長方形の高さ　壁の厚さ分
    };
    SDL_RenderFillRect(mRenderer, &wall);

    // 下の壁
    wall.y = 768 - thickness; // 上の壁の高さを変えればいい
    SDL_RenderFillRect(mRenderer, &wall);

    // 右の壁
    wall.x = 1024 - thickness; // 画面右端から厚さ分戻る
    wall.y = 0; // 画面上端から
    wall.w = thickness; //幅は壁の厚さ分
    wall.h = 1024; // 高さは画面いっぱい
    SDL_RenderFillRect(mRenderer, &wall);

    // パドル
    SDL_Rect paddle{static_cast<int>(mPaddlePos.x),
                    static_cast<int>(mPaddlePos.y - paddleH / 2),
                    thickness,
                    static_cast<int>(paddleH)};
    SDL_RenderFillRect(mRenderer, &paddle);

    // ボール
    SDL_Rect ball{static_cast<int>(mBallPos.x - thickness / 2),
                  static_cast<int>(mBallPos.y - thickness / 2),
                  thickness,
                  thickness};
    // ボールの色は赤
    SDL_SetRenderDrawColor(mRenderer, 240, 0, 0, 255);
    SDL_RenderFillRect(mRenderer, &ball);

    // バックバッファとフロントバッファを交換
    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
