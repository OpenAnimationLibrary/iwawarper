//---------------------------------------------------
// ColorSettings
// �C���^�t�F�[�X�̐F�����i�[����
//---------------------------------------------------
#ifndef COLORSETTINGS_H
#define COLORSETTINGS_H

#include <iostream>

//----------------------------------------------------
// �F�̃��X�g
//----------------------------------------------------
#define Color_CorrPoint "CorrPoint"
#define Color_ActiveCorr "ActiveCorr"
#define Color_CtrlPoint "CtrlPoint"
#define Color_ActiveCtrl "ActiveCtrl"
#define Color_CorrNumber "CorrNumber"
#define Color_Background "Background"
#define Color_FreeHandToolTracedLine "FreeHandToolTracedLine"
#define Color_InbetweenCtrl "InbetweenCtrl"
#define Color_InbetweenCorr "InbetweenCorr"
//----------------------------------------------------

#include <QMap>
#include <QColor>

typedef const char* ColorId;

class ColorSettings {  // singleton
  QMap<QString, QColor> m_colors;

  ColorSettings();

public:
  ~ColorSettings();
  static ColorSettings* instance();

  // �w�肳�ꂽColorId�ɑ΂��ĐF���擾�i0�`1.0�j�A���glColor3dv�Ŏg���邩�H
  void getColor(double* colorVec, ColorId colorId);
  // �F��QColor�ŕԂ�
  QColor getQColor(ColorId colorId);

  // �ۑ�/���[�h
  void saveData();
  void loadData();
};

#endif