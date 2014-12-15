#include <QPainter>
#include <QPropertyAnimation>
#include "PieceGraphicsItem.h"
#include "BoardScene.h"
#include "insist.h"

/*
 * BoardScene is the QGraphicsScene that draws the chessboard. The squares are drawn as the QGraphicsScene's background.
 * and the chess pieces are QGraphicPixmapItems (subclassed as PieceGraphicsItem)
 *
 * BoardScene handles the user's piece moving, using an Engine to determine if a move is legal or not. It makes sure
 * after every move the piece is positioned in the center of its square, and it undoes any illegal move
 * the user might have made, using an animation.
 *
 * square colors taken from the site where i stole the piece icons from.
 * http:/poisson.phc.unipi.it/~monge/chess_art.php
 */

#define DARK_SQUARE_RGB (91, 109, 141)
#define LIGHT_SQUARE_RGB (200, 194, 170)
#define MARGIN 0.1

/*
 * BoardScene's constructor makes the pixmap lists. These should probably be statically
 * allocated and shared across instances of BoardScenes, but you can't allocate QPixmaps
 * statically because qt requires a GQuiApplication to be constructed before any QPixmaps are.
 */

BoardScene::BoardScene (PieceList*pieceList, Engine*engine, qreal width, qreal height, QObject*parent) : QGraphicsScene (0, 0, width, height, parent),
  plainPixmaps
  ({
{PieceList::wPawn,new QPixmap (":/images/pieces/wp.png")},
{PieceList::wPawn, new QPixmap (":/images/pieces/wp.png")},
{PieceList::bPawn, new QPixmap (":/images/pieces/bp.png")},
{PieceList::wKnight, new QPixmap (":/images/pieces/wn.png")},
{PieceList::bKnight, new QPixmap (":/images/pieces/bn.png")},
{PieceList::wBishop, new QPixmap (":/images/pieces/wb.png")},
{PieceList::bBishop, new QPixmap (":/images/pieces/bb.png")},
{PieceList::wRook, new QPixmap (":/images/pieces/wr.png")},
{PieceList::bRook, new QPixmap (":/images/pieces/br.png")},
{PieceList::wQueen, new QPixmap (":/images/pieces/wq.png")},
{PieceList::bQueen, new QPixmap (":/images/pieces/bq.png")},
{PieceList::wKing, new QPixmap (":/images/pieces/wk.png")},
{PieceList::bKing, new QPixmap (":/images/pieces/bk.png")}
   }
   ),
  shadowPixmaps
  ({
{PieceList::wPawn,new QPixmap (":/images/pieces_shaded/wp.png")},
{PieceList::wPawn, new QPixmap (":/images/pieces_shaded/wp.png")},
{PieceList::bPawn, new QPixmap (":/images/pieces_shaded/bp.png")},
{PieceList::wKnight, new QPixmap (":/images/pieces_shaded/wn.png")},
{PieceList::bKnight, new QPixmap (":/images/pieces_shaded/bn.png")},
{PieceList::wBishop, new QPixmap (":/images/pieces_shaded/wb.png")},
{PieceList::bBishop, new QPixmap (":/images/pieces_shaded/bb.png")},
{PieceList::wRook, new QPixmap (":/images/pieces_shaded/wr.png")},
{PieceList::bRook, new QPixmap (":/images/pieces_shaded/br.png")},
{PieceList::wQueen, new QPixmap (":/images/pieces_shaded/wq.png")},
{PieceList::bQueen, new QPixmap (":/images/pieces_shaded/bq.png")},
{PieceList::wKing, new QPixmap (":/images/pieces_shaded/wk.png")},
{PieceList::bKing, new QPixmap (":/images/pieces_shaded/bk.png")}
   }
   )
{
  insist (pieceList);
  insist (engine);
  this->pieceList = pieceList;
  this->engine = engine;
  refreshPieces ();
}


/*
 * the width & height of BoardScene might not be the same between the time the user
 * resizes the BoardWindow and the time that we correct the size programatically to maintain
 * a square aspect ratio. Use the smaller of these dimensions (if they are different) in
 * the drawing code.
 */

int BoardScene::side ()
{
  return qMin (width (), height ());
}

/*
 * return the scene coordinates of a piece at boardIndex
 */
QPointF BoardScene::boardIndexToPos (int boardIndex)
{
  insist (boardIndex >= 0 && boardIndex < 64);
  int squareSize = side () / 8;
  int margin = squareSize * MARGIN;

  int x, y;

  if (engine->getHumanIsWhite ())
  {
    x = boardIndex % 8;
    y = 7 - boardIndex / 8;
  }
  else
  {
    x = boardIndex % 8;
    y = boardIndex / 8;
  }

  x = squareSize * x + margin;
  y = squareSize * y + margin;
  return QPointF (x, y);
}

/*
 * return the board index of the square containing scene coordinate pos
 */
int BoardScene::boardIndexFromPos (const QPointF&pos)
{
  int squareSize = side () / 8;
  int x = pos.x ();
  int y = height () - pos.y ();

  int index = 8 * (y / squareSize) + x / squareSize;
  insist (index >= 0 && index < 64);

  if (!engine->getHumanIsWhite ())
    index = 63 - index;
  return index;
}


/*
 * delete any existing PieceGraphicsItems, and create new ones corresponding to
 * the current state of the pieceList. this should be called whenever the scene
 * size changes or when the pieceList changes.
 */

void BoardScene::refreshPieces ()
{
  insist (pieceList);

  /*remove all items*/
  auto items = this->items ();
  for (auto i = items.begin (); i != items.end (); i++)
  {
    removeItem (*i);
    delete *i;
  }

  /*go through the pieceList, creating items, positioning them, and adding them to the scene*/
  int squareSize = side () / 8;
  int margin = squareSize * MARGIN;

  for (int i = 0; i < 64; i++)
  {
    PieceList::Piece piece = pieceList->getPiece (i);
    if (piece != PieceList::None)
    {
      PieceGraphicsItem*g = new PieceGraphicsItem (squareSize - 2 * margin, i, *plainPixmaps [piece], *shadowPixmaps [piece]);
      g->setPos (boardIndexToPos (i));
      addItem (g); //QGraphicsScene owns item now
      connect (g, &PieceGraphicsItem::released, this, &BoardScene::released);
    }
  }
}


/*
 * draw the chessboard's squares.
 */

void BoardScene::drawBackground (QPainter*painter, const QRectF&)
{
  insist (painter);

  /*
   * the width & height of BoardScene might not be the same between the time the user
   * resizes the BoardWindow and the time that we correct the size programatically to maintain
   * a square aspect ratio. Use the smaller of these dimensions (if they are different) in
   * the drawing code.
   */

  int side = this->side ();

  painter->setRenderHint (QPainter::Antialiasing, true);

  QBrush squareBrushes [2] = { QBrush (QColor LIGHT_SQUARE_RGB), QBrush (QColor DARK_SQUARE_RGB)};

  int squareSide = side / 8;
  insist (squareSide > 0);

  for (int y = 0; y < 8; y++)
  {
    int startBrush = y % 2; //what square brush the row starts on
    if (!engine->getHumanIsWhite ()) startBrush ++; //if the board is flipped, the color pattern reverses
    for (int x = 0; x < 8; x++)
    {
      /*
       * adjust the origin and size of the squares along the edges to make them
       * draw slightly off screen for the top and leftmost row and column, and to stretch to
       * on the rightmost and bottom column and row to take into account the extra space leftover
       * when the size of the scene is not a multiple of 8
       */

      int yoff = y == 0 ? 2 : 0;
      int xoff = x == 0 ? 2 : 0;
      int ySide = squareSide + yoff + (y == 7 ? 4 + side % 8 : 0);
      int xSide = squareSide + xoff + (x == 7 ? 4 + side % 8 : 0);;

      painter->fillRect (QRect (x * squareSide - xoff, y * squareSide - yoff, xSide, ySide), squareBrushes [(x + startBrush) % 2]);
    }
  }
}

/*
 * slot called when a piece has been moved. mousePos is where the mouse was
 * in screen coordinates. the piece's pos might not be in the square where the piece
 * is because pos is its top-left origin and the user might have dropped the piece
 * near or across square borders. so we use the mouse position to resolve this.
 * Also the piece graphics have transparency which makes it even more likely that the user
 * can drag the origin of the piece to a different square than the piece itself is on.
 *
 *
 */

void BoardScene::released (PieceGraphicsItem*piece, const QPointF&mousePos)
{
  insist (piece);
  insist (pieceList);
  insist (engine);

  /*get the from and to indexes. the piece hasn't been moved yet except visibly on the screen*/

  int oldBoardIndex = piece->getBoardIndex ();
  int newBoardIndex = boardIndexFromPos (mousePos);
  insist (oldBoardIndex >= 0 && oldBoardIndex < 64);
  insist (newBoardIndex >= 0 && newBoardIndex < 64);

  /*
   * if the move is legal, make the move in the engine, and also update the index information
   * on the pieceList's piece.
   */

  if (engine->humanMove (oldBoardIndex, newBoardIndex))
  {
    PieceList::Piece p = pieceList->getPiece (oldBoardIndex);
    pieceList->setPiece (oldBoardIndex, PieceList::None);
    pieceList->setPiece (newBoardIndex, p);
    piece->setBoardIndex (newBoardIndex);
    piece->setPos (boardIndexToPos (piece->getBoardIndex ()));
  }

  /*
   * animate the piece either to the center of its new square or back where
   * it came from, if it was an illegal move
  */

  QPropertyAnimation*animation = new QPropertyAnimation (piece, "pos");
  animation->setDuration (ANIMATION_DURATION);
  animation->setStartValue (piece->pos ());
  animation->setEndValue (boardIndexToPos (piece->getBoardIndex ()));
  animation->start (QAbstractAnimation::DeleteWhenStopped);

  /*arrange to have the piece's shadow turned off when the animation is finished*/
  connect (animation, &QAbstractAnimation::finished, [piece] ()
  {
    piece->setShadow (false);
  });

}

