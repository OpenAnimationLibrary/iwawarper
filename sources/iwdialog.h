//---------------------------------------------------
// IwDialog
// �l�ݒ�f�[�^�ɃW�I���g���f�[�^��
// �ǂݏ�������悤�ɂ���
//---------------------------------------------------

#ifndef IWDIALOG_H
#define IWDIALOG_H

#include <QDialog>
#include <QRect>

#include "personalsettingsmanager.h"

class IwDialog : public QDialog {
  bool m_isResizable;
  SettingsId m_dialogName;

  QRect m_geom;

public:
  // �R���X�g���N�^�ŁA�ʒu/�T�C�Y�����[�h
  IwDialog(QWidget* parent, SettingsId dialogName, bool isResizable);
  // �f�X�g���N�^�ŁA�ʒu/�T�C�Y��ۑ�
  ~IwDialog();
  // �_�C�A���O����Ԃ�
  SettingsId getDialogName() { return m_dialogName; }

protected:
  // �J���Ƃ��A�ʒu/�T�C�Y���W�I���g���ɓK�p
  void showEvent(QShowEvent*);
  // ����Ƃ��A�W�I���g�����L�[�v
  void hideEvent(QHideEvent*);
};

#endif
