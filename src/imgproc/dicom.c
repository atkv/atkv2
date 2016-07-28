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
#include <at/imgproc.h>
#include <stdio.h>
#include <ctype.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
#define AT_DICM_OFFSET        0x80 // 128 bits
#define AT_DICOM_IMPLICIT_VR 0x2D2D
#define AT_READ_BITS(variable,num_bits) fread(&variable,sizeof(uint ## num_bits ## _t),1,file)

typedef enum _AtDicomValueRepresentation{
  AT_DICOM_VR_AE = 0x4145,
  AT_DICOM_VR_AS = 0x4153,
  AT_DICOM_VR_AT = 0x4154,
  AT_DICOM_VR_CS = 0x4353,
  AT_DICOM_VR_DA = 0x4441,
  AT_DICOM_VR_DS = 0x4453,
  AT_DICOM_VR_DT = 0x4454,
  AT_DICOM_VR_FD = 0x4644,
  AT_DICOM_VR_FL = 0x464C,
  AT_DICOM_VR_IS = 0x4953,
  AT_DICOM_VR_LO = 0x4C4F,
  AT_DICOM_VR_LT = 0x4C54,
  AT_DICOM_VR_PN = 0x504E,
  AT_DICOM_VR_SH = 0x5348,
  AT_DICOM_VR_SL = 0x534C,
  AT_DICOM_VR_SS = 0x5353,
  AT_DICOM_VR_ST = 0x5354,
  AT_DICOM_VR_TM = 0x544D,
  AT_DICOM_VR_UI = 0x5549,
  AT_DICOM_VR_UL = 0x554C,
  AT_DICOM_VR_US = 0x5553,
  AT_DICOM_VR_UT = 0x5554,
  AT_DICOM_VR_OB = 0x4F42,
  AT_DICOM_VR_OW = 0x4F57,
  AT_DICOM_VR_SQ = 0x5351,
  AT_DICOM_VR_UN = 0x554E,
  AT_DICOM_VR_QQ = 0x3F3F,
  AT_DICOM_VR_RT = 0x5254,
} AtDicomValueRepresentation;

typedef enum _AtDicomTag{
  AT_DICOM_TAG_PIXEL_REPRESENTATION       = 0x00280103,
  AT_DICOM_TAG_TRANSFER_SYNTAX_UID        = 0x00020010,
  AT_DICOM_TAG_MODALITY                   = 0x00080060,
  AT_DICOM_TAG_SLICE_THICKNESS            = 0x00180050,
  AT_DICOM_TAG_SLICE_SPACING              = 0x00180088,
  AT_DICOM_TAG_SAMPLES_PER_PIXEL          = 0x00280002,
  AT_DICOM_TAG_PHOTOMETRIC_INTERPRETATION = 0x00280004,
  AT_DICOM_TAG_PLANAR_CONFIGURATION       = 0x00280006,
  AT_DICOM_TAG_NUMBER_OF_FRAMES           = 0x00280008,
  AT_DICOM_TAG_ROWS                       = 0x00280010,
  AT_DICOM_TAG_COLUMNS                    = 0x00280011,
  AT_DICOM_TAG_PIXEL_SPACING              = 0x00280030,
  AT_DICOM_TAG_BITS_ALLOCATED             = 0x00280100,
  AT_DICOM_TAG_WINDOW_CENTER              = 0x00281050,
  AT_DICOM_TAG_WINDOW_WIDTH               = 0x00281051,
  AT_DICOM_TAG_RESCALE_INTERCEPT          = 0x00281052,
  AT_DICOM_TAG_RESCALE_SLOPE              = 0x00281053,
  AT_DICOM_TAG_RED_PALETTE                = 0x00281201,
  AT_DICOM_TAG_GREEN_PALETTE              = 0x00281202,
  AT_DICOM_TAG_BLUE_PALETTE               = 0x00281203,
  AT_DICOM_TAG_ICON_IMAGE_SEQUENCE        = 0x00880200,
  AT_DICOM_TAG_PIXEL_DATA                 = 0x7FE00010,
} AtDicomTag;

/**
 * @brief Remove initial and final space.
 *
 * Get the inital location with proper content and the end.
 * Move these content to start of pointer and delete the remaining memory
 *
 * [_,_,a,b,c,d,e,_,_,_]
 * =>
 * [a,b,c,d,e]
 *
 * @param str
 * @return
 */
static char*
trim_in_place(char* str){
  char* beg = str;
  char* end = beg + strlen(beg) - 1;

  while(isspace(*beg)) ++beg;
  while(end >= beg && isspace(*end)) --end;
  end[1] = 0;
  return memmove(str,beg, end - beg + 2);
}

// Dicom get data
/**
 * @brief Read `num_bytes` bytes from `file` and put to `variable`, and update
 *             byte counter (`location`)
 * @param num_bytes
 * @param variable
 * @param location
 * @param file
 */
static void
at_dicom_get_bytes(size_t num_bytes, void* variable, size_t* location, FILE* file){
  fread(variable,1,num_bytes,file);
  (*location) += num_bytes;
}

/**
 * @brief particular version of at_dicom_get_bytes for 2 bytes (little or big
 *        endian)
 * @param variable
 * @param location
 * @param file
 * @param little_endian
 */
static void
at_dicom_get_uint16(uint16_t* variable, size_t* location, FILE* file, uint8_t little_endian){
  uint8_t b[2];
  at_dicom_get_bytes(2, b, location, file);
  uint16_t b1 = (uint16_t)b[1];
  uint16_t b0 = b[0];
  if(little_endian)
    *variable = b1 << 8 | b0;
  else
    *variable = b0 << 8 | b1;
}

/**
 * @brief particular version of at_dicom_get_bytes for 4 bytes (little or big
 *        endian)
 * @param variable
 * @param location
 * @param file
 * @param little_endian
 */
static void
at_dicom_get_uint32(uint32_t* variable, size_t* location, FILE* file, uint8_t little_endian){
  uint8_t b[4];
  at_dicom_get_bytes(4, b, location, file);
  if(little_endian)
    *variable = (uint32_t)(b[3] << 24)|(uint32_t)(b[2] << 16) | (uint32_t)(b[1] << 8) | b[0];
  else
    *variable = (uint32_t)(b[0] << 24)|(uint32_t)(b[1] << 16) | (uint32_t)(b[2] << 8) | b[3];
}


/**
 * @brief The tags have variable length, but defined at the file. Let's get how
 *        the tag value is
 * @param variable
 * @param location
 * @param file
 * @param little_endian
 */
static void
at_dicom_get_length(uint32_t* variable, size_t* location, FILE* file, uint8_t little_endian){
  uint8_t b[4];
  uint32_t b3, b2;
  uint16_t vr;
  at_dicom_get_bytes(4, b, location, file);
  vr = ((uint16_t)b[0] << 8) | b[1];
  switch(vr){
  case AT_DICOM_VR_OB:
  case AT_DICOM_VR_OW:
  case AT_DICOM_VR_SQ:
  case AT_DICOM_VR_UN:
  case AT_DICOM_VR_UT:
    if((b[2] == 0) || (b[3] == 0)){
      at_dicom_get_uint32(variable, location, file, little_endian);
    }else{
      vr = AT_DICOM_IMPLICIT_VR;
      if(little_endian)
        *variable = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
      else
        *variable = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
    }
    break;
  case AT_DICOM_VR_AE:
  case AT_DICOM_VR_AS:
  case AT_DICOM_VR_AT:
  case AT_DICOM_VR_CS:
  case AT_DICOM_VR_DA:
  case AT_DICOM_VR_DS:
  case AT_DICOM_VR_DT:
  case AT_DICOM_VR_FD:
  case AT_DICOM_VR_FL:
  case AT_DICOM_VR_IS:
  case AT_DICOM_VR_LO:
  case AT_DICOM_VR_LT:
  case AT_DICOM_VR_PN:
  case AT_DICOM_VR_SH:
  case AT_DICOM_VR_SL:
  case AT_DICOM_VR_SS:
  case AT_DICOM_VR_ST:
  case AT_DICOM_VR_TM:
  case AT_DICOM_VR_UI:
  case AT_DICOM_VR_UL:
  case AT_DICOM_VR_US:
  case AT_DICOM_VR_QQ:
  case AT_DICOM_VR_RT:
    b3 = (uint32_t)b[3];
    b2 = (uint32_t)b[2];
    if(little_endian)
      *variable = (b3 << 8) | b2;
    else
      *variable = (b2 << 8) | b3;
    break;
  default:
    vr = AT_DICOM_IMPLICIT_VR;
    if(little_endian)
      *variable = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
    else
      *variable = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
    break;
  }
}

static void
at_dicombase_read(AtDicomBase* base, const char* filename, FILE** fpp, size_t* offset, uint64_t* size, AtError** error){
  FILE    * fp;
  char    * s                          = NULL;
  char    * scale                      = NULL;
  char    * scale_index                = NULL;
  char    * spacing                    = NULL;
  char    * center                     = NULL;
  char    * center_index               = NULL;
  char    * window_width_str           = NULL;
  char    * window_width_index         = NULL;
  char    * slope_str                  = NULL;
  char    * intercept_str              = NULL;
  double    xscale;
  double    yscale;
  double    pixel_width;
  double    pixel_height;
  double    pixel_depth;
  size_t    location                   = 0;
  float     window_center;
  float     window_width;
  uint32_t  element_length;
  uint32_t  tag;
  uint32_t  n_images;
  uint16_t  group_word;
  uint16_t  element_word;
  uint16_t  planar_configuration;
  uint16_t  width;
  uint16_t  height;
  char      dicm_id[5];
  uint8_t   decoding_tags              = true;
  uint8_t   little_endian              = 1;
  uint8_t   big_endian_transfer_syntax = 0;
  uint8_t   odd_locations;
  uint8_t   in_sequence                = 0;

  *fpp = fopen(filename, "rb");
  if(!*fpp){
    at_error_set(error,"[at_dicom_read] Couldn't open %s for reading", filename);
    return;
  }
  fp = *fpp;
  base->filename       = filename;
  base->slope          = 1;
  base->intercept      = 0;
  base->unit           = "mm";
  base->bits_allocated = 16;
  fseek(fp, AT_DICM_OFFSET, SEEK_SET);
  location += AT_DICM_OFFSET;

  at_dicom_get_bytes(4, dicm_id, &location, fp);
  dicm_id[4] = 0;
  if(strcmp(dicm_id, "DICM") == 0){
    while(decoding_tags){
      at_dicom_get_uint16(&group_word, &location, fp, little_endian);
      if(group_word == 0x0800 && big_endian_transfer_syntax){
        little_endian = 0;
        group_word = 0x0008;
      }
      at_dicom_get_uint16(&element_word, &location, fp, little_endian);
      tag = (group_word << 16) | element_word;
      at_dicom_get_length(&element_length, &location, fp, little_endian);
      // Hack to read some GE files
      if(element_length == 13 && !odd_locations)
        element_length = 10;

      // "Undefined" element length
      // This is a sort of bracket that encloses a sequence of
      // elements
      if(element_length == -1){
        element_length = 0;
        in_sequence    = 1;
      }

      if(location & 1) odd_locations = 1;
      if(in_sequence){
        //at_dicom_add_tag_info(tag,NULL);
        in_sequence = 0;
        continue;
      }

      // Analyse tag
      switch(tag){
      case AT_DICOM_TAG_TRANSFER_SYNTAX_UID:
        s = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, s, &location, fp);
        if(strstr(s,"1.2.4") || strstr(s,"1.2.5")){
          fclose(fp);
          free(s);
          at_error_set(error, "[at_dicom_read] Wrong DICOM version");
          return;
        }
        if(strstr(s,"1.2.840.10008.1.2.2"))
          big_endian_transfer_syntax = 1;
        free(s);
        break;
      case AT_DICOM_TAG_MODALITY:
        base->modality = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, base->modality, &location, fp);
        break;
      case AT_DICOM_TAG_NUMBER_OF_FRAMES:
        s = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, s, &location, fp);
        double frames = strtof(s, NULL);
        if(frames > 1.0)
          n_images = (uint32_t) frames;
        free(s);
        break;
      case AT_DICOM_TAG_SAMPLES_PER_PIXEL:
        at_dicom_get_uint16(&base->samples_per_pixel, &location, fp, little_endian);
        break;
      case AT_DICOM_TAG_PHOTOMETRIC_INTERPRETATION:
        base->photo_interpretation = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, base->photo_interpretation, &location, fp);
        base->photo_interpretation = trim_in_place(base->photo_interpretation);
        break;
      case AT_DICOM_TAG_PLANAR_CONFIGURATION:
        at_dicom_get_uint16(&planar_configuration, &location, fp, little_endian);
        break;
      case AT_DICOM_TAG_ROWS:
        at_dicom_get_uint16(&height, &location, fp, little_endian);
        break;
      case AT_DICOM_TAG_COLUMNS:
        at_dicom_get_uint16(&width, &location, fp, little_endian);
        break;
      case AT_DICOM_TAG_PIXEL_SPACING:
        scale = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, scale, &location, fp);
        xscale = yscale = 0;
        scale_index = strstr(scale, "\\");
        if(scale_index - scale > 1){
          yscale = strtof(scale, NULL);
          xscale = strtof(scale_index+1,NULL);
        }
        if(xscale != 0.0 && yscale != 0.0){
          pixel_width = xscale;
          pixel_height = yscale;
          base->unit = "mm";
        }
        free(scale);
        break;
      case AT_DICOM_TAG_SLICE_THICKNESS:
      case AT_DICOM_TAG_SLICE_SPACING:
        spacing = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, spacing, &location, fp);
        pixel_depth = strtof(spacing, NULL);
        free(spacing);
        break;
      case AT_DICOM_TAG_BITS_ALLOCATED:
        at_dicom_get_uint16(&base->bits_allocated, &location, fp, little_endian);
        break;
      case AT_DICOM_TAG_PIXEL_REPRESENTATION:
        at_dicom_get_uint16(&base->pixel_representation, &location, fp, little_endian);
        break;
      case AT_DICOM_TAG_WINDOW_CENTER:
        center = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, center, &location, fp);
        center_index = strstr(center, "\\");
        if(center_index) center = center_index;
        window_center = strtof(center, NULL);
        free(center);
        break;
      case AT_DICOM_TAG_WINDOW_WIDTH:
        window_width_str = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, window_width_str, &location, fp);
        if(window_width_index) window_width_str = window_width_index;
        window_width = strtof(window_width_str, NULL);
        free(window_width_str);
        break;
      case AT_DICOM_TAG_RESCALE_INTERCEPT:
        intercept_str = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, intercept_str, &location, fp);
        base->intercept = strtof(intercept_str, NULL);
        free(intercept_str);
        break;
      case AT_DICOM_TAG_RESCALE_SLOPE:
        slope_str = malloc(element_length + 1);
        at_dicom_get_bytes(element_length, slope_str, &location, fp);
        base->intercept = strtof(slope_str, NULL);
        free(slope_str);
        break;
      case AT_DICOM_TAG_RED_PALETTE:
        break;
      case AT_DICOM_TAG_GREEN_PALETTE:
        break;
      case AT_DICOM_TAG_BLUE_PALETTE:
        break;
      case AT_DICOM_TAG_PIXEL_DATA:
        if(element_length){
          *offset = location;
          decoding_tags = 0;
        }
        base->pixel_data_tag_found = 1;
        size[0] = height;
        size[1] = width;
        size[2] = base->samples_per_pixel;
        break;
       default:
        fseek(fp, element_length, SEEK_CUR);
        location += element_length;
        break;
      }
    }
  }else{
    at_error_set(error, "[at_dicom_read] Error");
  }
}

static void
at_dicomu16_read_pixels(AtDicomU16* dicom, size_t offset, FILE* file){
  uint16_t* d            = dicom->image->data;
  int16_t * ds           = (uint16_t*) d;
  uint8_t * d8           = (uint8_t*)  d;
  uint64_t  nelem        = dicom->image->h.num_elements;
  uint64_t  i , i2;          // our counters
  size_t    nbytes       = 2;// bytes per pixel
  uint32_t  p32;             // pixel in 32 bits
  uint16_t  us;              // unsigned
  int16_t   ss;              // signed
  uint8_t   b0, b1;          // just 2 bytes
  uint8_t   si = false;      // is signed image?

  // Read all file data and put to array
  fseek(file, offset, SEEK_SET);
  fread(d, 1, nbytes, file);

  if(dicom->base.samples_per_pixel == 1){
    for(i = 0; i < nelem; i++){
      // If unsigned image
      if(!dicom->base.pixel_representation){
        i2 = i << 1;
        b0 = d8[i2];
        b1 = d8[i2+1];
        us = ((uint16_t) b1 << 8) | b0;
        p32 = (uint32_t) us * dicom->base.slope + dicom->base.intercept;
        if(strstr(dicom->base.photo_interpretation, "MONOCHROME1"))
          p32 = 0xFFFF - p32;
      // If signed image
      }else{
        ss   = ds[i];
        p32 = (uint32_t) ss * dicom->base.slope + dicom->base.intercept;
        if(strstr(dicom->base.photo_interpretation, "MONOCHROME1"))
          p32 = 0xFFFF - p32;
      }
      // As we use 12 bits, we don't worry to stack overflows
      // So, we use int16.
      ds[i] = (int16_t) p32;

      // We flag the need for normalization if it's signed
      if(ds[i] < 0)
        si = true;
    }
    // Turn everything to positive if signed image
    if(si){
      for(i = 0; i < nelem; i++){
        d[i] = (uint16_t)(ds[i] - INT16_MIN);
      }
    }
  }
}


/*=============================================================================
 PUBLIC API
 ============================================================================*/
static AtDicomU16*
at_dicomu16_new(){
  return malloc(sizeof(AtDicomU16));
}

AtDicomU16*
at_dicomu16_read(const char *fname, AtError** error){
  AtDicomU16* d     = at_dicomu16_new();
  FILE*             fp    = NULL;
  uint64_t          shape[3];
  size_t            offset;
  at_dicombase_read(&d->base,fname,&fp,&offset,shape,error);
  if(*error != NULL){
    free(d);
    return NULL;
  }
  d->image = at_arrayu16_new(3,shape);
  at_dicomu16_read_pixels(d,offset,fp);
  if(fp) fclose(fp);
  return d;
}

void
at_dicomu16_write(AtDicomU16 *dicom, const char *filename){

}
