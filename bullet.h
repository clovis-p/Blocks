#ifndef BULLET_H
#define BULLET_H

void shootBullet(gameObject *bullet, gameObject *shooter);
void updateBulletsPos(gameObject *bullet);

extern int shoot;

#endif // BULLET_H
