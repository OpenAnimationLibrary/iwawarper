//---------------------------------
// Free Hand Dialog
// FreeHandTool�̃I�v�V������\������_�C�A���O
// ���[�_���ł͂Ȃ�
//---------------------------------

#include "freehanddialog.h"

#include "iwapp.h"
#include "mainwindow.h"
#include "menubarcommandids.h"

#include <QSlider>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QPushButton>
#include <QFrame>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

FreeHandDialog::FreeHandDialog()
    : IwDialog(IwApp::instance()->getMainWindow(), "FreeHandDialog", false) {
  m_tool = dynamic_cast<FreeHandTool*>(IwTool::getTool("T_Freehand"));

  setSizeGripEnabled(false);

  //---- �I�u�W�F�N�g�̐錾
  // �J�[�u�̐��x�X���C�_
  m_precisionSlider   = new QSlider(Qt::Horizontal, this);
  m_precisionLineEdit = new QLineEdit(this);

  // �R���g���[���|�C���g���̕\��
  QFrame* cpFrame      = new QFrame(this);
  m_controlPointsLabel = new QLabel(this);

  // ���̃{�^���Q
  m_closeShapeButton  = new QPushButton(tr("&Close Shape"), this);
  m_deleteShapeButton = new QPushButton(tr("&Delete Shape"), this);

  //---- �v���p�e�B�̐ݒ�
  m_precisionSlider->setRange(0, 100);
  m_precisionLineEdit->setFixedWidth(37);

  QIntValidator* intValidator = new QIntValidator(0, 100, this);
  m_precisionLineEdit->setValidator(intValidator);

  cpFrame->setFrameStyle(QFrame::StyledPanel);
  cpFrame->setObjectName("CpFrame");
  cpFrame->setStyleSheet("#CpFrame {border: 1px solid black;}");

  m_precisionSlider->setValue(0);
  m_precisionLineEdit->setText("0");

  m_closeShapeButton->setCheckable(true);
  m_closeShapeButton->setChecked(m_tool->isClosed());

  //---- ���C�A�E�g
  QVBoxLayout* mainLay = new QVBoxLayout();
  mainLay->setSpacing(5);
  mainLay->setMargin(5);
  {
    // ���x
    mainLay->addWidget(new QLabel(tr("Precision:"), this), 0,
                       Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout* precLay = new QHBoxLayout();
    precLay->setSpacing(3);
    precLay->setMargin(0);
    {
      precLay->addWidget(m_precisionSlider, 1);
      precLay->addWidget(m_precisionLineEdit, 0);
    }
    mainLay->addLayout(precLay, 0);

    // �R���g���[���|�C���g
    QHBoxLayout* cpLay = new QHBoxLayout();
    cpLay->setSpacing(5);
    cpLay->setMargin(5);
    {
      cpLay->addWidget(new QLabel(tr("Control Points :"), this), 1);
      cpLay->addWidget(m_controlPointsLabel, 1);
    }
    cpFrame->setLayout(cpLay);
    mainLay->addWidget(cpFrame, 0);

    // ���̃{�^���Q
    QGridLayout* buttonLay = new QGridLayout();
    buttonLay->setMargin(0);
    buttonLay->setHorizontalSpacing(7);
    buttonLay->setVerticalSpacing(5);
    {
      buttonLay->addWidget(m_closeShapeButton, 0, 0);
      // buttonLay->addWidget(closeButton,			0,1);
      buttonLay->addWidget(m_deleteShapeButton, 0, 1);
      // buttonLay->addWidget(helpButton,			1,1);
    }
    mainLay->addLayout(buttonLay, 0);

    mainLay->addStretch(1);
  }
  setLayout(mainLay);

  //------ �V�O�i��/�X���b�g�ڑ�

  // ���x
  connect(m_precisionSlider, SIGNAL(sliderMoved(int)), this,
          SLOT(onPrecSliderMoved(int)));
  connect(m_precisionLineEdit, SIGNAL(editingFinished()), this,
          SLOT(onPrecLineEditEditingFinished()));

  // �R���g���[���|�C���g���̍X�V
  connect(m_tool, SIGNAL(shapeChanged(int)), this, SLOT(updateCPCount(int)));

  // ���̃{�^���Q�̑���
  //  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(m_closeShapeButton, SIGNAL(clicked(bool)), this,
          SLOT(onCloseShapeButtonClicked(bool)));
  connect(m_deleteShapeButton, SIGNAL(clicked()), m_tool,
          SLOT(onDeleteCurrentShape()));
}

//---------------------------------------------------

void FreeHandDialog::onPrecSliderMoved(int val) {
  // LineEdit�𓯊�
  m_precisionLineEdit->setText(QString::number(val));
  update();

  // �c�[���̒l���X�V
  m_tool->onPrecisionChanged(val);
}

//---------------------------------------------------

void FreeHandDialog::onPrecLineEditEditingFinished() {
  int val = m_precisionLineEdit->text().toInt();
  // �X���C�_�𓯊�
  m_precisionSlider->setValue(val);
  update();

  // �c�[���̒l���X�V
  m_tool->onPrecisionChanged(val);
}

//---------------------------------------------------

// �R���g���[���|�C���g���̍X�V
void FreeHandDialog::updateCPCount(int amount) {
  // amount = 0�̏ꍇ�A������
  if (amount == 0) {
    m_precisionSlider->setEnabled(false);
    m_precisionLineEdit->setEnabled(false);

    m_closeShapeButton->setEnabled(false);
    m_closeShapeButton->setChecked(m_tool->isClosed());

    m_deleteShapeButton->setEnabled(false);

    m_controlPointsLabel->setText("");

    update();

    return;
  }

  m_precisionSlider->setEnabled(true);
  m_precisionLineEdit->setEnabled(true);

  m_closeShapeButton->setEnabled(true);
  m_closeShapeButton->setChecked(m_tool->isClosed());

  m_deleteShapeButton->setEnabled(true);

  m_controlPointsLabel->setText(QString::number(amount));

  update();
}

//---------------------------------------------------

void FreeHandDialog::onCloseShapeButtonClicked(bool close) {
  m_tool->toggleCloseShape(close);
}

//---------------------------------------------------

OpenPopupCommandHandler<FreeHandDialog> openFreeHandDialog(MI_FreehandOptions);