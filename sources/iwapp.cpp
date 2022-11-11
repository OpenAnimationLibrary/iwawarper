#include "iwapp.h"

#include "mainwindow.h"
#include "iwprojecthandle.h"
#include "iwselectionhandle.h"
#include "iwtoolhandle.h"
#include "iwlayerhandle.h"

IwApp::IwApp()
    : m_mainWindow(0)
    , m_projectHandle(0)
    , m_selectionHandle(0)
    , m_toolHandle(0)
    , m_layerHandle(0) {
  m_projectHandle   = new IwProjectHandle();
  m_selectionHandle = IwSelectionHandle::getCurrent();
  m_toolHandle      = new IwToolHandle();
  m_layerHandle     = new IwLayerHandle();

  connect(m_layerHandle, &IwLayerHandle::layerChanged,
          [&]() { m_projectHandle->setDirty(); });
  // �v���W�F�N�g���؂�ւ������I������������
  connect(m_projectHandle, SIGNAL(projectSwitched()), m_selectionHandle,
          SLOT(onProjectSwitched()));
}

//-----------------------------------------------------------------------------

IwApp* IwApp::instance() {
  static IwApp _instance;
  return &_instance;
}

//-----------------------------------------------------------------------------

IwApp::~IwApp() {}

//-----------------------------------------------------------------------------

// ���[�h����Ă���v���W�F�N�g���X�g�Ƀv���W�F�N�g��}��
void IwApp::insertLoadedProject(IwProject* project) {
  m_loadedProjects.append(project);
}

//-----------------------------------------------------------------------------
// �X�e�[�^�X�o�[�Ƀ��b�Z�[�W��\������
void IwApp::showMessageOnStatusBar(const QString& message, int timeout) {
  if (m_mainWindow) m_mainWindow->showMessageOnStatusBar(message, timeout);
}