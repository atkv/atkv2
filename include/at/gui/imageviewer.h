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
#ifndef AT_IMAGEVIEWER_H
#define AT_IMAGEVIEWER_H

#include <gtk/gtk.h>
#include <stdint.h>
#include <at/gui.h>

AT_BEGIN_DECLS

#define AT_TYPE_IMAGEVIEWER at_imageviewer_get_type()
G_DECLARE_DERIVABLE_TYPE(AtImageViewer, at_imageviewer, AT, IMAGEVIEWER, GtkWidget)

struct _AtImageViewerClass{
  GtkWidgetClass parent_class;
};

/**
 * @brief at_imageviewer_new
 * @return
 */
AtImageViewer*
at_imageviewer_new();

/**
 * @brief at_imageviewer_get_array
 * @param imgv
 * @return
 */
AtArrayU8*
at_imageviewer_get_array(AtImageViewer* imgv);

/**
 * @brief Zoom based on center
 * @param imgv
 */
void
at_imageviewer_zoom_in(AtImageViewer* imgv);
/**
 * @brief at_imageviewer_zoom_out
 * @param imgv
 */
void
at_imageviewer_zoom_out(AtImageViewer* imgv);
/**
 * @brief at_imageviewer_zoom_in_at
 * @param imgv
 * @param cx
 * @param cy
 */
void
at_imageviewer_zoom_in_at(AtImageViewer *imgv, int16_t cx, int16_t cy);
/**
 * @brief at_imageviewer_zoom_out_at
 * @param imgv
 * @param cx
 * @param cy
 */
void
at_imageviewer_zoom_out_at(AtImageViewer *imgv, int16_t cx, int16_t cy);

/**
 * @brief at_imageviewer_zoom_fit
 * @param imgv
 */
void
at_imageviewer_zoom_fit(AtImageViewer* imgv);

/**
 * @brief at_imageviewer_reset_zoom
 * @param imgv
 */
void
at_imageviewer_zoom_reset(AtImageViewer* imgv);
/**
 * @brief at_imageviewer_move_to
 * @param imgv
 * @param offsetx
 * @param offsety
 */
void
at_imageviewer_move_to(AtImageViewer* imgv, int16_t offsetx, int16_t offsety);
/**
 * @brief at_imageviewer_move_to_default
 * @param imgv
 * @param alignmentX
 * @param aligmentY
 */
void
at_imageviewer_move_to_default(AtImageViewer* imgv, AtAlignment alignmentX, AtAlignment aligmentY);
/**
 * @brief at_imageviewer_move
 * @param imgv
 * @param offsetx
 * @param offsety
 */
void
at_imageviewer_move(AtImageViewer* imgv, int16_t offsetx, int16_t offsety);
/**
 * @brief at_imageviewer_reset_translation
 * @param imgv
 */
void
at_imageviewer_move_reset(AtImageViewer* imgv);
/**
 * @brief at_imageviewer_set
 * @param imgv
 * @param array
 */
void
at_imageviewer_set(AtImageViewer* imgv, AtArrayU8* array);
/**
 * @brief at_imageviewer_get_pixel
 * @param imgv the widget
 * @param pixel array of values to be modified
 * @param x the x coordinate of mouse (in widget space)
 * @param y the y coordinate of mouse (in widget space)
 * @return true if coordinates are inside image, false otherwise.
 */
gboolean
at_imageviewer_get_pixel(AtImageViewer* imgv, uint8_t *pixel, uint16_t x, uint16_t y);
/**
 * @brief at_imageviewer_get_img_pos
 * @param imgv
 * @param pos
 * @param x
 * @param y
 * @return
 */
gboolean
at_imageviewer_get_array_pos(AtImageViewer* imgv, int16_t *pos,uint16_t x, uint16_t y);
/**
 * @brief at_imageviewer_set_mouse_callback
 * @param imgv
 * @param mouse_callback
 * @param user_data
 */
void
at_imageviewer_set_mouse_callback(AtImageViewer* imgv, AtMouseCallback mouse_callback, void* user_data);
AT_END_DECLS

#endif
