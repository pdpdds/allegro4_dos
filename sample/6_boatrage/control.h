#define GHOSTKEY_FWD    1
#define GHOSTKEY_LEFT  (1<<1)
#define GHOSTKEY_RIGHT (1<<2)
#define MAXGHOST        4096

class boatcontroller {
 public:
  boatcontroller() {}
  ~boatcontroller() {}
  virtual void check() = 0;
  virtual int keyup() = 0;
  virtual int keyleft() = 0;
  virtual int keyright() = 0;
};

class ctl_keyboard : public boatcontroller 
{
 public:
  ctl_keyboard(int up,int left,int right) : boatcontroller() {
   upkey=up; leftkey=left; rightkey=right;
  }
  void check() {}
  int keyup() { return key[upkey]; }
  int keyleft() { return key[leftkey]; }
  int keyright() { return key[rightkey]; }
 protected:
  int upkey,leftkey,rightkey;
};

class ctl_mouse : public boatcontroller {
 public:
  ctl_mouse() : boatcontroller() {position_mouse(25,0);}
  void check() { if (mouse_x>50) position_mouse(50,0); }
  int keyup() { return mouse_b & 1; }
  int keyleft() { return mouse_x<1; }
  int keyright() { return mouse_x>49; }
 protected:
  int mx,my;
};

/*
class ctl_joystick : public boatcontroller {
 public:
  ctl_joystick() : boatcontroller() {install_joystick(JOY_TYPE_AUTODETECT);}
  void check() { poll_joystick(); }
  int keyup()    { return (joy[0].button[0].b!=0);         }
  int keyleft()  { return (joy[0].stick[0].axis[0].d1!=0); }
  int keyright() { return (joy[0].stick[0].axis[0].d2!=0); }
};
*/


/*
struct ghostrec {
 short frame;
 char keys;
 char other; //UNUSED (pad to 4 bytes)
};


//THIS IS MY FIRST TRY AT A GHOST PLAYER...DOESN'T WORK YET
class ghost : public boatcontroller {
 public:
  ghost() {goblin=new ghostrec[MAXGHOST];}
  ~ghost() {delete[] goblin;}
  void write_to_disk(char *filename);
  void load_from_disk(char *filename);
  void start_record() {
   recpos=0;
   frame=0;
   recording=1;
  }
  void start_playback() {
   recpos=0;
   frame=0;
   recording=0;
  }
  void record_event(int keyfwd,int keyleft,int keyright) {
   if (!recording) return;
   if (recpos>=MAXGHOST) return;
   int val=0;
   if (keyfwd)   val|=GHOSTKEY_FWD;
   if (keyleft)  val|=GHOSTKEY_LEFT;
   if (keyright) val|=GHOSTKEY_RIGHT;
   if (val!=goblin[recpos].keys) {
    recpos++;
    goblin[recpos].frame=frame;
    goblin[recpos].keys=val;
    goblin[recpos].other=0;
   }
  }
  void playback_event(int &keyfwd, int &keyleft, int &keyright) {
   if (recording) return;
   if (frame==goblin[recpos].frame) {
   int val=goblin[recpos].keys;
   if (val&GHOSTKEY_FWD)   keyfwd=1; else keyfwd=0;
   if (val&GHOSTKEY_LEFT)  keyleft=1; else keyleft=0;
   if (val&GHOSTKEY_RIGHT) keyright=1; else keyright=0;
   recpos++;
  }
  void advanceframe() { frame++; }
 protected:
  int recpos;
  int frame;
  int recording;
  ghostrec *goblin;
};
  */
