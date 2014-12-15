#include <iostream>
#include "Application.h"
#include "BoardWindow.h"
#include "insist.h"

/*
 * qt application to play chess against a human
 *
 * in writing this i followed some rules involving c++ and qt.
 * 1. avoid using const and unsigned.
 * 2. use c++11 features.
 * 3. when in doubt about UX, only care about it making sense on the mac.
 * 4. use c++ features over qt features (for instance collections).
 * 5. getters are named with get* to avoid clashes with member names.
 * 6. the chess-related constants 8 and 64 are used directly, without #defines or the like.
 */

int main (int argc, char *argv[])
{
  try
  {
    setbuf(stdout, NULL);
    Application a (argc, argv);
    return a.exec ();
  }
  catch (InsistException&e)
  {
    std::cout<<e.getMessage() << std::endl;
  }
}
