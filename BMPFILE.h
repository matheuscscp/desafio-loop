/*
 * BMPFILE.h
 *
 *  Created on: Jul 11, 2014
 *      Author: Pimenta
 */

#ifndef BMPFILE_H_
#define BMPFILE_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct BMPFILE {
  // unused 16 bit field to align this data structure
  uint16_t unused0;
  
  // bitmap header
  uint16_t magic_number;
  uint32_t file_size;
  uint16_t unused1;
  uint16_t unused2;
  uint32_t data_offset;
  uint32_t header_size;
  uint32_t width;
  uint32_t height;
  uint16_t planes;
  uint16_t bpp;
  uint32_t compression;
  uint32_t data_size;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  uint32_t palette_size;
  uint32_t important_colors;
  
  // number of bytes in one row (not part of the bitmap header)
  uint16_t row_size;
  
  // pixels
  void* buf;
  
  // header and footer buffers
  uint32_t header_footer_size;
  uint32_t footer_size;
  void* header_footer;
  void* footer;
} BMPFILE;

BMPFILE* BMP_Read(const char* fn) {
  BMPFILE* bfp = 0;
  FILE* fp = fopen(fn, "rb");
  if (fp) {
    bfp = (BMPFILE*)malloc(sizeof(BMPFILE));
    bfp->unused0 = 0;
    fread(&bfp->magic_number, 54, 1, fp);
    bfp->row_size = 4*((bfp->width*bfp->bpp + 31)/32);
    bfp->header_footer_size = bfp->data_offset - 54;
    if (bfp->header_footer_size) {
      bfp->header_footer = malloc(bfp->header_footer_size);
      fread(bfp->header_footer, bfp->header_footer_size, 1, fp);
    }
    bfp->buf = malloc(bfp->width*bfp->height*bfp->bpp/8);
    fread(bfp->buf, bfp->width*bfp->height*bfp->bpp/8, 1, fp);
    bfp->footer_size = bfp->file_size - ftell(fp);
    if (bfp->footer_size) {
      bfp->footer = malloc(bfp->footer_size);
      fread(bfp->footer, bfp->footer_size, 1, fp);
    }
    fclose(fp);
  }
  return bfp;
}

void BMP_Write(const char* fn, BMPFILE* bfp) {
  if (bfp) {
    FILE* fp = fopen(fn, "wb");
    fwrite(&bfp->magic_number, 54, 1, fp);
    if (bfp->header_footer_size) {
      fwrite(bfp->header_footer, bfp->header_footer_size, 1, fp);
    }
    fwrite(bfp->buf, bfp->width*bfp->height*bfp->bpp/8, 1, fp);
    if (bfp->footer_size) {
      fwrite(bfp->footer, bfp->footer_size, 1, fp);
    }
    fclose(fp);
  }
}

void BMP_Free(BMPFILE* bfp) {
  if (bfp) {
    free(bfp->buf);
    if (bfp->header_footer_size) {
      free(bfp->header_footer);
    }
    if (bfp->footer_size) {
      free(bfp->footer);
    }
    free(bfp);
  }
}

#endif /* BMPFILE_H_ */
