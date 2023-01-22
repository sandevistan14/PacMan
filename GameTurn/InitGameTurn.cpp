#include "GhostH/iaghost.h"
#include "GhostH/PhaseGhost.h"
#include "DisplayH/Display.h"
#include "InitialisationH/Initialisation.h"
#include "MovementH/Movement.h"
#include "mingl/audio/audioengine.h"


using namespace std;

void InitPacMan(Entity & PacMan){
    PacMan.viewdirection = "Left";
    PacMan.ident = 'P';
    PacMan.SpriteMap = initSpriteMap("../sae102/res/sprites/pacman/spriteMap");
}

void InitRedGhost(Entity & RedGhost){
    RedGhost.state = "Flee";
    RedGhost.viewdirection = "Left";
    RedGhost.ident = 'R';
    RedGhost.SpriteMap = initSpriteMap("../sae102/res/sprites/redghost/spriteMap");
}

void InitOrangeGhost(Entity & OrangeGhost){
    OrangeGhost.state = "Flee";
    OrangeGhost.viewdirection = "Top";
    OrangeGhost.ident = 'O';
    OrangeGhost.SpriteMap = initSpriteMap("../sae102/res/sprites/orangeghost/spriteMap");
}

void InitPinkGhost(Entity & PinkGhost){
    PinkGhost.state = "Flee";
    PinkGhost.viewdirection = "Bottom";
    PinkGhost.ident = 'K';
    PinkGhost.SpriteMap = initSpriteMap("../sae102/res/sprites/pinkghost/spriteMap");
}

void InitBlueGhost(Entity & BlueGhost){
    BlueGhost.state = "Flee";
    BlueGhost.viewdirection = "Bottom";
    BlueGhost.ident = 'B';
    BlueGhost.SpriteMap = initSpriteMap("../sae102/res/sprites/blueghost/spriteMap");
}

