#include <GL/gl.h>
#include "character.h"
#include "texturing.h"
#include <stdbool.h>

int WINDOW_HEIGHT = 768; //����� ������� ������� ���� ������ (� �� ���� ���������� ���������� ������������ ��� ��������� ����������� ���������)
float FLOOR_LEVEL = 200; //������� ��������

Character* createCharacter(float x, float y, unsigned int sprite)
{
    Character* ch = malloc(sizeof(Character));

    ch->positionX = x;
    ch->positionY = y;
    ch->width = 100.0f;
    ch->height = 100.0f;

    ch->spritesheet = sprite;

    ch->turnedAround = false;
    ch->inAir = true;
    ch->animation = 2;
    ch->frame = 0;
    ch->velocityX = 0;
    ch->velocityY = 0;
};

void drawCharacter(Character *ch)
{
    float positionXTransformed = ch -> positionX - ch -> width * 0.5f;
    float positionYTransformed = WINDOW_HEIGHT - ch -> positionY - ch -> height;

    float vertices[] = {
         // positions                                                                 // colors           // texture coords
         positionXTransformed + ch->width, positionYTransformed + ch->height, 0.0f,   1.0f, 1.0f, 1.0f,   0.125f + 0.125f * ch->frame, 0.33333f * ch->animation,         // top right
         positionXTransformed + ch->width, positionYTransformed,              0.0f,   1.0f, 1.0f, 1.0f,   0.125 + 0.125f * ch->frame,  0.33333f * (ch->animation - 1),   // bottom right
         positionXTransformed,             positionYTransformed,              0.0f,   1.0f, 1.0f, 1.0f,   0.0f + 0.125f * ch->frame,   0.33333f * (ch->animation - 1),   // bottom left
         positionXTransformed,             positionYTransformed + ch->height, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f + 0.125f * ch->frame,   0.33333f * ch->animation          // top left
    };

    if (ch->turnedAround)                                   //�������� ����������� �� ���������
    {
        vertices[0] = vertices [8] -= ch -> width;
        vertices[16] = vertices [24] += ch -> width;
    }

    physics(ch);                                            //������������ ������

    renderImageFromMatrix(vertices, ch->spritesheet);       //�������� ��������� � ������������ � �������� ������ ����

    ch->frame += 1;                                         //����������� ���� �������� �� ���������
}

void changeAnimation (Character *ch, int animationNumber)   //��������� ������������ ��������
{
    if (ch->animation != animationNumber)
    {
        ch->animation = animationNumber;
        ch->frame = 0;
    }
}

void addVelocity(Character *ch, float horizontal, float vertical)
{
    ch->velocityX += horizontal;
    ch->velocityY += vertical;
}

void physics(Character *ch)                 //����������? ���������
{
    if (ch->inAir)                          //��������� ���������� ������� (���� ���������� � �������, �����)
    {
        ch->velocityY -= 9.8f;
    }
    else
    {
        ch->velocityX *= 0.8f;              //����������?
        ch->velocityY *= 0.8f;
    }

    ch->positionX += ch->velocityX;         //����������� �������� � ��������� ���������
    ch->positionY += ch->velocityY;


    if (ch->inAir && ch->positionY + ch->velocityY<FLOOR_LEVEL) //������� � �����?
    {
        ch->velocityY = 0;
        ch->inAir = false;
        ch->positionY = FLOOR_LEVEL;
    }
}

void drawVelocityVector (Character *ch)     //���������� ������ �������� ���������
{
    glPushMatrix();
        glBegin(GL_LINES);
            glVertex2f(ch -> positionX, WINDOW_HEIGHT - ch -> positionY);
            glVertex2f(ch -> positionX + ch -> velocityX, WINDOW_HEIGHT - ch -> positionY - ch -> velocityY);
        glEnd();
    glPopMatrix();
}

void playerControl (Character *ch)          //���������� ���������� (��� �� � ����� ����������� ��� �� switch...)
{
    if (ch->inAir)
    {
        changeAnimation(ch, 1);
    }
    else if (GetKeyState(VK_LEFT)<0)
    {
        addVelocity(ch, -10.0f, 0.0f);
        ch -> turnedAround = true;
        changeAnimation(ch, 2);

    }
    else if(GetKeyState(VK_RIGHT)<0)
    {
        addVelocity(ch, 10.0f, 0.0f);
        ch -> turnedAround = false;
        changeAnimation(ch, 2);
    }
    else
    {
        changeAnimation(ch, 3);
    }

    if (GetKeyState(VK_UP)<0 && ch->inAir != true)
    {
        addVelocity(ch, 0.0f, 70.0f);
        ch -> inAir = true;
    }
    if (GetKeyState(VK_DOWN)<0 && ch->inAir)
    {
        addVelocity(ch, 0.0f, -100.0f);
    }
}
