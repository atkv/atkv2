#include <at/chart.h>
#include <math.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
typedef struct _AtChartViewerPrivate{
  AtChart* chart;
}AtChartViewerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtChartViewer, at_chartviewer, GTK_TYPE_WIDGET)

static gboolean
at_chartviewer_draw(GtkWidget* widget, cairo_t* cr){
  AtChartViewer       * self    = AT_CHARTVIEWER(widget);
  AtChartViewerPrivate* priv    = at_chartviewer_get_instance_private(self);
  GtkAllocation         allocation;
  AtVec4D64             rect;
  gtk_widget_get_allocated_size(widget,&allocation,NULL);
  rect.x = 0;rect.width = allocation.width;
  rect.y = 0;rect.height = allocation.height;
  at_chart_render(cr,priv->chart,rect);
  return false;
}

static void
at_chartviewer_realize(GtkWidget* widget){
  GtkAllocation         allocation;
  GdkWindowAttr         attributes;
  gint                  attributes_mask;
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

}

static void
at_chartviewer_get_preferred_width(GtkWidget* widget, gint *minimal_width, gint *natural_width){
  *minimal_width = 100;
  *natural_width = 100;
}

static void
at_chartviewer_get_preferred_height(GtkWidget* widget, gint *minimal_height, gint *natural_height){
  *minimal_height = 0;
  *natural_height = 0;
}

static void
at_chartviewer_init(AtChartViewer *self){

}

static void
at_chartviewer_class_init(AtChartViewerClass *klass){
  GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
  widget_class->draw                 = at_chartviewer_draw;
  widget_class->realize              = at_chartviewer_realize;
  widget_class->get_preferred_width  = at_chartviewer_get_preferred_width;
  widget_class->get_preferred_height = at_chartviewer_get_preferred_height;
}


/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtChartViewer*
at_chartviewer_new(){
  return g_object_new(AT_TYPE_CHARTVIEWER, NULL);
}

void
at_chartviewer_set(AtChartViewer* self, AtChart* chart){
  AtChartViewerPrivate*priv = at_chartviewer_get_instance_private(self);
  priv->chart = chart;
  gtk_widget_queue_draw(GTK_WIDGET(self));
}

void
at_chartviewer_write(AtChartViewer* self, const char *name){
  AtChartViewerPrivate*priv = at_chartviewer_get_instance_private(self);
  GtkAllocation allocation;
  gtk_widget_get_allocated_size(GTK_WIDGET(self),&allocation,NULL);
  at_chart_write(priv->chart,name,allocation.width,allocation.height);
}
