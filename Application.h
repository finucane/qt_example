#ifndef Application_h
#define Application_h

#include <QApplication>
#include <BoardWindow.h>
#include <QMenuBar>

class Application : public QApplication
{
  Q_OBJECT

  public:
  explicit Application (int&argc, char**argv);
  static Application*application ();
  virtual bool notify (QObject*receiver, QEvent*e);

  QMenuBar*getMenuBar ()
  {
    return menuBar;
  }

  private:
  QMenuBar*menuBar;
  QMenu*fileMenu;
  QMenu*gameMenu;
  QAction*newAction;
  QAction*closeAction;
  QAction*aboutAction;
  QAction*quitAction;
  QAction*humanIsWhiteAction;
  QAction*pauseResumeAction;
  QAction*offerDrawAction;
  void createActions ();
  void createMenus ();

  public:
  std::vector<BoardWindow*>boardWindows ();

  public slots:
  void newBoard ();
  void closeBoard ();
  void quit ();
  void about ();
  void boardDestroyed (QObject*);
};


#endif //Application_h
