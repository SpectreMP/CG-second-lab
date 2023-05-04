#ifndef MENU_H
#define MENU_H

int Menu_AddButton(char *name, float x, float y, float width, float height, float textScale, void (*fun)());
void Menu_ShowMenu();

int Menu_MouseMove(float x, float y);
int Menu_MouseDown();
void Menu_MouseUp();


#endif MENU_H
