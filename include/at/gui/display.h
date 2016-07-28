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
#ifndef AT_DISPLAY_H
#define AT_DISPLAY_H
#include <at/core.h>
#include <at/gui.h>
AT_BEGIN_DECLS
/*=============================================================================
 PUBLIC STRUCTURES
 ============================================================================*/


/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_display_show_image
 * @param window
 * @param image
 * @return
 */
const char*
at_display_show_image(AtImageWindow* window, AtArrayU8* image);
/**
 * @brief at_display_show_image_by_name
 * @param name
 * @param image
 * @return
 */
AtImageWindow*
at_display_show_image_by_name(const char* name, AtArrayU8* image);
/**
 * @brief at_display_wait_key
 * @return
 */
AtKey
at_display_wait_key();
/**
 * @brief at_display_wait_key_until
 * @param miliseconds
 * @return
 */
AtKey
at_display_wait_key_until(uint32_t miliseconds);
/**
 * @brief at_display_imagewindow
 * @param name
 * @return
 */
AtImageWindow*
at_display_imagewindow(const char* name);

/**
 * @brief at_display_set_mouse_callback
 * @param window
 * @param mouse_callback
 * @param user_data
 */
void
at_display_set_mouse_callback(AtImageWindow* window, AtMouseCallback mouse_callback, void* user_data);

/**
 * @brief at_display_add_trackbar
 * @param window
 * @param trackname
 * @param vmin
 * @param vmax
 * @return
 */
AtTrackbar*
at_display_add_trackbar(AtImageWindow* window, const char* trackname, double *variable, double vmin, double vmax);
/**
 * @brief at_display_remove_trackbar
 * @param window
 * @param trackname
 */
void
at_display_remove_trackbar(AtImageWindow* window, const char* trackname);

AT_END_DECLS
#endif
