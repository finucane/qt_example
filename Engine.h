#ifndef Engine_h
#define Engine_h

class Engine
{
  public:
  Engine (bool humanIsWhite = true);
  bool humanMove (int fromBoardIndex, int toBoardIndex);
  bool getHumanIsWhite ()
  {
    return humanIsWhite;
  }

  private:
  bool humanIsWhite;
};

#endif // Engine_h
