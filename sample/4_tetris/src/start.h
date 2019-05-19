

#ifndef start_h
#define start_h

struct BITMAP;

class start {

public:
	start();
	int startuj();
	void allegro_off();

private:
	BITMAP * tlo;

	void allegro_on();

	void myszka();
	int mx;
	int my;
	int mb;

};

#endif
