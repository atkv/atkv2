#include <at/imgproc.h>
AtArrayU8*
at_arrayu8_cvt_color(AtArrayU8 *array, AtColorType from, AtColorType to){
  AtArrayU8* newar;
  uint8_t*   newardata;
  uint32_t*  ardata32;
  uint32_t*  newardata32;
  uint64_t   shape[3] = {array->h.shape[0],array->h.shape[1],0};
  uint64_t   num_elements32,i,j,k;
  size_t     size;
  uint8_t    dim;
  switch(to){
  case AT_GRAY:
    dim = 2;shape[2]=1;break;
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


  else if(from == AT_GRAY){
    size = array->h.elemsize*array->h.num_elements;
    if(to==AT_RGBA || to == AT_BGRA){
      for(i = 0,k = 0;k < array->h.num_elements; i+=4,k++){
        newar->data[i  ] = array->data[k];
        newar->data[i+1] = array->data[k];
        newar->data[i+2] = array->data[k];
        newar->data[i+3] = 255;
      }
    }else if(to == AT_ARGB || to == AT_ABGR){
      for(i = 0,k = 0;k < array->h.num_elements; i+=4,k++){
        newar->data[i  ] = 255;
        newar->data[i+1] = array->data[k];
        newar->data[i+2] = array->data[k];
        newar->data[i+3] = array->data[k];
      }
    }
  }else if(from == AT_RGBA){
    switch(to){
    case AT_BGRA:
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i  ] = array->data[i+2];
        newar->data[i+1] = array->data[i+1];
        newar->data[i+2] = array->data[i  ];
        newar->data[i+3] = array->data[i+3];
      }
      break;
    case AT_ABGR:
      ardata32       = (uint32_t*)array->data;
      newardata32    = (uint32_t*)newar->data;
      num_elements32 = array->h.num_elements >> 2;
      for(i = 0;i < num_elements32; i++)
        newardata32[i] = __bswap_32(ardata32[i]);
      break;
    case AT_ARGB:
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i  ] = array->data[i+3];
        newar->data[i+1] = array->data[i  ];
        newar->data[i+2] = array->data[i+1];
        newar->data[i+3] = array->data[i+2];
      }
      break;
    case AT_GRAY:
      for(i = 0,j = 0; i < array->h.num_elements; i+=4,j++){
        newar->data[j  ] = array->data[i  ]*0.299 +
                           array->data[i+1]*0.587 +
                           array->data[i+2]*0.114;
      }
      break;
    }

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
    switch(to){
    case AT_RGBA:
      for(i = 0;i < array->h.num_elements; i+=4){
        memcpy(&newar->data[i],&array->data[i],3*sizeof(uint8_t));
        newar->data[i+3] = 255;
      }
      break;
    case AT_BGRA:
      for(i = 0, j = 0;i < array->h.num_elements; i+=3, j+=4){
        newar->data[j  ] = array->data[i+2];
        newar->data[j+1] = array->data[i+1];
        newar->data[j+2] = array->data[i  ];
        newar->data[j+3] = 255;
      }break;
    case AT_ABGR:
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i  ] = 255;
        newar->data[i+1] = array->data[i+2];
        newar->data[i+2] = array->data[i+1];
        newar->data[i+3] = array->data[i  ];
      }break;
    case AT_ARGB:
      for(i = 0;i < array->h.num_elements; i+=4){
        newar->data[i] = 255;
        memcpy(&newar->data[i+1],&array->data[i+1],3*sizeof(uint8_t));
      }break;
    case AT_GRAY:
      for(i = 0,j = 0; i < array->h.num_elements; i+=3,j++){
        newar->data[j  ] = array->data[i  ]*0.299 +
                           array->data[i+1]*0.587 +
                           array->data[i+2]*0.114;
      }
      break;
    }
  }
  return newar;
}

AtArrayU8*
at_arrayu8_lut_random(AtArrayU8* array){
  uint64_t   i;
  AtArrayU8* arraylut = at_arrayu8_new(array->h.dim, array->h.shape);
  uint8_t    lut[256];

  for(i = 0; i < 256; i++) lut[i] = rand();
  for(i = 0; i < array->h.num_elements; i++)
    arraylut->data[i] = lut[array->data[i]];
  return arraylut;
}

AtArrayU8*
at_arrayu8_lut_random_multi(AtArrayU8* array, uint8_t nchannels){
  uint64_t   i,j,k,slut;
  uint64_t   shape[256];
  memcpy(shape,array->h.shape,array->h.dim*sizeof(uint64_t));
  shape[array->h.dim] = nchannels;
  AtArrayU8* arraylut = at_arrayu8_new(array->h.dim+1, shape);
  slut = 256*nchannels;
  uint8_t    *lut = malloc(slut);

  for(i = 0; i < slut; i++) lut[i] = rand();
  for(i = 0,k=0; i < arraylut->h.num_elements; i+=nchannels,k++)
    memcpy(&arraylut->data[i],&lut[array->data[k]*nchannels],nchannels);
  free(lut);
  return arraylut;
}

AtArrayU32*
at_arrayu32_lut(AtArrayU32* array, uint64_t *lut){
  uint64_t   i;
  AtArrayU32* arraylut = at_arrayu32_new(array->h.dim, array->h.shape);
  for(i = 0; i < array->h.num_elements; i++)
    arraylut->data[i] = lut[array->data[i]];
  return arraylut;
}

void
at_arrayu8_add_brightness(AtArrayU8* array, int16_t pos){
  uint64_t i,j;
  uint8_t c;
  uint8_t totalchan,colorchan;

  // Treatment for grayscale, rgb and rgba
  if(array->h.dim == 2 || array->h.shape[2] == 1) {totalchan = 1;colorchan=1;}
  else if(array->h.shape[2] == 3){totalchan = 3;colorchan=3;}
  else{totalchan = 4; colorchan = 3;}

  // Saturated sum and without alpha
  for(i = 0; i < array->h.num_elements; i+=totalchan){
    for(j = i; j < i+colorchan; j++){
      c = array->data[j] + pos;
      if(pos > 0 && c < array->data[j])      c = UINT8_MAX;
      else if(pos < 0 && c > array->data[j]) c = 0;
      array->data[j] = c;
    }
  }
}

