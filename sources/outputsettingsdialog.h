//---------------------------------
// Output Settings Dialog
// �����_�����O���ʂ̕ۑ��t�@�C���`��/�p�X���w�肷��
//---------------------------------
#ifndef OUTPUTSETTINGSDIALOG_H
#define OUTPUTSETTINGSDIALOG_H

#include "iwdialog.h"

class QComboBox;
class QLineEdit;
class QCheckBox;
class QLabel;
class OutputSettings;

class OutputSettingsDialog : public IwDialog {
  Q_OBJECT

  QLineEdit* m_startFrameEdit;
  QLineEdit* m_endFrameEdit;
  QLineEdit* m_stepFrameEdit;

  QComboBox* m_shapeTagCombo;

  QComboBox* m_saverCombo;
  QLineEdit* m_directoryEdit;

  QLineEdit* m_initialFrameNumberEdit;
  QLineEdit* m_incrementEdit;
  QLineEdit* m_numberOfDigitsEdit;
  QLineEdit* m_extensionEdit;

  QCheckBox* m_useSourceCB;
  QCheckBox* m_addNumberCB;
  QCheckBox* m_replaceExtCB;

  QLineEdit* m_formatEdit;
  QLabel* m_exampleLabel;

  OutputSettings* getCurrentSettings();

public:
  OutputSettingsDialog();

protected:
  // project���؂�ւ������A���e���X�V����
  void showEvent(QShowEvent*);
  void hideEvent(QHideEvent*);

protected slots:

  // ���݂�OutputSettings�ɍ��킹�ĕ\�����X�V����
  void updateGuis();

  void onStartFrameEditted();
  void onEndFrameEditted();
  void onStepFrameEditted();

  void onSaverComboActivated(const QString& text);
  void onParametersBtnClicked();
  void onDirectoryEditted();
  void onOpenBrowserBtnClicked();
  void onInitialFrameNumberEditted();
  void onIncrementEditted();
  void onNumberOfDigitsEditted();
  // �`�F�b�N�{�b�N�X�͂R�܂Ƃ߂ē���SLOT�ɂ���
  void onCheckBoxClicked();
  void onFormatEditted();

  void updateShapeTagComboItems();
  void onShapeTagComboActivated();

  void onRenderButtonClicked();
};

#endif
