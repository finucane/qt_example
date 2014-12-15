#ifndef BoardScene_h
#define BoardScene_h

#include <unordered_map>
#include <QGraphicsScene>
#include "PieceGraphicsItem.h"
#include "PieceList.h"
#include "Engine.h"

class BoardScene : public QGraphicsScene
{
  Q_OBJECT
  public:
  explicit BoardScene (PieceList*pieceList, Engine*engine, qreal width, qreal height, QObject*parent=0);
  void refreshPieces ();

  protected:
  virtual void drawBackground (QPainter*painter, const QRectF&rect);

  private:
  enum
  {
    ANIMATION_DURATION = 300
  };
  Engine*engine;
  PieceList*pieceList;
  std::unordered_map <int, QPixmap*>plainPixmaps;
  std::unordered_map <int, QPixmap*>shadowPixmaps;
  int side ();
  int boardIndexFromPos (const QPointF&pos);
  QPointF boardIndexToPos (int boardIndex);

  signals:

  public slots:
  void released (PieceGraphicsItem*piece, const QPointF&mousePos);
};

#endif // BoardScene_h
