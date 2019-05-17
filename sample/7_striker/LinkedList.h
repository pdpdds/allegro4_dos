#pragma once
#include<iostream>
#include"Object.h"
#include "Power.h"
#include <allegro.h>

struct Node
{
    Object* object;
    Node* next;
    Node* prev;
    ~Node()
    {
        //std::cout<<"Node Deleted"<<endl;
        delete object;
    }
};

class LinkedList
{
private:
    int length;
    Node* head;
    Node* tail;
    BITMAP* image;
    SAMPLE* hitsound;
    SAMPLE* explosionsound;
public:
    LinkedList(BITMAP*);
    ~LinkedList();
    void Append(Object*);
    void InsertAt(Object*, int);
    Object* DeleteAt(int);
    Object* Pop();
    void CleanAll();
    int Length();
    void MoveAll();
    Node* GetHead();
    void DrawAll(BITMAP*, bool);
    void CheckCollision(Object*, Object*);
};

