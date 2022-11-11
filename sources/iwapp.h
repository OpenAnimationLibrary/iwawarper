//------------------------------
// IwApp
// �A�v���P�[�V�����̌��݂̏󋵂�S�Ď���
//------------------------------
#ifndef IWAPP_H
#define IWAPP_H

#include <QObject>
#include <QList>

class IwProject;
class MainWindow;
class IwProjectHandle;
class IwSelectionHandle;
class IwToolHandle;
class IwLayerHandle;

class IwApp : public QObject  // singleton
{
  Q_OBJECT

  // ���C���E�B���h�E�ւ̃|�C���^�B�_�C�A���O�̐e�ɂȂ�B
  MainWindow* m_mainWindow;
  // �v���W�F�N�g���
  IwProjectHandle* m_projectHandle;
  // �I�� ���
  IwSelectionHandle* m_selectionHandle;
  // �c�[�����
  IwToolHandle* m_toolHandle;
  // ���C�����
  IwLayerHandle* m_layerHandle;

  // ���݃A�v���P�[�V�����Ƀ��[�h����Ă���v���W�F�N�g
  QList<IwProject*> m_loadedProjects;

  IwApp();

public:
  static IwApp* instance();
  ~IwApp();

  // ���C���E�B���h�E�ւ̃|�C���^��Ԃ�
  MainWindow* getMainWindow() const { return m_mainWindow; }
  void setMainWindow(MainWindow* mainWindow) { m_mainWindow = mainWindow; }

  // �v���W�F�N�g���
  IwProjectHandle* getCurrentProject() const { return m_projectHandle; }
  // �I�� ���
  IwSelectionHandle* getCurrentSelection() const { return m_selectionHandle; }
  // �c�[�����
  IwToolHandle* getCurrentTool() const { return m_toolHandle; }
  // ���C�����
  IwLayerHandle* getCurrentLayer() const { return m_layerHandle; }

  // ���[�h����Ă���v���W�F�N�g���X�g�Ƀv���W�F�N�g��}��
  void insertLoadedProject(IwProject* project);

  // �X�e�[�^�X�o�[�Ƀ��b�Z�[�W��\������
  void showMessageOnStatusBar(const QString& message, int timeout = 0);
};

#endif