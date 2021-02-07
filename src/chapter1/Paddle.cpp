#include "paddle.hpp"

namespace mygame {

int Paddle::NUM_OF_PADDLE = 1;
const double Paddle::WIDTH = 15.0;
double Paddle::HEIGHT = 100.0;
double Paddle::SPEED = 300.0;

/**
 * @brief Construct a new Paddle:: Paddle object
 * 
 */
Paddle::Paddle(void)
    : direction(0),
      pos(Vector2{10.0, 768.0 / 2.0}),
      upKey(SDL_SCANCODE_W),
      downKey(SDL_SCANCODE_S) {}

/**
 * @brief Construct a new Paddle:: Paddle object
 * 
 * @param position initial position of padlle as Vector2
 */
Paddle::Paddle(Vector2 position)
    : direction(0),
      pos(position),
      upKey(SDL_SCANCODE_W),
      downKey(SDL_SCANCODE_S) {}

/**
 * @brief Set key bind which key lifts up or gets down the paddle
 * 
 * @param up Key bind what key lifts up this paddle as SDL_SCANCODE_XXX
 * @param down Key bind what key gets down this paddle as SDL_SCANCODE_XXX
 */
void Paddle::SetKeyBind(int up, int down) {
    upKey = up;
    downKey = down;
}

/**
 * @brief Update this paddle position
 *
 * @param deltaTime delta time between this frame and previous frame
 */
void Paddle::UpdatePosition(const double deltaTime) {
    if (direction != 0) {
        pos.y += direction * SPEED * deltaTime;
        // if paddle is out of range, set to limit position
        if (pos.y < (HEIGHT / 2.0f + WIDTH)) {
            pos.y = HEIGHT / 2.0f + WIDTH;
        } else if (pos.y > (768.0f - HEIGHT / 2.0f - WIDTH)) {
            pos.y = 768.0f - HEIGHT / 2.0f - WIDTH;
        }
    }
}

/**
 * @brief Change this paddle direction if Binded key in keyboard state are pushed
 * 
 * @param state keyboard state got from SDL_GetKeyboardState()
 */
void Paddle::UpdateDirection(const Uint8 *state) {
    direction = 0;
    if (state[upKey]) {
        direction += Paddle::UP;
    }
    if (state[downKey]) {
        direction += Paddle::DOWN;
    }
}

}  // namespace mygame