#include "Engine.h"

Engine::Engine (bool humanIsWhite)
{
  this->humanIsWhite = humanIsWhite;
}

/*
 * if it's legal, move the piece from "from" to "to and return true.
 * return false otherwise.
 */

bool Engine::humanMove (int fromBoardIndex, int toBoardIndex)
{
  return fromBoardIndex % 2;
}
