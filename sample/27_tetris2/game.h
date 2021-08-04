#pragma once

void InitGame(int screen_width, int screen_height);
void ProcessLogic(int key_left, int key_right, int key_up, int key_down, int key_space);
void CheckGameEnd();
void DrawWorld();

extern void RenderRect(int x1, int y1, int x2, int y2, int r, int g, int b, int fill); //�簢���� �׸��� �Լ�
extern void RenderCircle(int x, int y, int radius, int r, int g, int b); // ���� �׸��� �Լ�
extern void RenderLine(int x, int y, int x2, int y2, int r, int g, int b);
extern void RenderText(int x, int y, char* szMessage);