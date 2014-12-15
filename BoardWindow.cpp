#include <QtGui>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include "BoardWindow.h"
#include "Application.h"
#include "insist.h"

/*
 * Window to implement the chessboard. The drawing is done in a QGraphicsView/QGraphicsScene where the
 * QGraphicsScene background draws the squares and the pieces are QGraphicsItems.
 *
 */

BoardWindow::BoardWindow (bool humanIsWhite, QWidget*parent) : QMainWindow (parent)
{
  /*make a new game engine*/
  engine = new Engine (humanIsWhite);

  setMinimumSize (QSize (MIN_DIMENSION, MIN_DIMENSION));
  setMaximumSize (QSize (MAX_DIMENSION, MAX_DIMENSION));

  /*closing the board window means killing it, not just hiding it*/
  setAttribute(Qt::WA_DeleteOnClose);

  /* set up the graphics view. the scene is always going to be the same size as the view --
   * we are going to make sure of this by overriding the BoardView's resize event handler. we make
   * sure the the scrollbars are never displayed to avoid some possible recursion in the
   * resizeEvent hander.
   */

  view = new BoardView ();
  view->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
  view->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

  /* make the scene. its actual size will be set when its view is shown for the first time
   * and whenever its view is resized
   */

  scene = new BoardScene (&pieceList, engine, width(), height(), this);
  view->setScene (scene);
  view->setRenderHints (QPainter::Antialiasing | QPainter::TextAntialiasing);
  setCentralWidget (view);
}

BoardWindow::~BoardWindow ()
{
  delete engine;
}


bool BoardWindow::confirmClose ()
{
  return QMessageBox::warning (this, tr ("Close"),
                               tr ("Do you really want to quit the game?"),
                               QMessageBox::No | QMessageBox::Yes,
                               QMessageBox::Yes) == QMessageBox::Yes;

}

void BoardWindow::closeEvent (QCloseEvent*event)
{
  if (confirmClose ())
    event->accept ();
  else
    event->ignore ();
}

/*
 * override resizeEvent to make sure that the window is always square.
 * we do this by calling resize () on the widget if
 * height and width aren't the same. this will cause
 * another call to resizeEvent, eventually, but the next
 * time through the width and height will be the same
 * and there the recursion stops.
 */

void BoardWindow::resizeEvent (QResizeEvent*event)
{
  QSize size = this->size ();
  if (size.height () != size.width ())
  {
    if (size.width () < size.height ())
      size.setWidth (size.height ());
    else
      size.setHeight (size.width ());
    resize (size);
  }

  QMainWindow::resizeEvent (event);
}
