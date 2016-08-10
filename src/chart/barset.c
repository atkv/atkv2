#include <at/chart.h>

static void
at_barset_init(AtBarSet* barset){
  memset(barset, 0, sizeof(AtBarSet));
}

AtBarSet*
at_barset_new(double* value, uint32_t num, char* label){
  AtBarSet* barset = malloc(sizeof(AtBarSet));
  barset->label = label;
  barset->value = value;
  barset->num   = num;
  return barset;
}

void
at_barset_destroy(AtBarSet** barsetp){
  if(*barsetp) free(*barsetp);
  *barsetp = NULL;
}
