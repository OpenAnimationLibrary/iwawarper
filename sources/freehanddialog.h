//---------------------------------
// Free Hand Dialog
// FreeHandTool�̃I�v�V������\������_�C�A���O
// ���[�_���ł͂Ȃ�
//---------------------------------

#ifndef FREEHANDDIALOG_H
#define FREEHANDDIALOG_H

#include "iwdialog.h"

#include "freehandtool.h"

class QSlider;
class QLineEdit;
class QLabel;

class FreeHandDialog : public IwDialog {
  Q_OBJECT

  FreeHandTool* m_tool;

  // �J�[�u�̐��x�X���C�_
  QSlider* m_precisionSlider;
  QLineEdit* m_precisionLineEdit;

  // ���݂̃J�[�u�̃R���g���[���|�C���g��
  QLabel* m_controlPointsLabel;

  // �{�^��
  QPushButton* m_closeShapeButton;
  QPushButton* m_deleteShapeButton;

public:
  FreeHandDialog();

protected slots:
  void onPrecSliderMoved(int val);
  void onPrecLineEditEditingFinished();

  // �R���g���[���|�C���g���̍X�V
  void updateCPCount(int amount);

  void onCloseShapeButtonClicked(bool);
};

#endif