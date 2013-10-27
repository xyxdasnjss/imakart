#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "GameEnumerations.hpp"
#include "RaceEventHandler.hpp"
#include "MenuEventHandler.hpp"
#include "Player.hpp"

/**
 * @brief Classe principale d'Imakart
 * Gère le déroulement du jeu.
 */
class GameEngine
{
public:
  GameEngine();

  void init();
  void update();

  const GameState getState() const
    {return state;}

  void setState(GameState newState)
    {state = newState;}

  const EventHandler& getHandler() const;

  Player& getPlayer()
    {return player;}

private:
  GameState state;
  RaceEventHandler raceEventHandler;
  MenuEventHandler menuEventHandler;
  Player player;
};

#endif // GAMEENGINE_HPP