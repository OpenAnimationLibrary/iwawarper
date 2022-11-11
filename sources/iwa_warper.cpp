#include "iwa_warper.h"

#include "sceneviewer.h"
#include "timelinewindow.h"

iwa_warper::iwa_warper(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  //--- �I�u�W�F�N�g�̐錾

  // �r���[�A
  m_mainViewer = new SceneViewer(this);
  // �^�C�����C��
  m_timeLineWindow = new TimeLineWindow(this);

  // Widget�̃Z�b�g
  setCentralWidget(m_mainViewer);

  addDockWidget(Qt::BottomDockWidgetArea, m_timeLineWindow);
}

iwa_warper::~iwa_warper() {}
