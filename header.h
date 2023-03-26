#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>

void windowNormalize(HWND hwnd, float offset) {
    RECT rct;
    GetClientRect(hwnd, &rct);
    float offsetReverse = 1.0f - offset;
    glOrtho(0.0f - rct.right * offset, rct.right * offsetReverse, 0.0f - rct.bottom * offset, rct.bottom * offsetReverse, 1, -1);
}


struct Button {
    char name;
    float height;
    float width;
    float position[2];
};

struct Button createButton(char theName, float theHeight, float theWidth) {
    struct Button result;
    result.height = theHeight;
    result.width = theWidth;
    //result.position[2] = thePosition[2];
    result.name = theName;

    printf(result.name);

    return result;
}
