#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdbool.h>


typedef struct
{
    float positionX;
    float positionY;

    float height;
    float width;

    bool turnedAround;
    bool inAir;

    unsigned int spritesheet;

    int animation;
    int frame;

    float velocityX;
    float velocityY;

} Character;

Character* createCharacter(float x, float y, unsigned int sprite);
void drawCharacter(Character *ch);
void changeAnimation (Character *ch, int animationNumber);
void addVelocity(Character *ch, float horizontal, float vertical);

#endif CHARACTER_H
