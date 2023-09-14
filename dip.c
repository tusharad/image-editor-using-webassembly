#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <emscripten/emscripten.h>

typedef struct Pixel{
	unsigned char r,g,b;
}Pixel;

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

 EXTERN EMSCRIPTEN_KEEPALIVE char* mirrorPPM(const char* ppmData) {
    char* header = NULL;
    int width, height, maxColor;
    int* image = NULL;

    // Parse PPM header from the string
    char* inputCopy = strdup(ppmData);  // Make a copy to avoid modifying the original
    header = strtok(inputCopy, "\n");
    
    // Parse image dimensions and max color value
    char* line = strtok(NULL, "\n");
    sscanf(line,"%d %d",&width,&height);
  

    line = strtok(NULL, "\n");
    sscanf(line,"%d",&maxColor);
  
    // Allocate memory for the image data
    size_t imageDataSize = width * height * 3 * sizeof(int);
    image = (int*)malloc(imageDataSize);

       // Parse image data from the string
    char* dataStart = strchr(ppmData, '\n');
    if (dataStart != NULL) {
        dataStart++;  // Move past the first newline character
        char* token = strtok(dataStart, " \t\n");
        for (int i = 0; i < width * height * 3; i++) {
            if (token == NULL || sscanf(token, "%d", &image[i]) != 1) {
                fprintf(stderr, "Invalid image data\n");
                free(header);
                free(image);
                free(inputCopy);
                return NULL;
            }
            token = strtok(NULL, " \t\n");
        }
    }

    // Mirror the image data
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width / 2; j++) {
            for (int k = 0; k < 3; k++) {
                int temp = image[(i * width + j) * 3 + k];
                image[(i * width + j) * 3 + k] = image[(i * width + (width - 1 - j)) * 3 + k];
                image[(i * width + (width - 1 - j)) * 3 + k] = temp;
            }
        }
    }

    // Create a string for the mirrored image
    char* mirroredImage = (char*)malloc(imageDataSize * 2);
    int offset = 0;

    // Write the header to the output string
    offset += sprintf(mirroredImage, "P3\n%d %d\n%d\n", width, height, maxColor);

    // Write the mirrored image data to the output string
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                offset += sprintf(mirroredImage + offset, "%d ", image[(i * width + j) * 3 + k]);
            }
        }
        offset += sprintf(mirroredImage + offset, "\n");
    }

    free(inputCopy);
    free(image);
    return mirroredImage;
}

int main(){
  return 0;
}