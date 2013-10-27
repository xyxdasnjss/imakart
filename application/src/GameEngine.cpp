#include "GameEngine.hpp"

GameEngine::GameEngine()
  : raceEventHandler(this), menuEventHandler(this){
  state = IN_RACE;
}

void GameEngine::init()
{
  //Charger les données du jeu

}

void GameEngine::update()
{
  //Quand il faudra gérer les IA...
}

const EventHandler& GameEngine::getHandler() const
{
  //En fonction de l'état actuel du jeu, renvoyer le bon handler
  if (state == IN_MENU)
    return menuEventHandler;
  else if (state == IN_RACE)
    return raceEventHandler;

  return raceEventHandler;
}