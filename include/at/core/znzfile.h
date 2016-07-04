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
#ifndef AT_ZNZFILE_H
#define AT_ZNZFILE_H
#include <at/core/macro.h>
#include <at/core/error.h>
#include <stdint.h>
#include <stdio.h>
#include <zlib.h>
#include <stdarg.h>
AT_BEGIN_DECLS

typedef struct AtZnzFile{
  FILE* nzfptr; /*00+8*//*!< pointer to file */
#ifdef HAVE_ZLIB
  gzFile zfptr; /*08+8*//*!< pointer to zlib file */
#endif
  uint8_t withz;/*16+1*//*!< compressed file? */
  uint8_t pd[7];/*17+7*//*!< padding */
                // Total: 24 bytes (or 16 without zlib)
}AtZnzFile;

/**
 * @brief at_znzfile_open
 * @param path
 * @param mode
 * @param withz
 * @return
 */
AtZnzFile*
at_znzfile_open(const char* path, const char* mode, uint8_t withz);
/**
 * @brief at_znzfile_close
 * @param fp
 * @return
 */
int
at_znzfile_close(AtZnzFile* fp);
/**
 * @brief at_znzfile_read
 * @param fp
 * @param buf
 * @param size
 * @param nmemb
 * @return
 */
size_t
at_znzfile_read(AtZnzFile* fp,void* buf, size_t size, size_t nmemb);
/**
 * @brief at_znzfile_write
 * @param fp
 * @param buf
 * @param size
 * @param nmemb
 * @return
 */
size_t
at_znzfile_write(AtZnzFile* fp,const void* buf, size_t size, size_t nmemb);
/**
 * @brief at_znzfile_seek
 * @param file
 * @param offset
 * @param whence
 * @return
 */
long
at_znzfile_seek(AtZnzFile* file, long offset, int whence);
/**
 * @brief at_znzfile_tell
 * @param file
 * @return
 */
long
at_znzfile_tell(AtZnzFile* file);
/**
 * @brief at_znzfile_puts
 * @param str
 * @param file
 * @return
 */
int
at_znzfile_puts(const char * str, AtZnzFile* file);
/**
 * @brief at_znzfile_rewind
 * @param stream
 * @return
 */
int
at_znzfile_rewind(AtZnzFile* stream);

/**
 * @brief at_znzfile_gets
 * @param str
 * @param size
 * @param file
 * @return
 */
char *
at_znzfile_gets(char* str, int size, AtZnzFile* file);
/**
 * @brief at_znzfile_flush
 * @param file
 * @return
 */
int
at_znzfile_flush(AtZnzFile* file);
/**
 * @brief at_znzfile_eof
 * @param file
 * @return
 */
int
at_znzfile_eof(AtZnzFile* file);
/**
 * @brief at_znzfile_putc
 * @param c
 * @param file
 * @return
 */
int
at_znzfile_putc(int c, AtZnzFile* file);
/**
 * @brief at_znzfile_getc
 * @param file
 * @return
 */
int
at_znzfile_getc(AtZnzFile* file);
/**
 * @brief at_znzfile_printf
 * @param stream
 * @param format
 * @return
 */
int
at_znzfile_printf(AtZnzFile* stream, const char* format, ...);


AT_END_DECLS
#endif
