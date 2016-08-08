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
#ifndef AT_TRACKBAR_H
#define AT_TRACKBAR_H
#include <stdint.h>
#include <gtk/gtk.h>
G_BEGIN_DECLS
/*=============================================================================
 PUBLIC STRUCTURE
 ============================================================================*/
#define AT_TYPE_TRACKBAR at_trackbar_get_type()
G_DECLARE_DERIVABLE_TYPE(AtTrackbar, at_trackbar, AT, TRACKBAR, GtkBox)
/// @cond IGNORE
typedef struct _AtTrackbarClass{
  GtkBoxClass parent_class;
}AtTrackbarClass;
/// @endcond

/*=============================================================================
 PUBLIC API
 ============================================================================*/
typedef void (*AtTrackbarFunc)(AtTrackbar* t, double pos);
typedef void (*AtTrackbarDataFunc)(AtTrackbar* t, double pos, void* user_data);

/**
 * @brief at_trackbar_new
 * @return
 */
AtTrackbar*
at_trackbar_new();
/**
 * @brief at_trackbar_new_named
 * @param name
 * @return
 */
AtTrackbar*
at_trackbar_new_named(const char *name);
/**
 * @brief at_trackbar_set_min
 * @param t
 * @param minv
 * @return
 */
void
at_trackbar_set_min(AtTrackbar* t, double minv);
/**
 * @brief at_trackbar_set_max
 * @param t
 * @param maxv
 * @return
 */
void
at_trackbar_set_max(AtTrackbar* t, double maxv);
/**
 * @brief at_trackbar_set_pos
 * @param t
 * @param pos
 * @return
 */
void
at_trackbar_set_pos(AtTrackbar* t, double pos);
/**
 * @brief at_trackbar_set_name
 * @param t
 * @param name
 */
void
at_trackbar_set_name(AtTrackbar* t, const char *name);
/**
 * @brief at_trackbar_set_variable
 * @param t
 * @param variable
 */
void
at_trackbar_set_variable(AtTrackbar* t, double* variable);
/**
 * @brief at_trackbar_get_min
 * @param t
 * @return
 */
double
at_trackbar_get_min(AtTrackbar* t);
/**
 * @brief at_trackbar_get_max
 * @param t
 * @return
 */
double
at_trackbar_get_max(AtTrackbar* t);
/**
 * @brief at_trackbar_get_pos
 * @param t
 * @return
 */
double
at_trackbar_get_pos(AtTrackbar* t);
/**
 * @brief at_trackbar_get_name
 * @param t
 * @return
 */
const char*
at_trackbar_get_name(AtTrackbar* t);
/**
 * @brief at_trackbar_on_change
 * @param t
 * @param variable
 * @param cb
 */
void
at_trackbar_on_change(AtTrackbar* t, AtTrackbarFunc cb);
/**
 * @brief at_trackbar_on_change_data
 * @param t
 * @param cbd
 * @param data
 */
void
at_trackbar_on_change_data(AtTrackbar* t, AtTrackbarDataFunc cbd, void* data);
G_END_DECLS
#endif
