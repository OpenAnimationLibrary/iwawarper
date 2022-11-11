//---------------------------------
// MySider
// �X���C�_��LineEdit�̑g�ݍ��킹
//---------------------------------

#include "myslider.h"

#include <QSlider>
#include <QLineEdit>
#include <QIntValidator>
#include <QHBoxLayout>

MyIntSlider::MyIntSlider(int min, int max, QWidget* parent)
    : QWidget(parent), m_isDragging(false) {
  // �I�u�W�F�N�g�̐錾
  m_slider = new QSlider(Qt::Horizontal, this);
  m_edit   = new QLineEdit(this);

  // �v���p�e�B�̐ݒ�
  m_slider->setRange(min, max);
  QIntValidator* validator = new QIntValidator(min, max, this);
  m_edit->setValidator(validator);

  // ���C�A�E�g
  QHBoxLayout* mainLay = new QHBoxLayout();
  mainLay->setMargin(0);
  mainLay->setSpacing(5);
  {
    mainLay->addWidget(m_slider, 2);
    mainLay->addWidget(m_edit, 1);
  }
  setLayout(mainLay);

  // �V�O�i���^�X���b�g�ڑ�
  connect(m_slider, SIGNAL(sliderPressed()), this, SLOT(onSliderPressed()));
  connect(m_slider, SIGNAL(sliderReleased()), this, SLOT(onSliderReleased()));
  connect(m_slider, SIGNAL(valueChanged(int)), this,
          SLOT(onSliderValueChanged(int)));
  connect(m_edit, SIGNAL(editingFinished()), this,
          SLOT(onEditEditingFinished()));
}

//---------------------------------

int MyIntSlider::value() { return m_slider->value(); }

//---------------------------------

void MyIntSlider::setValue(int val) {
  m_edit->setText(QString::number(val));
  m_slider->setValue(val);
}

//---------------------------------

void MyIntSlider::onSliderValueChanged(int val) {
  m_edit->setText(QString::number(val));
  emit valueChanged(m_isDragging);
}

//---------------------------------

void MyIntSlider::onSliderPressed() { m_isDragging = true; }

//---------------------------------

void MyIntSlider::onSliderReleased() {
  m_isDragging = false;
  emit valueChanged(false);
}

//---------------------------------

void MyIntSlider::onEditEditingFinished() {
  int val = m_edit->text().toInt();
  m_slider->setValue(val);
  emit valueChanged(false);
}