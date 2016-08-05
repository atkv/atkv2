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
#if !defined(AT_IMGPROC_H_INSIDE)
#error "Only <at/imgproc.h> can be included directly."
#endif
#ifndef AT_DICOM_H
#define AT_DICOM_H
#include <at/core.h>
#include <stdint.h>
AT_BEGIN_DECLS
/**
 * @brief Dicom Base
 */
typedef struct AtDicomBase{
  const char* filename;            /*!< Name of DICOM File*/                                                 /*00+08*/
  char      * modality;            /*!< Modality (e.g.,MR for magnetic ressonance )*/                        /*08+08*/
  char      * unit;                /*!< Unit type */                                                         /*16+08*/
  uint8_t   * photo_interpretation;/*!< photo_interpretation */                                              /*24+08*/
  float       intercept;           /*!< intercept */                                                         /*32+08*/
  float       slope;               /*!< slope */                                                             /*40+08*/
  uint16_t    samples_per_pixel;   /*!< samples_per_pixel */                                                 /*48+02*/
  uint16_t    bits_allocated;      /*!< bits_allocated */                                                    /*50+02*/
  uint16_t    pixel_representation;/*!< pixel_representation */                                              /*52+02*/
  uint8_t     pixel_data_tag_found;/*!< pixel_data_tag_found */                                              /*53+01*/
  uint8_t     pd[2];               /*!< just padding (the size becomes multiple of 8 bytes). Don't use it */ /*54+02*/
                                   // Total: 56 bytes
}AtDicomBase;
/**
 * @brief Dicom 16 bits
 */
typedef struct AtDicomU16{
  AtDicomBase base; /*!< Base of DICOM *//*00+56*/
  AtArrayU16* luts; /*!< Lookup Table  *//*56+08*/
  AtArrayU16* image;/*!< Proper Image  *//*64+08*/
                          //72 bytes
}AtDicomU16;
/**
 * @brief at_dicomu16_read
 * @param fname
 * @param error
 * @return
 */
AtDicomU16*
at_dicomu16_read(const char* fname, AtError **error);
/**
 * @brief at_dicomu16_write
 * @param dicom
 * @param filename
 */
void
at_dicomu16_write(AtDicomU16* dicom, const char* filename);

#define at_dicom_read(dicom, filename, error) dicom = _Generic((dicom), \
 AtDicomU16*: at_dicomu16_read)(filename, error)
#define at_dicom_write(dicom, filename) _Generic((dicom), \
 AtDicomU16*: at_dicomu16_write)(dicom, filename)

AT_END_DECLS
#endif
