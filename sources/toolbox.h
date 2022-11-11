//---------------------------------------
// Tool Box
// MainWindow�������̃c�[���{�b�N�X
//---------------------------------------
#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QToolBar>
#include <QDockWidget>

#include "menubarcommand.h"

class ToolBoxDock : public QDockWidget {
public:
  ToolBoxDock(QWidget *parent);
};

class ToolBox : public QToolBar {
  void addActionToToolBox(CommandId id);

public:
  ToolBox(QWidget *parent);
};

#endif