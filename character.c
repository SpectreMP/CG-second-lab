#include <GL/gl.h>
#include "character.h"
#include "texturing.h"
#include <stdbool.h>



Character* createCharacter(float x, float y, unsigned int* sprite)
{
    Character* ch = malloc(sizeof(Character));

    ch->positionX = x;
    ch->positionY = y;
    ch->width = 100.0f;
    ch->height = 100.0f;

    ch->spritesheet = sprite;

    ch->turnedAround = false;
    ch->inAir = false;
    ch->animation = 2;
    ch->frame = 0;
    ch->velocityX = 0;
    ch->velocityY = 0;
};

void drawCharacter(Character *ch)
{
    float vertices[] = {
         // positions                                                     // colors           // texture coords
         ch->positionX + ch->width, ch->positionY + ch->height,   0.0f,   1.0f, 1.0f, 1.0f,   0.125f + 0.125f * ch->frame, 0.33333f * ch->animation,         // top right
         ch->positionX + ch->width, ch->positionY,                0.0f,   1.0f, 1.0f, 1.0f,   0.125 + 0.125f * ch->frame,  0.33333f * (ch->animation - 1),   // bottom right
         ch->positionX,             ch->positionY,                0.0f,   1.0f, 1.0f, 1.0f,   0.0f + 0.125f * ch->frame,   0.33333f * (ch->animation - 1),   // bottom left
         ch->positionX,             ch->positionY + ch->height,   0.0f,   1.0f, 1.0f, 1.0f,   0.0f + 0.125f * ch->frame,   0.33333f * ch->animation          // top left
    };

    renderImageFromMatrix(vertices, ch->spritesheet);

    physics(ch);

    ch->frame += 1;                   //Переключаем кадр анимации на следующий
}

void changeAnimation (Character *ch, int animationNumber)
{
    ch->animation = animationNumber;
    ch->frame = 0;
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

    ch->positionX += ch->velocityX;   //Преобразуем скорость в изменение координат
    ch->positionY += ch->velocityY;

    ch->velocityX *= 0.5f;            //Инертность?
    ch->velocityY *= 0.5f;
}


