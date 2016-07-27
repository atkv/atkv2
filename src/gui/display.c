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

static GHashTable* hash;
static GtkApplication* app;
static AtKey lastKey;

static void
at_display_activate(GtkApplication* app, gpointer user_data){

}

static void __attribute__((constructor))
at_display_init(){
  hash = g_hash_table_new(g_str_hash,g_str_equal);
  app  = gtk_application_new("io.github.atkv",G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(at_display_activate), NULL);
  g_application_run(G_APPLICATION(app), 0, NULL);
}

static void __attribute__((destructor))
at_display_fini(){
  g_hash_table_destroy(hash);
  g_object_unref(app);
}

static gboolean
at_display_key_press_event(GtkWidget* widget,
                           GdkEvent * event,
                           gpointer   user_data){
  GdkEventKey* eventkey = (GdkEventKey*) event;
  switch(eventkey->keyval){
  case GDK_KEY_A:
  case GDK_KEY_a:
    lastKey = AT_KEY_A;break;
  case GDK_KEY_Escape:
    lastKey = AT_KEY_ESCAPE;break;
  }

  if(eventkey->keyval != GDK_KEY_space &&
     eventkey->keyval != GDK_KEY_Super_L &&
     eventkey->keyval != GDK_KEY_Super_R &&
     eventkey->keyval != GDK_KEY_Control_L &&
     eventkey->keyval != GDK_KEY_Control_R &&
     eventkey->keyval != GDK_KEY_Shift_L &&
     eventkey->keyval != GDK_KEY_Shift_R &&
     eventkey->keyval != GDK_KEY_Alt_L
   )
    gtk_main_quit();
  return false;
}

static gboolean
at_display_key_release_event(GtkWidget* widget,
                             GdkEvent * event,
                             gpointer   user_data){
  return false;
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/


const char*
at_display_show_image(AtImageWindow *window, AtArrayU8 *image){
  at_imagewindow_set(window,image);
  gtk_widget_show_all(GTK_WIDGET(window));
}

AtImageWindow*
at_display_show_image_by_name(const char *name, AtArrayU8 *image){
  AtImageWindow* imagewindow = at_display_imagewindow(name);
  at_imagewindow_set(imagewindow,image);
  gtk_widget_show_all(GTK_WIDGET(imagewindow));
  return imagewindow;
}

AtKey
at_display_wait_key(){
  gtk_main();
  return lastKey;
}

AtKey
at_display_wait_key_until(uint32_t miliseconds){

}

AtImageWindow*
at_display_imagewindow(const char *name){
  AtImageWindow* imagewindow;
  if(!g_hash_table_contains(hash,name)){
    imagewindow = at_imagewindow_new();
    g_signal_connect(GTK_WIDGET(imagewindow),"key-press-event"  , G_CALLBACK(at_display_key_press_event), NULL);
    g_signal_connect(GTK_WIDGET(imagewindow),"key-release-event", G_CALLBACK(at_display_key_release_event), NULL);
    g_hash_table_insert(hash,(gpointer)name,imagewindow);
  }else{
    imagewindow = g_hash_table_lookup(hash,name);
  }
  return imagewindow;
}
void
at_display_set_mouse_callback(AtImageWindow* window, AtMouseCallback mouse_callback, void* user_data){
  at_imagewindow_set_mouse_callback(window, mouse_callback, user_data);
}


AtTrackbar*
at_display_add_trackbar(AtImageWindow *window, const char *trackname, double *variable, double vmin, double vmax){
  return at_imagewindow_add_trackbar(window, trackname, variable, vmin, vmax);
}

void
at_display_remove_trackbar(AtImageWindow *window, const char *trackname){
  at_imagewindow_remove_trackbar(window, trackname);
}
