#include <at/gui.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/

typedef struct AtTrackbarPrivate{
  double           * variable;
  const char       * name;
  GtkWidget        * scale;
  GtkWidget        * hbox;
  GtkWidget        * lbl_min;
  GtkWidget        * lbl_max;
  GtkWidget        * lbl_name;
  AtTrackbarFunc     cb;
  AtTrackbarDataFunc cbd;
  void*              cbd_data;
  double             vmin;
  double             vmax;
}AtTrackbarPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtTrackbar, at_trackbar, GTK_TYPE_BOX)

static gboolean
at_trackbar_changed(GtkRange* range, GtkScrollType type, gdouble value, gpointer user_data){
  AtTrackbar* t = AT_TRACKBAR(user_data);
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  if(priv->variable) *(priv->variable) = value;
  if(priv->cb)       priv->cb(t, min(priv->vmax,value));
  if(priv->cbd)      priv->cbd(t, min(priv->vmax,value),priv->cbd_data);
  return false;
}


static void
at_trackbar_init(AtTrackbar *self){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(self);
  memset(priv,0,sizeof(priv));
  priv->scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL,NULL);
  priv->hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  priv->lbl_name = gtk_label_new("variable");
  gtk_scale_set_digits(GTK_SCALE(priv->scale),0);
  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->lbl_name,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->scale,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(self),priv->hbox,TRUE,TRUE,0);
  g_signal_connect(priv->scale,"change-value",G_CALLBACK(at_trackbar_changed),self);
}

static void
at_trackbar_class_init(AtTrackbarClass *klass){

}


/*=============================================================================
 PUBLIC API
 ============================================================================*/

AtTrackbar*
at_trackbar_new(){
  return g_object_new(AT_TYPE_TRACKBAR, NULL);
}

AtTrackbar*
at_trackbar_new_named(const char* name){
  AtTrackbar* trackbar = at_trackbar_new();
  at_trackbar_set_name(trackbar, name);
  return trackbar;
}

void
at_trackbar_set_min(AtTrackbar* t, double minv){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  priv->vmin = minv;
  GtkAdjustment* adjustment = gtk_range_get_adjustment(GTK_RANGE(priv->scale));
  gtk_adjustment_set_lower(adjustment,minv);
  gtk_range_set_adjustment(GTK_RANGE(priv->scale),adjustment);
}

void
at_trackbar_set_max(AtTrackbar* t, double maxv){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  priv->vmax = maxv;
  GtkAdjustment* adjustment = gtk_range_get_adjustment(GTK_RANGE(priv->scale));
  gtk_adjustment_set_upper(adjustment,maxv);
  gtk_range_set_adjustment(GTK_RANGE(priv->scale),adjustment);
}

void
at_trackbar_set_pos(AtTrackbar* t, double pos){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  gtk_range_set_value(GTK_RANGE(priv->scale), pos);
}

void
at_trackbar_set_name(AtTrackbar* t, const char* name){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  priv->name = name;
  gtk_label_set_label(GTK_LABEL(priv->lbl_name),name);
}
void
at_trackbar_set_step(AtTrackbar* t, double step, double page){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  gtk_range_set_increments(GTK_RANGE(priv->scale), step, page);
}

void
at_trackbar_set_variable(AtTrackbar* t, double* variable){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  priv->variable = variable;
}


double
at_trackbar_get_min(AtTrackbar *t){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  return priv->vmin;
}

double
at_trackbar_get_max(AtTrackbar *t){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  return priv->vmax;
}

double
at_trackbar_get_pos(AtTrackbar *t){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  gtk_range_get_value(GTK_RANGE(priv->scale));
}

const char*
at_trackbar_get_name(AtTrackbar* t){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  return priv->name;
}

void
at_trackbar_on_change(AtTrackbar* t, AtTrackbarFunc cb){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  priv->cb = cb;
}

void
at_trackbar_on_change_data(AtTrackbar* t, AtTrackbarDataFunc cbd, void* data){
  AtTrackbarPrivate* priv = at_trackbar_get_instance_private(t);
  priv->cbd = cbd;
  priv->cbd_data = data;
}
