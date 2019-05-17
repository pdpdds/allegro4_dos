#include <iostream>
#include "LinkedList.h"
#include"Explosion.h"


LinkedList::LinkedList(BITMAP* image)
{
    hitsound = load_sample("Sounds/explosionSound.wav"); // or .voc
    explosionsound = load_sample("Sounds/ImpactSound.wav"); // or .voc
    head = NULL;
    tail = NULL;
    length = 0;
    this->image = image;
}

LinkedList::~LinkedList()
{
    Node* temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
	destroy_sample(hitsound);
}

void LinkedList::Append(Object* object)
{
    if (head == NULL)
    {
        head = new Node;
        tail = head;
        head->object = object;
        head->next = NULL;
        head->prev = NULL;
    }
    else
    {
        Node* temp = new Node;
        temp->object = object;
        temp->next = NULL;
        temp->prev = tail;
        tail->next = temp;
        tail = tail->next; //We can also write tail = temp
    }

    length++;
}

int LinkedList::Length()
{
    return length;
}

void LinkedList::CleanAll()
{
    Node* temp = head;
    while(temp != NULL)
    {

        if(temp == head)
        {
            if(temp->object != NULL)
            {
                if(temp->object->Alive() == false)
                {
                    head = head->next;
                    if(head!=NULL)
                    {
                        head->prev = NULL;
                    }
                    delete temp;
                    temp = head;
                }
                else
                {
                    temp = temp->next;
                }
            }
            else
            {
                break;
            }
        }
        else if(temp == tail)
        {
            if(temp->object!=NULL)
            {
                if(temp->object->Alive() == false)
                {
                    tail = tail->prev;
                    tail->next = NULL;
                    delete temp;
                    temp = NULL;
                }
                else
                {
                    temp = temp->next;
                }
            }
        }
        else
        {
            if(temp->object->Alive() == false)
            {
                Node* keeper = temp;
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                temp = temp->next;
                delete keeper;
            }
            else
            {
                temp = temp->next;
            }
        }
    }
}

Node* LinkedList::GetHead()
{
    return head;
}

void LinkedList::MoveAll()
{
    Node* temp = head;
    while(temp!=NULL)
    {
        if((rand()%2)==0)
        {
            Object* enemyFire = temp->object->Fire();

            if(enemyFire != NULL && temp->object->GetType() == 22){

                 Append(enemyFire);
            }

        }
        temp->object->Move();
        temp = temp->next;
    }
    //This loop checks collisions after every move

    Node* firstNode = head;
    while(firstNode!=NULL)
    {
        Node* secondNode = head;
        while(secondNode!=NULL)
        {
            CheckCollision(firstNode->object, secondNode->object);
            secondNode = secondNode->next;
        }
        firstNode = firstNode->next;
    }
}


void LinkedList::DrawAll(BITMAP* buffer, bool debug)
{
    Node* temp = head;
    while(temp!=NULL)
    {
        temp->object->Draw(buffer, debug);
        temp = temp->next;
    }
}

void LinkedList::CheckCollision(Object* object1, Object* object2)
{
    if(object1 == NULL || object2 == NULL)
        return;
    bool collision = false;
    if(object1->GetType() != object2->GetType())
    {
        if(object1->Alive() == true && object2->Alive() == true)
        {
            if(object1->GetType() >10 && object1->GetType()<20 )
            {
                /*
                if(object1->GetBoundingBox()->bottom > object2->GetBoundingBox()->bottom &&
                   object1->GetBoundingBox()->top < object2->GetBoundingBox()->top &&
                   object1->GetBoundingBox()->left > object2->GetBoundingBox()->left &&
                   object1->GetBoundingBox()->right < object2->GetBoundingBox()->right)
                {
                    collision = true;
                }
                */
                collision = true;

				if (object1->GetBoundingBox() == 0 || object2->GetBoundingBox() == 0)
				{
					collision = false;
				}
				else
				{
					if (object1->GetBoundingBox()->bottom <  object2->GetBoundingBox()->top)
						collision = false;
					else if (object1->GetBoundingBox()->top >  object2->GetBoundingBox()->bottom)
						collision = false;
					else if (object1->GetBoundingBox()->right <  object2->GetBoundingBox()->left)
						collision = false;
					else if (object1->GetBoundingBox()->left >  object2->GetBoundingBox()->right)
						collision = false;
				}                
            }
        }
    }
    if(collision == true)
    {
        int obj1Damage = object1->GetHealth();
        int obj2Damage = object2->GetHealth();
        object1->TakeDamage(obj2Damage);
        object2->TakeDamage(obj1Damage);
        play_sample(hitsound, 255, 128, 1000, 0);//Playing hitsound
        if(object1->GetType()>20 && object1->GetType()<23)
        {
            if(object1->Alive() == false)
            {
                Explosion* explosion = new Explosion(image, 688, 0, 46, 46, object1->GetPosition().x, object1->GetPosition().y, -100);
                Append(explosion);

               if(rand()%100 < 50){

                    Power* power = new Power(image, object1->GetPosition(), (rand() % 3));

                    if(power != NULL) { //Dont really need to check this
                        this->Append(power);
                    }

                }
                play_sample(explosionsound, 255, 128, 1000, 0);//Playing explosion
            }
        }
        if(object2->GetType()>20 && object2->GetType()<23)
        {
            if(object2->Alive() == false)
            {
                Explosion* explosion = new Explosion(image, 688, 0, 46, 46, object2->GetPosition().x, object2->GetPosition().y, -100);
                Append(explosion);
                play_sample(explosionsound, 255, 128, 1000, 0);//Playing explosion
                if(rand()%100 < 50){
                    Power* power = new Power(image, object2->GetPosition(), (rand() % 3));

                    if(power != NULL) {
                        this->Append(power);
                    }
                    }
            }
        }
        if(object1->GetType() == 0 && object2->GetType() >22){
            //add armor
            if(object2->Alive() == true) {
                object2->SetAlive(false);
            }
        }
        if(object2->GetType() == 0 && object1->GetType() >22){
                //add armor
            if(object1->Alive() == true){
                object1->SetAlive(false);
            }
        }
    }
}

