#ifndef FONCTION_H
#define FONCTION_H
#include "type.h"



bool checkCollision (CMat Grid, unsigned X, unsigned Y);

std::map <std::string, std::vector <std::string>> initSpriteMap (const std::string & sourceFile);

nsGui::Sprite initSprite (Entity & Entity, CMat & entityGrid, unsigned short & tick);

void pacManDirection(MinGL & window, CMat & entityGrid, Entity & PacMan);

void GhostMovement(CMat & entityGrid, Entity & PacMan,short unsigned & tick);

void pacManMovement(CMat & entityGrid, Entity & PacMan,short unsigned & tick);

std::pair <CMat, std::map<char, CPos>> initEntityMaze (const std::string & sourceFile);

CMat initGumMaze(const std::string & sourceFile);

void showGumInMaze(MinGL & window, const CMat & gumGrid);

void gumEating(Entity PacMan, CMat & gumGrid);

void move_entity_in_mat(CMat Mat, Entity entity);

#endif // FONCTION_H