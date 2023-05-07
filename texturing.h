#ifndef TEXTURING_H
#define TEXTURING_H

void createTexture(char* path, unsigned int *texture);
void renderImage (float width, float height, float xPosition, float yPosition, unsigned int texture);
void renderImageFromMatrix (float vertices[], unsigned int texture);


#endif TEXTURING_H
