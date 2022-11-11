//----------------------------------
// MainStatusBar
// ���C���E�B���h�E�̃X�e�[�^�X�o�[
//----------------------------------
#ifndef MAINSTATUSBAR_H
#define MAINSTATUSBAR_H

#include <QToolBar>

class QStatusBar;
class QLineEdit;
class QSlider;
class QIntValidator;

class MainStatusBar : public QToolBar {
  Q_OBJECT

  QStatusBar* m_status;
  QLineEdit* m_frameEdit;
  QSlider* m_frameSlider;

  QIntValidator* m_validator;

public:
  MainStatusBar(QWidget* parent);

  // �X�e�[�^�X�o�[�Ƀ��b�Z�[�W��\������
  void showMessageOnStatusBar(const QString& message, int timeout = 0);

protected slots:
  // �v���W�F�N�g�����ς������X���C�_�͈̔͂��X�V����
  void onProjectChanged();
  // �J�����g�t���[����ύX���鑀��
  void onFrameSliderMoved(int);
  void onFrameEditFinished();
  // ���̑���iKeyFrameEditor�Ȃǁj�ŃJ�����g�t���[�����ς�������A�X���C�_�𓯊�����]
  void onViewFrameChanged();
};

#endif