#ifndef BoardWindow_h
#define BoardWindow_h

#include <QMainWindow>
#include <QGraphicsView>
#include "BoardScene.h"
#include "BoardView.h"

class BoardWindow : public QMainWindow
{
  Q_OBJECT

  public:
  BoardWindow (bool humanIsWhite, QWidget*parent=0);
  ~BoardWindow ();
  bool confirmClose ();

  private:
  enum
  {
    MIN_DIMENSION = 300,
    MAX_DIMENSION = 900
  };
  Engine*engine;
  BoardView*view;
  BoardScene*scene;
  PieceList pieceList;

  protected:
  virtual void closeEvent (QCloseEvent*event);
  virtual void resizeEvent (QResizeEvent*event);

  public slots:
};

#endif //BoardWindow_h
