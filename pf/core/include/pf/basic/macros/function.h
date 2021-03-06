/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id function.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2016/05/04 22:43
 * @uses The base define function macros.
*/
#ifndef PF_BASIC_MACROS_FUNCTION_H_
#define PF_BASIC_MACROS_FUNCTION_H_

#include "pf/basic/macros/platform.h"

//c max/min, use std::max<T> will better.
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

//c output/string functions
#if OS_WIN
#ifndef snprintf
#define snprintf _snprintf
#endif
#ifndef stricmp
#define stricmp _stricmp
#endif
#ifndef strnicmp
#define strnicmp _strnicmp
#endif
#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif
#define strtoint64(pointer,endpointer,base) \
  _strtoi64(pointer,endpointer,base)
#define strtouint64(pointer,endpointer,base) \
  _strtoui64(pointer,endpointer,base)
#elif OS_UNIX
#ifndef stricmp
#define stricmp strcasecmp
#endif
#ifndef strnicmp
#define strnicmp strncasecmp
#endif
#define strtoint64(pointer,endpointer,base) strtoll(pointer,endpointer,base)
#define strtouint64(pointer,endpointer,base) strtoull(pointer,endpointer,base) 
#endif

#if OS_WIN
#define access _access
#define mkdir(dir,mode) _mkdir(dir)
#endif

#ifndef is_null
#define is_null(x) (nullptr == (x))
#endif

#define POINTER_TOINT64(pointer) \
  (int64_t)(reinterpret_cast<int64_t *>(pointer))
#define INT64_TOPOINTER(type,number) \
  reinterpret_cast<type *>((int64_t *)(number))

#define safe_delete(ptr) if (ptr) { delete ptr; ptr = nullptr; }
#define safe_delete_array(ptr) if (ptr) { delete[] ptr; ptr = nullptr; }

#endif //PF_BASIC_MACROS_FUNCTION_H_
