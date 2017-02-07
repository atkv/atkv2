/**
 ** This file is part of the atkv project.
 ** Copyright 2016-2017 Anderson Tavares <nocturne.pe@gmail.com>.
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
#ifndef AT_SLICEVIEWER_H
#define AT_SLICEVIEWER_H
#include <at/gui.h>
AT_BEGIN_DECLS

/*=============================================================================
 PUBLIC STRUCTURE
 ============================================================================*/
typedef enum{
  AT_SLICEVIEWER_3_1,
  AT_SLICEVIEWER_2_2
}AtSliceViewerLayout;

typedef enum{
  AT_AXIAL   = 0,
  AT_CORONAL = 1,
  AT_SAGITAL = 2
}AtSliceType;

#define AT_TYPE_SLICEVIEWER at_sliceviewer_get_type()
G_DECLARE_DERIVABLE_TYPE(AtSliceViewer,at_sliceviewer,AT,SLICEVIEWER,GtkBin)
///@cond IGNORE
typedef struct _AtSliceViewerClass{
  GtkBinClass parent_class;
}AtSliceViewerClass;
///@endcond

/*=============================================================================
 PUBLIC API
 ============================================================================*/

/**
 * @brief at_sliceviewer_new
 * @return
 */
AtSliceViewer*
at_sliceviewer_new();

/**
 * @brief at_sliceviewer_set_layout
 * @param viewer
 * @param layout
 */
void
at_sliceviewer_set_layout(AtSliceViewer* viewer, AtSliceViewerLayout layout);
/**
 * @brief at_sliceviewer_set_nifti
 * @param viewer
 * @param nifti
 */
void
at_sliceviewer_set_nifti(AtSliceViewer* viewer, AtNiftiImage* nifti);
/**
 * @brief at_sliceviewer_rotate_cw
 * @param viewer
 * @param type
 */
void
at_sliceviewer_rotate_cw(AtSliceViewer* viewer, AtSliceType type);
/**
 * @brief at_sliceviewer_rotate_ccw
 * @param viewer
 * @param type
 */
void
at_sliceviewer_rotate_ccw(AtSliceViewer* viewer, AtSliceType type);
/**
 * @brief at_sliceviewer_set_index
 * @param viewer
 * @param type
 * @param index
 */
void
at_sliceviewer_set_index(AtSliceViewer* viewer, AtSliceType type, uint16_t index);
AT_END_DECLS
#endif
