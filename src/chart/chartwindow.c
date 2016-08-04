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

#include <at/chart.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
static GHashTable* windows;

typedef struct _AtChartWindowPrivate{
  AtChartViewer* viewer;
  GtkWidget    * box;
  GtkWidget    * toolbar;
  GtkToolItem  * btn_save;
  GtkToolItem  * btn_about;
  GtkToolItem  * btn_zoomin;
  GtkWidget    * btn_zoomout;
  GtkWidget    * btn_zoomori;
  GtkWidget    * btn_zoomfit;
  GtkWidget    * btn_left;
  GtkWidget    * statusbar;
}AtChartWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtChartWindow, at_chartwindow, GTK_TYPE_WINDOW)


__attribute__ ((constructor))
static void
init(){
  windows = g_hash_table_new(g_str_hash, g_str_equal);
}

static void
btn_save_clicked(GtkToolButton* toolbutton, gpointer user_data){
  AtChartWindow* self = AT_CHARTWINDOW(user_data);
  AtChartWindowPrivate* priv = at_chartwindow_get_instance_private(self);
  GtkWidget* dialog = gtk_file_chooser_dialog_new("Save chart",
                                                  GTK_WINDOW(self),
                                                  GTK_FILE_CHOOSER_ACTION_SAVE,
                                                  _("_Cancel"),GTK_RESPONSE_CANCEL,
                                                  _("_Save"),  GTK_RESPONSE_ACCEPT,
                                                  NULL);
  GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
  gtk_file_chooser_set_do_overwrite_confirmation(chooser,true);
  char          * filename;
  gint            res   = gtk_dialog_run(GTK_DIALOG(dialog));
  if(res == GTK_RESPONSE_ACCEPT){
    filename = gtk_file_chooser_get_filename(chooser);
    at_chartviewer_write_pdf(priv->viewer,filename);
  }
  gtk_widget_destroy(dialog);
}

static void
at_chartwindow_init(AtChartWindow *self){
  AtChartWindowPrivate* priv = at_chartwindow_get_instance_private(self);
  priv->viewer  = at_chartviewer_new();
  priv->toolbar = gtk_toolbar_new();
  priv->btn_zoomin = gtk_tool_button_new(NULL,NULL);
  priv->btn_save = gtk_tool_button_new(NULL,NULL);
  priv->btn_about = gtk_tool_button_new(NULL,NULL);
  priv->box     = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  priv->statusbar = gtk_statusbar_new();

  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoomin)   ,"zoom-in");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_save)     ,"document-save");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_about)     ,"help-about");
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_save),0);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoomin),1);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_about),2);
  gtk_toolbar_set_icon_size(GTK_TOOLBAR(priv->toolbar),GTK_ICON_SIZE_SMALL_TOOLBAR);

  g_signal_connect(priv->btn_save, "clicked", G_CALLBACK(btn_save_clicked),self);

  gtk_box_pack_start(GTK_BOX(priv->box),priv->toolbar,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(priv->box),GTK_WIDGET(priv->viewer),TRUE, TRUE, 0);
  gtk_box_pack_end(GTK_BOX(priv->box),priv->statusbar,FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(self),priv->box);
}

static void
at_chartwindow_class_init(AtChartWindowClass *klass){

}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtChartWindow*
at_chartwindow_new(){
  return g_object_new(AT_TYPE_CHARTWINDOW, "type", GTK_WINDOW_TOPLEVEL, NULL);
}

void
at_chartwindow_set(AtChartWindow* window, AtChart* chart){
  AtChartWindowPrivate* priv = at_chartwindow_get_instance_private(window);
  at_chartviewer_set(priv->viewer,chart);
}

AtChartWindow*
at_chart_show(const char* name, AtChart* chart){
  AtChartWindow* self = NULL;
  if(g_hash_table_contains(windows,name)){
    self = g_hash_table_lookup(windows,name);
  }else{
    self = at_chartwindow_new();
    g_hash_table_insert(windows,(gpointer)name,self);
    //g_signal_connect(self,"delete-event",G_CALLBACK(gtk_widget_hide_on_delete),NULL);
    g_signal_connect(self,"destroy",G_CALLBACK(gtk_main_quit),NULL);
  }

  at_chartwindow_set(self,chart);
  gtk_widget_show_all(GTK_WIDGET(self));
  gtk_main();
  return self;
}
