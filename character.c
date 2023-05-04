#include <GL/gl.h>
#include "character.h"
#include "texturing.h"
#include <stdbool.h>


typedef struct
{
    float positionX;
    float positionY;

    bool turnedAround;

    unsigned int spritesheet;

    int animation;
    int frame;

    float velocityX;
    float velocityY;

} character;

character* createCharacter(int x, int y, unsigned int sprite)
{

};


