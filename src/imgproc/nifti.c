/**
 ** This file is part of the atkv project.
 ** Copyright 2016-2017 Anderson Tavares <nocturne.pe@gmail.com>.
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
#include <at/core.h>
#include <sys/stat.h>
#include <endian.h>
#include <math.h>
#include <time.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/

typedef struct AtNiftiFile{
  const char* fname;    // 00+8: name/path of nifti file
  const char* ext;      // 08+8: where extension begins
  const char* basename; // 16+8: where basename begins
  size_t      fsize;    // 24+8: size of file
  uint8_t     is_gz;    // 32+1: is it compressed?
  uint8_t     pd[7];    // 33+7: padding
}AtNiftiFile;           // Total: 40 bytes

typedef struct {
   /* header info fields - describes the header    overlap with NIfTI */
   /*                                              ------------------ */
   uint32_t sizeof_hdr;             /*  0 +  4        same              */
   char data_type[10];              /*  4 + 10       same              */
   char db_name[18];                /* 14 + 18      same              */
   uint32_t extents;                /* 32 +  4       same              */
   uint16_t session_error;          /* 36 +  2       same              */
   char regular;                    /* 38 +  1       same              */
   char hkey_un0;                   /* 39 +  1                40 bytes */

   /* image dimension fields - describes image sizes */
   short int dim[8];                /* 0 + 16       same              */
   short int unused8;               /* 16 + 2       intent_p1...      */
   short int unused9;               /* 18 + 2         ...             */
   short int unused10;              /* 20 + 2       intent_p2...      */
   short int unused11;              /* 22 + 2         ...             */
   short int unused12;              /* 24 + 2       intent_p3...      */
   short int unused13;              /* 26 + 2         ...             */
   short int unused14;              /* 28 + 2       intent_code       */
   short int datatype;              /* 30 + 2       same              */
   short int bitpix;                /* 32 + 2       same              */
   short int dim_un0;               /* 34 + 2       slice_start       */
   float pixdim[8];                 /* 36 + 32      same              */

   float vox_offset;                /* 68 + 4       same              */
   float funused1;                  /* 72 + 4       scl_slope         */
   float funused2;                  /* 76 + 4       scl_inter         */
   float funused3;                  /* 80 + 4       slice_end,        */
                                                 /* slice_code,       */
                                                 /* xyzt_units        */
   float cal_max;                   /* 84 + 4       same              */
   float cal_min;                   /* 88 + 4       same              */
   float compressed;                /* 92 + 4       slice_duration    */
   float verified;                  /* 96 + 4       toffset           */
   int glmax,glmin;                 /* 100 + 8              108 bytes */

   /* data history fields - optional */
   char descrip[80];                /* 0 + 80       same              */
   char aux_file[24];               /* 80 + 24      same              */
   char orient;                     /* 104 + 1      NO GOOD OVERLAP   */
   char originator[10];             /* 105 + 10     FROM HERE DOWN... */
   char generated[10];              /* 115 + 10                       */
   char scannum[10];                /* 125 + 10                       */
   char patient_id[10];             /* 135 + 10                       */
   char exp_date[10];               /* 145 + 10                       */
   char exp_time[10];               /* 155 + 10                       */
   char hist_un0[3];                /* 165 + 3                        */
   int views;                       /* 168 + 4                        */
   int vols_added;                  /* 172 + 4                        */
   int start_field;                 /* 176 + 4                        */
   int field_skip;                  /* 180 + 4                        */
   int omax, omin;                  /* 184 + 8                        */
   int smax, smin;                  /* 192 + 8              200 bytes */
} AtNiftiAnalyze75;                                   /* total:  348 bytes */


/*! \struct nifti_1_header
    \brief Data structure defining the fields in the nifti1 header.
           This binary header should be found at the beginning of a valid
           NIFTI-1 header file.
 */
                        /*************************/  /************************/
struct AtNifti1Header { /* NIFTI-1 usage         */  /* ANALYZE 7.5 field(s) */
                        /*************************/  /************************/

                                           /*--- was header_key substruct ---*/
 int   sizeof_hdr;    /*!< MUST be 348           */  /* int sizeof_hdr;      */
 char  data_type[10]; /*!< ++UNUSED++            */  /* char data_type[10];  */
 char  db_name[18];   /*!< ++UNUSED++            */  /* char db_name[18];    */
 int   extents;       /*!< ++UNUSED++            */  /* int extents;         */
 short session_error; /*!< ++UNUSED++            */  /* short session_error; */
 char  regular;       /*!< ++UNUSED++            */  /* char regular;        */
 char  dim_info;      /*!< MRI slice ordering.   */  /* char hkey_un0;       */

                                      /*--- was image_dimension substruct ---*/
 short dim[8];        /*!< Data array dimensions.*/  /* short dim[8];        */
 float intent_p1 ;    /*!< 1st intent parameter. */  /* short unused8;       */
                                                     /* short unused9;       */
 float intent_p2 ;    /*!< 2nd intent parameter. */  /* short unused10;      */
                                                     /* short unused11;      */
 float intent_p3 ;    /*!< 3rd intent parameter. */  /* short unused12;      */
                                                     /* short unused13;      */
 short intent_code ;  /*!< NIFTI_INTENT_* code.  */  /* short unused14;      */
 short datatype;      /*!< Defines data type!    */  /* short datatype;      */
 short bitpix;        /*!< Number bits/voxel.    */  /* short bitpix;        */
 short slice_start;   /*!< First slice index.    */  /* short dim_un0;       */
 float pixdim[8];     /*!< Grid spacings.        */  /* float pixdim[8];     */
 float vox_offset;    /*!< Offset into .nii file */  /* float vox_offset;    */
 float scl_slope ;    /*!< Data scaling: slope.  */  /* float funused1;      */
 float scl_inter ;    /*!< Data scaling: offset. */  /* float funused2;      */
 short slice_end;     /*!< Last slice index.     */  /* float funused3;      */
 char  slice_code ;   /*!< Slice timing order.   */
 char  xyzt_units ;   /*!< Units of pixdim[1..4] */
 float cal_max;       /*!< Max display intensity */  /* float cal_max;       */
 float cal_min;       /*!< Min display intensity */  /* float cal_min;       */
 float slice_duration;/*!< Time for 1 slice.     */  /* float compressed;    */
 float toffset;       /*!< Time axis shift.      */  /* float verified;      */
 int   glmax;         /*!< ++UNUSED++            */  /* int glmax;           */
 int   glmin;         /*!< ++UNUSED++            */  /* int glmin;           */

                                         /*--- was data_history substruct ---*/
 char  descrip[80];   /*!< any text you like.    */  /* char descrip[80];    */
 char  aux_file[24];  /*!< auxiliary filename.   */  /* char aux_file[24];   */

 short qform_code ;   /*!< NIFTI_XFORM_* code.   */  /*-- all ANALYZE 7.5 ---*/
 short sform_code ;   /*!< NIFTI_XFORM_* code.   */  /*   fields below here  */
                                                     /*   are replaced       */
 float quatern_b ;    /*!< Quaternion b param.   */
 float quatern_c ;    /*!< Quaternion c param.   */
 float quatern_d ;    /*!< Quaternion d param.   */
 float qoffset_x ;    /*!< Quaternion x shift.   */
 float qoffset_y ;    /*!< Quaternion y shift.   */
 float qoffset_z ;    /*!< Quaternion z shift.   */

 float srow_x[4] ;    /*!< 1st row affine transform.   */
 float srow_y[4] ;    /*!< 2nd row affine transform.   */
 float srow_z[4] ;    /*!< 3rd row affine transform.   */

 char intent_name[16];/*!< 'name' or meaning of data.  */

 char magic[4] ;      /*!< MUST be "ni1\0" or "n+1\0". */

} ;                   /**** 348 bytes total ****/
typedef struct AtNifti1Header AtNifti1Header ;

/* NIfTI-1.1 extension codes:
   see http://nifti.nimh.nih.gov/nifti-1/documentation/faq#Q21 */
#define AT_NIFTI_ECODE_IGNORE           0  /* changed from UNKNOWN, 29 June 2005 */
#define AT_NIFTI_MAX_ECODE             26  /******* maximum extension code *******/

#define AT_MAX_NIA_EXT_LEN         100000  /* maximum ascii extension length on file */

#define at_nifti_is_valid_ecode(ecode) (ecode >= AT_NIFTI_ECODE_IGNORE && ecode <= AT_NIFTI_MAX_ECODE && !(ecode & 1U))
#define at_nifti_check_extension(nim,size,code,rem) at_nifti_is_valid_ecode(code) && size >= 16 && size < rem && size & 0xf && (nim->nifti_type != AT_NIFTI_TYPE_ASCII || size <= AT_MAX_NIA_EXT_LEN)
#define at_nifti_hdrimg_onefile(m) (m[1] == '+') // magic 'n+1' means header and image are located in the same file
#define at_nifti_version(m)                        \
 ((m[0]=='n' && m[3]=='\0'  &&     \
 ( m[1]=='i' || m[1]=='+' ) &&     \
 ( m[2]>='1' && m[2]<='9' )   )    \
  ?m[2]-'0':0)
#define at_xyzt_to_space(xyzt)       ( (xyzt) & 0x07 )
#define at_xyzt_to_time(xyzt)        ( (xyzt) & 0x38 )
#define at_space_time_to_xyzt(ss,tt) ( (((char)(ss)) & 0x07) \
                                      |(((char)(tt)) & 0x38) )
#define at_dim_info_to_freq_dim(di)   ( ((di)     ) & 0x03 )
#define at_dim_info_to_phase_dim(di)  ( ((di) >> 2) & 0x03 )
#define at_dim_info_to_slice_dim(di)  ( ((di) >> 4) & 0x03 )

#define ASSIF(p,v) if( (p)!=NULL ) *(p) = (v)
static void
at_nifti_datatype_sizes( AtNiftiDatatype datatype , uint8_t *swapsize)
{
   int ss=0 ;
   switch( datatype ){
     case AT_DT_INT8:
     case AT_DT_UINT8:       ss =  0 ; break ;

     case AT_DT_INT16:
     case AT_DT_UINT16:      ss =  2 ; break ;

     case AT_DT_RGB24:       ss =  0 ; break ;
     case AT_DT_RGBA32:      ss =  0 ; break ;

     case AT_DT_INT32:
     case AT_DT_UINT32:
     case AT_DT_FLOAT32:     ss =  4 ; break ;

     case AT_DT_COMPLEX64:   ss =  4 ; break ;

     case AT_DT_FLOAT64:
     case AT_DT_INT64:
     case AT_DT_UINT64:      ss =  8 ; break ;

     case AT_DT_FLOAT128:    ss = 16 ; break ;

     case AT_DT_COMPLEX128:  ss =  8 ; break ;

     case AT_DT_COMPLEX256:  ss = 16 ; break ;
   }

   ASSIF(swapsize,ss) ; return ;
}

static int
at_niftiimage_read_extensions(AtNiftiImage* nim, AtZnzFile* fp, int remain){
  AtNifti1Extension* list  = NULL; // extension list
  AtNifti1Extension  tmp;
  uint32_t           next  = 0;    // total number of extensions
  uint32_t           lsize = 16;   // size of allocated extension list (more than count)
  uint32_t           count;
  uint32_t           extsize;
  char               extdr[4];     // extender message
  uint8_t            swap  = nim->byteorder != BYTE_ORDER;
  uint8_t            keep  = true;

  list = malloc(sizeof(AtNifti1Extension) * lsize);

  next = at_znzfile_read(fp,extdr,1,4);
  // there is no extension
  if(extdr[0] == 0) return 0;
  remain -= 4;

  // here we have extensions
  while(keep){
    count  = at_znzfile_read(fp,&tmp.esize,4,1);
    count += at_znzfile_read(fp,&tmp.ecode,4,1);
    if(count != 2){
      at_znzfile_seek(fp,-(count << 2),SEEK_CUR);
      break;
    }
    if(swap){
      tmp.esize = __bswap_32(tmp.esize);
      tmp.ecode = __bswap_32(tmp.ecode);
    }
    if(!at_nifti_check_extension(nim,tmp.esize, tmp.ecode, remain)){
      if(at_znzfile_seek(fp,-8,SEEK_CUR) < 0) keep = false;
      continue;
    }
    extsize = tmp.esize - 8;
    tmp.edata = malloc(extsize * sizeof(uint8_t));
    count = at_znzfile_read(fp,tmp.edata, 1, extsize);
    if(count < extsize){
      free(tmp.edata);
      keep = false;
    }else{
      list[next] = tmp;
      remain -= tmp.esize;
      next++;
      // get more space whether needed
      if(next = lsize){
        lsize <<= 1;
        list = realloc(list,lsize * sizeof(AtNifti1Extension));
      }
    }
  }
  nim->num_ext  = next;
  nim->ext_list = list;
}

AtNiftiFile*
at_extract_nifti_finfo(const char* name){
  AtNiftiFile* file = malloc(sizeof(AtNiftiFile));
  struct stat buf;
  stat(name,&buf);

  file->ext   = strrchr(name,'.');
  file->is_gz = strstr(file->ext,".gz") != NULL;
  file->fname = name;
  file->fsize = buf.st_size;

  return file;
}

#define bswap16(var) var = __bswap_16(var)
#define bswap32(var) var = __bswap_32(var)
#define bswap64(var) var = __bswap_64(var)

/**
 * @brief at_nifti_swap_as_analyze
 * @param h
 * @return non-zero on failure
 */
static uint8_t
at_nifti_swap_as_analyze(AtNiftiAnalyze75 *h)
{
  uint16_t* dim = h->dim;
  if( !h ) return 1;

  bswap32(h->sizeof_hdr); bswap32(h->extents);    bswap16(h->session_error);
  bswap16(dim[0]);        bswap16(dim[1]);        bswap16(dim[2]);           bswap16(dim[3]);
  bswap16(dim[4]);        bswap16(dim[5]);        bswap16(dim[6]);           bswap16(dim[7]);
  bswap16(h->unused8);    bswap16(h->unused9);    bswap16(h->unused10);      bswap16(h->unused11);
  bswap16(h->unused12);   bswap16(h->unused13);   bswap16(h->unused14);      bswap16(h->datatype);
  bswap16(h->bitpix);     bswap16(h->dim_un0);
  bswap32(h->pixdim[0]);  bswap32(h->pixdim[1]);  bswap32(h->pixdim[2]);     bswap32(h->pixdim[3]);
  bswap32(h->pixdim[4]);  bswap32(h->pixdim[5]);  bswap32(h->pixdim[6]);     bswap32(h->pixdim[7]);
  bswap32(h->vox_offset); bswap32(h->funused1);   bswap32(h->funused2);      bswap32(h->funused3);
  bswap32(h->cal_max);    bswap32(h->cal_min);    bswap32(h->compressed);    bswap32(h->verified);
  bswap32(h->glmax);      bswap32(h->glmin);      bswap32(h->views);         bswap32(h->vols_added);
  bswap32(h->start_field);bswap32(h->field_skip); bswap32(h->omax);          bswap32(h->omin);
  bswap32(h->smax);       bswap32(h->smin);

  return 0;
}

/**
 * @brief at_swap_nifti_header
 * @param h
 * @param is_nifti
 */
static void
at_nifti_swap_header(AtNifti1Header *h)
{
  uint16_t* dim = h->dim;
  float* pixdim = h->pixdim, *srow_x = h->srow_x, *srow_y = h->srow_y, *srow_z = h->srow_z;
  bswap32(h->sizeof_hdr); bswap32(h->extents);  bswap16(h->session_error);
  bswap16(dim[0]);        bswap16(dim[1]);      bswap16(dim[2]);           bswap16(dim[3]);
  bswap16(dim[4]);        bswap16(dim[5]);      bswap16(dim[6]);           bswap16(dim[7]);
  bswap32(h->intent_p1);  bswap32(h->intent_p2);bswap32(h->intent_p3);
  bswap16(h->intent_code);bswap16(h->datatype); bswap16(h->bitpix);        bswap16(h->slice_start);
  bswap32(pixdim[0]);     bswap32(pixdim[1]);   bswap32(pixdim[2]);        bswap32(pixdim[3]);
  bswap32(pixdim[4]);     bswap32(pixdim[5]);   bswap32(pixdim[6]);        bswap32(pixdim[7]);
  bswap32(h->vox_offset); bswap32(h->scl_slope);bswap32(h->scl_inter);     bswap16(h->slice_end);
  bswap32(h->cal_max);    bswap32(h->cal_min);  bswap32(h->slice_duration);bswap32(h->toffset);
  bswap32(h->glmax);      bswap32(h->glmin);    bswap16(h->qform_code);    bswap16(h->sform_code);
  bswap32(h->quatern_b);  bswap32(h->quatern_c);bswap32(h->quatern_d);     bswap32(h->qoffset_x);
  bswap32(h->qoffset_y);  bswap32(h->qoffset_z);bswap32(srow_x[0]);        bswap32(srow_x[1]);
  bswap32(srow_x[2]);     bswap32(srow_x[3]);   bswap32(srow_y[0]);        bswap32(srow_y[1]);
  bswap32(srow_y[2]);     bswap32(srow_y[3]);   bswap32(srow_z[0]);        bswap32(srow_z[1]);
  bswap32(srow_z[2]);     bswap32(srow_z[3]);
  return ;
}

static int
at_niftiimage_load(AtNiftiImage* nim, AtNiftiFile* nfp){
  AtZnzFile  * zfp;   // 00+8
  AtNiftiFile* imgnfp;// 08+8
  uint64_t*    data64;// 16+8
  uint32_t*    data32;// 24+8
  uint16_t*    data16;// 32+8
  uint64_t     n;     // 40+8
  uint64_t     i;     // 48+8
  size_t       nbytes;// 56+8
  size_t       nread; // 64+8
  long         ioff;  // 72+8 (x64)

  // extract file info

  imgnfp = at_extract_nifti_finfo(nfp->fname);
  uint64_t shape[3] = {nim->dim[2],nim->dim[1],nim->dim[0]};
  nim->ar = at_arrayu16_new(nim->ndim,shape);

  // open the file and position the filepointer
  ioff   = nim->iname_offset;
  zfp    = at_znzfile_open(nfp->fname, "rb", imgnfp->is_gz);
  at_znzfile_seek(zfp,ioff,SEEK_SET);

  // get total bytes
  nbytes = nim->ar->h.nelem * sizeof(uint16_t);

  // read it
  nread = at_znzfile_read(zfp, nim->ar->data, 1, nbytes);

  // swap bytes
  if(nim->swapsize > 1 && nim->byteorder != BYTE_ORDER){
    n      = nbytes/nim->swapsize;
    switch(nim->swapsize){
      case 2: data16 = (uint16_t*)nim->ar->data; for(i = 0; i < n; i++) *data16++ = __bswap_16(*data16); break;
      case 4: data32 = (uint32_t*)nim->ar->data; for(i = 0; i < n; i++) *data32++ = __bswap_32(*data32); break;
      case 8: data64 = (uint64_t*)nim->ar->data; for(i = 0; i < n; i++) *data64++ = __bswap_64(*data64); break;
    }
  }
  at_znzfile_close(zfp);
  if(imgnfp) free(imgnfp);
  return 0;
}

static int8_t
at_nifti_has_ascii_header(AtZnzFile* fp){
  char   buf[16];
  size_t nread;

  // Does it has more than 12 bytes?
  nread = at_znzfile_read(fp,buf,1,12);
  buf[12] = '\0';
  if(nread < 12) return -1;

  at_znzfile_rewind(fp);
  if(strcmp(buf,"<nifti_image") == 0) return 1;
  return 0;
}

static void
at_nifti_quatern_to_mat4(AtNiftiImage* nim){
  float a, b=nim->quatern[0],c=nim->quatern[1],d=nim->quatern[2],xd,yd,zd;
  /* compute a parameter from b,c,d */
  float sqr = b*b+c*c+d*d;
  a = 1.0 - sqr ;
  if( a < 1.e-7){                   /* special case */
    a = 1.0 / sqrt(sqr) ;
    b *= a ; c *= a ; d *= a ;        /* normalize (b,c,d) vector */
    a = 0.0;                        /* a = 0 ==> 180 degree rotation */
  } else{
    a = sqrt(a) ;                     /* angle = 2*arccos(a) */
  }
  xd = (nim->pixdim[0] > 0.0) ? nim->pixdim[0] : 1.0;       /* make sure are positive */
  yd = (nim->pixdim[1] > 0.0) ? nim->pixdim[1] : 1.0;
  zd = (nim->pixdim[2] > 0.0) ? nim->pixdim[2] : 1.0;

  if( nim->qfac < 0.0 ) zd = -zd ;         /* left handedness? */

  nim->qto_xyz[ 0] =       (a*a+b*b-c*c-d*d) * xd ;
  nim->qto_xyz[ 4] = 2.0 * (b*c-a*d        ) * yd ;
  nim->qto_xyz[ 8] = 2.0 * (b*d+a*c        ) * zd ;
  nim->qto_xyz[ 1] = 2.0 * (b*c+a*d        ) * xd ;
  nim->qto_xyz[ 5] =       (a*a+c*c-b*b-d*d) * yd ;
  nim->qto_xyz[ 9] = 2.0 * (c*d-a*b        ) * zd ;
  nim->qto_xyz[ 2] = 2.0 * (b*d-a*c        ) * xd ;
  nim->qto_xyz[ 6] = 2.0 * (c*d+a*b        ) * yd ;
  nim->qto_xyz[10] =       (a*a+d*d-c*c-b*b) * zd ;
  nim->qto_xyz[12] = nim->qoffset[0];
  nim->qto_xyz[13] = nim->qoffset[1];
  nim->qto_xyz[14] = nim->qoffset[2];
}
static void
at_mat4_transpose3x4(float* i, float* o){
  o[ 0]=i[ 0];  o[ 4]=i[ 1];  o[ 8]=i[ 2];  o[12]=i[ 3];
  o[ 1]=i[ 4];  o[ 5]=i[ 5];  o[ 9]=i[ 6];  o[13]=i[ 7];
  o[ 2]=i[ 8];  o[ 6]=i[ 9];  o[10]=i[10];  o[14]=i[11];
}

static void
at_mat4_inverse(float* i, float* o){
  double r11,r12,r13,r21,r22,r23,r31,r32,r33,v1,v2,v3 , deti ;
                                                      /*  INPUT MATRIX IS:  */
  r11 = i[ 0]; r12 = i[ 4]; r13 = i[ 8];  /* [ r11 r12 r13 v1 ] */
  r21 = i[ 1]; r22 = i[ 5]; r23 = i[ 9];  /* [ r21 r22 r23 v2 ] */
  r31 = i[ 2]; r32 = i[ 6]; r33 = i[10];  /* [ r31 r32 r33 v3 ] */
  v1  = i[12]; v2  = i[13]; v3  = i[14];  /* [  0   0   0   1 ] */

  deti = r11*r22*r33-r11*r32*r23-r21*r12*r33
        +r21*r32*r13+r31*r12*r23-r31*r22*r13 ;

  if( deti != 0.0l ) deti = 1.0l / deti ;

  o[ 0] = deti*( r22*r33-r32*r23) ;
  o[ 4] = deti*(-r12*r33+r32*r13) ;
  o[ 8] = deti*( r12*r23-r22*r13) ;
  o[12] = deti*(-r12*r23*v3+r12*v2*r33+r22*r13*v3
                    -r22*v1*r33-r32*r13*v2+r32*v1*r23) ;

  o[ 1] = deti*(-r21*r33+r31*r23) ;
  o[ 5] = deti*( r11*r33-r31*r13) ;
  o[ 9] = deti*(-r11*r23+r21*r13) ;
  o[13] = deti*( r11*r23*v3-r11*v2*r33-r21*r13*v3
                    +r21*v1*r33+r31*r13*v2-r31*v1*r23) ;

  o[2]  = deti*( r21*r32-r31*r22) ;
  o[6]  = deti*(-r11*r32+r31*r12) ;
  o[10] = deti*( r11*r22-r21*r12) ;
  o[14] = deti*(-r11*r22*v3+r11*r32*v2+r21*r12*v3
                    -r21*r32*v1-r31*r12*v2+r31*r22*v1) ;

  o[3]  = o[7] = o[11] = 0.0l ;
  o[15] = (deti == 0.0l) ? 0.0l : 1.0l ; /* failure flag if deti == 0 */
}
static AtNiftiImage*
at_niftiimage_new(){
  return malloc(sizeof(AtNiftiImage));
}

static AtNiftiImage*
at_niftiimage_from_nifti1(AtNifti1Header* nhdr, char* name){
  AtNiftiImage* nim  = at_niftiimage_new();
  float*        pdim = nhdr->pixdim;
  uint16_t*     dim  = nhdr->dim;
  uint16_t      d0   = dim[0];
  uint8_t       is_nifti;
  int8_t        swap;
  uint8_t        i;
  is_nifti = at_nifti_version(nhdr->magic);
  swap = (d0 > 7) && (__bswap_16(d0) <= 7);
  if(!is_nifti){
    nim->analyze75_orient = (AtAnalyze75OrientCode)*((char*)(&nhdr->qform_code));
    if(swap) at_nifti_swap_as_analyze((AtNiftiAnalyze75*) nhdr);
  }else if(swap)
    at_nifti_swap_header(nhdr);

  for(i = 2; i <= dim[0]; i++)    if( dim[i] == 0  )  dim[i] = 1;
  for(i = dim[0]+1; i <= 7; i++)  if( dim[i]  > 1  )  dim[i] = 1;
  for(i = 1; i < dim[0]; i++)     if(pdim[i] == 0.0) pdim[i] = 1.0;

  nim->nifti_type = (is_nifti)?(at_nifti_hdrimg_onefile(nhdr->magic)?AT_NIFTI_TYPE_NIFTI1_1
                                                             :AT_NIFTI_TYPE_NIFTI1_2)
                              :AT_NIFTI_TYPE_ANALYZE;
  if(swap)
#if BYTE_ORDER==BIG_ENDIAN
    nim->byteorder = LITTLE_ENDIAN;
#else
    nim->byteorder = BIG_ENDIAN;
#endif
  else nim->byteorder = BYTE_ORDER;

  nim->ndim     = nhdr->dim[0];
  memcpy(nim->dim, &nhdr->dim[1], 7*sizeof(uint16_t));
  nim->datatype = nhdr->datatype;
  at_nifti_datatype_sizes(nim->datatype,&(nim->swapsize));
  memcpy(nim->pixdim, &nhdr->pixdim[1], 7*sizeof(uint16_t));
  if(!is_nifti || nhdr->qform_code <= 0){
    memset(nim->qto_xyz,0,15*sizeof(float));
    nim->qto_xyz[ 0] = nim->pixdim[0];
    nim->qto_xyz[ 5] = nim->pixdim[1];
    nim->qto_xyz[10] = nim->pixdim[2];
    nim->qform_code  = AT_NIFTI_XFORM_UNKNOWN;
  }else{
    // copy quatern and qoffset
    memcpy(nim->quatern, &nhdr->quatern_b,sizeof(float)*6);
    nim->qfac = (nhdr->pixdim[0] < 0.0) ? -1.0:1.0;
    memset(&nim->qto_xyz[12],0,3*sizeof(float));
    at_nifti_quatern_to_mat4(nim);
    nim->qform_code = nhdr->qform_code;
  }
  nim->qto_xyz[15]   = 1.0;
  at_mat4_inverse(nim->qto_xyz, nim->qto_ijk);
  if(!is_nifti || nhdr->sform_code <= 0)
    nim->sform_code  = AT_NIFTI_XFORM_UNKNOWN;
  else{
    at_mat4_transpose3x4(nhdr->srow_x,nim->sto_xyz);
    nim->sto_ijk[3]     = nim->sto_ijk[7] = nim->sto_ijk[11] = 0.0;
    nim->sto_ijk[15]    = 1.0;
    at_mat4_inverse(nim->sto_xyz, nim->sto_ijk);
    nim->sform_code     = nhdr->sform_code;
  }
  if(is_nifti){
    nim->scl_slope      = nhdr->scl_slope;
    nim->scl_inter      = nhdr->scl_inter;
    nim->slice_end      = nhdr->slice_end;
    nim->slice_code     = nhdr->slice_code;
    nim->xyz_units      = at_xyzt_to_space(nhdr->xyzt_units);
    nim->time_units     = at_xyzt_to_time(nhdr->xyzt_units);
    nim->freq_dim       = at_dim_info_to_freq_dim (nhdr->dim_info);
    nim->phase_dim      = at_dim_info_to_phase_dim(nhdr->dim_info);
    nim->slice_dim      = at_dim_info_to_slice_dim(nhdr->dim_info);
    nim->slice_start    = nhdr->slice_start;
    nim->slice_duration = nhdr->slice_duration;
  }
  nim->cal_min = nhdr->cal_min;
  nim->cal_max = nhdr->cal_max;
  memcpy(nim->descrip , nhdr->descrip , 79); nim->descrip[79] ='\0';
  memcpy(nim->aux_file, nhdr->aux_file, 23); nim->aux_file[23]='\0';
  if(is_nifti && at_nifti_hdrimg_onefile(nhdr->magic))
    nim->iname_offset = max((size_t)nhdr->vox_offset,sizeof(nhdr));
  else
    nim->iname_offset = (size_t)nhdr->vox_offset;
  if(name != NULL){
    nim->fname = name;
    nim->iname = name;
  }
  nim->num_ext  = 0;
  nim->ext_list = NULL;
  return nim;
}
static AtNiftiImage*
at_niftiimage_from_ascii(const char* str, size_t* bread){
  AtNiftiImage* nim = NULL;
  size_t        spos;
  size_t        nn;
  char          lhs[1024];
  char          rhs[1024];

  spos = 0;
  sscanf(str+spos,"%1023s%l", lhs, &nn);
  spos += nn;

  return nim;
}

// Close file, free memory and return
#define at_niftiimage_read_close_return(ret) \
  if(zfp) at_znzfile_close(zfp); \
  if(nfp) free(nfp);             \
  return ret;

// Set error, close file, free memory and return
#define at_niftiimage_read_error(ret,...) \
  at_error_set(error, __VA_ARGS__);  \
  at_niftiimage_read_close_return(ret)

/*=============================================================================
 PUBLIC API
 ============================================================================*/

AtNiftiImage*
at_niftiimage_read(const char *name, uint8_t read_data, AtError** error){
  AtNiftiImage  * nim   = NULL; // nifti image
  AtZnzFile     * zfp   = NULL; // znz file pointer
  AtNiftiFile   * nfp;          // nifti file pointer
  uint8_t       * sbuf;         //
  uint64_t        nbytes;       //
  uint64_t        txtsize;      //
  uint64_t        remaining;    //
  uint64_t        nh;           // number of bytes read
  AtNifti1Header  nhdr;         // nifti 1 header (first bytes of a binary header)
  int8_t          ascii;        // ascii header?
  uint8_t         pd[7];        // padding

  // Extract necessary file information
  nfp = at_extract_nifti_finfo(name);

  // Open File
  zfp = at_znzfile_open(name,"rb",nfp->is_gz);
  if(zfp == NULL){
    at_niftiimage_read_error(NULL,"[at_niftiimage_read] Couldn't open %s", name);
  }

  // See if header is ascii (ascii=0||1) and nifti (ascii=1) or not (ascii=-1)
  // Remember: it's a binary file
  ascii = at_nifti_has_ascii_header(zfp);
  switch(ascii){
  case -1: // the file is smaller than 12 bytes (size of ascii nifti header)
    at_niftiimage_read_error(NULL,"[at_niftiimage_read] Couldn't open %s", name);
  case 1:  // the file is ascii and begins with ascii nifti header
    sbuf = malloc((nfp->fsize+1) * sizeof(char));
    at_znzfile_read(zfp,sbuf,1,nfp->fsize);
    sbuf[nfp->fsize] = '\0';

    nim  = at_niftiimage_from_ascii(sbuf, &txtsize);
    nim->nifti_type = AT_NIFTI_TYPE_ASCII;
    nbytes = sizeof(uint16_t) * nim->ar->h.nelem;
    remaining = nfp->fsize - txtsize - nbytes;
    if(remaining > 4){
      at_znzfile_seek(zfp, txtsize, SEEK_SET);
      at_niftiimage_read_extensions(nim,zfp,remaining);
    }
    nim->iname_offset = 0;
    free(sbuf);
    break;
  case 0:  // the file doesn't have ascii nifti header (it's binary)
    // read binary header
    nh = at_znzfile_read(zfp, &nhdr, 1, sizeof(nhdr));

    if(nh < sizeof(nhdr)){
      at_niftiimage_read_error(NULL,"[at_niftiimage_read] file smaller than expected binary nifti header");
    }
    nim = at_niftiimage_from_nifti1(&nhdr,(char*)name);

    if(nim == NULL){
      at_niftiimage_read_error(NULL,"[at_niftiimage_read] can't create nifti image from nifti1 header. Memory unavailable? Wrong/Corrupt file?");
    }
    if(at_nifti_hdrimg_onefile(nhdr.magic)) remaining = nim->iname_offset - sizeof(nhdr);
    else                                    remaining = nfp->fsize        - sizeof(nhdr);

  }
  if(remaining >= 16){
    at_niftiimage_read_extensions(nim, zfp, remaining);
  }

  if(read_data){

    if(at_niftiimage_load(nim, nfp) < 0){
      free(nim);
      at_niftiimage_read_error(NULL,"[at_niftiimage_read] couldn't load image");
    }
  }
  at_niftiimage_read_close_return(nim);
}

void
at_niftiimage_destroy(AtNiftiImage** np){
  AtNiftiImage* n = *np;
  if(n){
    if(n->ar) at_arrayu16_destroy(&n->ar);
    free(n);
  }
  *np = NULL;
}
