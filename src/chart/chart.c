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
#include <cairo.h>
#include <cairo-pdf.h>
#include <math.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
static void
at_chart_init(AtChart* chart){
  memset(chart, 0, sizeof(AtChart));
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtChart*
at_chart_new(){
  AtChart* chart = malloc(sizeof(AtChart));
  at_chart_init(chart);
  return chart;
}

AtLinePlot*
at_chart_plot_u8(AtChart* chart, uint8_t* values, uint64_t num){
  if(chart->axis == NULL) chart->axis = at_axis_new(2);
  AtLinePlot* lineplot = at_lineplot_new();
  chart->lineplots = at_slist_append(chart->lineplots,lineplot);
  at_lineplot_fill_u8(lineplot,values, num);
  at_axis_fill(&chart->axis[1],lineplot->values,num);
  chart->axis[0].vmin = 0;
  chart->axis[1].vmax = num;
  chart->nplots++;
  return lineplot;
}

AtLinePlot*
at_chart_plot_d64(AtChart* chart, double* values, uint64_t num){
  if(chart->axis == NULL) chart->axis = at_axis_new(2);
  AtLinePlot* lineplot = at_lineplot_new();
  chart->lineplots = at_slist_append(chart->lineplots,lineplot);
  at_lineplot_fill_d64(lineplot,values,num);
  at_axis_fill(&chart->axis[1],lineplot->values,num);
  chart->axis[0].vmin = 0;
  chart->axis[0].vmax = num;
  chart->nplots++;
  return lineplot;
//  {
//    chart->axis    = malloc(sizeof(AtAxis)*2);
//    chart->axis[0].vmin = 0;
//    chart->axis[0].vmax = num;
//    chart->axis[1].vmin = INFINITY;
//    chart->axis[1].vmax = -INFINITY;
//  }

//  chart->lineplots         = at_lineplot_new();
//  chart->lineplots->nelem  = num;
//  chart->lineplots->values = malloc(num<<3);
//  double* plotdata = chart->lineplots->values;
//  uint64_t i;
//  for(i = 0; i < num;i++){
//    plotdata[i] = values[i];
//    chart->axis[1].vmin = min(chart->axis[1].vmin,plotdata[i]);
//    chart->axis[1].vmax = max(chart->axis[1].vmax,plotdata[i]);
//  }
//  chart->nplots    = 1;
//  return chart->lineplots;
}

void
at_chart_write_pdf(AtChart* chart, const char* filename, double width, double height){
  cairo_surface_t* surface;
  cairo_t *cr;
  double title_margin_top         ;
  double title_margin_bottom      ;
  double title_font               ;
  double axis_number_margin_left  ;
  double axis_number_margin_right ;
  double axis_number_margin_top   ;
  double axis_number_margin_bottom;
  surface = cairo_pdf_surface_create(filename,width,height);
  cr      = cairo_create(surface);
  double                vmax    = chart->axis[1].vmax;

  double    * values = ((AtLinePlot*)chart->lineplots->value)->values;
  uint64_t i, nelem  = ((AtLinePlot*)chart->lineplots->value)->nelem;
  double spacing = width/nelem;

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr,1);


  // Draw Title
  cairo_text_extents_t title_extents;
  title_extents.height = 0;

  title_margin_top    = min(width/100.0,10);
  title_margin_bottom = title_margin_top;
  title_font          = min(width/100.0*5,20);

  if(chart->title){
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, title_font);
    cairo_text_extents(cr,chart->title,&title_extents);
    cairo_move_to(cr,(width - title_extents.width)/2,title_extents.height+title_margin_top);
    cairo_show_text(cr,chart->title);
  }

  // Draw Axis
  cairo_text_extents_t axis_number_extents;
  double axis_number_font = min(width/100.0*5,12);
  char axis_number_str[12];
  sprintf(axis_number_str,"%.2f",chart->axis[1].vmax);
  cairo_set_font_size(cr, axis_number_font);
  cairo_text_extents(cr,axis_number_str,&axis_number_extents);

  double rect_size[4];
  axis_number_margin_left  = min(width/100.0,10);
  axis_number_margin_right = axis_number_margin_left;
  axis_number_margin_top   = min(height/100.0,10);
  axis_number_margin_bottom= axis_number_margin_top;
  rect_size[0] = axis_number_extents.width + axis_number_margin_left + axis_number_margin_right;
  rect_size[1] = title_margin_bottom+title_margin_top+title_extents.height;
  rect_size[2] = width-rect_size[0]-5-axis_number_extents.width;
  rect_size[3] = height-rect_size[1]-5-axis_number_extents.height-axis_number_margin_top-axis_number_margin_bottom;
  cairo_rectangle(cr, rect_size[0],rect_size[1],rect_size[2],rect_size[3]);

  double axis_number_cur;

  for(i = 0; i < 6; i++){
    // Vertical axis
    axis_number_cur = chart->axis[1].vmax/5*i;
    sprintf(axis_number_str,"%.2f",axis_number_cur);
    cairo_move_to(cr,axis_number_margin_left,rect_size[1] + rect_size[3] - i*rect_size[3]/5 + axis_number_extents.height/2.0);
    cairo_show_text(cr,axis_number_str);

    // Horizontal axis
    axis_number_cur = chart->axis[0].vmax/5*i;
    sprintf(axis_number_str,"%.0f",axis_number_cur);
    cairo_move_to(cr,rect_size[0]+i*rect_size[2]/5.0-axis_number_extents.width/2.0,rect_size[1]+rect_size[3]+axis_number_margin_top+axis_number_extents.height);
    cairo_show_text(cr,axis_number_str);
  }

  // Draw Data
  cairo_move_to        (cr,rect_size[0],rect_size[1]);
  spacing = rect_size[2]/nelem;
  for(i = 0; i < nelem; i++){
    cairo_line_to(cr,rect_size[0]+spacing*i,rect_size[3] - values[i]/vmax * rect_size[3] + rect_size[1]);
  }
  cairo_stroke(cr);
  cairo_show_page(cr);
  cairo_surface_destroy(surface);
  cairo_destroy(cr);
}

void
at_chart_destroy(AtChart** chart){
  if(*chart){
    AtSList* list = (*chart)->lineplots;
    while(list){
      AtLinePlot* lineplot = (AtLinePlot*)list->value;
      at_lineplot_destroy(&lineplot);
      list = list->next;
    }
    at_slist_free(list);
    free(*chart);
    *chart = NULL;
  }
}
