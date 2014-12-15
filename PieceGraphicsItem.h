#ifndef PieceGraphicsItem_h
#define PieceGraphicsItem_h

#include <QGraphicsPixmapItem>
#include "insist.h"

class PieceGraphicsItem : public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT
  Q_PROPERTY (QPointF pos READ pos WRITE setPos)

  public:
  PieceGraphicsItem (int size, int boardIndex, QPixmap&plainPixmap, QPixmap&shadowPixmap);
  void setShadow (bool shaded);
  void setSize (int size);

  private:
  enum
  {
    TOP_Z = 65,
    BOTTOM_Z = 0
  };
  int size;
  int boardIndex;
  QPointF buttonDownPos;
  QPixmap plainPixmap;
  QPixmap shadowPixmap;
  bool shadow = false;
  void resetPixmap ();

  public:
  int getBoardIndex ()
  {
    return boardIndex;
  }

  void setBoardIndex (int boardIndex)
  {
    insist (boardIndex >= 0 && boardIndex < 64);
    this->boardIndex = boardIndex;
  }
  protected:
  virtual void mousePressEvent (QGraphicsSceneMouseEvent*event);
  virtual void mouseMoveEvent (QGraphicsSceneMouseEvent*event);
  virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent*event);

  signals:
  void released (PieceGraphicsItem*piece, const QPointF&mousePos);
};

#endif // PieceGraphicsItem_h
