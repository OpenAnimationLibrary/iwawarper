//----------------------------------
// MainStatusBar
// ���C���E�B���h�E�̃X�e�[�^�X�o�[
// �������Ƀ��b�Z�[�W�A�E�Ƀt���[���X���C�_��\��
//----------------------------------

#include "mainstatusbar.h"

#include "iwapp.h"
#include "iwprojecthandle.h"
#include "iwproject.h"

#include <QStatusBar>
#include <QLineEdit>
#include <QSlider>
#include <QHBoxLayout>
#include <QIntValidator>

MainStatusBar::MainStatusBar(QWidget* parent) : QToolBar(parent) {
  setMovable(false);

  //--- �I�u�W�F�N�g�̐錾
  m_status      = new QStatusBar(this);
  m_frameEdit   = new QLineEdit(this);
  m_frameSlider = new QSlider(this);

  //--- �v���p�e�B�̐ݒ�
  m_status->setSizeGripEnabled(false);
  m_frameSlider->setOrientation(Qt::Horizontal);
  m_frameSlider->setRange(1, 1);

  m_validator = new QIntValidator(1, 1, this);
  m_frameEdit->setValidator(m_validator);

  m_frameSlider->setValue(1);
  m_frameEdit->setText(QString::number(1));

  //--- �E�Ɏ��܂�t���[���X���C�_
  QWidget* fsWidget     = new QWidget(this);
  QHBoxLayout* fsLayout = new QHBoxLayout();
  fsLayout->setMargin(0);
  fsLayout->setSpacing(3);
  {
    fsLayout->addWidget(m_status, 1);
    fsLayout->addWidget(m_frameSlider, 1);
    fsLayout->addWidget(m_frameEdit, 0);
  }
  fsWidget->setLayout(fsLayout);

  addWidget(fsWidget);

  // �V�O�i���^�X���b�g�ڑ�

  // �v���W�F�N�g�����ς������X���C�_�͈̔͂��X�V����
  IwProjectHandle* projectHandle = IwApp::instance()->getCurrentProject();
  connect(projectHandle, SIGNAL(projectChanged()), this,
          SLOT(onProjectChanged()));
  // ���̑���iKeyFrameEditor�Ȃǁj�ŃJ�����g�t���[�����ς�������A�X���C�_�𓯊�����
  connect(projectHandle, SIGNAL(viewFrameChanged()), this,
          SLOT(onViewFrameChanged()));

  // �J�����g�t���[����ύX���鑀��
  connect(m_frameSlider, SIGNAL(sliderMoved(int)), this,
          SLOT(onFrameSliderMoved(int)));
  connect(m_frameEdit, SIGNAL(editingFinished()), this,
          SLOT(onFrameEditFinished()));
}

//----------------------------------
// �X�e�[�^�X�o�[�Ƀ��b�Z�[�W��\������
//----------------------------------
void MainStatusBar::showMessageOnStatusBar(const QString& message,
                                           int timeout) {
  m_status->showMessage(message, timeout);
}

// �v���W�F�N�g�����ς������X���C�_�͈̔͂��X�V����
void MainStatusBar::onProjectChanged() {
  IwProject* prj  = IwApp::instance()->getCurrentProject()->getProject();
  int frameLength = prj->getProjectFrameLength();

  if (frameLength == 0) return;
  // �O�Ɠ����l�Ȃ�return
  if (m_frameSlider->maximum() == frameLength) return;

  m_frameSlider->setMaximum(frameLength);
  m_validator->setRange(1, frameLength);

  update();
}

// �J�����g�t���[����ύX���鑀��
void MainStatusBar::onFrameSliderMoved(int frame) {
  // �G�f�B�b�g�𓯊�
  m_frameEdit->setText(QString::number(frame));

  IwProject* prj = IwApp::instance()->getCurrentProject()->getProject();

  prj->setViewFrame(frame - 1);
}

void MainStatusBar::onFrameEditFinished() {
  int frame = m_frameEdit->text().toInt();

  m_frameSlider->setValue(frame);

  IwProject* prj = IwApp::instance()->getCurrentProject()->getProject();

  prj->setViewFrame(frame - 1);
}

// ���̑���iKeyFrameEditor�Ȃǁj�ŃJ�����g�t���[�����ς�������A�X���C�_�𓯊�����
void MainStatusBar::onViewFrameChanged() {
  IwProject* prj = IwApp::instance()->getCurrentProject()->getProject();
  if (!prj) return;

  int frame = prj->getViewFrame() + 1;

  // �X���C�_�𓯊�
  m_frameSlider->setValue(frame);
  // �G�f�B�b�g�𓯊�
  m_frameEdit->setText(QString::number(frame));

  update();
}