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
#include <math.h>


static double    at_chart_render_title(cairo_t* cr, const char* title, AtVec4D64 rect);
static void      at_chart_render_subchart(cairo_t* cr, AtSubchart* subchart, AtVec4D64 rect);
static AtVec4D64 at_chart_render_legend(cairo_t* cr, AtSubchart* sc , AtVec4D64 rect);
static double    at_chart_render_subtitle(cairo_t* cr, const char* title, AtVec4D64 rect);
static AtVec4D64 at_chart_render_axis(cairo_t* cr, AtAxis* axis, AtVec4D64 rect);
static void      at_chart_render_plot(cairo_t* cr, AtLinePlot* plot, AtVec4D64 rect, AtAxis *ay);
static void      at_chart_render_marker(cairo_t* cr, AtLinePlot* plot, AtVec4D64 rect, AtAxis *ay);

double
at_chart_render_title(cairo_t *cr, const char *title, AtVec4D64 rect){
  double title_margin_top    = min(rect.width/100.0,10);
  if(title){
    cairo_text_extents_t title_extents;
    double title_font    = min(rect.width/100.0*5,20);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size   (cr, title_font);
    cairo_text_extents    (cr,title,&title_extents);
    cairo_move_to         (cr,(rect.width - title_extents.width)/2,title_extents.height+title_margin_top);
    cairo_show_text       (cr,title);
    return (title_margin_top * 2) + title_extents.height;
  }
  return title_margin_top;
}

void
at_chart_render(cairo_t* cr, AtChart* chart, AtVec4D64 rect){
  // Background
  cairo_set_source_rgb (cr,1,1,1);
  cairo_set_line_width(cr,1);
  cairo_paint(cr);

  // Foreground color
  cairo_set_source_rgb (cr,0,0,0);

  // Draw Title
  AtVec4D64 recttitle;
  double titleheight;
  recttitle.x = 0; recttitle.width  = rect.width;
  recttitle.y = 0; recttitle.height = rect.height;
  titleheight = at_chart_render_title(cr,chart->title,recttitle);

  // Draw Subcharts
  AtVec4D64 rectsub;
  rectsub.width    = rect.width / chart->gridsize[0];
  rectsub.height   = (rect.height - titleheight)/chart->gridsize[1];
  AtSList* subitem = chart->subcharts;
  uint8_t i, j;
  for(i = 0; i < chart->gridsize[1]; i++){
    for(j = 0; j < chart->gridsize[0];j++){
      if(subitem){
        rectsub.x = j*rectsub.width;
        rectsub.y = recttitle.y + titleheight + i*rectsub.height;
        at_chart_render_subchart(cr,(AtSubchart*)subitem->value,rectsub);
        subitem = subitem->next;
      }else break;
    }
  }
}

double
at_chart_render_subtitle(cairo_t* cr, const char* title, AtVec4D64 rect){
  double title_margin_top    = min(rect.width/100.0,5);
  if(title){
    cairo_text_extents_t title_extents;
    double title_font    = min(rect.width/100.0*3,15);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size   (cr, title_font);
    cairo_text_extents    (cr,title,&title_extents);
    cairo_move_to         (cr,rect.x + (rect.width - title_extents.width)/2,rect.y + title_extents.height);
    cairo_set_source_rgb  (cr,0,0,0);
    cairo_show_text       (cr,title);
    return title_margin_top + title_extents.height;
  }
  return title_margin_top;
}

void
at_chart_render_subchart(cairo_t* cr, AtSubchart* subchart, AtVec4D64 rect){
  // Draw Title
  AtVec4D64 recttitle;
  double titleheight;
  recttitle.x = rect.x; recttitle.width  = rect.width;
  recttitle.y = rect.y; recttitle.height = rect.height;
  titleheight = at_chart_render_subtitle(cr,subchart->title,recttitle);

  // Remove title space from rectangle
  rect.y      += titleheight;
  rect.height -= titleheight;

  // Draw Legend
  rect = at_chart_render_legend(cr,subchart,rect);

  // Draw Axis
  rect = at_chart_render_axis(cr,subchart->axis,rect);

  // Draw Data
  AtSList* item = subchart->plotlist;
  uint8_t i;
  for(i = 0; i < subchart->nplots; i++){
    at_chart_render_plot(cr,(AtLinePlot*)item->value,rect,&subchart->axis[1]);
    at_chart_render_marker(cr,(AtLinePlot*)item->value,rect,&subchart->axis[1]);
    item = item->next;
  }
}

static AtVec4D64
at_chart_render_axis(cairo_t* cr, AtAxis* axis, AtVec4D64 rect){
  // Get offsets
  cairo_text_extents_t offset;
  double font = min(rect.width/100.0*5,12);
  char str[12];
  sprintf(str,"%.2f",axis[1].vmax);
  cairo_set_font_size(cr, font);
  cairo_text_extents(cr,str,&offset);

  // Draw rectangle
  AtVec4D64 rectaxis;
  double marginh  = min(rect.width/100.0,10);
  double marginv  = min(rect.height/100.0,10);
  rectaxis.x      = rect.x + marginh * 2 + offset.width;
  rectaxis.y      = rect.y;
  rectaxis.width  = rect.width - (rectaxis.x - rect.x) - offset.width/2;
  rectaxis.height = rect.height - (offset.height + marginv);
  cairo_set_source_rgb(cr,0,0,0);
  cairo_set_line_width(cr,1);
  cairo_set_dash(cr,NULL,0,0);
  cairo_rectangle(cr, rectaxis.x, rectaxis.y, rectaxis.width, rectaxis.height);
  cairo_stroke(cr);

  // Draw axis numbers
  uint8_t i;
  double axis_number_cur;
  for(i = 0; i < 6; i++){
    // Vertical axis
    axis_number_cur = (axis[1].vmax-axis[1].vmin)/5*i;
    sprintf(str,"%.2f",axis_number_cur+axis[1].vmin);
    cairo_text_extents(cr,str,&offset);
    cairo_move_to(cr,rect.x+marginh,rect.y + rectaxis.height - i*rectaxis.height/5 + offset.height/2.0);
    cairo_show_text(cr,str);

    // Horizontal axis
    axis_number_cur = (axis[0].vmax-axis[0].vmin)/5*i;
    sprintf(str,"%.0f",axis_number_cur+axis[0].vmin);
    cairo_text_extents(cr,str,&offset);
    cairo_move_to(cr,rectaxis.x + i*rectaxis.width/5.0-offset.width/2.0,rectaxis.y + rectaxis.height + offset.height + marginv);
    cairo_show_text(cr,str);
  }
  return rectaxis;
}

static void
at_chart_render_plot(cairo_t* cr, AtLinePlot* plot, AtVec4D64 rect, AtAxis* ay){
  double spacing = rect.width/plot->nelem;
  double x,y;
  double dashes[] = {10.0, 10.0};
  double dots[] = {2.0, 5.0};
  uint64_t i;
  AtColor color = plot->linecolor;
  cairo_set_source_rgba(cr,color.r,color.g,color.b,color.a);
  x = rect.x;
  y = rect.y + rect.height - (plot->values[0]-ay->vmin)/(ay->vmax-ay->vmin) * rect.height;
  cairo_move_to        (cr,x,y);
  cairo_set_line_width(cr,plot->linewidth);
  for(i = 0; i < plot->nelem; i++){
    x = rect.x + spacing*i;
    y = rect.y + rect.height - (plot->values[i]-ay->vmin)/(ay->vmax-ay->vmin) * rect.height;
    switch(plot->linestyle){
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
}

static void
at_chart_render_marker(cairo_t* cr, AtLinePlot* plot, AtVec4D64 rect, AtAxis* ay){
  double spacing = rect.width/plot->nelem;
  cairo_move_to        (cr,rect.x, rect.y);
  uint64_t i;
  double x, y;
  for(i = 0; i < plot->nelem; i++){
    x = rect.x + spacing * i;
    y = rect.height + rect.y - (plot->values[i]-ay->vmin)/(ay->vmax-ay->vmin) * rect.height;
    switch(plot->marker){
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
}

static AtVec4D64
at_chart_render_legend(cairo_t* cr, AtSubchart* sc , AtVec4D64 rect){
  AtVec4D64 rectlegend;
  if(sc->legend){
    AtLinePlot* plot;
    AtSList   * item = sc->plotlist;
    cairo_text_extents_t extents;
    uint8_t     i;
    cairo_set_font_size(cr,min(5+rect.width/100.0*2,15));
    double legmax = -INFINITY;
    for(i = 0; i < sc->nplots; i++){
      plot = (AtLinePlot*)item->value;
      cairo_text_extents(cr,plot->name,&extents);
      legmax = max(legmax,extents.width);
      item = item->next;
    }
    legmax += 25;

    switch(sc->legendspos){
      case AT_LEFT:
        rectlegend.x = rect.x;
        rectlegend.y = rect.y;
        rectlegend.width = legmax;
        rectlegend.height= (extents.height * 2) * sc->nplots;
        rect.x += legmax;
        rect.width -= legmax;
      break;
      case AT_RIGHT:
        rectlegend.x = rect.x + rect.width - legmax;
        rectlegend.y = rect.y;
        rectlegend.width = legmax;
        rectlegend.height= (extents.height * 2) * sc->nplots;
        rect.width -= legmax;
      break;
      case AT_TOP:
        rectlegend.width = (extents.width + 25) * sc->nplots;
        rectlegend.x = rect.x + (rect.width-rectlegend.width)/2;
        rectlegend.y = rect.y;
        rectlegend.height= extents.height * 2;
        rect.y += extents.height * 3;
        rect.height -= extents.height * 3;
      break;
      case AT_BOTTOM:
        rectlegend.width = (extents.width + 25) * sc->nplots;
        rectlegend.x = rect.x + (rect.width-rectlegend.width)/2;
        rectlegend.y = rect.y + rect.height - extents.height * 3;
        rectlegend.height= extents.height * 2;
        rect.height -= extents.height * 3 + 5;
      break;
    }

    // Draw rectangle
    cairo_rectangle(cr,rectlegend.x,rectlegend.y,rectlegend.width,rectlegend.height);
    cairo_stroke(cr);

    // Draw plots codes
    double x = rectlegend.x + 2;
    double y = rectlegend.y;

    item = sc->plotlist;
    switch(sc->legendspos){
      case AT_TOP:
      case AT_BOTTOM:
        y += extents.height;
        break;
    }
    for(i = 0; i < sc->nplots; i++){
      plot = (AtLinePlot*)item->value;
      cairo_text_extents(cr,plot->name,&extents);
      cairo_set_source_rgb(cr,plot->linecolor.r,plot->linecolor.g,plot->linecolor.b);
      switch(sc->legendspos){
        case AT_LEFT:
        case AT_RIGHT:
          y += extents.height;
          break;
      }

      // line
      cairo_move_to(cr,x,y);
      cairo_line_to(cr,x+15,y);
      cairo_stroke(cr);
      // marker
      switch(plot->marker){
        case AT_MARKER_POINT:
          cairo_arc(cr,x+8,y,3,0,2*M_PI);
          cairo_fill(cr);
          break;
        case AT_MARKER_SQUARE:
          cairo_rectangle(cr,x+5,y-3,6,6);
          cairo_fill(cr);
        break;
        case AT_MARKER_TRIANGLE_DOWN:
          cairo_move_to(cr,  x+5.5,y+3.5);
          cairo_line_to(cr,  x+5.5,y+3.5);
          cairo_line_to(cr,  x+11.5,y+3.5);
          cairo_line_to(cr,  x+8  ,y-3.5);
          cairo_fill(cr);
        break;
      }

      // name
      cairo_move_to(cr,x+20,y+extents.height/2);
      cairo_show_text(cr,plot->name);
      switch(sc->legendspos){
        case AT_TOP:
        case AT_BOTTOM:
          x += extents.width + 25;
          break;
        case AT_LEFT:
        case AT_RIGHT:
          y += extents.height * (0.5);
          break;
      }


      item = item->next;
    }

  }
  return rect;
}
