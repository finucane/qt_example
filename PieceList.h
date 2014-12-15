#ifndef PieceList_h
#define PieceList_h
#include "insist.h"

class PieceList
{
  public:
  enum Piece
  {
    wPawn,
    bPawn,
    wKnight,
    bKnight,
    wBishop,
    bBishop,
    wRook,
    bRook,
    wQueen,
    bQueen,
    wKing,
    bKing,
    None
  };

  private:
  Piece squares [64]; //LERF mapping, squares [0] = a1, squares [63] = h7

  public:
  PieceList();
  void setPiece (int rank, int file, Piece piece);
  void setPiece (int boardIndex, Piece piece);
  Piece getPiece (int boardIndex);
  Piece getPiece (int rank, int file);
  bool isWhite (Piece piece);
  Piece other (Piece piece);
  void reset ();
};

#endif // PieceList_h
