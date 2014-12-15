#ifndef BoardView_h
#define BoardView_h

#include <QGraphicsView>

class BoardView : public QGraphicsView
{
  Q_OBJECT
  public:
  explicit BoardView(QWidget*parent=0);

  protected:
  virtual void resizeEvent (QResizeEvent*event);
  virtual void showEvent(QShowEvent*event);

  private:
  void updateSceneRect ();

  signals:

  public slots:

};

#endif // BoardView_h
