#ifndef COLLISION_H
#define COLLISION_H

int overlap();
void checkIfWithinBounds(gameObject *o);
void checkEnemyCollisions(gameObject *enemy, gameObject player);

extern int reset;

#endif // COLLISION_H


