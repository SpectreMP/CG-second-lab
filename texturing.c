#include "texturing.h"
#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"


unsigned int createTexture(char* path)
{
    int width, height, nrChannels;

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0); //��������� ������ ����������� � ������
    printf("%i %i %i", width, height, nrChannels);

    unsigned int texture;                                                   //������ ��������
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);                                  //������ �, ����� ����������� ���������� ������� ����������� � ���

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //��������� ������ ����������� � ��������� �������� --> <--

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return texture;
}



