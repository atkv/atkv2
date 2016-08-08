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
#ifndef AT_NIFTI_H
#define AT_NIFTI_H
#include <at/core.h>
#include <stdint.h>
AT_BEGIN_DECLS

/**
 * @brief Nifti File Type
 */
typedef enum{
  AT_NIFTI_TYPE_ANALYZE  = 0,
  AT_NIFTI_TYPE_NIFTI1_1 = 1,
  AT_NIFTI_TYPE_NIFTI1_2 = 2,
  AT_NIFTI_TYPE_ASCII    = 3,
  AT_NIFTI_TYPE_MAX      = 3,
} AtNiftiFileType;

/**
 * @enum AtAnalyze75OrientCode
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

/**
 * @brief Nifti XForm Code
 */
typedef enum{
  AT_NIFTI_XFORM_UNKNOWN      = 0,
  AT_NIFTI_XFORM_SCANNER_ANAT = 1,
  AT_NIFTI_XFORM_ALIGNED_ANAT = 2,
  AT_NIFTI_XFORM_TALAIRACH    = 3,
  AT_NIFTI_XFORM_MNI_152      = 4
}AtNiftiXformCode;

/**
 * @brief Nifti Data Type
 */
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

/**
 * @brief Nifti Extension
 */
typedef struct AtNifti1Extension{
  uint8_t* edata; /*!< raw data, with no byte swapping (length is esize-8)  */
  uint32_t esize; /*!< size of extension, in bytes (must be multiple of 16) */
  uint32_t ecode; /*!< extension code, one of the NIFTI_ECODE_ values       */
}AtNifti1Extension;     // Total: 16 bytes

/**
 * @brief Nifti Image
 */
typedef struct AtNiftiImage{
  AtNifti1Extension    *ext_list;        /*!< List of extensions */   /*000+008*/
  AtArrayU16           *ar;              /*!< Array of data */        /*008+008*/
  char                 *fname;           /*!< filename */             /*016+008*/
  char                 *iname;           /*!< name of image file */   /*024+008*/
  float                 qto_xyz[16];     /*!< quaternion xyz */       /*032+128*/
  float                 sto_xyz[16];     /*!< */                      /*160+128*/
  float                 qto_ijk[16];     /*!< quaternion ijk */       /*288+128*/
  float                 sto_ijk[16];     /*!< */                      /*416+128*/
  float                 quatern[3];      /*!< quaternion */           /*544+024*/
  float                 qoffset[3];      /*!< offset of quaternion */ /*568+024*/
  float                 qfac;            /*!<  */                     /*592+008*/
  float                 scl_slope;       /*!<  */                     /*600+008*/
  float                 scl_inter;       /*!<  */                     /*608+008*/
  float                 slice_duration;  /*!<  */                     /*616+008*/
  float                 cal_min;         /*!<  */                     /*624+008*/
  float                 cal_max;         /*!<  */                     /*632+008*/
  uint32_t              iname_offset;    /*!<  */                     /*640+004*/
  uint32_t              num_ext;         /*!<  */                     /*644+004*/
  uint32_t              byteorder;       /*!<  */                     /*648+004*/
  AtNiftiDatatype       datatype;        /*!<  */                     /*652+002*/
  uint16_t              ndim;            /*!<  */                     /*654+002*/
  uint16_t              dim[7];          /*!<  */                     /*656+014*/
  uint16_t              pixdim[7];       /*!<  */                     /*670+014*/
  uint16_t              slice_start;     /*!<  */                     /*684+002*/
  uint16_t              slice_end;       /*!<  */                     /*686+002*/
  AtNiftiXformCode      qform_code;      /*!<  */                     /*688+001*/
  AtNiftiXformCode      sform_code;      /*!<  */                     /*689+001*/
  AtNiftiFileType       nifti_type;      /*!<  */                     /*690+001*/
  AtAnalyze75OrientCode analyze75_orient;/*!<  */                     /*691+001*/
  char                  descrip[80];     /*!<  */                     /*692+080*/
  char                  aux_file[24];    /*!<  */                     /*772+024*/
  uint8_t               swapsize;        /*!<  */                     /*796+001*/
  uint8_t               slice_code;      /*!<  */                     /*797+001*/
  uint8_t               xyz_units;       /*!<  */                     /*798+001*/
  uint8_t               time_units;      /*!<  */                     /*799+001*/
  uint8_t               freq_dim;        /*!<  */                     /*800+001*/
  uint8_t               phase_dim;       /*!<  */                     /*801+001*/
  uint8_t               slice_dim;       /*!<  */                     /*802+001*/
  uint8_t               pd[5];           /*!< Just padding */         /*803+005*/
}AtNiftiImage;

/**
 * @brief at_nifti_image_read
 * @param name
 * @param read_data
 * @return
 */
AtNiftiImage*
at_niftiimage_read(const char *name, uint8_t read_data, AtError** error);
/**
 * @brief at_niftiimage_destroy
 * @param np
 */
void
at_niftiimage_destroy(AtNiftiImage** np);

AT_END_DECLS
#endif
