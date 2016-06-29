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
#ifndef AT_IMGIO_H
#define AT_IMGIO_H
#include <at/array.h>
#include <at/error.h>
#include <at/macro.h>
AT_BEGIN_DECLS
/*=============================================================================
 MACROS (PUBLIC API)
 ============================================================================*/
#define at_array_read_image(format, array, filename, error) array = _Generic((array), \
  AtArray_uint8_t*:at_array_uint8_t_read_##format)(filename,error)
#define at_array_read_png(array, filename, error) at_array_read_image(png,array,filename,error)
#define at_array_read_jpg(array, filename, error) at_array_read_image(jpg,array,filename,error)
#define at_array_read_pgm(array, filename, error) at_array_read_image(pgm,array,filename,error)
#define at_array_read_ppm(array, filename, error) at_array_read_image(ppm,array,filename,error)

#define at_array_write_image(format, array, filename, error) _Generic((array), \
  AtArray_uint8_t*: at_array_uint8_t_write_##format)(array,filename,error)
#define at_array_write_png(array, filename, error) at_array_write_image(png, array, filename, error)
#define at_array_write_jpg(array, filename, error) at_array_write_image(jpg, array, filename, error)
#define at_array_write_pgm(array, filename, error) at_array_write_image(pgm, array, filename, error)
#define at_array_write_ppm(array, filename, error) at_array_write_image(ppm, array, filename, error)

/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_array_uint8_t_read_png
 * @param filename
 * @return
 */
AtArray_uint8_t*
at_array_uint8_t_read_png(const char* filename, AtError** error);

/**
 * @brief at_array_uint8_t_read_jpg
 * @param filename
 * @return
 */
AtArray_uint8_t*
at_array_uint8_t_read_jpg(const char* filename, AtError** error);

/**
 * @brief at_array_uint8_t_read_pgm
 * @param filename
 * @param error
 * @return
 */
AtArray_uint8_t*
at_array_uint8_t_read_pgm(const char* filename, AtError** error);

/**
 * @brief at_array_uint8_t_read_ppm
 * @param filename
 * @param error
 * @return
 */
AtArray_uint8_t*
at_array_uint8_t_read_ppm(const char* filename, AtError** error);

/**
 * @brief at_array_uint8_t_write_png
 * @param image
 * @param filename
 */
void
at_array_uint8_t_write_png(AtArray_uint8_t* image, const char *filename, AtError **error);

/**
 * @brief at_array_uint8_t_write_jpg
 * @param image
 * @param filename
 */
void
at_array_uint8_t_write_jpg(AtArray_uint8_t* image, const char* filename, AtError **error);

/**
 * @brief at_array_uint8_t_write_pgm
 * @param image
 * @param filename
 * @param error
 */
void
at_array_uint8_t_write_pgm(AtArray_uint8_t* image, const char* filename, AtError **error);

/**
 * @brief at_array_uint8_t_write_ppm
 * @param image
 * @param filename
 * @param error
 */
void
at_array_uint8_t_write_ppm(AtArray_uint8_t* image, const char* filename, AtError **error);

AT_END_DECLS
#endif
