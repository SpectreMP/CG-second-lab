#include <windows.h>
#include <string.h>
#include <gl/gl.h>
#include <stdio.h>
#include "stb-master/stb_easy_font.h"

void windowNormalize(HWND hwnd, float offset) {
    RECT rct;
    GetClientRect(hwnd, &rct);
    float offsetReverse = 1.0f - offset;
    glOrtho(0.0f - rct.right * offset, rct.right * offsetReverse, 0.0f - rct.bottom * offset, rct.bottom * offsetReverse, 1, -1);
}

struct Button {         //Структура кнопки
    char *name;
    float height;
    float width;
    float x;
    float y;

    void *rawTextVertexBuffer;

};

struct RGBColor {       //Структура цвета, чисто для удобства. Скорее всего уже есть встроенная в GL, но я не нашёл "\_( -_-)_/"
    float red;
    float green;
    float blue;
};

struct Button createButton(char theName[31], float theHeight, float theWidth, float positionX, float positionY) {       //Генератор(?) кнопки. Генерирует экземпляр кнопки в соответствии с заданными параметрами
    struct Button result;

    result.height = theHeight;
    result.width = theWidth;
    result.name = theName;
    result.x = positionX;
    result.y = positionY;

    //GLuint rawBufferSize = 3000.0f; ТЕКСТ НЕ РАБОТАЕТ!

    //result.rawTextVertexBuffer = malloc(rawBufferSize);

    return result;
}

void renderButton (struct Button button, struct RGBColor color) {               //Рендерер кнопки. Рендерит кнопку каждый кадр, задаёт цвет кнопке
    glBegin(GL_QUADS);
        glColor3f(color.red, color.green, color.blue);
        glVertex2f(button.x, button.y);
        glVertex2f(button.x + button.height, button.y);
        glVertex2f(button.x + button.height, button.y + button.width);
        glVertex2f(button.x, button.y + button.width);
    glEnd();
    //stb_easy_font_print(button.x + button.height * 0.1f, button.y + button.width * 0.1f, button.name, NULL, button.rawTextVertexBuffer, 3000.0f); ДА ЧТО ЖЕ С ТОБОЙ ДЕЛАТЬ
}
