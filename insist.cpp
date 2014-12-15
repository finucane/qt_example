#include <stdarg.h>
#include <stdio.h>
#include "insist.h"

InsistException::InsistException (const char*fmt,...)
{
  va_list args;
  va_start (args, fmt);
  vsnprintf (message, MAX_MESSAGE, fmt, args);
  va_end (args);
}

