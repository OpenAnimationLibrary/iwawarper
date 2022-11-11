#pragma once
//---------------------------------
// Render Shape Image Dialog
// �V�F�C�v�摜�̏����o���ݒ�
//---------------------------------

#ifndef RENDERSHAPEIMAGEDIALOG_H
#define RENDERSHAPEIMAGEDIALOG_H

#include "iwdialog.h"

class QLineEdit;
class QComboBox;
class OutputSettings;

class RenderShapeImageDialog : public IwDialog {
  Q_OBJECT

  QLineEdit* m_startFrameEdit;
  QLineEdit* m_endFrameEdit;
  QLineEdit* m_fileNameEdit;

  QLineEdit* m_directoryEdit;

  QComboBox* m_sizeCombo;

  OutputSettings* getCurrentSettings();

public:
  RenderShapeImageDialog();

protected:
  // project���؂�ւ������A���e���X�V����
  void showEvent(QShowEvent*);
  void hideEvent(QHideEvent*);

protected slots:

  // ���݂�OutputSettings�ɍ��킹�ĕ\�����X�V����
  void updateGuis();
  void onStartFrameEditted();
  void onEndFrameEditted();
  void onFileNameEditted();
  void onDirectoryEditted();
  void onOpenBrowserBtnClicked();
  void onSizeComboActivated();
  void onRenderButtonClicked();
};

#endif