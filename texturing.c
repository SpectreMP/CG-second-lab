#include "texturing.h"
#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"


void createTexture(char* path, unsigned int *texture)
{


    int width, height, nrChannels;

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0); //Считываем данные изображения в память
    printf("%i %i %i\n", width, height, nrChannels);


    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);                                  //Биндим её, чтобы последующие текстурные команды применялись к ней

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //Совмещаем данные изображения и созданную текстуру --> <--

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}



