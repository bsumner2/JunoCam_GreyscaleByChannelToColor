/** (C) 14 of October, 2025 Burt Sumner */
/** Free to use, but this copyright message must remain here */

#ifndef _ERROR_IO_
#define _ERROR_IO_

#ifdef __cplusplus
extern "C" {
#endif  /* C++ Name mangler guard */
#include <stdio.h>
#define PERRF(fmt, ...) fprintf(stderr, "\x1b[1;31m[Error]:\x1b[0m " fmt, __VA_ARGS__)
#define PERR(s) fputs("\x1b[1;31m[Warning]:\x1b[0m " s, stderr)

#define WARNF(fmt, ...) fprintf(stderr, "\x1b[1;33m[Warning]:\x1b[0m " fmt, __VA_ARGS__)
#define WARN(s) fputs("\x1b[1;33m[Warning]:\x1b[0m " s, stderr)

#ifdef __cplusplus
}
#endif  /* C++ Name mangler guard */

#endif  /* _ERROR_IO_ */
