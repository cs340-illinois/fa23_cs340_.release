#include "png-hideGIF.h" 

int png_hideGIF(const char *png_filename_source, const char *gif_filename, const char *png_filename_out) {
#ifdef SOLUTION
  // Open the image file specified in png_filename_source for reading as a PNG:
  PNG *png = PNG_open(png_filename_source, "r");
  if (!png) { return ERROR_INVALID_FILE; }

  // Open the gif file specified in gif_filename for reading:
  FILE *gif = fopen(gif_filename, "r");

  // Open the image file specified in png_filename_out for writing as a PNG:
  PNG *out = PNG_open(png_filename_out, "w");
  if (!out) { 
    fclose(gif);
    return ERROR_INVALID_FILE; 
  }

  // Get the gif size
  fseek(gif, 0, SEEK_END);
  long gifSize = ftell(gif);
  fseek(gif, 0, SEEK_SET);

  // Get the gif data
  unsigned char *gifData = malloc(gifSize + 1);
  fread(gifData, 1, gifSize, gif);

  // Create the new chunk
  PNG_Chunk *gifChunk = malloc(sizeof(PNG_Chunk));
  gifChunk->len = gifSize;
  gifChunk->type[0] = 'u';
  gifChunk->type[1] = 'i';
  gifChunk->type[2] = 'u';
  gifChunk->type[3] = 'c';
  gifChunk->data = gifData;

  // Go to the IEND chunk
  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    PNG_Chunk chunk;
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      fclose(gif);
      PNG_close(out);
      return ERROR_INVALID_CHUNK_DATA;
    }

    // Check for the "IEND" chunk to exit:
    if (strcmp(chunk.type, "IEND") == 0) {
      // Write the new `uiuc` CHUNK
      PNG_write(out, gifChunk);

      // Write the IEND again and return
      PNG_write(out, &chunk);
      PNG_free_chunk(&chunk);
      break;
    } else {
      PNG_write(out, &chunk);
    }

    // Free the memory associated with the chunk we just read:
    PNG_free_chunk(&chunk);
  }

  // Close the image and gif files:
  fclose(gif);
  PNG_close(png);
  PNG_close(out);

  // Free the GIF chunk:
  free(gifData);
  free(gifChunk);
  return 0;
#else
  return 255;  // Change the to a zero to indicate success, when your implementaiton is complete.
#endif
}
