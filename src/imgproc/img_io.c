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

#include <at/imgproc/img_io.h>
#include <at/core/error.h>
#include <png.h>
#include <jpeglib.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/

struct at_jpeg_error_mgr{
  struct jpeg_error_mgr pub;
  jmp_buf               setjmp_buffer;
};
typedef struct at_jpeg_error_mgr* at_jpeg_error_ptr;

static void
at_jpg_error_exit(j_common_ptr cinfo){
  at_jpeg_error_ptr jpg_err = (at_jpeg_error_ptr) cinfo->err;
  longjmp(jpg_err->setjmp_buffer,1);
}

static void
at_pgm_skip_comm(char* s, size_t m, FILE* fp){
  while(fgets(s,m,fp) != NULL)
    if(s[0]!='#' && s[0]!='\n') break;
}

static void
at_arrayu8_write_pgm_ppm(AtArrayU8 *array, const char *filename, AtError **error, uint8_t format){
  FILE    * fp;
  uint64_t* shape;
  uint8_t * data;
  uint64_t  num_elements;
  uint8_t   array_max;

  fp           = fopen(filename, "wb");
  if(!fp){
    at_error_set(error,"[write_pgm] Can't open file for writing");
    return;
  }
  data         = array->data;
  shape        = array->h.shape;
  num_elements = array->h.num_elements;
  array_max    = at_array_max(array);

  fprintf(fp,  "P%d\n", format);
  fprintf(fp,"%d %d\n", shape[1], shape[0]);
  fprintf(fp,   "%d\n", array_max);
  fwrite(data, sizeof(uint8_t), num_elements, fp);
  fclose(fp);
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/


AtArrayU8*
at_arrayu8_read_png(const char* filename, AtError** error){
  AtArrayU8* array;
  uint8_t*         data;
  png_structp      png_ptr;
  png_infop        info_ptr;
  png_byte         header[8];
  FILE* infile   = fopen(filename, "rb");
  if(!infile){
    at_error_set(error, "[read_png_file] File %s could not be opened for reading", filename);
    return NULL;
  }

  fread(header, 1, 8, infile);
  if(png_sig_cmp(header, 0, 8)){
    at_error_set(error, "[read_png_file] File %s is not recognized as a PNG file", filename);
    fclose(infile);
    return NULL;
  }

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr){
    at_error_set(error, "[read_png_file] png_create_read_struct failed");
    fclose(infile);
    return NULL;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr){
    at_error_set(error, "[read_png_file] png_create_info_struct failed");
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
    fclose(infile);
    return NULL;
  }

  if(setjmp(png_jmpbuf(png_ptr))){
    at_error_set(error, "[read_png_file] Error during init_io");
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
    fclose(infile);
    return NULL;
  }
  png_init_io(png_ptr, infile);

  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  uint64_t size[3];
  size[0] = png_get_image_height(png_ptr, info_ptr);
  size[1] = png_get_image_width(png_ptr, info_ptr);
  size[2] = png_get_channels(png_ptr, info_ptr);
  //uint8_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  array = at_arrayu8_new(3, size);

  png_read_update_info(png_ptr, info_ptr);

  // Read file
  uint8_t**buffer   = (uint8_t**)malloc(sizeof(uint8_t*) * size[0]);
  size_t row_stride = png_get_rowbytes(png_ptr, info_ptr);
  uint32_t i;
  data = (array)->data;
  for(i = 0; i < size[0]; i++) buffer[i] = data + row_stride * i;
  if(setjmp(png_jmpbuf(png_ptr))){
    at_error_set(error, "[read_png_file] Error during read_image");
    free(buffer);
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
    fclose(infile);
    return NULL;
  }
  png_read_image(png_ptr, buffer);
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);
  fclose(infile);
  free(buffer);
  return array;
}

AtArrayU8*
at_arrayu8_read_jpg(const char* filename, AtError **error){
  AtArrayU8*              array = NULL;
  FILE*                         infile;
  uint8_t**                     buffer;
  struct jpeg_decompress_struct cinfo;
  struct at_jpeg_error_mgr      jerr;
  uint64_t                      shape[3];
  uint64_t                      row_stride;
  uint32_t i;

  if((infile = fopen(filename, "rb")) == NULL){
    at_error_set(error, "[at_array_read_jpg] File %s could not be opened for reading", filename);
    return NULL;
  }
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = at_jpg_error_exit;
  if(setjmp(jerr.setjmp_buffer)){
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return NULL;
  }
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  shape[0] = cinfo.output_height;
  shape[1] = cinfo.output_width;
  shape[2] = cinfo.output_components;
  array = at_arrayu8_new(3, shape);
  row_stride = array->h.step[0];
  buffer = (uint8_t**) malloc(sizeof(uint8_t*) * shape[0]);
  for(i = 0; i < shape[0];i++)
    buffer[i] = array->data + row_stride * i;
  i = 0;
  while(cinfo.output_scanline < cinfo.output_height){
    jpeg_read_scanlines(&cinfo, &buffer[i++],1);
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  free(buffer);
  return array;
}

AtArrayU8*
at_arrayu8_read_ppm_pgm(const char* filename, AtError** error){
  AtArrayU8* array = NULL;
  FILE*    fp;
  uint64_t i;
  uint64_t max_gray;
  uint64_t shape[3];
  uint64_t num_elements;
  uint32_t width, height;
  uint16_t dim;
  uint8_t* data;
  char     version[4];
  char     line[256];

  fp = fopen(filename,"rb");
  if(!fp){
    at_error_set(error, "[at_array_read_ppm_pgm] File %s could not be opened for reading", filename);
    return NULL;
  }

  fgets(version, sizeof(version), fp);
  if(version[0] == 'P' && (version[1] == '5' || version[1] == '2' || version[1] == '6')){
    if(version[1] == '6') dim = 3;
    else                  dim = 2;
    at_pgm_skip_comm(line,256,fp);
    sscanf(line, "%d %d\n", &width, &height);
    at_pgm_skip_comm(line,256,fp);
    sscanf(line, "%lu", &max_gray);

    shape[0]     = height;
    shape[1]     = width;
    shape[2]     = 3;
    array        = at_arrayu8_new(dim, shape);
    data         = array->data;
    num_elements = array->h.num_elements;
    if(version[1] == '2')
      for(i = 0; i < num_elements; i++)
        fscanf(fp, "%cu", &data[i]);
    else
      fread(data, sizeof(uint8_t), num_elements,fp);
  }else{
    at_error_set(error, "[at_array_read_ppm_pgm] File %s could not be opened for reading", filename);
  }
  fclose(fp);
  return array;
}

AtArrayU8*
at_arrayu8_read_pgm(const char* filename, AtError** error){
  return at_arrayu8_read_ppm_pgm(filename, error);
}

AtArrayU8*
at_arrayu8_read_ppm(const char* filename, AtError** error){
  return at_arrayu8_read_ppm_pgm(filename, error);
}

void
at_arrayu8_write_png(AtArrayU8* array, const char* filename, AtError** error){
  uint64_t*   shape;
  uint64_t*   step;
  uint8_t*    data;
  uint8_t**   buffer;
  png_structp png_ptr;
  png_infop   info_ptr;
  uint32_t    i;
  uint8_t     dim;

  /* create file */
  FILE *outfile = fopen(filename, "wb");
  if (!outfile){
    at_error_set(error, "[write_png] File %s could not be opened for writing", filename);
    return;
  }
  /* initialize stuff */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr){
    at_error_set(error, "[write_png] png_create_write_struct failed");
    fclose(outfile);
    return;
  }
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr){
    at_error_set(error, "[write_png] png_create_info_struct failed");
    fclose(outfile);
    return;
  }
  if (setjmp(png_jmpbuf(png_ptr))){
    at_error_set(error, "[write_png] Error during init_io");
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(outfile);
    return;
  }
  png_init_io(png_ptr, outfile);

  /* write header */
  if (setjmp(png_jmpbuf(png_ptr))){
    at_error_set(error, "[write_png] Error during writing header");
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(outfile);
    return;
  }

  dim   = array->h.dim;
  data  = array->data;
  shape = array->h.shape;
  step  = array->h.step;

  int color_type;
  if(dim == 2 || shape[2] < 3) color_type = PNG_COLOR_TYPE_GRAY;
  else if(shape[2] == 3) color_type = PNG_COLOR_TYPE_RGB;
  else color_type = PNG_COLOR_TYPE_RGBA;

  png_set_IHDR(png_ptr, info_ptr, shape[1], shape[0],
               sizeof(uint8_t) << 3, color_type, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr))){
    at_error_set(error, "[write_png] Error during writing bytes");
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(outfile);
    return;
  }

  buffer = malloc(sizeof(uint8_t*)*shape[0]);
  for(i = 0; i < shape[0]; i++) buffer[i] = data + step[0] * i * sizeof(uint8_t);
  png_write_image(png_ptr, buffer);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr))){
    at_error_set(error, "[write_png] Error during end of write");
    free(buffer);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(outfile);
    return;
  }

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  free(buffer);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(outfile);
}

void
at_arrayu8_write_jpg(AtArrayU8* array, const char* filename, AtError** error){
  FILE                      * outfile;
  uint8_t                   * row_pointer[1];
  uint8_t                   * data  = array->data;
  uint64_t                  * shape = array->h.shape;
  uint64_t                  * step  = array->h.step;
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr       jerr;
  uint16_t                    dim   = array->h.dim;

  if((outfile = fopen(filename, "wb"))==NULL){
    at_error_set(error,"[write_jpg] can't open %s", filename);
    return;
  }
  row_pointer[0] = data;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress      (&cinfo);
  jpeg_stdio_dest           (&cinfo, outfile);

  cinfo.image_width      = shape[1];
  cinfo.image_height     = shape[0];
  cinfo.input_components = (dim > 2)?shape[2]:1;
  cinfo.in_color_space   = (dim > 2 && shape[2] == 3)?JCS_RGB:JCS_GRAYSCALE;

  jpeg_set_defaults  (&cinfo);
  jpeg_set_quality   (&cinfo, 90, TRUE);
  jpeg_start_compress(&cinfo, TRUE);
  while(cinfo.next_scanline < shape[0]){
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
    row_pointer[0] += step[0];
  }
  jpeg_finish_compress (&cinfo);
  fclose               (outfile);
  jpeg_destroy_compress(&cinfo);
}

void
at_arrayu8_write_pgm(AtArrayU8 *array, const char *filename, AtError **error){
  at_arrayu8_write_pgm_ppm(array, filename, error, 5);
}

void
at_arrayu8_write_ppm(AtArrayU8 *array, const char *filename, AtError **error){
  at_arrayu8_write_pgm_ppm(array, filename, error, 6);
}
