#include "png-extractGIF.h"

int png_extractGIF(const char *png_filename, const char *gif_filename) {
#ifdef SOLUTION
  // Open the image file specified in argv[1] for reading:
  PNG *png = PNG_open(png_filename, "r");
  if (!png) { return ERROR_INVALID_FILE; }

  // Open the gif file specified in argv[2] for writing:
  FILE *gif = fopen(gif_filename, "w");

  // Find the uiuc chunk
  PNG_Chunk chunk;
  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      fclose(gif);
      return ERROR_NO_UIUC_CHUNK;
    }

    // Check for the uiuc chunk to exit:
    if ( strcmp(chunk.type, "uiuc") == 0 ) {
      break;
    }

    // Check for IEND to exit
    if ( strcmp(chunk.type, "IEND") == 0 ) {
      PNG_close(png);
      PNG_free_chunk(&chunk);
      fclose(gif);
      return ERROR_NO_UIUC_CHUNK;
    }
    
    PNG_free_chunk(&chunk);
  }

  // Write the gif data into the gif file
  fwrite(chunk.data, chunk.len, 1, gif);

  // Close the files and free the chunk
  fclose(gif);
  PNG_close(png);
  PNG_free_chunk(&chunk);
  return 0;
#else
  return 255;  // Change the to a zero to indicate success, when your implementaiton is complete.
#endif
}
