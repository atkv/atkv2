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
#include <at/imgproc/znzfile.h>
#include <string.h>
#include <stdlib.h>

#undef ZNZ_MAX_BLOCK_SIZE
#define ZNZ_MAX_BLOCK_SIZE (1<<30)

AtZnzFile*
at_znzfile_new(){
  return malloc(sizeof(AtZnzFile));
}

AtZnzFile*
at_znzfile_open(const char* path, const char* mode, uint8_t withz){
  AtZnzFile* fp = at_znzfile_new();
  memset(fp,0,sizeof(AtZnzFile));
#ifdef HAVE_ZLIB
  fp->withz = withz;
  if (withz) {
    if((fp->zfptr = gzopen(path,mode)) == NULL) {
      free(fp);
      return NULL;
    }
  } else {
#endif
    if((fp->nzfptr = fopen(path,mode)) == NULL) {
      free(fp);
      return NULL;
    }
#ifdef HAVE_ZLIB
  }
#endif

  return fp;
}

int
at_znzfile_close(AtZnzFile* fp){
  int retval = 0;

  if (fp != NULL) {
#ifdef HAVE_ZLIB
    if (fp->zfptr!=NULL)  { retval = gzclose(fp->zfptr); }
#endif
    if (fp->nzfptr!=NULL) { retval = fclose(fp->nzfptr); }

    free(fp);
  }
  return retval;
}

size_t
at_znzfile_read(AtZnzFile* fp,void* buf, size_t size, size_t nmemb){
  size_t     remain = size*nmemb;
  uint32_t   nread;
  uint32_t   n2read;
  char     * cbuf = (char *)buf;
  if (fp == NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (fp->zfptr!=NULL) {
    /* gzread/write take unsigned int length, so maybe read in int pieces
       (noted by M Hanke, example given by M Adler)   6 July 2010 [rickr] */
    while( remain > 0 ) {
       n2read = (remain < ZNZ_MAX_BLOCK_SIZE) ? remain : ZNZ_MAX_BLOCK_SIZE;
       nread = gzread(fp->zfptr, (void *)cbuf, n2read);
       if( nread < 0 ) return nread; /* returns -1 on error */

       remain -= nread;
       cbuf += nread;

       /* require reading n2read bytes, so we don't get stuck */
       if( nread < (int)n2read ) break;  /* return will be short */
    }

    /* warn of a short read that will seem complete */
    if( remain > 0 && remain < size )
       fprintf(stderr,"** znzread: read short by %u bytes\n",(unsigned)remain);

    return nmemb - remain/size;   /* return number of members processed */
  }
#endif
  return fread(buf,size,nmemb,fp->nzfptr);
}

size_t
at_znzfile_write(AtZnzFile* fp, const void* buf, size_t size, size_t nmemb){
  size_t     remain = size*nmemb;
  const char * cbuf = (const char *)buf;
  unsigned   n2write;
  int        nwritten;
  if (fp==NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (fp->zfptr!=NULL) {
    while( remain > 0 ) {
      n2write = (remain < ZNZ_MAX_BLOCK_SIZE) ? remain : ZNZ_MAX_BLOCK_SIZE;
      nwritten = gzwrite(fp->zfptr, (const void *)cbuf, n2write);

      /* gzread returns 0 on error, but in case that ever changes... */
      if( nwritten < 0 ) return nwritten;

      remain -= nwritten;
      cbuf += nwritten;

      /* require writing n2write bytes, so we don't get stuck */
      if( nwritten < (int)n2write ) break;
    }

    /* warn of a short write that will seem complete */
    if( remain > 0 && remain < size )
      fprintf(stderr,"** znzwrite: write short by %u bytes\n",(unsigned)remain);

    return nmemb - remain/size;   /* return number of members processed */
  }
#endif
  return fwrite(buf,size,nmemb,fp->nzfptr);
}

long
at_znzfile_seek(AtZnzFile* file, long offset, int whence)
{
  if (file==NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (file->zfptr!=NULL) return (long) gzseek(file->zfptr,offset,whence);
#endif
  return fseek(file->nzfptr,offset,whence);
}

int
at_znzfile_rewind(AtZnzFile* stream)
{
  if (stream==NULL) { return 0; }
#ifdef HAVE_ZLIB
  /* On some systems, gzrewind() fails for uncompressed files.
     Use gzseek(), instead.               10, May 2005 [rickr]

     if (stream->zfptr!=NULL) return gzrewind(stream->zfptr);
  */

  if (stream->zfptr!=NULL) return (int)gzseek(stream->zfptr, 0L, SEEK_SET);
#endif
  rewind(stream->nzfptr);
  return 0;
}

long
at_znzfile_tell(AtZnzFile* file)
{
  if (file==NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (file->zfptr!=NULL) return (long) gztell(file->zfptr);
#endif
  return ftell(file->nzfptr);
}

int
at_znzfile_puts(const char * str, AtZnzFile* file)
{
  if (file==NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (file->zfptr!=NULL) return gzputs(file->zfptr,str);
#endif
  return fputs(str,file->nzfptr);
}


char *
at_znzfile_gets(char* str, int size, AtZnzFile* file)
{
  if (file==NULL) { return NULL; }
#ifdef HAVE_ZLIB
  if (file->zfptr!=NULL) return gzgets(file->zfptr,str,size);
#endif
  return fgets(str,size,file->nzfptr);
}


int
at_znzfile_flush(AtZnzFile* file)
{
  if (file==NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (file->zfptr!=NULL) return gzflush(file->zfptr,Z_SYNC_FLUSH);
#endif
  return fflush(file->nzfptr);
}


int
at_znzfile_eof(AtZnzFile* file)
{
  if (file==NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (file->zfptr!=NULL) return gzeof(file->zfptr);
#endif
  return feof(file->nzfptr);
}


int
at_znzfile_putc(int c, AtZnzFile* file)
{
  if (file==NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (file->zfptr!=NULL) return gzputc(file->zfptr,c);
#endif
  return fputc(c,file->nzfptr);
}


int
at_znzfile_getc(AtZnzFile* file)
{
  if (file==NULL) { return 0; }
#ifdef HAVE_ZLIB
  if (file->zfptr!=NULL) return gzgetc(file->zfptr);
#endif
  return fgetc(file->nzfptr);
}

int
at_znzfile_printf(AtZnzFile* stream, const char* format, ...){
  va_list va;
  char   *tmpstr;
  int     retval=0;
  size_t  size;
  if (stream==NULL) { return 0; }
  va_start(va, format);
#ifdef HAVE_ZLIB
  if (stream->zfptr!=NULL) {
    size   = strlen(format) + (1<<16);
    tmpstr = malloc(size);
    vsprintf(tmpstr,format,va);
    retval = gzprintf(stream->zfptr,"%s",tmpstr);
    free(tmpstr);
  } else {
#endif
   retval  = vfprintf(stream->nzfptr,format,va);
#ifdef HAVE_ZLIB
  }
#endif
  va_end(va);
  return retval;
}
