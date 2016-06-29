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
#ifndef AT_DICOM_H
#define AT_DICOM_H
#include <at/core/macro.h>
#include <at/core/array.h>
#include <at/core/error.h>
#include <stdint.h>
AT_BEGIN_DECLS
typedef struct AtDicomBase{
  const char* filename;            // 00+08
  char      * modality;            // 08+08
  char      * unit;                // 16+08
  uint8_t   * photo_interpretation;// 24+08
  float       intercept;           // 32+08
  float       slope;               // 40+08
  uint16_t    samples_per_pixel;   // 48+02
  uint16_t    bits_allocated;      // 50+02
  uint16_t    pixel_representation;// 52+02
  uint8_t     pixel_data_tag_found;// 53+01
  uint8_t     pd[2];               // 54+02
                                   // Total: 56 bytes
}AtDicomBase;

typedef struct AtDicom_uint16_t{
  AtDicomBase       base; //00+56
  AtArray_uint16_t* luts; //56+08
  AtArray_uint16_t* image;//64+08
                          //72 bytes
}AtDicom_uint16_t;

AtDicom_uint16_t*
at_dicom_uint16_t_read(const char* fname, AtError **error);

void
at_dicom_uint16_t_write(AtDicom_uint16_t* dicom, const char* filename);

#define at_dicom_read(dicom, filename, error) dicom = _Generic((dicom), \
 AtDicom_uint16_t*: at_dicom_uint16_t_read)(filename, error)
#define at_dicom_write(dicom, filename) _Generic((dicom), \
 AtDicom_uint16_t*: at_dicom_uint16_t_write)(dicom, filename)

AT_END_DECLS
#endif
