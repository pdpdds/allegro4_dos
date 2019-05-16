//VIEW.H

//en kamera som kan f”lja efter t ex en b†t
class view {
 public:
  view(BITMAP *canv) {bmp=canv;}
  ~view() {;}
  void draw();
  void follow(sprite *sp);
  void followboat(boat *b);
  void scrollto(fixed x, fixed y);
  void scrollx(fixed x);
  void scrolly(fixed y);
  fixed getsx() {return sx;}
  fixed getsy() {return sy;}
 protected:
  BITMAP *bmp; //var kameran ska rita ut vad den ser
  fixed sx,sy; //var vi vill titta
};

