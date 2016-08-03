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
#include <at/gui.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
typedef struct _AtSliceWindowPrivate{
  AtSliceViewer* viewer;
  GtkWidget    * box;
}AtSliceWindowPrivate;
G_DEFINE_TYPE_WITH_PRIVATE(AtSliceWindow, at_slicewindow, GTK_TYPE_WINDOW)

static void
at_slicewindow_init(AtSliceWindow *self){
  AtSliceWindowPrivate* priv = at_slicewindow_get_instance_private(self);
  priv->viewer = at_sliceviewer_new();
  priv->box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_box_pack_start(GTK_BOX(priv->box),GTK_WIDGET(priv->viewer),TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(self),priv->box);
}

static void
at_slicewindow_class_init(AtSliceWindowClass *klass){

}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtSliceWindow*
at_slicewindow_new(){
  return g_object_new(AT_TYPE_SLICEWINDOW, NULL);
}

void
at_slicewindow_set_nifti(AtSliceWindow* window, AtNiftiImage* nifti){
  AtSliceWindowPrivate* priv = at_slicewindow_get_instance_private(window);
  at_sliceviewer_set_nifti(priv->viewer,nifti);
}
