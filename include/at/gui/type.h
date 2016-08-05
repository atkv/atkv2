/**
 ** This file is part of the atkv project.
 ** Copyright 2016 Anderson Tavares <nocturne.pe@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#if !defined(AT_GUI_H_INSIDE)
#error "Only <at/gui.h> can be included directly."
#endif
#ifndef AT_GUI_TYPE_H
#define AT_GUI_TYPE_H
#include <stdint.h>

/**
 * @brief Enumerator for keys (for key events)
 */
typedef enum{
  AT_KEY_A,
  AT_KEY_B,
  AT_KEY_C,
  AT_KEY_D,
  AT_KEY_E,
  AT_KEY_F,
  AT_KEY_G,
  AT_KEY_H,
  AT_KEY_I,
  AT_KEY_J,
  AT_KEY_K,
  AT_KEY_L,
  AT_KEY_M,
  AT_KEY_N,
  AT_KEY_O,
  AT_KEY_P,
  AT_KEY_Q,
  AT_KEY_R,
  AT_KEY_S,
  AT_KEY_T,
  AT_KEY_U,
  AT_KEY_V,
  AT_KEY_W,
  AT_KEY_X,
  AT_KEY_Y,
  AT_KEY_Z,
  AT_KEY_ESCAPE,
}AtKey;

/**
 * @brief Enumerator for mouse event types
 */
typedef enum{
  AT_MOUSE_LEFT_DOWN,
  AT_MOUSE_LEFT_UP,
  AT_MOUSE_RIGHT_DOWN,
  AT_MOUSE_RIGHT_UP,
  AT_MOUSE_MOVE
}AtMouseEventType;

/**
 * @brief Enumerator for basic alignment
 */
typedef enum{
  AT_TOP,
  AT_BOTTOM,
  AT_LEFT,
  AT_RIGHT,
  AT_CENTER
}AtAlignment;

/**
 * @brief Mouse Event Structure
 */
typedef struct AtMouseEvent{
  int16_t x;            /*!< x coordinate of mouse event */
  int16_t y;            /*!< y coordinate of mouse event */
  AtMouseEventType type;/*!< type of mouse event */
}AtMouseEvent;

/**
 * @brief Pointer for Mouse Callback Functions
 */
typedef void
(*AtMouseCallback)(AtMouseEvent* event, void* user_data);

#endif
