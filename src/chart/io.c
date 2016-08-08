#include <at/chart.h>

const char *
get_filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";
  return dot + 1;
}

void
at_chart_write_pdf(AtChart *chart, const char *filename, double width, double height){
  cairo_surface_t* surface = cairo_pdf_surface_create(filename,width,height);
  cairo_t        * cr      = cairo_create(surface);
  AtVec4D64        rect    = {0,0,width,height};
  at_chart_render(cr,chart,rect);
  cairo_show_page(cr);
  cairo_surface_destroy(surface);
  cairo_destroy(cr);
}

void
at_chart_write_png(AtChart *chart, const char *filename, double width, double height){
  cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t        * cr      = cairo_create(surface);
  AtVec4D64        rect    = {0,0,width,height};
  at_chart_render(cr,chart,rect);
  cairo_surface_write_to_png(surface, filename);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}

void
at_chart_write_svg(AtChart *chart, const char *filename, double width, double height){
  cairo_surface_t* surface = cairo_svg_surface_create(filename, width,height);
  cairo_t        * cr      = cairo_create(surface);
  AtVec4D64        rect    = {0,0,width,height};
  at_chart_render(cr,chart,rect);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}

void
at_chart_write(AtChart* chart, const char* filename, double width, double height){
  const char* ext = get_filename_ext(filename);
  if(strcmp(ext,"png") == 0)
    at_chart_write_png(chart,filename,width,height);
  else if(strcmp(ext,"pdf") == 0)
    at_chart_write_pdf(chart,filename,width,height);
  else if(strcmp(ext,"svg") == 0)
    at_chart_write_svg(chart,filename,width,height);
}
