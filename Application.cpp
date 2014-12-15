#include <iostream>
#include <QMessageBox>
#include "Application.h"
#include "BoardWindow.h"
#include "insist.h"
/*
 * Application handles application-wide stuff:
 *
 * - a global menu bar that doesn't require the existence of any main windows
 * - slots for the menu items that do stuff like quit, close, new game, etc,
 *   these slots in some cases operate on the current chess board window,
 *   this is simpler than having each BoardWindow modify the menubar when
 *   it becomes the active window.
 *
 *   menu items are enabled/disabled in straight code rather than using qt's state machine
 *   stuff just out of simplicty.
 */

Application::Application (int&argc, char**argv) : QApplication (argc, argv)
{
  /*make the global menubar*/
  menuBar = new QMenuBar (0);

  createActions ();
  createMenus ();
  setQuitLockEnabled (false);
}
/*
 * Application is a singleton, return its global instance
 */

Application*Application::application ()
{
  return (Application*)qApp;
}

/*
 * override notify to handle exceptions, which in practice are just our assertions.
 * this doesn't work...
 */

bool Application::notify (QObject*receiver, QEvent*e)
{
  try
  {
    return QApplication::notify (receiver, e);
  }
  catch (InsistException&e)
  {
    std::cout<<e.getMessage() << std::endl;
  }
  return true;
}

/*
 * return list of all the board windows
 * this just goes through all the top level widgets and selects
 * those that are of type BoardWindow
 */

std::vector<BoardWindow*>Application::boardWindows ()
{
  std::vector<BoardWindow*>v;
  QList<QWidget*>widgets = QApplication::topLevelWidgets ();
  for (int i = 0; i < widgets.size (); i++)
  {
    BoardWindow*bw = dynamic_cast<BoardWindow*>(widgets [i]);
    if (bw)
      v.push_back (bw);
  }
  return v;
}

/*
 * create the actions for the global menu.
 */

void Application::createActions ()
{
  newAction = new QAction (tr ("&New Game"), this);
  newAction->setShortcut (QKeySequence::New);
  connect (newAction, &QAction::triggered, this, &Application::newBoard);

  closeAction = new QAction (tr ("&Close Game"), this);
  closeAction->setShortcut (QKeySequence::Close);
  connect (closeAction, &QAction::triggered, this, &Application::closeBoard);

  aboutAction = new QAction (tr ("&About %1").arg (qApp->applicationDisplayName()), this);
  aboutAction->setMenuRole (QAction::AboutRole);
  connect (aboutAction, &QAction::triggered, this, &Application::about);

  quitAction = new QAction (tr ("&Quit %1").arg (qApp->applicationDisplayName()), this);
  quitAction->setMenuRole (QAction::QuitRole);
  connect (quitAction, &QAction::triggered, this, &Application::quit);

  /*
   * this action doesn't have any slots since it's used used for its state
   * when a new board window is created
   */

  humanIsWhiteAction = new QAction (tr ("Human Plays &White"), this);
  humanIsWhiteAction->setCheckable (true);
  humanIsWhiteAction->setChecked (true);
  pauseResumeAction = new QAction (tr ("&Pause"), this);
  offerDrawAction = new QAction (tr ("&Offer Draw"), this);
}

/*
 * create the global menu bar and its menus. the menus won't show up unless they
 * are populated before they are added to the menu bar.
 */
void Application::createMenus ()
{
  insist (newAction && quitAction && aboutAction && quitAction);
  insist (pauseResumeAction && offerDrawAction);

  /*the menu won't show up unless we add its menu items first*/
  fileMenu = new QMenu (tr ("&File"));

  fileMenu->addAction (newAction);
  fileMenu->addAction (closeAction);
  closeAction->setEnabled (false); //disabled until there's at least one board window up

  /* these will get automatically moved off the fileMenu depending on their menuRoles*/
  fileMenu->addAction (aboutAction);
  fileMenu->addAction (quitAction);
  menuBar->addMenu (fileMenu);

  gameMenu = new QMenu (tr ("&Game"));
  gameMenu->addAction (humanIsWhiteAction);
  gameMenu->addAction (pauseResumeAction);
  gameMenu->addAction (offerDrawAction);
  menuBar->addMenu (gameMenu);
}


/*slots*/
/*
 * slot called to create and show a new board window
 */
void Application::newBoard ()
{
  BoardWindow*bw = new BoardWindow (humanIsWhiteAction->isChecked ());
  bw->show ();

  /*now that there's a board window, enable close and disable new.
    we're only allowing one open window at a time right now.*/

  closeAction->setEnabled (true);
  newAction->setEnabled (!closeAction->isEnabled ());

  /*connect a slot to the board's destroyed signal so we can update some menu state*/
  connect (bw, &QWidget::destroyed, this, &Application::boardDestroyed);
}

/*
 * slot called when the closeAction is triggered.
 */
void Application::closeBoard ()
{
  BoardWindow*bw = dynamic_cast<BoardWindow*>(activeWindow());

  insist (bw);

  /*possibly close the window. if the user says no, the nothing will happen*/
  bw->close ();
}

/*
 * slot called when a boardWindow is destroyed. this just enable/disables the new/close menu actions
 */
void Application::boardDestroyed (QObject*)
{
  closeAction->setEnabled (activeWindow () != 0);
  newAction->setEnabled (!closeAction->isEnabled ());
}

/*
 * slot called from quit action. if there are any open boards, asks to close each one
 * before qutting.
 */
void Application::quit ()
{
  std::vector<BoardWindow*>windows = boardWindows();
  for (size_t i = 0; i < windows.size(); i++)
  {
    if (!windows [i]->confirmClose ())
      return;
  }
  QCoreApplication::quit ();
}

/*
 * slot called from about action
 */

void Application::about ()
{
  quitAction = new QAction (tr ("&Quit %1").arg (qApp->applicationDisplayName()), this);

  QMessageBox::about (getMenuBar(), tr ("About %1").arg (qApp->applicationDisplayName ()),
                      tr ("<center><b>DrB</b></center><p>DrB is a chess playing program written in C++/qt. \
                          It's named after the main character in <i>Schachnovelle</i> by Stefan Zweig. \
                          <p><p><center>2014 David Finucane</center>"));
}
