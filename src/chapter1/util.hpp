#pragma once

namespace mygame {

/**
* @brief 2-dim Vector by double
* @details 
*/
struct Vector2 {
    double x; /** @brief X-axis value */
    double y; /** @brief X-axis value */
};

/**
* @brief red, green, blue and alpha valuesindicating a color
* @details 
*/
struct RGBA {
  Uint8 r; /** @brief red (0-255) */
  Uint8 g; /** @brief green (0-255) */
  Uint8 b; /** @brief blue (0-255) */
  Uint8 a; /** @brief alpha (0-255) */
};

}