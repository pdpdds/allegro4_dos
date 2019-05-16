#include"game.h"

TPlayer::TPlayer(TLevel *alevel): TObject(alevel, OBJ_PLAYER)
{
    size.x = TILE_W; size.y = TILE_H; /* typical */
    jlen = JLEN;
    score = 0;
    carry = 0;
    carrx = 0;
    lives = PL_LIVES;
    inv = new TColly(10,5);
    delQ = new TColly(10,5);
    elev = NULL;
    home();

    frames[STANDING][RIGHT][0] = PL_FBASE;
    frames[STANDING][LEFT][0] = PL_FBASE;
    frames[JUMPING][RIGHT][0] = PL_FBASE + 2*PL_WFRAMES - 1;
    frames[JUMPING][LEFT][0] = PL_FBASE + 2*PL_WFRAMES;
    frames[FALLING][RIGHT][0] = PL_FBASE + 2*PL_WFRAMES - 1;
    frames[FALLING][LEFT][0] = PL_FBASE + 2*PL_WFRAMES;
    frames[DYING][LEFT][0] = PL_FBASE;
    frames[DYING][RIGHT][0] = PL_FBASE;
    frames[DEAD][LEFT][0] = IMG_NULL;
    frames[DEAD][RIGHT][0] = IMG_NULL;

    for (int i = 1; i<PL_WFRAMES; i++) {
        frames[STANDING][RIGHT][i] = PL_FBASE + i;
        frames[STANDING][LEFT][i] = PL_FBASE + PL_WFRAMES + i - 1;
        frames[JUMPING][RIGHT][i] = PL_FBASE + 2*PL_WFRAMES - 1;
        frames[JUMPING][LEFT][i] = PL_FBASE + 2*PL_WFRAMES;
        frames[FALLING][RIGHT][i] = PL_FBASE + 2*PL_WFRAMES - 1;
        frames[FALLING][LEFT][i] = PL_FBASE + 2*PL_WFRAMES;
        frames[DYING][LEFT][i] = PL_FBASE + 2*PL_WFRAMES + i;
        frames[DYING][RIGHT][i] = PL_FBASE + 2*PL_WFRAMES + i;
        frames[DEAD][LEFT][i] = IMG_NULL;
        frames[DEAD][RIGHT][i] = IMG_NULL;
    }
}

TPlayer::~TPlayer()
{
    purge(delQ, inv, 1);
    purge(inv, NULL, 1);
    delete inv;
    delete delQ;
//    fprintf(stderr, "player destroyed\n");
}

void TPlayer::control_state(TPoint t, TPoint e)
{
    if (state == STANDING) {
            if (!(get_close(DOWN,t,e) & TL_WALL)) {
                state = FALLING, pos.y+=2,jc=-8; return;
            }
            if (key[KEY_UP]) state = JUMPING, jc = jlen;
    }
    if (state == DYING) {
        if ((++cX & 1) == 0) wf++; else return;
        if (wf > PL_WFRAMES-1) state = DEAD, wf = 0, cX = DEAD_TIMER;
        return;
    }
    if (state == DEAD) {
        if (!(cX--)) if (lives) lives--, home();
        else level->remove_object(this);
        return;
    }
    if (state == JUMPING)
      if (get_close(UP,t,e) & TL_WALL) {
          state = FALLING;
          jc = -4; carry = 0;
          pos.y = (t.y+!e.y)*TILE_H;
      } else if (--jc) pos.y-=jc/4; else state = FALLING, jc = -4;
    if (state == FALLING)
        if (get_close(DOWN,t,e) & TL_WALL) {
            state = STANDING;
/*            state = JUMPING; */
            pos.y = t.y*TILE_H;
            if (jc<=-JLIM) { die(); return; }
            jc = -4; carry = 0;
/*            jc = -jc; carry = 0; */
            if (!wf) wf++;
        } else pos.y-=((jc>-JLIM)?jc--:jc)/4;
    carry = carry + pos.y & 1;
    pos.y &= (-2);
    if (!(carry & 1)) pos.y+= carry, carry = 0;
/*    carrx = carrx + pos.x & 1;
    pos.x &= (-2);
    if (!(carrx & 1)) pos.x+= carrx, carrx = 0;*/
}

inline void TPlayer::fixpos(TPoint t, int flags)
{
    if (flags & 1) pos.y = t.y * TILE_H, carry = 0;
    if (flags & 2) pos.x = t.x * TILE_W, carrx = 0;
}

void TPlayer::add_object(TObject *p)
{
    inv->insert((void *) p);
    p->owner = this;
}

int TPlayer::remove_object(TObject *p)
{
    if (inv->indexof((void *) p) >= 0) {
        delQ->insert((void *) p);
        p->owner = NULL;
        return 1;
    } else return 0;
}

void TPlayer::home()
{
    wf = 2;
    wdir = level->home.wdir;
    pos = level->home.pos;
    state = STANDING;
    jc = -4;
    cX = 0; cD = 0;
}

void TPlayer::die()
{
    if (state == DYING || state == DEAD) return;
    wf = 0; carry = 0; jc = -4; state = STANDING;
    state = DYING;
}

void TPlayer::draw(BITMAP *b, int x, int y)
{
    draw_sprite(b, Images[frames[state][wdir][wf]], x, y);
}

void TPlayer::show_status(BITMAP *b)
{
    int x = 0;
    TObject *p;
    for (int i = 0; i<inv->count; i++) {
        p = (TObject *) inv->items[i];
        p->draw(b, x, 0);
        x += p->size.x;
    }
    textprintf(b, font, 0, 16, 157, "Score: %9d    Lives: %d    Level: %d",
      score, lives, level->no);
}

void TPlayer::walk(int odir, int adir, int anim, int speed,
  TPoint t, TPoint e)
{
    int k;
    if (adir == LEFT || adir == RIGHT) {
      if (odir != adir && anim) wf = 0, cX = 1;
      if (speed == 2 && pos.x & 1) speed = 3;
      if (!((k=get_close(adir,t,e)) & TL_WALL) && (!anim || odir == adir)) {
          if (anim) {
              if ((++cX & 1) == 0) wf++;
              if (wf>PL_WFRAMES-1) wf = 1;
          }
          pos.x += delta_x[adir]*speed;
      }
      if (k & TL_WALL && (pos.x & 1)) if (adir == RIGHT) pos.x = t.x * TILE_W;
        else pos.x = (t.x+1)*TILE_W;
    } else {
      if (speed == 2 && pos.y & 1) speed = 3;
      if (!((k=get_close(adir,t,e)) & TL_WALL)) {
          pos.y += delta_y[adir]*speed;
      }
      if (k & TL_WALL && (pos.y & 1)) if (adir == DOWN) pos.y = t.y * TILE_H;
        else pos.y = (t.y+1)*TILE_H;
    }
}

void TPlayer::run()
{
    TPoint t,e;
    int odir;
    int i,j,x2,y2;
    TTile *p;

    if (++cD & 1) return;

    /* use objects */
    for (int i = 0; i<inv->count; i++)
      if (!((TObject *)inv->items[i])->useit(this)) {
          ((TObject *)inv->items[i])->drop(this);
          remove_object((TObject *) inv->items[i]);
      }
    purge(delQ, inv, 0);

    /* check around */
    calc_pos(&t,&e);
    if (state != DEAD && state != DYING) {
    y2 = t.y + !e.y; x2 = t.x + !e.x;
    for (i = t.y; i <= y2; i++)
      for (j = t.x; j <= x2; j++) {
          p = &level->map[i][j];
          if (p->flags & TL_ITEM) {
              p->flags -= TL_ITEM;
              p->img = 0;
              score += SCORE_DIAMOND;
          }
          if (p->flags & TL_TRAP) {
              die();
              goto out;
          }
          if (p->obj != NULL) p->obj->handle_visit(this);
      }
    }
out:
    /* jump/fall/stand/die */
    control_state(t,e);
    if (state == DEAD) return;
//    walk(wdir, RIGHT, 0, 1, t, e);
    if (state == DYING) return;

    calc_pos(&t,&e);
    if (e.y) {
        p = &level->map[t.y+1][t.x];
        if (p->obj != NULL) {
            if (p->obj->type == OBJ_LRBLOCK) {
                p->obj->handle_visit(this);
            }
        }
        if (!e.x) {
            p = &level->map[t.y+1][t.x+1];
            if (p->obj != NULL)
              if (p->obj->type == OBJ_LRBLOCK) p->obj->handle_visit(this);
        }
    }
    p = &level->map[t.y+1][t.x];
    if (p->obj != NULL) {
        if (p->obj->type == OBJ_UDBLOCK) {
            p->obj->handle_visit(this);
        }
    }
    if (!e.x) {
        p = &level->map[t.y+1][t.x+1];
        if (p->obj != NULL)
          if (p->obj->type == OBJ_UDBLOCK) p->obj->handle_visit(this);
    }
    if (elev != NULL) {
        if (state != JUMPING) state = STANDING;
        walk(wdir, elev->wdir, 0, 2, t, e);
        elev = NULL;
    }


    /* walk */
    odir = wdir;
    if (key[KEY_RIGHT]) wdir = RIGHT; else if (key[KEY_LEFT]) wdir = LEFT;
    else return;
    calc_pos(&t,&e);
    walk(odir, wdir, 1, 2, t, e);
/*    if (odir != wdir) wf = 0, cX = 1;
    if (!(get_close(wdir,t,e) & TL_WALL) && odir == wdir) {
        if ((++cX & 1) == 0) wf++; if (wf>PL_WFRAMES-1) wf = 1;
        pos.x += delta_x[wdir]*2;
    }*/
}

