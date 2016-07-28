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
#include <cairo.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/


typedef struct AtImageViewerPrivate{
  AtArrayU8      * array;
  cairo_surface_t* surface;
  GdkCursor      * cursor_press;
  GdkCursor      * cursor_default;
  void           * mousecallbackdata;
  AtMouseCallback  mousecallback; // pointer to user mouse callback function
  double           zoom;
  AtMouseEvent     mouseevent;
  int16_t          offset[2];     // non-zero when moving image
  int16_t          position[2];   // position without offset
  int16_t          ar_pos[2];     // mouse position in array coordinates
  int16_t          center[2];     // (0,0)
  int16_t          last[2];       //
  uint8_t          space_pressed;
}AtImageViewerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtImageViewer, at_imageviewer, GTK_TYPE_WIDGET)


static gboolean
at_imageviewer_draw(GtkWidget* imgv, cairo_t* cr){
  AtImageViewerPrivate *priv = at_imageviewer_get_instance_private(AT_IMAGEVIEWER(imgv));
  if(priv->array){
    cairo_scale(cr,priv->zoom,priv->zoom);
    cairo_set_source_surface(cr,priv->surface,(priv->position[0] + priv->offset[0])/priv->zoom,(priv->position[1] + priv->offset[1])/priv->zoom);
    cairo_pattern_set_filter(cairo_get_source(cr),CAIRO_FILTER_FAST);
  }else{
    cairo_set_source_rgb(cr,1,0,0);
  }
  cairo_paint (cr);
  return false;
}

static void
at_imageviewer_realize(GtkWidget* widget){
  GtkAllocation         allocation;
  GdkWindowAttr         attributes;
  gint                  attributes_mask;
  //AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(AT_IMAGEVIEWER(widget));

  gtk_widget_get_allocation(widget, &allocation);

  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x           = allocation.x;
  attributes.y           = allocation.y;
  attributes.width       = allocation.width;
  attributes.height      = allocation.height;
  attributes.wclass      = GDK_INPUT_OUTPUT;
  attributes.visual      = gtk_widget_get_visual(widget);
  attributes.event_mask  = gtk_widget_get_events(widget) |
      GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK | GDK_FOCUS_CHANGE_MASK | GDK_ENTER_NOTIFY_MASK;
  attributes_mask        = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

  GdkWindow* window = gdk_window_new(gtk_widget_get_parent_window(widget),&attributes,attributes_mask);
  gtk_widget_set_window(widget,window);
  gtk_widget_register_window(widget,window);
  gtk_widget_set_realized(widget,TRUE);
  gtk_widget_set_can_focus(widget,true);

  AtImageViewer       * self = AT_IMAGEVIEWER(widget);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->cursor_press   = gdk_cursor_new_from_name(gtk_widget_get_display(widget),"grabbing");
  priv->cursor_default = gdk_cursor_new_from_name(gtk_widget_get_display(widget),"default");
}

static void
at_imageviewer_get_size(AtImageViewer* self, GtkOrientation direction,int* minimal, int* natural){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  int size_min;
  uint8_t index;

  switch(direction){
  case GTK_ORIENTATION_HORIZONTAL:
    index = 1;size_min = 320;break;
  case GTK_ORIENTATION_VERTICAL:
    index = 0;size_min = 240;break;
  }

  // Minimal size
  *minimal = size_min;

  // Natural size
  if(priv->array != NULL)
    *natural = *minimal < priv->array->h.shape[index]?priv->array->h.shape[index]:*minimal;
  else
    *natural = *minimal;
}

static void
at_imageviewer_get_preferred_width(GtkWidget* widget, gint *minimal_width, gint *natural_width){
  at_imageviewer_get_size(AT_IMAGEVIEWER(widget),GTK_ORIENTATION_HORIZONTAL,minimal_width,natural_width);
}
static void
at_imageviewer_get_preferred_height(GtkWidget* widget, gint *minimal_height, gint *natural_height){
  at_imageviewer_get_size(AT_IMAGEVIEWER(widget),GTK_ORIENTATION_VERTICAL,minimal_height,natural_height);
}

static gboolean
at_imageviewer_scroll_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageViewer* self = AT_IMAGEVIEWER(widget);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  GdkEventScroll* sevent = (GdkEventScroll*)event;
  switch(sevent->direction){
    case GDK_SCROLL_DOWN:
      at_imageviewer_zoom_out_at(self,sevent->x,sevent->y);
    break;
    case GDK_SCROLL_UP:
      at_imageviewer_zoom_in_at(self,sevent->x,sevent->y);
    break;
  }
}

static gboolean
at_imageviewer_key_press_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageViewer       * self = AT_IMAGEVIEWER(widget);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);

  GdkEventKey* eventkey = (GdkEventKey*)event;
  switch(eventkey->keyval){
    case GDK_KEY_space:
      priv->space_pressed = true;
      gdk_window_set_cursor(gtk_widget_get_window(widget),priv->cursor_press);
      return true;
    break;
  }
  return false;
}
static gboolean
at_imageviewer_key_release_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageViewer       * self = AT_IMAGEVIEWER(widget);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);

  GdkEventKey* eventkey = (GdkEventKey*)event;
  switch(eventkey->keyval){
    case GDK_KEY_space:
      priv->space_pressed = false;
      priv->position[0] += priv->offset[0];
      priv->position[1] += priv->offset[1];
      priv->offset[0] = 0;
      priv->offset[1] = 0;
      gdk_window_set_cursor(gtk_widget_get_window(widget),priv->cursor_default);
      return true;
    break;
  }
  return false;
}

static gboolean
at_imageviewer_button_press_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageViewer       * self        = AT_IMAGEVIEWER(widget);
  AtImageViewerPrivate* priv        = at_imageviewer_get_instance_private(self);
  GdkEventButton      * eventbutton = (GdkEventButton*) event;
  if(!priv->space_pressed){
    priv->mouseevent.type = (eventbutton->button == 3)?AT_MOUSE_RIGHT_DOWN:AT_MOUSE_LEFT_DOWN;
    at_imageviewer_get_array_pos(self,priv->ar_pos,eventbutton->x,eventbutton->y);
    priv->mouseevent.x    = priv->ar_pos[0];
    priv->mouseevent.y    = priv->ar_pos[1];
    priv->mousecallback(&priv->mouseevent,priv->mousecallbackdata);
  }
  return priv->space_pressed;
}

static gboolean
at_imageviewer_button_release_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageViewer       * self        = AT_IMAGEVIEWER(widget);
  AtImageViewerPrivate* priv        = at_imageviewer_get_instance_private(self);
  GdkEventButton      * eventbutton = (GdkEventButton*) event;
  if(!priv->space_pressed){
    priv->mouseevent.type = (eventbutton->button == 3)?AT_MOUSE_RIGHT_UP:AT_MOUSE_LEFT_UP;
    at_imageviewer_get_array_pos(self,priv->ar_pos,eventbutton->x,eventbutton->y);
    priv->mouseevent.x    = priv->ar_pos[0];
    priv->mouseevent.y    = priv->ar_pos[1];
    priv->mousecallback(&priv->mouseevent,priv->mousecallbackdata);
  }
  return priv->space_pressed;
}

static gboolean
at_imageviewer_motion_notify_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageViewer       * self = AT_IMAGEVIEWER(widget);
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  GdkEventMotion* eventmotion = (GdkEventMotion*) event;
  if(priv->space_pressed){
    priv->offset[0] = eventmotion->x - priv->last[0];
    priv->offset[1] = eventmotion->y - priv->last[1];
    gtk_widget_queue_draw(widget);
    return true;
  }else{
    priv->last[0] = eventmotion->x;
    priv->last[1] = eventmotion->y;
    priv->mouseevent.type = AT_MOUSE_MOVE;
    at_imageviewer_get_array_pos(self,priv->ar_pos,eventmotion->x,eventmotion->y);
    priv->mouseevent.x    = priv->ar_pos[0];
    priv->mouseevent.y    = priv->ar_pos[1];
    if(priv->mousecallback) priv->mousecallback(&priv->mouseevent,priv->mousecallbackdata);
  }
  return false;
}

static gboolean
at_imageviewer_enter_notify_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  gtk_widget_grab_focus(widget);
}

static void
at_imageviewer_init(AtImageViewer *self){
  AtImageViewerPrivate *priv = at_imageviewer_get_instance_private(self);
  memset(priv,0,sizeof(priv));
  priv->zoom = 1;
  g_signal_connect(GTK_WIDGET(self),"scroll-event"        ,G_CALLBACK(at_imageviewer_scroll_event)        ,NULL);
  g_signal_connect(GTK_WIDGET(self),"key-press-event"     ,G_CALLBACK(at_imageviewer_key_press_event)     ,NULL);
  g_signal_connect(GTK_WIDGET(self),"key-release-event"   ,G_CALLBACK(at_imageviewer_key_release_event)   ,NULL);
  g_signal_connect(GTK_WIDGET(self),"button-press-event"  ,G_CALLBACK(at_imageviewer_button_press_event)  ,NULL);
  g_signal_connect(GTK_WIDGET(self),"button-release-event",G_CALLBACK(at_imageviewer_button_release_event),NULL);
  g_signal_connect(GTK_WIDGET(self),"motion-notify-event" ,G_CALLBACK(at_imageviewer_motion_notify_event) ,NULL);
  g_signal_connect(GTK_WIDGET(self),"enter-notify-event"  ,G_CALLBACK(at_imageviewer_enter_notify_event) ,NULL);
  g_signal_connect(GTK_WIDGET(self),"enter-notify-event"  ,G_CALLBACK(at_imageviewer_enter_notify_event) ,NULL);
}

static void
at_imageviewer_class_init(AtImageViewerClass *klass){
  GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
  widget_class->draw = at_imageviewer_draw;
  widget_class->realize = at_imageviewer_realize;
  widget_class->get_preferred_width = at_imageviewer_get_preferred_width;
  widget_class->get_preferred_height = at_imageviewer_get_preferred_height;
}

static void
at_imageviewer_center_image(AtImageViewer* self){

}
/*=============================================================================
 PUBLIC API
 ============================================================================*/

AtImageViewer*
at_imageviewer_new(){
  return g_object_new(AT_TYPE_IMAGEVIEWER, NULL);
}

void
at_imageviewer_set(AtImageViewer* imgv, AtArrayU8* array){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  AtColorType colortype;
  if(array->h.dim == 2 || array->h.shape[2] == 1)
    colortype = AT_GRAY;
  else if(array->h.dim == 3){
    if(array->h.shape[2] == 3) colortype = AT_RGB;
    else colortype = AT_RGBA;
  }

  priv->array   = at_arrayu8_cvt_color(array, colortype, AT_BGRA);
  int stride    = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32,priv->array->h.shape[1]);
  if(priv->surface) cairo_surface_destroy(priv->surface);
  priv->surface = cairo_image_surface_create_for_data(priv->array->data,CAIRO_FORMAT_ARGB32,priv->array->h.shape[1],priv->array->h.shape[0],stride);
  //gtk_widget_queue_draw(GTK_WIDGET(imgv));
  gtk_widget_queue_resize(GTK_WIDGET(imgv));
}

AtArrayU8*
at_imageviewer_get_array(AtImageViewer* imgv){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  return priv->array;
}

void
at_imageviewer_move(AtImageViewer *imgv, int16_t offsetx, int16_t offsety){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  priv->offset[0] += offsetx;
  priv->offset[1] += offsety;
  gtk_widget_queue_draw(GTK_WIDGET(imgv));
}

void
at_imageviewer_move_to(AtImageViewer *imgv, int16_t offsetx, int16_t offsety){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  priv->position[0] = offsetx;
  priv->position[1] = offsety;
  gtk_widget_queue_draw(GTK_WIDGET(imgv));
}

void
at_imageviewer_zoom_in(AtImageViewer *imgv){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  if(priv->zoom >= 1)
    priv->zoom++;
  else
    priv->zoom *= 2.0;
  gtk_widget_queue_draw(GTK_WIDGET(imgv));
}

void
at_imageviewer_zoom_in_at(AtImageViewer *imgv, int16_t cx, int16_t cy){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  double oldzoom = priv->zoom;
  at_imageviewer_zoom_in(imgv);
  priv->position[0] = priv->zoom/oldzoom*(priv->position[0]-cx)+cx;
  priv->position[1] = priv->zoom/oldzoom*(priv->position[1]-cx)+cx;
}

void
at_imageviewer_zoom_out(AtImageViewer *imgv){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  if(priv->zoom > 1)
    priv->zoom--;
  else if(priv->zoom > 0.04){
    priv->zoom /= 2.0;
  }
  gtk_widget_queue_draw(GTK_WIDGET(imgv));
}

void
at_imageviewer_zoom_out_at(AtImageViewer *imgv, int16_t cx, int16_t cy){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  double oldzoom = priv->zoom;
  at_imageviewer_zoom_out(imgv);
  priv->position[0] = priv->zoom/oldzoom*(priv->position[0]-cx)+cx;
  priv->position[1] = priv->zoom/oldzoom*(priv->position[1]-cx)+cx;
}

void
at_imageviewer_zoom_reset(AtImageViewer *imgv){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  priv->zoom = 1;
  priv->position[0] = 0;
  priv->position[1] = 0;
  priv->offset[0] = 0;
  priv->offset[1] = 0;
  gtk_widget_queue_draw(GTK_WIDGET(imgv));
}

void
at_imageviewer_zoom_fit(AtImageViewer *imgv){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  gint width, height;
  GtkAllocation alloc;
  GtkRequisition reqm, reqn;
  int baseline;
  gtk_widget_get_allocated_size(GTK_WIDGET(imgv), &alloc, &baseline);
  gtk_widget_get_preferred_size(GTK_WIDGET(imgv), &reqm, &reqn);
  priv->zoom = min((double)alloc.height/(double)priv->array->h.shape[0],(double)alloc.width/(double)priv->array->h.shape[1]);
  gtk_widget_queue_draw(GTK_WIDGET(imgv));
}

gboolean
at_imageviewer_get_pixel(AtImageViewer* imgv, uint8_t* pixel,uint16_t x, uint16_t y){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  int16_t cx = (x - priv->position[0])/priv->zoom;
  int16_t cy = (y - priv->position[1])/priv->zoom;
  uint64_t offset;
  if(cx >= 0 && cy >= 0 && cx < priv->array->h.shape[1] && cy < priv->array->h.shape[0]){
    offset = cy * priv->array->h.step[0] + cx * priv->array->h.step[1];
    memcpy(pixel,&priv->array->data[offset],priv->array->h.shape[2] * sizeof(uint8_t));
    return true;
  }
  return false;
}
gboolean
at_imageviewer_get_array_pos(AtImageViewer* imgv, int16_t *pos,uint16_t x, uint16_t y){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(imgv);
  pos[0] = (x - priv->position[0])/priv->zoom;
  pos[1] = (y - priv->position[1])/priv->zoom;
  return pos[0] >= 0 && pos[1] >= 0 && pos[0] < priv->array->h.shape[1] && pos[1] < priv->array->h.shape[0];
}

void
at_imageviewer_set_mouse_callback(AtImageViewer* self, AtMouseCallback mouse_callback, void* user_data){
  AtImageViewerPrivate* priv = at_imageviewer_get_instance_private(self);
  priv->mousecallback        = mouse_callback;
  priv->mousecallbackdata    = user_data;
}
