#include "qsortrank.h"

#include <errno.h>
#include <string.h>

static void memswap (void *a, void *b, size_t len) {
  char    c[len];
  memcpy (c, a, len);
  memcpy (a, b, len);
  memcpy (b, c, len);
}

// do imcomplete qsort to find rank
int qsortrank (void *data, size_t nmemb, size_t size, int rank, int (*compare) (const void *, const void *)) {
  int     pl = 0;
  int     pr = nmemb;
  // ------------------
  // check arguments
  // ------------------
  // negative rank to find from the tail
  if (rank < 0)
    rank += nmemb;
  // range check
  if (rank < 0 || rank >= nmemb) {
    errno = ERANGE;
    return -1;
  }
  while (pl < pr) {
    // select pivot
    int     i = pl;
    int     j = pr;
    int     p = i;
    if (p != rank)
      memswap (data + size * p, data + size * rank, size);
    do {
      do {
        i++;
      } while (i <= j && compare (data + size * i, data + size * p) < 0);
      do {
        j--;
      } while (i <= j && compare (data + size * p, data + size * j) < 0);
      if (i < j)
        memswap (data + size * i, data + size * j, size);
    } while (i < j);
    memswap (data + size * p, data + size * j, size);
    // finding out the rank
    if (j == rank)
      return 0;
    if (j < rank)
      // rank is in right group
      pl = j;
    else
      // rank is in left group
      pr = j;
  }
  // sorted completely
  return 0;
}
