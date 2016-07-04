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
#ifndef AT_NIFTI_H
#define AT_NIFTI_H
#include <at/core/macro.h>
#include <at/core/array.h>
#include <at/core/error.h>
#include <stdint.h>
AT_BEGIN_DECLS

typedef enum{
  AT_NIFTI_TYPE_ANALYZE  = 0,
  AT_NIFTI_TYPE_NIFTI1_1 = 1,
  AT_NIFTI_TYPE_NIFTI1_2 = 2,
  AT_NIFTI_TYPE_ASCII    = 3,
  AT_NIFTI_TYPE_MAX      = 3,
} AtNiftiFileType;

/**
 * @enum analyze_75_orient_code
 * @brief Old-style analyze75 orientation
 *        codes.
 */
typedef enum _AtAnalyze75OrientCode {
  AT_A75_TRANSVERSE_UNFLIPPED = 0,
  AT_A75_CORONAL_UNFLIPPED    = 1,
  AT_A75_SAGITTAL_UNFLIPPED   = 2,
  AT_A75_TRANSVERSE_FLIPPED   = 3,
  AT_A75_CORONAL_FLIPPED      = 4,
  AT_A75_SAGITTAL_FLIPPED     = 5,
  AT_A75_ORIENT_UNKNOWN       = 6
} AtAnalyze75OrientCode;

typedef enum{
  AT_NIFTI_XFORM_UNKNOWN      = 0,
  AT_NIFTI_XFORM_SCANNER_ANAT = 1,
  AT_NIFTI_XFORM_ALIGNED_ANAT = 2,
  AT_NIFTI_XFORM_TALAIRACH    = 3,
  AT_NIFTI_XFORM_MNI_152      = 4
}AtNiftiXformCode;

typedef enum{
  AT_DT_INT8       = 256,
  AT_DT_UINT8      = 2,
  AT_DT_INT16      = 4,
  AT_DT_UINT16     = 512,
  AT_DT_INT32      = 8,
  AT_DT_UINT32     = 768,
  AT_DT_INT64      = 1024,
  AT_DT_UINT64     = 1280,
  AT_DT_RGB24      = 128,
  AT_DT_RGBA32     = 2304,
  AT_DT_FLOAT32    = 16,
  AT_DT_FLOAT64    = 64,
  AT_DT_COMPLEX64  = 32,
  AT_DT_FLOAT128   = 1536,
  AT_DT_COMPLEX128 = 1792,
  AT_DT_COMPLEX256 = 2048
}AtNiftiDatatype;


typedef struct AtNifti1Extension{
  uint8_t* edata; /*!< raw data, with no byte swapping (length is esize-8)  */
  uint32_t esize; /*!< size of extension, in bytes (must be multiple of 16) */
  uint32_t ecode; /*!< extension code, one of the NIFTI_ECODE_ values       */
}AtNifti1Extension;     // Total: 16 bytes


typedef struct AtNiftiImage{
  AtNifti1Extension    *ext_list;
  AtArrayU16     *ar;
  char                 *fname;
  char                 *iname;
  float                 qto_xyz[16];
  float                 sto_xyz[16];
  float                 qto_ijk[16];
  float                 sto_ijk[16];
  float                 quatern[3];
  float                 qoffset[3];
  float                 qfac;
  float                 scl_slope;
  float                 scl_inter;
  float                 slice_duration;
  float                 cal_min;
  float                 cal_max;
  uint32_t              iname_offset;
  uint32_t              num_ext;
  uint32_t              byteorder;
  AtNiftiDatatype       datatype;
  uint16_t              ndim;
  uint16_t              dim[7];
  uint16_t              pixdim[7];
  uint16_t              slice_start;
  uint16_t              slice_end;
  AtNiftiXformCode      qform_code;
  AtNiftiXformCode      sform_code;
  AtNiftiFileType       nifti_type;
  char                  descrip[80];
  char                  aux_file[24];
  uint8_t               swapsize;
  uint8_t               slice_code;
  uint8_t               xyz_units;
  uint8_t               time_units;
  uint8_t               freq_dim;
  uint8_t               phase_dim;
  uint8_t               slice_dim;
  AtAnalyze75OrientCode analyze75_orient;
}AtNiftiImage;

/**
 * @brief at_nifti_image_read
 * @param name
 * @param read_data
 * @return
 */
AtNiftiImage*
at_niftiimage_read(const char *name, uint8_t read_data, AtError** error);

void
at_niftiimage_destroy(AtNiftiImage** np);

AT_END_DECLS
#endif