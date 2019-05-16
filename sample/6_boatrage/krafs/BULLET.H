class bullet : public sprite {
 public:
  bullet(fixed x, fixed y, fixed dx, fixed dy, fixed angle, int type);
  void draw(BITMAP *bmp, fixed sx, fixed sy);
  void move();
 protected:
  fixed dx,dy;
  fixed angle;
  int type;
  BITMAP *pic;
};


