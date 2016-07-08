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

#include <at/imgproc/drawing.h>
#include <float.h>
#include <math.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
enum { XY_SHIFT = 16, XY_ONE = 1 << XY_SHIFT, DRAWING_STORAGE_BLOCK = (1<<12) - 256 };

static void
at_arrayu8_draw_line_no_shift(AtArrayU8* array, AtVec2U16 p0, AtVec2U16 p1,
                              AtVec4U8 color, AtAdjacency adjacency){

}

static void
at_arrayu8_draw_line_n8(AtArrayU8* array, AtVec2U16 p0, AtVec2U16 p1, AtVec4U8 color){

}

#define at_magsqr(d) d.x * d.x + d.y * d.y;

static void
at_arrayu8_fill_convex_poly(AtArrayU8* array, AtVec2U16* pt, uint8_t num_pt, AtVec4U8 color, AtAdjacency adjacency,uint8_t shift){

}

static void
at_array_draw_circle(AtArrayU8* array, AtVec2U16 c, uint8_t thickness, AtVec4U8 color, int shift){

}

//static void
//at_arrayu8_draw_line_thick(AtArrayU8*  array,
//                           AtVec2U16   p0,
//                           AtVec2U16   p1,
//                           AtVec4U8    color,
//                           uint8_t     thickness,
//                           AtAdjacency adjacency,
//                           uint8_t     flags,
//                           uint8_t     shift){
//  static const double INV_XY_ONE = 1./XY_ONE;
//  p0.v <<= (uint16_t)(XY_SHIFT - shift);
//  p1.v <<= (uint16_t)(XY_SHIFT - shift);
//  if(thickness <= 1){
//    if(adjacency == AT_ADJACENCY_4 || shift == 0){
//      p0.v = (p0.v + (XY_ONE >> 1)) >> XY_SHIFT;
//      p1.v = (p1.v + (XY_ONE >> 1)) >> XY_SHIFT;
//      at_arrayu8_draw_line_no_shift(array,p0,p1,color,adjacency);
//    }else
//      at_arrayu8_draw_line_n8(array,p0,p1,color);
//  }else{// Thickness > 1
//    double    r;
//    AtVec2D64 d;
//    AtVec2U16 pt[4];
//    AtVec2U16 dp = {0,0};
//    AtVec2U16 c;
//    AtVec2U16 p = {thickness,thickness};
//    uint8_t   i;
//    uint8_t   odd_thickness = thickness & 1;

//    d.x = (p0.x-p1.x)*INV_XY_ONE;
//    d.y = (p0.y-p1.y)*INV_XY_ONE;
//    r = at_magsqr(d);
//    thickness <<= XY_SHIFT - 1;
//    if(fabs(r) > DBL_EPSILON){
//      r = (thickness + odd_thickness*XY_ONE*.5)/sqrt(r);
//      dp.x = round(d.y * r);
//      dp.y = round(d.x * r);
//      pt[0].v = p0.v + dp.v;
//      pt[1].v = p0.v - dp.v;
//      pt[2].v = p1.v - dp.v;
//      pt[3].v = p1.v + dp.v;
//      at_arrayu8_fill_convex_poly(array,pt,4,color,adjacency,XY_SHIFT);
//    }
//    for(i = 0; i < 2; i++){
//      if(flags & (i+1)){
//        if(adjacency == AT_ADJACENCY_4 || adjacency == AT_ADJACENCY_8){
//          c.v = (p0.v + (XY_ONE>>1)) >> XY_SHIFT;
//          at_array_draw_circle(array,c,(thickness+(XY_ONE>>1))>>XY_SHIFT, color, 1);
//        }else{
//          //at_array_draw_ellipse_ex(array,p0,p,0,0,360,color,0,adjacency);
//        }
//      }
//      p0 = p1;
//    }
//  }
//}
static void
at_arrayu8_draw_line_one(AtArrayU8* array, AtVec2I16 p0, AtVec2I16 p1, AtVec4U8 color, AtAdjacency adj){
  uint64_t coords[2];
  int16_t  dx        =  abs(p1.x-p0.x), sx = p0.x < p1.x ? 1: -1;
  int16_t  dy        = -abs(p1.y-p0.y), sy = p0.y < p1.y ? 1: -1;
  int16_t  err       =  dx+dy, e2;
  uint8_t  passed;
  for(;;){
    coords[1] = p0.x; coords[0] = p0.y;
    if(array->h.dim == 2) at_arrayu8_set_nd(array,coords,color.d[0]);
    else                  at_arrayu8_set_nd_many(array, coords, color.d);
    if(p0.x == p1.x && p0.y == p1.y) break;
    e2 = err<<1;
    passed = false;
    if(e2 >= dy){err += dy; p0.x += sx; passed = true;}
    if((adj == AT_ADJACENCY_8 || !passed) && e2 <= dx){err += dx; p0.y += sy;}
  }
}

#define at_set_pixel(x,y,array,coords,color)     \
  coords[1]= x; coords[0]= y;                    \
  if(array->h.dim != 2)                          \
    at_arrayu8_set_nd_many(array,coords,color.d);\
  else                                           \
    at_arrayu8_set_nd(array,coords,color.d[0]);

static void
at_arrayu8_draw_circle_one(AtArrayU8* array, AtVec2I16 c, uint16_t r, AtVec4U8 color){
  uint64_t coords[2];
  int16_t x = -r, y = 0, err = 2-(r<<1), ri = r;
  do{
    at_set_pixel(c.x-x, c.y+y,array,coords,color);
    at_set_pixel(c.x-y, c.y-x,array,coords,color);
    at_set_pixel(c.x+x, c.y-y,array,coords,color);
    at_set_pixel(c.x+y, c.y+x,array,coords,color);
    ri = err;
    if(ri <= y) err += (++y<<1)+1;
    if(ri > x || err > y) err += (++x<<1)+1;
  }while (x < 0);
}
#undef at_set_pixel

#define draw_line(vy) \
p[0].x = min(max(c.x+x,0),shape[1]);    p[0].y = min(max(c.y+vy,0),shape[0]);        \
p[1].x = min(max(c.x-x,0),shape[1]);    p[1].y = min(max(c.y+vy,0),shape[0]);        \
if(p[0].x > p[1].x) *tmp = *tmp << 32 | *tmp >> 32;                                 \
length = (p[1].x - p[0].x)+1;                                                         \
ind    = p[0].y*step[0] + p[0].x * step[1];                                         \
indend = ind+length*step[1];                                                        \
if(array->h.dim == 2)                     memset(&array->data[ind],color.r,length); \
else for(i = ind; i < indend; i+=step[1]) memcpy(&array->data[i],color.d,step[1]);


static void
at_arrayu8_draw_circle_filled(AtArrayU8* array, AtVec2I16 c, uint16_t r, AtVec4U8 color){
  uint64_t *step  = array->h.step;
  uint64_t *shape = array->h.shape;
  uint64_t *tmp;
  uint64_t  ind, indend, i;
  AtVec2I16 p[2];
  int16_t   x = -r, y = 0, err = 2-(r<<1), ri = r;
  uint16_t  length;

  if((c.x-r) <= (int16_t)shape[1] && (c.x+r) >= 0 &&
     (c.y-r) <= (int16_t)shape[0] && (c.y+r) >= 0){
    tmp = (uint64_t*)p;
    do{
      // Drawing lower semicircle
      draw_line((-y));
      draw_line((y));

      // Updating coordinates
      ri = err;
      if(ri <= y) err += (++y<<1)+1;
      if(ri > x || err > y) err += (++x<<1)+1;
    }while (x < 0);
  }

}
#undef draw_line

static void
at_arrayu8_draw_line_thick(AtArrayU8* array, AtVec2I16 p0, AtVec2I16 p1, AtVec4U8 color, uint8_t thickness)
{
  float    ed;
  int16_t  dx        =  abs(p1.x-p0.x), sx = p0.x < p1.x ? 1: -1;
  int16_t  dy        =  abs(p1.y-p0.y), sy = p0.y < p1.y ? 1: -1;
  int16_t  err       =  dx-dy, e2, x2, y2; /* error value e_xy */
  ed                 =  dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
  float    cond      =  ed * thickness;
  uint64_t coords[3];
  coords[2] = 1;

  for (thickness = (thickness+1)>>1; ; ) {                                   /* pixel loop */
    coords[1] = p0.x; coords[0] = p0.y;
    at_arrayu8_set_nd_many(array,coords,color.d);
    e2 = err; x2 = p0.x;
    if ((e2<<1) >= -dx) {                                           /* x step */
      for (e2 += dy, y2 = p0.y; e2 < cond && (p1.y != y2 || dx > dy); e2 += dx){
        coords[1] = p0.x; coords[0] = y2 += sy;
        at_arrayu8_set_nd_many(array,coords,color.d);
      }
      if (p0.x == p1.x) break;
      e2 = err; err -= dy; p0.x += sx;
    }
    if ((e2<<1) <= dy) {                                            /* y step */
      for (e2 = dx-e2; e2 < cond && (p1.x != x2 || dx < dy); e2 += dy){
        coords[1] = x2+=sx; coords[0] = p0.y;
        at_arrayu8_set_nd_many(array,coords,color.d);
      }
      if (p0.y == p1.y) break;
      err += dx; p0.y += sy;
    }
   }
}


/*=============================================================================
 PUBLIC API
 ============================================================================*/
void
at_arrayu8_draw_line(AtArrayU8*  array,
                     AtVec2I16   p0,
                     AtVec2I16   p1,
                     AtVec4U8    color,
                     uint8_t     thickness,
                     AtAdjacency adjacency,
                     uint8_t     shift){
  if(thickness == 1)
    at_arrayu8_draw_line_one(array,p0,p1,color,adjacency);
  else
    at_arrayu8_draw_line_thick(array,p0,p1,color,thickness);
}


void
at_arrayu8_clip_line(AtVec2U16   size,
                     AtVec2U16   p0,
                     AtVec2U16   p1){

}

void
at_arrayu8_draw_circle(AtArrayU8*  array,
                       AtVec2I16   center,
                       uint16_t    radius,
                       AtVec4U8    color,
                       uint8_t     thickness,
                       AtAdjacency adjacency,
                       uint8_t     shift){
  if(thickness == 1)
    at_arrayu8_draw_circle_one(array,center,radius,color);
  else if(thickness == 0)
    at_arrayu8_draw_circle_filled(array,center,radius,color);
}


void
at_arrayu8_draw_rectangle(AtArrayU8*  array,
                          AtVec4U16   rect,
                          AtVec4U8    color,
                          uint8_t     thickness,
                          AtAdjacency adjacency,
                          uint8_t     shift){
  uint64_t  step[3];
  uint64_t  p[4], s[2], i;
  p[0] = rect.y0*array->h.step[0] + rect.x0*array->h.step[1];
  p[1] = rect.y0*array->h.step[0] + rect.x1*array->h.step[1];
  p[2] = rect.y1*array->h.step[0] + rect.x0*array->h.step[1];
  p[3] = rect.y1*array->h.step[0] + rect.x1*array->h.step[1];
  s[0] = rect.x1 - rect.x0 + 1;
  s[1] = rect.y1 - rect.y0 + 1;
  memcpy(step,array->h.step,sizeof(uint64_t)*array->h.dim);
  if(thickness == 1){
    if(array->h.dim == 2){
      memset(&array->data[p[0]],color.r,s[0]);
      memset(&array->data[p[2]],color.r,s[0]);
      for(i = p[0]; i <= p[2]; i+=step[0]) array->data[i] = color.r;
      for(i = p[1]; i <= p[3]; i+=step[0]) array->data[i] = color.r;
    }else{
      for(i = p[0]; i <= p[1]; i+=step[1]) memcpy(&array->data[i],color.d,step[1]);
      memcpy(&array->data[p[2]],&array->data[p[0]],s[0]*step[2]);
      for(i = p[0]; i <= p[2]; i+=step[0]) memcpy(&array->data[i],color.d,step[1]);
      for(i = p[1]; i <= p[3]; i+=step[0]) memcpy(&array->data[i],color.d,step[1]);
    }
  }else if(thickness == 0){
    if(array->h.dim == 2)
      for(i = p[0]; i <= p[2]; i+=step[0]) memset(&array->data[i],color.r,s[0]);
    else{
      for(i = p[0]; i <= p[1]; i+=step[1]) memcpy(&array->data[i],color.d,step[1]);
      for(i = p[0]+step[0]; i <= p[2]; i+=step[0])
        memcpy(&array->data[i],&array->data[p[0]],s[0]*step[2]);
    }
  }
}


uint16_t
at_ellipse_to_poly(AtVec2I16   center,
                   AtVec2I16   axes,
                   uint16_t    angle,
                   uint16_t    arc_start,
                   uint16_t    arc_end,
                   uint8_t     delta,
                   AtVec4U16** pts){

}


