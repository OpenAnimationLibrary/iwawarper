//---------------------------------
// Render Settings Dialog
// ���[�t�B���O�A�����_�����O�̐ݒ���s��
//---------------------------------
#ifndef RENDERSETTINGSDIALOG_H
#define RENDERSETTINGSDIALOG_H

#include "iwdialog.h"

class QComboBox;

class RenderSettingsDialog : public IwDialog {
  Q_OBJECT

  QComboBox* m_warpPrecisionCombo;

public:
  RenderSettingsDialog();

protected:
  // project���؂�ւ������A���e���X�V����
  void showEvent(QShowEvent*);
  void hideEvent(QHideEvent*);
protected slots:
  // ���݂�OutputSettings�ɍ��킹�ĕ\�����X�V����
  void updateGuis();

  void onPrecisionComboActivated(int index);
};

#endif