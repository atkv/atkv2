#include <at/imgproc/color.h>
AtArrayU8*
at_arrayu8_cvt_color(AtArrayU8 *array, AtColorType from, AtColorType to){
  AtArrayU8* newar;
  uint8_t*   newardata;
  uint32_t*  ardata32;
  uint32_t*  newardata32;
  uint64_t   shape[3] = {array->h.shape[0],array->h.shape[1],0};
  uint64_t   num_elements32;
  uint64_t   i;
  size_t     size;
  uint8_t    dim;
  switch(to){
  case AT_GRAY:
    dim = 1;shape[2]=1;break;
  case AT_RGB:
  case AT_BGR:
    dim = 3;shape[2]=3;break;
  case AT_ABGR:
  case AT_ARGB:
  case AT_BGRA:
  case AT_RGBA:
    dim = 3;shape[2]=4;break;
  }

  newar = at_arrayu8_new(dim,shape);

  if(from == to)
    memcpy(newar->data,array->data,array->h.elemsize*array->h.num_elements);


  if(from == AT_GRAY){
    size = array->h.elemsize*array->h.num_elements;
    if(to == AT_RGB || to == AT_BGRA || to == AT_RGBA) newardata = newar->data;
    else if(to == AT_ARGB || to == AT_ABGR)            newardata = newar->data + array->h.num_elements;
    memcpy(newardata                           ,array->data,size);
    memcpy(newardata+ array->h.num_elements    ,array->data,size);
    memcpy(newardata+(array->h.num_elements<<1),array->data,size);
  }else if(from == AT_RGBA){
    if(to == AT_BGRA){
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i  ] = array->data[i+2];
        newar->data[i+1] = array->data[i+1];
        newar->data[i+2] = array->data[i  ];
        newar->data[i+3] = array->data[i+3];
      }
    }else if(to == AT_ABGR){
      ardata32       = (uint32_t*)array->data;
      newardata32    = (uint32_t*)newar->data;
      num_elements32 = array->h.num_elements >> 2;
      for(i = 0;i < num_elements32; i++)
        newardata32[i] = __bswap_32(ardata32[i]);
    }else if(to == AT_ARGB){
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i  ] = array->data[i+3];
        newar->data[i+1] = array->data[i  ];
        newar->data[i+2] = array->data[i+1];
        newar->data[i+3] = array->data[i+2];
      }
    }
  }else if(from == AT_RGB){
    if(to == AT_RGBA){
      for(i = 0;i < array->h.num_elements; i+=4){
        memcpy(&newar->data[i],&array->data[i],3*sizeof(uint8_t));
        newar->data[i+3] = 255;
      }
    }else if(to == AT_BGRA){
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i  ] = array->data[i+2];
        newar->data[i+1] = array->data[i+1];
        newar->data[i+2] = array->data[i  ];
        newar->data[i+3] = 255;
      }
    }else if(to == AT_ABGR){
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i  ] = 255;
        newar->data[i+1] = array->data[i+2];
        newar->data[i+2] = array->data[i+1];
        newar->data[i+3] = array->data[i  ];
      }
    }else if(to == AT_ARGB){
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i] = 255;
        memcpy(&newar->data[i+1],&array->data[i+1],3*sizeof(uint8_t));
      }
    }
  }
  return newar;
}

