#pragma once

void InitGame(int screen_width, int screen_height);
void ProcessLogic(int key_left, int key_right, int key_up, int key_down, int key_space);
void CheckGameEnd();
void DrawWorld();