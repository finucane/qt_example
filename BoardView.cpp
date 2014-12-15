#include "BoardView.h"
#include "BoardScene.h"
#include "insist.h"

/*
 * BoardView is the QGraphicsView that manages the BoardScene. It handles resize events by
 * resetting the scene's "sceneRect" and also by telling the scene to update its items to
 * take into account the new board size.
 */

BoardView::BoardView (QWidget*parent) : QGraphicsView (parent)
{
#if 0
  /*
   * set the background to be black, this hides that there's a 1-2 pixel
   * white margin around the scene
   */
  QPalette palette;
  palette.setColor(backgroundRole(), Qt::black);
  setPalette (palette);
  setAutoFillBackground (true);
#endif
}

/*
 * make sure the scene size matches the view size.
 * this is called from show and resize events.
 */

void BoardView::updateSceneRect ()
{
  BoardScene*scene = (BoardScene*)this->scene ();
  insist (scene);

  scene->setSceneRect (QRectF (QPointF (0, 0), size()));
  fitInView (scene->sceneRect());
  scene->refreshPieces ();
}


/*
 * override resizeEvent to make sure the QGraphicsScene
 * sceneRect matches the GraphicsView size.
 */

void BoardView::resizeEvent (QResizeEvent*event)
{
  updateSceneRect ();
  QGraphicsView::resizeEvent (event);
}

/*
 * override showEvent to make sure the GraphicsScene
 * sceneRect matches the GraphicsView size. This handler will
 * be where the GraphicsScene gets its initial sceneRect.
 */

void BoardView::showEvent (QShowEvent*event)
{
  updateSceneRect ();
  QGraphicsView::showEvent (event);
}
