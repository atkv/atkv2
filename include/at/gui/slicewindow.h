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
#ifndef AT_SLICEWINDOW_H
#define AT_SLICEWINDOW_H
#include <at/gui.h>
AT_BEGIN_DECLS

#define AT_TYPE_SLICEWINDOW at_slicewindow_get_type()
G_DECLARE_DERIVABLE_TYPE(AtSliceWindow, at_slicewindow, AT, SLICEWINDOW, GtkWindow)
///@cond IGNORE
typedef struct _AtSliceWindowClass{
  GtkWindowClass parent_class;
}AtSliceWindowClass;
///@endcond

AtSliceWindow*
at_slicewindow_new();

void
at_slicewindow_set_nifti(AtSliceWindow* window, AtNiftiImage* nifti);

AT_END_DECLS
#endif
