/*
 * ae.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Pimenta
 */

#include <stdio.h>

#include "BMPFILE.h"

int main() {
  
  BMPFILE* bfp = BMP_Read("testImage.bmp");
  for (int c = 1, i = 0; c;) {
    c = 0;
    for (int j = 0; j < 8; j++, i++) {
      c |= ((((char*)bfp->buf)[i] & 0x1) << j);
    }
    if (c) {
      printf("%c", (char)c);
    }
  }
  printf("\n");
  BMP_Free(bfp);
  
  return 0;
}
