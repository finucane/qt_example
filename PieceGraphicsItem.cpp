#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include "PieceGraphicsItem.h"
#include "insist.h"

/*
 * Class to handle displaying chess pieces on the board, remembering what square the chess piece is on,
 * and dealing with changing the size of the piece and whether it's shown with a shadow or not.
 *
 * QPixmap is one of qt's "shared objects" so it doesn't allocate tons of memory when it's copied and
 * this class goes along with qt's idiom on this and only deals with references to them, not pointers.
 * this simplifies the memory management.
 *
 * all of the pixmaps are assumed to be square, this matters for scaling.
 *
 */

PieceGraphicsItem::PieceGraphicsItem (int size, int boardIndex, QPixmap&plainPixmap, QPixmap&shadowPixmap):
  plainPixmap (plainPixmap),
  shadowPixmap (shadowPixmap)
{
  insist (size > 0 && boardIndex >= 0 && boardIndex < 64);

  this->size = size;
  this->boardIndex = boardIndex;

  /*set the initial pixmap*/
  resetPixmap ();
}


/*
 * create a new shaded or normal pixmap of the current size and set the QGraphicsPixmapItem baseclass's pixmap with it.
 * this is called internally whenever the size or the shadow state changes.
 */
void PieceGraphicsItem::resetPixmap ()
{
  insist (size > 0);

  if (shadow)
    setPixmap (shadowPixmap.scaledToWidth (size, Qt::SmoothTransformation));
  else
    setPixmap (plainPixmap.scaledToWidth (size, Qt::SmoothTransformation));
}

/*
 * change the shadow state of the piece. this will result in a new pixmap
 * being created in order to effect the actual visual change. shaded pieces
 * are also set so their z-order is topmost. when they become unshaded
 * then their z-order is bottommost.
 */

void PieceGraphicsItem::setShadow (bool shadow)
{
  if (this->shadow != shadow)
  {
    this->shadow = shadow;
    setZValue (shadow ? TOP_Z : BOTTOM_Z);
    resetPixmap ();
  }
}

/*
 * change the piece's size.
 */
void PieceGraphicsItem::setSize (int size)
{
  insist (size > 0);

  if (this->size != size)
  {
    prepareGeometryChange ();
    this->size = size;
    resetPixmap ();
  }
}

/*
 * event handlers
*/


/*
 * Event for when the mouse button is pressed.
 */

void PieceGraphicsItem::mousePressEvent (QGraphicsSceneMouseEvent*event)
{
  insist (event);

  /*
   * get the offset in the item's coordinates of where the mouse was initially pressed.
   * we'll use this to move the item as the mouse moves. event->buttonDownPos () doesn't
   * seem to be reliable, sometimes it returns stale data, so do all the math in scene coordinates.
   */
  buttonDownPos =  event->scenePos () - pos ();

  /*draw the piece with a shadow and also make sure its drawn on top of any other pieces*/
  setShadow (true);
}

/*
 * event handler for mouse move, which moves the piece, throwing out any events that occur
 * outside the scene so that the user can't drag pieces out of view.
 */

void PieceGraphicsItem::mouseMoveEvent (QGraphicsSceneMouseEvent*event)
{
  QPointF scenePos = event->scenePos ();

  if (scenePos.x () < 0 || scenePos.y () < 0 || scenePos.x () > scene ()->width () || scenePos.y () > scene ()->height ())
  {
    event->ignore ();
    return;
  }
  setPos (scenePos.x () - buttonDownPos.x (), scenePos.y () - buttonDownPos.y ());
  event->accept ();
}
/*
 * event for when the mouse is released. this just sends the "released" signal. it is up to some other code
 * to determine if the move was legal or not, and
 * to adjust the piece to be centered on its square, etc.
 */
void PieceGraphicsItem::mouseReleaseEvent (QGraphicsSceneMouseEvent*event)
{
  emit released (this, event->scenePos ());
}
