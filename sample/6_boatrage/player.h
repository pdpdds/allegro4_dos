//PLAYER.H

//en spelare
class player {
 public:
  player(char *pname, int _boat);
  void drawinfo(BITMAP *bmp);
  int boat; //vilken b†t?
  int engine; //motor?
  int rudder; //och roder?
//  int armor;
  int money; //hur mycket pengar spelaren har
  int lastposition; //vilken plats spelaren kom p† f”rra g†ngen
  char name[16]; //spelarens namn (funkar inte „n)
};
