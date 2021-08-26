#include "memstream.h"
#include "../../impl/open_memstream.h"

#include <stdlib.h>

bool
u_memstream_open(struct u_memstream *mem, char **bufp, size_t *sizep)
{
#if defined(_WIN32)
    bool success = false;

   char path[MAX_PATH];
   DWORD dwResult = GetTempPath(MAX_PATH, path);
   if ((dwResult > 0) && (dwResult < MAX_PATH)) {
      char *temp = mem->temp;
      UINT uResult = GetTempFileName(path, "MEMSTREAM", 0, temp);
      if (uResult != 0) {
         FILE *f = fopen(temp, "w+b");
         success = f != NULL;
         if (success)
         {
            mem->f = f;
            mem->bufp = bufp;
            mem->sizep = sizep;
         }
      }
   }

   return success;
#elif defined(__APPLE__)
    return false;
#else
    FILE *const f = open_memstream(bufp, sizep);
    mem->f = f;
    return f != NULL;
#endif
}

void
u_memstream_close(struct u_memstream *mem)
{
    FILE *const f = mem->f;

#ifdef _WIN32
    long size = ftell(f);
   if (size > 0) {
      char *buf = malloc(size);
      fseek(f, 0, SEEK_SET);
      fread(buf, 1, size, f);

      *mem->bufp = buf;
      *mem->sizep = size;
   }

   remove(mem->temp);
#endif

    fclose(f);
}
