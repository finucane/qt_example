#include "PieceList.h"

PieceList::PieceList()
{
  reset ();
}

void PieceList::setPiece (int rank, int file, Piece piece)
{
  insist (rank >= 0 && rank < 8);
  insist (file >= 0 && file < 8);

  squares [rank * 8 + file] = piece;
}

void PieceList::setPiece (int boardIndex, Piece piece)
{
  insist (boardIndex >= 0 && boardIndex < 64);

  squares [boardIndex] = piece;
}

PieceList::Piece PieceList::getPiece (int rank, int file)
{
  insist (rank >= 0 && rank < 8);
  insist (file >= 0 && file < 8);

  return squares [rank * 8 + file];
}

PieceList::Piece PieceList::getPiece (int boardIndex)
{
  insist (boardIndex >= 0 && boardIndex < 64);
  return squares [boardIndex];
}
/*
 * true if the piece is white. the pieces are ordered w,b,... starting at 0.
 */
bool PieceList::isWhite (Piece piece)
{
  return piece % 2 == 0;
}

/*
 * flip a piece's color. the pieces are ordered w,b,... starting at 0.
 */
PieceList::Piece PieceList::other (Piece piece)
{
  return (Piece) (isWhite (piece) ? piece + 1 : piece - 1);
}

/*
 * set the board to represent a newly set up game
 */

void PieceList::reset ()
{
  for (int i = 0; i < 64; i++)
    squares [i] = None;

  /*white side*/
  squares [0] = wRook;
  squares [1] = wBishop;
  squares [2] = wKnight;
  squares [3] = wKing;
  squares [4] = wQueen;
  squares [5] = squares [2]; //symmetric
  squares [6] = squares [1];
  squares [7] = squares [0];

  for (int i = 8; i < 16; i++)
    squares [i] = wPawn;

  /*black side mirrors white side*/
  for (int i = 0; i < 8; i++)
    squares [i + 48] = other (squares [i + 8]);
  for (int i = 0; i < 8; i++)
    squares [i + 56] = other (squares [i + 0]);
}
