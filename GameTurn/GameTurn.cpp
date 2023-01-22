/**
 *
 * @file : GameTurn.cpp
 * @date : 15 jav 2023
 * @author : Essalhi Ayoub, Lerouge Robin, Lestrelin Valentin, Brahmi Moundir, Ditlecadet Michael
 * @brief : Gestion d'une frame/tour de jeu.
 *
**/

#define FPS_LIMIT 120
#include <iostream>
#include <thread>
#include <fstream>
#include <map>
#include "mingl/mingl.h"
#include "mingl/gui/sprite.h"
#include "mingl/shape/rectangle.h"
#include "type.h"
#include "MiscH/Misc.h"
#include "GhostH/GhostMove.h"
#include "GhostH/iaghost.h"
#include "GhostH/PhaseGhost.h"
#include "DisplayH/Display.h"
#include "InitialisationH/Initialisation.h"
#include "MovementH/Movement.h"
#include "mingl/audio/audioengine.h"
#include "GameTurnH/InitGameTurn.h"

using namespace std;


void GameTurn(){

    unsigned score = 0;  // init score
    unsigned niveau = 1; // init niveau
    unsigned vies = 3;   //init vies

    MinGL window("PAC-MAN", nsGraphics::Vec2D(672, 790), nsGraphics::Vec2D(120, 120), nsGraphics::KBlack);
    window.initGlut();
    window.initGraphic();

    // Initialise le sous-système audio et les sons utilisés
    nsAudio::AudioEngine audioEngine;
    audioEngine.loadSound("../sae102/res/audio/pacman_ouvre.wav");
    audioEngine.loadSound("../sae102/res/audio/pacman_ferme.wav");
    audioEngine.loadSound("../sae102/res/audio/SUIII.wav");
    audioEngine.loadSound("../sae102/res/audio/pacman_death.wav");
    audioEngine.loadSound("../sae102/res/audio/scare_ghost.wav");


    // Variable qui tient le temps de frame
    chrono::microseconds frameTime = chrono::microseconds::zero();

//*****************************************************************************
//**************************     Init maze     ********************************
//*****************************************************************************

    pair <CMat, map<char, CPos>> gridInfo = initEntityMaze("../sae102/res/mazeinitialmap");
    CMat entityGrid = gridInfo.first;
    map<char, CPos> posMap = gridInfo.second;
    CMat gumGrid = initGumMaze("../sae102/res/guminitialmap");
    nsGui::Sprite maze("../sae102/res/sprites/maze0.si2", nsGraphics::Vec2D(0,0));


//*****************************************************************************
//****************************     Init entity     ****************************
//*****************************************************************************
    Entity PacMan;
    Entity RedGhost;
    Entity OrangeGhost;
    Entity PinkGhost;
    Entity BlueGhost;

    InitPacMan(PacMan);             //init param pacman
    InitRedGhost(RedGhost);         //init param RedGhost
    InitOrangeGhost(OrangeGhost);   //init param OrangeGhost
    InitPinkGhost(PinkGhost);       //init param PinkGhost
    InitBlueGhost(BlueGhost);       //init param BlueGhost

    PacMan.Pos = posMap[PacMan.ident];
    RedGhost.Pos = posMap[RedGhost.ident];
    OrangeGhost.Pos = posMap[OrangeGhost.ident];
    PinkGhost.Pos = posMap[PinkGhost.ident];
    BlueGhost.Pos = posMap[BlueGhost.ident];

//*****************************************************************************
//***************************     Autre Variable     **************************
//*****************************************************************************
    // init bool
    bool peutmanger = false;
    bool WinRound = false;
    bool Dead = false;

    // init int
    int tick2 = 0;

    // init unsigned
    unsigned dieTickAnimation = 0;
    unsigned NbGum;

    // init vector
    vector<bool> phase = {false,false,false,false,false,false,false};


//*****************************************************************************
//************************     Boucle de jeux     *****************************
//*****************************************************************************
    //     On fait tourner la boucle tant que la fenêtre est ouverte
    for (unsigned short tick = 0; window.isOpen(); ++tick)
    {

        // Récupère l'heure actuelle
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

        // On efface la fenêtre
        window.clearScreen();

        window << maze; //afficher le labyrinthe à chaque fois fait bugger le programme


        //*****************************************************************************
        //****************************     Victoire     *******************************
        //*****************************************************************************
        if (WinRound == true) {
            this_thread::sleep_for(chrono::milliseconds(500000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));
            ChangementNiveau(PacMan, RedGhost, OrangeGhost, PinkGhost, BlueGhost, posMap, gumGrid);
            WinRound = false;
        }

        //*****************************************************************************
        //******************************     Mort     *********************************
        //*****************************************************************************
        if (Dead)
        {
            window << nsGui::Sprite (PacMan.SpriteMap["Dead"][dieTickAnimation % PacMan.SpriteMap["Dead"].size()], nsGraphics::Vec2D(24*PacMan.Pos.first-12,24*PacMan.Pos.second-12));
            if (dieTickAnimation % PacMan.SpriteMap["Dead"].size() == PacMan.SpriteMap["Dead"].size()-1)
            {
                audioEngine.playSoundFromBuffer("../sae102/res/audio/pacman_death.wav");
                this_thread::sleep_for(chrono::milliseconds(250000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));
                reinitLevel(PacMan, RedGhost, PinkGhost, OrangeGhost, BlueGhost, posMap);
                Dead = false;
                dieTickAnimation = 0;
                --vies;
            }
            else{
                ++dieTickAnimation;
            }
        }
        else {
            showGumInMaze(window,gumGrid,NbGum);
            pacManDirection(window, entityGrid, PacMan);
            pacManMovement(entityGrid, PacMan, tick);

            if(gumGrid[PacMan.Pos.second][PacMan.Pos.first] == KSuperGum){
                peutmanger = true;
                if(tick2 != 0){
                    tick2 = 150;
                }
            }
            if (vies == 0){ //fin de partie
                 affichageNiveauGagne(window);
                 this_thread::sleep_for(chrono::milliseconds(50000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));
                 return;
            }

            gumEating(audioEngine, PacMan, gumGrid, NbGum, score);


            if (peutmanger == false){
                Dead = isDead(PacMan, RedGhost, BlueGhost, OrangeGhost, PinkGhost);
            }
            if (peutmanger == true && tick2 == 0){
                tick2 = 150;
            }
            if (peutmanger == true && tick2 != 0){
                tick2 -= 1;
            }
            if (peutmanger == true && tick2 == 1){
                peutmanger = false;
                RedGhost.state = "Hunt";
                PinkGhost.state = "Hunt";
                BlueGhost.state = "Hunt";
                OrangeGhost.state = "Hunt";
            }

            Phase(phase,tick,RedGhost);
            Phase(phase,tick,PinkGhost);
            Phase(phase,tick,OrangeGhost);
            Phase(phase,tick,BlueGhost);


            if (peutmanger == true)
            {
                RedGhost.state = "Hide";
                PinkGhost.state = "Hide";
                BlueGhost.state = "Hide";
                OrangeGhost.state = "Hide";

                if (PacMan.Pos == RedGhost.Pos){
                    score += 100;
                    this_thread::sleep_for(chrono::milliseconds(100000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));
                    RedGhost.Pos.first = 13;
                    RedGhost.Pos.second = 11;
                }
                if( PacMan.Pos == PinkGhost.Pos){
                    score += 200;
                    this_thread::sleep_for(chrono::milliseconds(100000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));
                    PinkGhost.Pos.first = 13;
                    PinkGhost.Pos.second = 11;
                }
                if(PacMan.Pos == BlueGhost.Pos){
                    score += 200;
                    this_thread::sleep_for(chrono::milliseconds(100000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));
                    BlueGhost.Pos.first = 13;
                    BlueGhost.Pos.second = 11;
                }
                if (PacMan.Pos == OrangeGhost.Pos){
                    score += 200;
                    this_thread::sleep_for(chrono::milliseconds(100000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));
                    OrangeGhost.Pos.first = 13;
                    OrangeGhost.Pos.second = 11;
                }
            }

            //*****************************************************************************
            //******************************     Ghost     ********************************
            //*****************************************************************************
            if (tick%2 == 0){   //Bouge un tour sur deux pour ralentir et facilité le jeu
                RedGhostMove(RedGhost,PacMan,gridInfo.first);
                PinkGhostMove(PinkGhost,PacMan,gridInfo.first);
                OrangeGhostMove(OrangeGhost,PacMan,gridInfo.first);
                BlueGhostMove(BlueGhost,PacMan,gridInfo.first);

                GhostMovement(entityGrid,RedGhost, tick);
                GhostMovement(entityGrid,BlueGhost, tick);
                GhostMovement(entityGrid,OrangeGhost, tick);
                GhostMovement(entityGrid,PinkGhost, tick);
            }

            //*****************************************************************************
            //******************     met les visuels des entitées     *********************
            //*****************************************************************************
            window << initSprite(RedGhost, entityGrid, tick, tick2);
            window << initSprite(BlueGhost, entityGrid, tick, tick2);
            window << initSprite(OrangeGhost, entityGrid, tick, tick2);
            window << initSprite(PinkGhost, entityGrid, tick, tick2);
            window << initSprite(PacMan, entityGrid, tick, tick2);

            affichageScore(window, score);
            affichageVies(PacMan.SpriteMap, window, vies);
            affichageNiveau(window, niveau);
        }


        if (tick == 65535) // si tick atteint le mex remettre a 0
            tick = 0;

        if (NbGum == 0){ // victoire -> niveau suivant
            affichageNiveauGagne(window);
            niveau +=1;
            tick = 0;
            WinRound = true;
        }

        // On finit la frame en cours
        window.finishFrame();

        // On vide la queue d'évènements
        window.getEventManager().clearEvents();

        // On attend un peu pour limiter le framerate et soulager le CPU
        this_thread::sleep_for(chrono::milliseconds(10000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));

        // On récupère le temps de frame
        frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);
    }
}
