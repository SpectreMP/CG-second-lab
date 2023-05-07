#include "texturing.h"
#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

//������� �������� �� �����������. �� ���� ����� ���� �� ����������� � ������ ���������� unsigned int, � ������� ������� ������� ��������.
void createTexture(char* path, unsigned int *texture)
{
    int width, height, nrChannels;

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);  //��������� ������ ����������� � ������
    //printf("%i %i %i\n", width, height, nrChannels);                       //������� ������ � ����������� ����������� � �������

    glGenTextures(1, texture);                                               //���������� �������� �� ��������� ���������� unsigned int
    glBindTexture(GL_TEXTURE_2D, *texture);                                  //������ �, ����� ����������� ���������� ������� ����������� � ���

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, nrChannels == 4? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data); //��������� ������ ����������� � ��������� �������� --> <--
    //1. ��������� � ����������� 2D ��������
    //2. � ������ ������ mipmap ����� ��������� �������� (� ��� ��� mimmap)
    //3. � ����� ������� ����� ������� ����� (rgbA ������ ��� ������ �� � ���)
    //4-5. ������ � ������ �����������
    //6. "Always zero because some legacy stuff"
    //7-8. ������ ������������ ����������� (RGB ��� RGBA, unsigned_byte �� �� unsigned_char)
    //9. C��������� ������ �����������

    glBindTexture(GL_TEXTURE_2D, 0);                                        //������� ���� � ��������

    stbi_image_free(data);                                                  //����������� ������, ������� ������� �����������
}

//����������� ����������� ���������� ������� � ��������� ����� � ��������� ���������.
void renderImage (float width, float height, float xPosition, float yPosition, unsigned int texture)
{
    float vertices[] = {
         // positions                                   // colors           // texture coords
         xPosition + width, yPosition + height, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,    // top right
         xPosition + width, yPosition,          0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,    // bottom right
         xPosition,         yPosition,          0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,    // bottom left
         xPosition,         yPosition + height, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f     // top left
    };
    //�� �����, ������ ����� � ���������� ������ ��� �� �����, �� � ����� �� ��������, �� ������ ���� ��� �� ����������� ��������� ���������� � �������� ��������.

    glEnable(GL_TEXTURE_2D);                    //�������� ������������� 2D �������
    glBindTexture(GL_TEXTURE_2D, texture);      //������ ��������, ����� ��� ��������� ������� ����������� ��������� � ���
    glEnable(GL_ALPHA_TEST);                    //�������� ���� ������������
    glAlphaFunc(GL_GREATER, 0.5);               //������� ��� ����� �������� � ������������� ������ 0.5

    glPushMatrix();

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        //���� ������� �������� 8 �������� (3 �������, 3 �����, 2 ���������� �������), ������� sride = 8 * ������ float. �������� ���� �� ��������� �� 6, ��� ��� ���������� � 6 �������.
        glVertexPointer(2, GL_FLOAT, 8 * sizeof(float), vertices);
        glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), vertices + 6);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);    //������

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    glDisable(GL_ALPHA_TEST);                   //��������� ��, ��� ��������
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

//����������� ����������� � ������������ � ��������� �������� ������, �� �� ������� ��� � ����, �� ��������� ����� ��������� ������ ����������� (��� ��������, ��������)
void renderImageFromMatrix (float vertices[], unsigned int texture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);

    glPushMatrix();

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_FLOAT, 8 * sizeof(float), vertices);
        glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), vertices + 6);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}



