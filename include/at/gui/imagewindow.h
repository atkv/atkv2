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
#ifndef AT_IMAGEWINDOW_H
#define AT_IMAGEWINDOW_H
#include <at/core/array.h>
#include <at/gui/imageviewer.h>
AT_BEGIN_DECLS
/*=============================================================================
 STRUCTURE
 ============================================================================*/
#define AT_TYPE_IMAGEWINDOW at_imagewindow_get_type()
G_DECLARE_DERIVABLE_TYPE(AtImageWindow, at_imagewindow, AT, IMAGEWINDOW, GtkWindow)

struct _AtImageWindowClass{
  GtkWindowClass parent_class;
};
/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_imagewindow_new
 * @return
 */
AtImageWindow*
at_imagewindow_new();
/**
 * @brief at_imagewindow_set
 * @param window
 * @param array
 */
void
at_imagewindow_set(AtImageWindow* window, AtArrayU8* array);
/**
 * @brief at_imagewindow_set_mouse_callback
 * @param window
 * @param mouse_callback
 * @param user_data
 */
void
at_imagewindow_set_mouse_callback(AtImageWindow* window,
                                  AtMouseCallback mouse_callback,
                                  void* user_data);
AT_END_DECLS
#endif
