#include <at/gui/imagewindow.h>
#include <at/core/version.h>
#include <at/core/i18n-lib.h>
#include <at/imgproc/img_io.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/

typedef struct AtImageWindowPrivate{
  AtImageViewer* imgv;
  GtkWidget    * toolbar;
  GtkWidget    * btn_zoomin;
  GtkWidget    * btn_zoomout;
  GtkWidget    * btn_zoomori;
  GtkWidget    * btn_zoomfit;
  GtkWidget    * btn_left;
  GtkWidget    * btn_right;
  GtkWidget    * btn_top;
  GtkWidget    * btn_bottom;
  GtkWidget    * btn_about;
  GtkWidget    * btn_save;
  GtkWidget    * btn_help;
  GtkWidget    * box_main;
  GtkWidget    * statusbar;
  GtkWidget    * lbl_color;
  GdkPixbuf    * logo;
  guint          statusctx;
  char           textposition[8];
  char           textcolor[128];
}AtImageWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtImageWindow, at_imagewindow, GTK_TYPE_WINDOW)

static void
at_imagewindow_class_init(AtImageWindowClass *klass){

}

static void
at_btn_save_clicked(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  GtkWidget* dialog = gtk_file_chooser_dialog_new("Save image",
                                                  GTK_WINDOW(window),
                                                  GTK_FILE_CHOOSER_ACTION_SAVE,
                                                  _("_Cancel"),GTK_RESPONSE_CANCEL,
                                                  _("_Save"),  GTK_RESPONSE_ACCEPT,
                                                  NULL);
  GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
  gtk_file_chooser_set_do_overwrite_confirmation(chooser,true);
  char          * filename;
  AtError       * error = NULL;
  gint            res   = gtk_dialog_run(GTK_DIALOG(dialog));
  if(res == GTK_RESPONSE_ACCEPT){
    filename = gtk_file_chooser_get_filename(chooser);
    at_arrayu8_write_png(at_imageviewer_get_array(priv->imgv),filename,&error);
  }
  gtk_widget_destroy(dialog);
}

static void
at_btn_zoomin_clicked(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  at_imageviewer_zoom_in(priv->imgv);
}

static void
at_btn_zoomout_clicked(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  at_imageviewer_zoom_out(priv->imgv);
}

static void
at_btn_zoomori_clicked(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  at_imageviewer_zoom_reset(priv->imgv);
}

static void
at_btn_zoomfit_clicked(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  at_imageviewer_zoom_fit(priv->imgv);
}

static gboolean
at_btn_left_pressed(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  at_imageviewer_move(priv->imgv,-5,0);
  return true;
}

static gboolean
at_btn_right_pressed(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  at_imageviewer_move(priv->imgv,5,0);
  printf("pressed\n");
  return false;
}

static gboolean
at_btn_bottom_pressed(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  at_imageviewer_move(priv->imgv,0,5);
  return true;
}

static gboolean
at_btn_top_pressed(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  at_imageviewer_move(priv->imgv,0,-5);
  return true;
}

static void
at_btn_about_clicked(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow* window = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate*priv=at_imagewindow_get_instance_private(window);
  g_auto(GStrv) authors   = g_strsplit("Anderson Tavares:Paulo Miranda", ":", -1);
  char path[] = AT_RESOURCE_PATH "/logo.png";
  gtk_show_about_dialog(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                        "program-name" ,"@kv",
                        "version"      ,AT_VERSION_STRING,
                        "website"      ,AT_WEBSITE,
                        "website-label",AT_WEBSITE,
                        "logo",priv->logo,
                        "title",_("About atkv"),
                        "copyright","All rights reserved (C) 2016",
                        "comments","Library for Graph/Computer Vision Problems",
                        "authors",authors,
                        "license-type",GTK_LICENSE_GPL_3_0,
                        NULL);
}
static void
at_btn_help_clicked(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_CLOSE,"Shortcuts:\n -[Space]+[Mouse Move]: Move Image\n -[Scrolling Wheel]: Zoom");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

static gboolean
at_imagewindow_viewer_motion_event(GtkWidget* widget, GdkEvent* event, gpointer user_data){
  AtImageWindow        * window      = AT_IMAGEWINDOW(gtk_widget_get_toplevel(widget));
  AtImageWindowPrivate * priv        = at_imagewindow_get_instance_private(window);
  GdkEventMotion       * eventmotion = (GdkEventMotion*) event;
  AtArrayU8            * array       = at_imageviewer_get_array(priv->imgv);
  int16_t                pos[2];
  uint8_t                pixel[3];

  if(at_imageviewer_get_pixel(priv->imgv,pixel,eventmotion->x, eventmotion->y)){
    if(array->h.dim == 2 || array->h.shape[2] == 1)
      sprintf(priv->textcolor,"(G = %03d)", pixel[0]);
    else
      sprintf(priv->textcolor,"(<span color=\"red\">R=%03d</span>, <span color=\"green\">G=%03d</span>, <span color=\"blue\">B=%03d</span>)", pixel[2],pixel[1],pixel[0]);
    gtk_label_set_label(GTK_LABEL(priv->lbl_color),priv->textcolor);
    at_imageviewer_get_array_pos(priv->imgv,pos,eventmotion->x,eventmotion->y);
    sprintf(priv->textposition,"(%d,%d)",pos[0],pos[1]);
    gtk_statusbar_push(GTK_STATUSBAR(priv->statusbar), priv->statusctx, priv->textposition);
  }
  return false;
}

static void
at_imagewindow_init(AtImageWindow *self){
  AtImageWindowPrivate* priv = at_imagewindow_get_instance_private(self);
  priv->imgv                 = at_imageviewer_new();
  priv->box_main             = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  priv->toolbar              = gtk_toolbar_new();
  priv->statusbar            = gtk_statusbar_new();
  priv->btn_left             = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_right            = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_top              = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_bottom           = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_zoomin           = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_zoomout          = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_zoomori          = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_zoomfit          = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_save             = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_about            = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->btn_help             = GTK_WIDGET(gtk_tool_button_new(NULL,NULL));
  priv->lbl_color            = gtk_label_new("");

  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_save)     ,"document-save");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoomin)   ,"zoom-in");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoomout)  ,"zoom-out");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoomori)  ,"zoom-original");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_zoomfit)  ,"zoom-fit-best");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_left)     ,"go-previous");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_right)    ,"go-next");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_top)      ,"go-up");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_bottom)   ,"go-down");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_about)    ,"help-about");
  gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(priv->btn_help)    ,"dialog-information");

  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_save)      ,"Save image");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_zoomin)    ,"Zoom In");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_zoomout)   ,"Zoom Out");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_zoomori)   ,"Zoom Original");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_zoomfit)   ,"Fit to Screen");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_left)      ,"Pan Left");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_right)     ,"Pan Right");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_bottom)    ,"Pan Bottom");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_top)       ,"Pan Top");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_about)     ,"About");
  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(priv->btn_help)      ,"Help");

  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_save)         ,"Save image");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_zoomin)       ,"Zoom In");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_zoomout)      ,"Zoom Out");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_zoomori)      ,"Zoom Original");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_zoomfit)      ,"Fit to Screen");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_left)         ,"Pan Left");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_right)        ,"Pan Right");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_bottom)       ,"Pan Bottom");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_top)          ,"Pan Top");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_about)        ,"About");
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(priv->btn_help)         ,"Help");

  gtk_label_set_markup(GTK_LABEL(priv->lbl_color), "");

  //gtk_widget_add_events(GTK_WIDGET(self),            GDK_BUTTON_PRESS_MASK);
  //gtk_widget_add_events(GTK_WIDGET(priv->imgv),      GDK_POINTER_MOTION_MASK);
  //gtk_widget_add_events(GTK_WIDGET(priv->toolbar),   GDK_BUTTON_PRESS_MASK);
  //gtk_widget_add_events(GTK_WIDGET(priv->btn_left),  GDK_BUTTON_PRESS_MASK);
  gtk_widget_add_events(GTK_WIDGET(priv->btn_right), GDK_BUTTON_PRESS_MASK);
  //gtk_widget_add_events(GTK_WIDGET(priv->btn_top),   GDK_BUTTON_PRESS_MASK);
  //gtk_widget_add_events(GTK_WIDGET(priv->btn_bottom),GDK_BUTTON_PRESS_MASK);
  g_signal_connect(GTK_WIDGET(priv->imgv), "motion-notify-event", G_CALLBACK(at_imagewindow_viewer_motion_event),self);
  //g_signal_connect(GTK_WIDGET(priv->imgv), "realize", G_CALLBACK(at_imagewindow_viewer_realize_event),self);

  g_signal_connect(priv->btn_save,   "clicked", G_CALLBACK(at_btn_save_clicked),self);
  g_signal_connect(priv->btn_zoomin, "clicked", G_CALLBACK(at_btn_zoomin_clicked),self);
  g_signal_connect(priv->btn_zoomout,"clicked", G_CALLBACK(at_btn_zoomout_clicked),self);
  g_signal_connect(priv->btn_zoomori,"clicked", G_CALLBACK(at_btn_zoomori_clicked),self);
  g_signal_connect(priv->btn_zoomfit,"clicked", G_CALLBACK(at_btn_zoomfit_clicked),self);
  g_signal_connect(priv->btn_left,   "button-press-event", G_CALLBACK(at_btn_left_pressed),self);
  g_signal_connect(priv->btn_right,  "button-press-event", G_CALLBACK(at_btn_right_pressed),self);
  g_signal_connect(priv->btn_bottom, "button-press-event", G_CALLBACK(at_btn_bottom_pressed),self);
  g_signal_connect(priv->btn_top,    "button-press-event", G_CALLBACK(at_btn_top_pressed),self);
  g_signal_connect(priv->btn_about,  "clicked", G_CALLBACK(at_btn_about_clicked),self);
  g_signal_connect(priv->btn_help,   "clicked", G_CALLBACK(at_btn_help_clicked),self);


  gtk_toolbar_set_icon_size(GTK_TOOLBAR(priv->toolbar),GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_save),0);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoomin),1);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoomout),2);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoomori),3);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_zoomfit),4);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_left),5);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_right),6);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_top),7);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_bottom),8);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_about),9);
  gtk_toolbar_insert(GTK_TOOLBAR(priv->toolbar),GTK_TOOL_ITEM(priv->btn_help),10);

  gtk_box_pack_end(GTK_BOX(priv->statusbar),priv->lbl_color,FALSE,FALSE,0);
  gtk_toolbar_set_icon_size(GTK_TOOLBAR(priv->toolbar),GTK_ICON_SIZE_SMALL_TOOLBAR);
  gtk_box_pack_start(GTK_BOX(priv->box_main),priv->toolbar,false,false,0);
  gtk_box_pack_start(GTK_BOX(priv->box_main),GTK_WIDGET(priv->imgv)   ,true ,true ,0);
  gtk_box_pack_end  (GTK_BOX(priv->box_main),GTK_WIDGET(priv->statusbar),false,false,0);
  gtk_container_add (GTK_CONTAINER(self)    ,priv->box_main);

  priv->logo = gdk_pixbuf_new_from_file(AT_RESOURCE_PATH G_DIR_SEPARATOR_S "logo.png", NULL);
  priv->statusctx = gtk_statusbar_get_context_id(GTK_STATUSBAR(priv->statusbar),"display_pixel_coord");
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtImageWindow*
at_imagewindow_new(){
  return g_object_new(AT_TYPE_IMAGEWINDOW, NULL);
}

void
at_imagewindow_set(AtImageWindow *window, AtArrayU8 *array){
  AtImageWindowPrivate* priv = at_imagewindow_get_instance_private(window);
  at_imageviewer_set(priv->imgv, array);
}

void
at_imagewindow_set_mouse_callback(AtImageWindow* window, AtMouseCallback mouse_callback, void* user_data){
  AtImageWindowPrivate* priv = at_imagewindow_get_instance_private(window);
  at_imageviewer_set_mouse_callback(priv->imgv, mouse_callback, user_data);
}
