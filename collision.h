#ifndef COLLISION_H
#define COLLISION_H
#include <stdbool.h>
#include "character.h"

bool isIntersecting (float x11, float x12, float y11, float y12, float x21, float x22, float y21, float y22);
void collision(Character *ch, float collisionMatrix[], unsigned long long matrixSize);


#endif COLLISION_H
