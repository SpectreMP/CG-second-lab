#include "collision.h"
#include <GL/gl.h>
#include <stdbool.h>
#include "character.h"

bool isIntersecting (float x11, float x12, float y11, float y12, float x21, float x22, float y21, float y22)
{
    if (x11 > x22 || x12 < x21 || y11 > y22 || y12 < y21) return false;
    else return true;
};


void collision(Character *ch, float collisionMatrix[], unsigned long long matrixSize) //Если интересно, зачем я запрашиваю размер матрицы, вместо того, чтобы посчитать
{                                                                                     //Её внутри функции, читай про Array Decay. Я вот об этом не знал, например 0_0
    float colliderWidth = ch->width * 0.5f;
    float colliderHeight = ch->height;

    float x11 = ch->positionX - colliderWidth * 0.5f;
    float x12 = ch->positionX + colliderWidth * 0.5f;
    float y11 = ch->positionY;
    float y12 = ch->positionY + colliderHeight;


//    Отрисовка хитбокса персонажа с учётом вектора скорости
//    glPushMatrix();
//        glBegin(GL_LINE_LOOP);
//            glVertex2f(x11 + ch->velocityX, 768 - (y11 + ch->velocityY));
//            glVertex2f(x11 + ch->velocityX, 768 - (y12 + ch->velocityY));
//            glVertex2f(x12 + ch->velocityX, 768 - (y12 + ch->velocityY));
//            glVertex2f(x12 + ch->velocityX, 768 - (y11 + ch->velocityY));
//        glEnd();
//    glPopMatrix();

    for (int i = 0; i < matrixSize/sizeof(float); i+=4)
    {
        float x21 = collisionMatrix[i];
        float x22 = collisionMatrix[i+1];
        float y21 = collisionMatrix[i+2];
        float y22 = collisionMatrix[i+3];

//        Отрисовка всех объектов коллизии
//        glPushMatrix();
//            glBegin(GL_LINE_LOOP);
//                glVertex2f(x21, 768 - y21);
//                glVertex2f(x21, 768 - y22);
//                glVertex2f(x22, 768 - y22);
//                glVertex2f(x22, 768 - y21);
//            glEnd();
//        glPopMatrix();

        if (isIntersecting(x11 + ch->velocityX , x12 + ch->velocityX, y11 + ch->velocityY, y12 + ch->velocityY, x21, x22, y21, y22)) //Если в следующем кадре произойдет пересечение с поверхностью
        {
            if (isIntersecting(x11, x12, y11, y12, x11, x21, y21, y22))     //Столкновение с левым краем
            {
                ch->velocityX = 0;
                ch->positionX = x21 - colliderWidth * 0.5f - 0.1f;
            }
            if (isIntersecting(x11, x12, y11, y12, x21, x22, y22, y12))     //Столкновение с верхним краем
            {
                ch->velocityY = 0;
                ch->positionY = y22 + 0.1f;
                ch->inAir = false;
            }
            if (isIntersecting(x11, x12, y11, y12, x22, x12, y21, y22))     //Столкновение с правым краем
            {
                ch->velocityX = 0;
                ch->positionX = x22 + colliderWidth * 0.5f + 0.1f;
            }
            if (isIntersecting(x11, x12, y11, y12, x21, x22, y11, y21))     //Столкновение с нижним краем
            {
                ch->velocityY = 0;
                ch->positionY = y21 - colliderHeight - 0.1f;
            }
        };
    };
};
