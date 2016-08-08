#include <at/chart.h>

void
at_lineplot_init(AtLinePlot* lp){
  AtColor edgecolor   = {0,0,0,1};
  lp->linestyle       = AT_LINESTYLE_SOLID;
  lp->linewidth       = 1;
  lp->marker          = AT_MARKER_POINT;
  lp->markeredgecolor = edgecolor;
  lp->linecolor       = edgecolor;
  lp->markeredgewidth = 1;
  lp->markerfacecolor = edgecolor;
  lp->markersize      = 1;
  lp->nelem           = 0;
  lp->y               = NULL;
  lp->type            = AT_PLOT_LINE;
}

AtLinePlot*
at_lineplot_new(){
  AtLinePlot* plot = malloc(sizeof(AtLinePlot));
  at_lineplot_init(plot);
  return plot;
}

#define AT_DEFINE_LINEPLOT_FILL(lower,UPPER,type)                                \
void                                                                      \
at_lineplot_fill_##lower(AtLinePlot* lineplot, type* y, uint64_t num){ \
  lineplot->nelem  = num;                                                 \
  lineplot->y = malloc(num<<3);                                      \
  uint64_t i;                                                             \
  for(i = 0; i < num;i++)                                                 \
    lineplot->y[i] = y[i];                                      \
}
AT_DEFINE_LINEPLOT_FILL(u8 ,U8 ,uint8_t)
AT_DEFINE_LINEPLOT_FILL(u16,U16,uint16_t)
AT_DEFINE_LINEPLOT_FILL(u32,U32,uint32_t)
AT_DEFINE_LINEPLOT_FILL(u64,U64,uint64_t)
AT_DEFINE_LINEPLOT_FILL(i8 ,I8 ,int8_t)
AT_DEFINE_LINEPLOT_FILL(i16,I16,int16_t)
AT_DEFINE_LINEPLOT_FILL(i32,I32,int32_t)
AT_DEFINE_LINEPLOT_FILL(i64,I64,int64_t)
AT_DEFINE_LINEPLOT_FILL(f32,F32,float)
AT_DEFINE_LINEPLOT_FILL(d64,D64,double)

void
at_lineplot_destroy(AtLinePlot** selfp){
  if(*selfp){
    if((*selfp)->y) free((*selfp)->y);
    free(*selfp);
    *selfp = NULL;
  }
}

