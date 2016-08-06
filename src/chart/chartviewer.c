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
  double              * values  = priv->chart->lineplots->values;
  double                spacing;
  double                title_margin_top;
  double                title_margin_bottom;
  double                title_font;
  double                axis_number_margin_left;
  double                axis_number_margin_right;
  double                axis_number_margin_top;
  double                axis_number_margin_bottom;
  double                vmax    = priv->chart->axis[1].vmax;
  double                vmin    = priv->chart->axis[1].vmin;
  uint64_t              nelem   = priv->chart->lineplots->nelem;
  uint64_t              i;
  GtkAllocation         allocation;
  gtk_widget_get_allocated_size(widget,&allocation,NULL);

  // Background

  cairo_set_source_rgb (cr,1,1,1);
  cairo_set_line_width(cr,1);
  cairo_paint(cr);

  // Foreground color
  cairo_set_source_rgb (cr,0,0,0);

  // Draw Title
  cairo_text_extents_t title_extents;
  title_extents.height = 0;

  title_margin_top    = min(allocation.width/100.0,10);
  title_margin_bottom = title_margin_top;
  title_font          = min(allocation.width/100.0*5,20);

  if(priv->chart->title){
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, title_font);
    cairo_text_extents(cr,priv->chart->title,&title_extents);
    cairo_move_to(cr,(allocation.width - title_extents.width)/2,title_extents.height+title_margin_top);
    cairo_show_text(cr,priv->chart->title);
  }

  // Draw Axis
  cairo_text_extents_t axis_number_extents;
  double axis_number_font = min(allocation.width/100.0*5,12);
  char axis_number_str[12];
  sprintf(axis_number_str,"%.2f",priv->chart->axis[1].vmax);
  cairo_set_font_size(cr, axis_number_font);
  cairo_text_extents(cr,axis_number_str,&axis_number_extents);

  double rect_size[4];
  axis_number_margin_left  = min(allocation.width/100.0,10);
  axis_number_margin_right = axis_number_margin_left;
  axis_number_margin_top   = min(allocation.height/100.0,10);
  axis_number_margin_bottom= axis_number_margin_top;
  rect_size[0] = axis_number_extents.width + axis_number_margin_left + axis_number_margin_right;
  rect_size[1] = title_margin_bottom+title_margin_top+title_extents.height;
  rect_size[2] = allocation.width-rect_size[0]-5-axis_number_extents.width;
  rect_size[3] = allocation.height-rect_size[1]-5-axis_number_extents.height-axis_number_margin_top-axis_number_margin_bottom;
  cairo_rectangle(cr, rect_size[0],rect_size[1],rect_size[2],rect_size[3]);
  //cairo_set_source_rgb(cr,1,1,1);
  cairo_stroke(cr);
  cairo_set_source_rgb(cr,0,0,0);

  double axis_number_cur;

  for(i = 0; i < 6; i++){
    // Vertical axis
    axis_number_cur = (priv->chart->axis[1].vmax-priv->chart->axis[1].vmin)/5*i;
    sprintf(axis_number_str,"%.2f",axis_number_cur+priv->chart->axis[1].vmin);
    cairo_move_to(cr,axis_number_margin_left,rect_size[1] + rect_size[3] - i*rect_size[3]/5 + axis_number_extents.height/2.0);
    cairo_show_text(cr,axis_number_str);

    // Horizontal axis
    axis_number_cur = (priv->chart->axis[0].vmax-priv->chart->axis[0].vmin)/5*i;
    sprintf(axis_number_str,"%.0f",axis_number_cur+priv->chart->axis[0].vmin);
    cairo_move_to(cr,rect_size[0]+i*rect_size[2]/5.0-axis_number_extents.width/2.0,rect_size[1]+rect_size[3]+axis_number_margin_top+axis_number_extents.height);
    cairo_show_text(cr,axis_number_str);
  }

  // Draw Data

  spacing = rect_size[2]/nelem;
  double x,y;
  double dashes[] = {10.0, 10.0};
  double dots[] = {2.0, 5.0};
  AtColor color = priv->chart->lineplots->linecolor;
  cairo_set_source_rgba(cr,color.r,color.g,color.b,color.a);
  x = rect_size[0];
  y = rect_size[3] + rect_size[1] - (values[0]-vmin)/(vmax-vmin) * rect_size[3];
  cairo_move_to        (cr,x,y);
  cairo_set_line_width(cr,priv->chart->lineplots->linewidth);
  for(i = 0; i < nelem; i++){
    x = rect_size[0] + spacing*i;
    y = rect_size[3] + rect_size[1] - (values[i]-vmin)/(vmax-vmin) * rect_size[3];
    switch(priv->chart->lineplots->linestyle){
      case AT_LINESTYLE_SOLID:
        cairo_line_to(cr,x,y);
        break;
      case AT_LINESTYLE_DASHED:
        cairo_set_dash(cr,dashes,2,0);
        cairo_line_to(cr,x,y);
        break;
      case AT_LINESTYLE_DOTTED:
        cairo_set_dash(cr,dots,2,0);
        cairo_line_to(cr,x,y);
        break;
    }
  }
  cairo_stroke(cr);
  cairo_move_to        (cr,rect_size[0],rect_size[1]);
  for(i = 0; i < nelem; i++){
    x = rect_size[0]+spacing*i;
    y = rect_size[3] + rect_size[1] - (values[i]-vmin)/(vmax-vmin) * rect_size[3];
    switch(priv->chart->lineplots->marker){
      case AT_MARKER_POINT:
        cairo_arc(cr,x,y,3,0,2*M_PI);
        cairo_fill(cr);
        break;
      case AT_MARKER_SQUARE:
        cairo_rectangle(cr,x-3,y-3,6,6);
        cairo_fill(cr);
      break;
      case AT_MARKER_TRIANGLE_DOWN:
        cairo_move_to(cr,  x-3.5,y+3.5);
        cairo_line_to(cr,  x-3.5,y+3.5);
        cairo_line_to(cr,  x+3.5,y+3.5);
        cairo_line_to(cr,  x    ,y-3.5);
        cairo_fill(cr);
      break;
    }
  }
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
at_chartviewer_write_pdf(AtChartViewer* self, const char *name){
  AtChartViewerPrivate*priv = at_chartviewer_get_instance_private(self);
  GtkAllocation allocation;
  gtk_widget_get_allocated_size(GTK_WIDGET(self),&allocation,NULL);
  at_chart_write_pdf(priv->chart,name,allocation.width,allocation.height);
}
