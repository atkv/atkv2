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
#if !defined(AT_CORE_H_INSIDE)
#error "Only <at/core.h> can be included directly."
#endif
#ifndef AT_ERROR_H
#define AT_ERROR_H
#include <at/core.h>
AT_BEGIN_DECLS
/**
 * @brief error
 */
typedef struct AtError{
  char* message; /*!< error message */
}AtError;
/**
 * @brief at_error_set
 * @param error
 * @param msg_pattern
 */
void
at_error_set(AtError** error, char* msg_pattern, ...);
/**
 * @brief at_error_destroy
 * @param error_ptr
 */
void
at_error_destroy(AtError **error_ptr);
AT_END_DECLS
#endif
