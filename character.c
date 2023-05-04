#include <GL/gl.h>
#include "character.h"
#include "texturing.h"
#include <stdbool.h>

int WINDOW_HEIGHT = 768; //Самый большой костыль всех времен
float FLOOR_LEVEL = 200; //Костыль поменьше

Character* createCharacter(float x, float y, unsigned int* sprite)
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
    float positionYTransformed = 768 - ch -> positionY - ch -> height;

    float vertices[] = {
         // positions                                                   // colors           // texture coords
         positionXTransformed + ch->width, positionYTransformed + ch->height, 0.0f,   1.0f, 1.0f, 1.0f,   0.125f + 0.125f * ch->frame, 0.33333f * ch->animation,         // top right
         positionXTransformed + ch->width, positionYTransformed,              0.0f,   1.0f, 1.0f, 1.0f,   0.125 + 0.125f * ch->frame,  0.33333f * (ch->animation - 1),   // bottom right
         positionXTransformed,             positionYTransformed,              0.0f,   1.0f, 1.0f, 1.0f,   0.0f + 0.125f * ch->frame,   0.33333f * (ch->animation - 1),   // bottom left
         positionXTransformed,             positionYTransformed + ch->height, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f + 0.125f * ch->frame,   0.33333f * ch->animation          // top left
    };

    if (ch->turnedAround) //Отражаем изображение по вертикали
    {
        vertices[0] = vertices [8] -= ch -> width;
        vertices[16] = vertices [24] += ch -> width;
    }

    renderImageFromMatrix(vertices, ch->spritesheet);

    physics(ch);

    printf("%f %f\n", ch->positionX, ch->positionY);
    ch->frame += 1;                   //Переключаем кадр анимации на следующий
}

void changeAnimation (Character *ch, int animationNumber)
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

void physics(Character *ch)
{
    if (ch->inAir)
    {
        ch->velocityY -= 9.8f;
    }
    else
    {
        ch->velocityX *= 0.8f;            //Инертность?
        ch->velocityY *= 0.8f;
    }

    ch->positionX += ch->velocityX;   //Преобразуем скорость в изменение координат
    ch->positionY += ch->velocityY;


    if (ch->inAir && ch->positionY + ch->velocityY<FLOOR_LEVEL) //Колизия с полом?
    {
        ch->velocityY = 0;
        ch->inAir = false;
        ch->positionY = FLOOR_LEVEL;
    }
}

void drawVelocityVector (Character *ch)
{
    glPushMatrix();
        glBegin(GL_LINES);
            glVertex2f(ch -> positionX, WINDOW_HEIGHT - ch -> positionY);
            glVertex2f(ch -> positionX + ch -> velocityX, WINDOW_HEIGHT - ch -> positionY - ch -> velocityY);
        glEnd();
    glPopMatrix();
}

