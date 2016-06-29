#include <at/core/error.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

AtError*
at_error_new(){
  AtError* error = malloc(sizeof(AtError));
  error->message = NULL;
  return error;
}

void
at_error_set(AtError **error_ptr, char *msg_pattern, ...){
  AtError* error;
  va_list  args;
  size_t   needed;

  if(*error_ptr == NULL) *error_ptr = at_error_new();
  error = *error_ptr;
  error->message = realloc(error->message,256);

  va_start(args, msg_pattern);
  needed = vsnprintf(error->message, 256, msg_pattern, args);
  error->message = realloc(error->message,needed);
  va_end(args);
}


void
at_error_destroy(AtError **error_ptr){
  AtError* error = *error_ptr;
  if(error){
    free(error->message);
    free(error);
  }
  *error_ptr = NULL;
}
