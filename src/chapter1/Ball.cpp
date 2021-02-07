#include "Ball.hpp"

namespace mygame {

int Ball::NUM_OF_BALL = 1;
double Ball::RADUIS = 7.5;


/**
 * @brief Construct a new Ball:: Ball object
 * 
 */
Ball::Ball(void)
    : pos(Vector2{0.0, 0.0}), vel(Vector2{0.0, 0.0}), radius(RADUIS) {}


/**
 * @brief Construct a new Ball:: Ball object
 * 
 * @param position initial center position of ball as Vetcor2
 * @param velocity initial velocity of ball as Vector2
 */
Ball::Ball(Vector2 position, Vector2 velocity)
    : pos(position), vel(velocity), radius(RADUIS) {}

/**
 * @brief Construct a new Ball:: Ball object
 * 
 * @param pos_x initial x-axis position of ball
 * @param pos_y initial y-axis position of ball
 * @param vel_x initial x-axis velocity of ball
 * @param vel_y initial y-axis velocity of ball
 */
Ball::Ball(double pos_x, double pos_y, double vel_x, double vel_y) {
    Ball(Vector2{pos_x, pos_y}, Vector2{vel_x, vel_y});
}

/**
 * @brief Update this ball position based on deltaTime
 * @param[in] deltaTime Time this ball has moved
 * @details
 */
void Ball::UpdatePosition(const double deltaTime) {
    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;
}

/**
 * @brief Judge that oneself is out of the field
 * @return true if oneself is out of the field
 * @details judge based on the position of the ball
 */
bool Ball::IsOutOfField(void) const {
    return (pos.x <= 0.0f) || (pos.x >= 1024.0f);
}

/**
 * @brief Update ball velocity if collide with the paddle
 * @details Use the default paddle width and height to determine if a collision will occur.
 * @param paddle 
 */
void Ball::CollideWithPaddle(const Paddle &paddle) {
    double hol_diff = pos.x - paddle.pos.x;
    double x_distance = (hol_diff < 0.0) ? -hol_diff : hol_diff;
    double ver_diff = pos.y - paddle.pos.y;
    double y_distance = (ver_diff < 0.0) ? -ver_diff : ver_diff;

    // y-axis collision
    if (paddle.pos.x-Paddle::WIDTH/2.0 <= pos.x &&
        pos.x <= paddle.pos.x+Paddle::WIDTH/2.0 &&
        y_distance <= radius + Paddle::HEIGHT/2.0 &&
        ver_diff * vel.y < 0.0) {
        vel.y *= -1.0;
    }
    // x-axis collision
    else if (paddle.pos.y-Paddle::HEIGHT/2.0 <= pos.y &&
             pos.y <= paddle.pos.y+Paddle::HEIGHT/2.0 &&
             x_distance <= radius + Paddle::WIDTH/2.0 &&
             hol_diff * vel.x < 0.0) {
        vel.x *= -1.0;
    }
}
/**
 * @brief Update ball velocity if collide with the wall
 * @details The ball will bounce if the distance between the center of the ball and the centerline of the wall is less than half the radius of the ball and the thickness of the wall.
 * @param wall 
 */
void Ball::CollideWithWall(const SDL_Rect &wall) {
    double hol_diff = pos.x - static_cast<double>(wall.x + wall.w / 2);
    double x_distance = (hol_diff < 0.0) ? -hol_diff : hol_diff;
    double ver_diff = pos.y - static_cast<double>(wall.y + wall.h / 2);
    double y_distance = (ver_diff < 0.0) ? -ver_diff : ver_diff;

    // y-axis collision
    if (static_cast<double>(wall.x)<= pos.x &&
        pos.x <= static_cast<double>(wall.x + wall.w)&&
        y_distance <= radius + static_cast<double>(wall.h / 2) &&
        ver_diff * vel.y < 0.0) {
        vel.y *= -1.0;
    }
    // x-axis collision
    else if (static_cast<double>(wall.y)<= pos.y &&
             pos.y <= static_cast<double>(wall.y + wall.h)&&
             x_distance <= radius + static_cast<double>(wall.w / 2) &&
             hol_diff * vel.x < 0.0) {
        vel.x *= -1.0;
    }
}

}  // namespace mygame