//---------------------------------
// Render Sewttings Dialog
// ���[�t�B���O�A�����_�����O�̐ݒ���s��
//---------------------------------

#include "rendersettingsdialog.h"

#include "iwapp.h"
#include "mainwindow.h"
#include "menubarcommandids.h"
#include "iwprojecthandle.h"
#include "iwproject.h"

#include "rendersettings.h"

#include <QComboBox>
#include <QLabel>
#include <QStringList>

#include <QHBoxLayout>
#include <QVBoxLayout>

//---------------------------------
RenderSettingsDialog::RenderSettingsDialog()
    : IwDialog(IwApp::instance()->getMainWindow(), "RenderSettingsDialog",
               false) {
  setSizeGripEnabled(false);
  //--- �I�u�W�F�N�g�̐錾
  m_warpPrecisionCombo = new QComboBox(this);
  //--- �v���p�e�B�̐ݒ�
  QStringList precList;
  precList << "Linear"
           << "Low"
           << "Medium"
           << "High"
           << "Very High"
           << "Super High";
  m_warpPrecisionCombo->addItems(precList);

  //--- ���C�A�E�g
  QHBoxLayout* mainLay = new QHBoxLayout();
  mainLay->setSpacing(5);
  mainLay->setMargin(10);
  {
    mainLay->addWidget(new QLabel(tr("Precision:"), this), 0);
    mainLay->addWidget(m_warpPrecisionCombo, 1);
  }
  setLayout(mainLay);

  //------ �V�O�i��/�X���b�g�ڑ�
  connect(m_warpPrecisionCombo, SIGNAL(activated(int)), this,
          SLOT(onPrecisionComboActivated(int)));
}

//---------------------------------
// project���؂�ւ������A���e���X�V����
//---------------------------------
void RenderSettingsDialog::showEvent(QShowEvent* event) {
  IwDialog::showEvent(event);

  IwProjectHandle* ph = IwApp::instance()->getCurrentProject();
  if (ph) connect(ph, SIGNAL(projectSwitched()), this, SLOT(updateGuis()));

  updateGuis();
}

//---------------------------------
void RenderSettingsDialog::hideEvent(QHideEvent* event) {
  IwDialog::hideEvent(event);

  IwProjectHandle* ph = IwApp::instance()->getCurrentProject();
  if (ph) disconnect(ph, SIGNAL(projectSwitched()), this, SLOT(updateGuis()));
}

//---------------------------------
// ���݂�OutputSettings�ɍ��킹�ĕ\�����X�V����
//---------------------------------
void RenderSettingsDialog::updateGuis() {
  // ���݂̃v���W�F�N�g��OutputSettings���擾
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;

  RenderSettings* settings = project->getRenderSettings();
  if (!settings) return;

  m_warpPrecisionCombo->setCurrentIndex(settings->getWarpPrecision());

  update();
}

//---------------------------------
void RenderSettingsDialog::onPrecisionComboActivated(int index) {
  // ���݂̃v���W�F�N�g��OutputSettings���擾
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;
  RenderSettings* settings = project->getRenderSettings();
  if (!settings) return;

  if (index == settings->getWarpPrecision()) return;

  settings->setWarpPrecision(index);
}

OpenPopupCommandHandler<RenderSettingsDialog> openRenderOptions(
    MI_RenderOptions);