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
const char* tracknames[3] = {"sagital","axial","coronal"};
typedef struct _AtSliceViewerPrivate{
  AtNiftiImage        * nifti;
  AtImageViewer       * viewers[3];
  GtkWidget           * hbox;
  AtTrackbar          * trackbars[3];
  AtArrayU8           * slices[3];
  uint16_t              indices[3];
  AtSliceViewerLayout   layout;
}AtSliceViewerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtSliceViewer, at_sliceviewer, GTK_TYPE_BIN)

static void
at_sliceviewer_trackbar_cb(AtTrackbar* t, double value, void* user_data){
  AtSliceViewer       * self = AT_SLICEVIEWER(user_data);
  AtSliceViewerPrivate* priv = at_sliceviewer_get_instance_private(self);
  uint8_t i;
  for(i = 0; i < 3; i++){
    if(priv->trackbars[i] == t){
      at_sliceviewer_set_index(self,i,min((uint16_t)value,priv->nifti->ar->h.shape[i]-1));
      break;
    }
  }
}

static void
at_sliceviewer_init(AtSliceViewer *self){
  AtSliceViewerPrivate*priv = at_sliceviewer_get_instance_private(self);
  uint8_t i;
  memset(priv,0,sizeof(priv));
  priv->hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  for(i = 0; i < 3; i++){
    priv->viewers[i] = at_imageviewer_new();
    priv->trackbars[i] = at_trackbar_new();
    gtk_box_pack_start(GTK_BOX(priv->hbox),GTK_WIDGET(priv->viewers[i]),FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(priv->hbox),GTK_WIDGET(priv->trackbars[i]),FALSE,FALSE,0);
    at_trackbar_on_change_data(priv->trackbars[i],at_sliceviewer_trackbar_cb,self);
    at_trackbar_set_name(priv->trackbars[i],tracknames[i]);
  }
  gtk_container_add(GTK_CONTAINER(self),priv->hbox);
}

static void
at_sliceviewer_size_allocate(GtkWidget* widget, GtkAllocation* allocation){
  GTK_WIDGET_CLASS(at_sliceviewer_parent_class)->size_allocate(widget,allocation);
  gtk_widget_set_allocation(gtk_bin_get_child(GTK_BIN(widget)), allocation);
}

static void
at_sliceviewer_class_init(AtSliceViewerClass *klass){
  GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
  widget_class->size_allocate = at_sliceviewer_size_allocate;
}

static void
at_sliceviewer_get_slice_size(AtNiftiImage* nifti, uint16_t i, uint64_t size[]){
  uint16_t j,k;
  for(j = 0, k = 0; j < 3; j++){
    if(i != j) size[k++] = nifti->ar->h.shape[i];
  }
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtSliceViewer*
at_sliceviewer_new(){
  return g_object_new(AT_TYPE_SLICEVIEWER, NULL);
}

void
at_sliceviewer_set_layout(AtSliceViewer* viewer, AtSliceViewerLayout layout){
  AtSliceViewerPrivate* priv = at_sliceviewer_get_instance_private(viewer);
  priv->layout = layout;
  // Update visually
}

void
at_sliceviewer_set_nifti(AtSliceViewer* viewer, AtNiftiImage* nifti){
  AtSliceViewerPrivate* priv = at_sliceviewer_get_instance_private(viewer);
  priv->nifti = nifti;
  // Get slices
  uint16_t i;
  for(i = 0; i < 3; i++){
    at_sliceviewer_set_index(viewer,i,priv->indices[i]);
    at_trackbar_set_min(priv->trackbars[i],0);
    at_trackbar_set_max(priv->trackbars[i],nifti->ar->h.shape[i]-1);
  }
}

void
at_sliceviewer_set_index(AtSliceViewer* viewer, AtSliceType type, uint16_t index){
  // We need to save the index for this slice, the slice itself and our viewer
  // which shows the new slice
  AtSliceViewerPrivate* priv = at_sliceviewer_get_instance_private(viewer);
  // index
  priv->indices[type] = index;
  // slice
  AtRange ranges[3]   = {AT_RANGE_ALL,AT_RANGE_ALL,AT_RANGE_ALL};
  ranges[type]        = at_range_at(index);
  at_arrayu16_sub_u8(priv->nifti->ar,ranges,&priv->slices[type]);
  at_arrayu8_squeeze(priv->slices[type]);
  at_imageviewer_set(priv->viewers[type],priv->slices[type]);
  at_trackbar_set_pos(priv->trackbars[type],index);
  // viewer
  gtk_widget_queue_draw(GTK_WIDGET(priv->viewers[type]));
}

void
at_sliceviewer_rotate_cw(AtSliceViewer* viewer, AtSliceType type){

}

void
at_sliceviewer_rotate_ccw(AtSliceViewer* viewer, AtSliceType type){

}
