#ifndef __IMAGE_WRITER_H
#define __IMAGE_WRITER_H

#include <stdlib.h>
#include "png.h"

template<class ImageType>
class ImageWriter {
public:
  static bool WritePNG(ImageType& image, const char* filename) {
    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;

    /* open the file */
    fp = fopen(filename, "wb");
    if (fp == NULL)
      return false;

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also check that
     * the library version is compatible with the one used at compile time,
     * in case we are using dynamically linked libraries.  REQUIRED.
    */
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (png_ptr == NULL) {
     fclose(fp);
     return false;
   }

   /* Allocate/initialize the image information data.  REQUIRED */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
     fclose(fp);
     png_destroy_write_struct(&png_ptr, NULL);
     return false;
   }

   png_init_io(png_ptr, fp);

   /* Set the image information here.  Width and height are up to 2^31,
    * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
    * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
    * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
    * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
    * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
    * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
    */
   png_set_IHDR(png_ptr, info_ptr, (png_uint_32)image.width(), (png_uint_32)image.height(), 8, 
     PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, 
     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   /* Write the file header information.  REQUIRED */
   png_write_info(png_ptr, info_ptr);


   /* The easiest way to write the image (you may have a different memory
    * layout, however, so choose what fits your needs best).  You need to
    * use the first method if you aren't handling interlacing yourself.
    */
   if (image.height() > PNG_UINT_32_MAX/png_sizeof(png_bytep))
     png_error (png_ptr, "Image is too tall to process in memory");

   /* The number of passes is either 1 for non-interlaced images,
    * or 7 for interlaced images.
    */
    /* If you are only writing one row at a time, this works */
    for (int y = 0; y < (int)image.height(); y++)
    {
      png_bytep row_pointer = (png_bytep)image.row_pointer(y);
      png_write_rows(png_ptr, &row_pointer, 1);
    }

   /* You can write optional chunks like tEXt, zTXt, and tIME at the end
    * as well.  Shouldn't be necessary in 1.1.0 and up as all the public
    * chunks are supported and you can use png_set_unknown_chunks() to
    * register unknown chunks into the info structure to be written out.
    */

   /* It is REQUIRED to call this to finish writing the rest of the file */
   png_write_end(png_ptr, info_ptr);

   //png_free(png_ptr, palette);
   //palette=NULL;

   /* Similarly, if you png_malloced any data that you passed in with
      png_set_something(), such as a hist or trans array, free it here,
      when you can be sure that libpng is through with it. */
   //png_free(png_ptr, trans);
   //trans=NULL;

   /* clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   fclose(fp);

   return true;
  }
};


#endif  // __IMAGE_WRITER_H
