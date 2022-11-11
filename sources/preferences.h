//-----------------------------------------------------------------------------
// Preferences
// �e��ݒ�̃N���X
//-----------------------------------------------------------------------------
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

class Preferences {  // singleton
public:
  // �x�W�G�̕\����̕�����
  enum BezierActivePrecision {
    SUPERHIGH = 100,
    HIGH      = 50,
    MEDIUM    = 20,
    LOW       = 10
  };

private:
  //----------------
  // GUI preferences
  //----------------
  // �x�W�G�̕\����̕�����
  BezierActivePrecision m_bezierActivePrecision;

  // Reshape Tool �̉E�N���b�N�R�}���h�uLock Positions�v
  // �i�N���b�N�����V�F�C�v��Active�ȃR���g���[���|�C���g���A
  // �@�Ŋ�� ���̃V�F�C�v��Active�ȃR���g���[���|�C���g��
  //   �t�B�b�g����悤�Ɉړ�������R�}���h�j�ŗp����B
  // ���̃R�}���h����������̂ɁAViewer�ł̌����ڏ�̃s�N�Z��������
  // ���̒l�����߂��K�v������
  int m_lockThreshold;

  // ON�̂Ƃ��A�I�����ꂽ�V�F�C�v�������^�C�����C���ɕ\�������
  // OFF�̂Ƃ��A�S�ẴV�F�C�v���\�������
  bool m_showSelectedShapesOnly;

  QString m_language;

  Preferences();

  void loadSettings();
  void saveSettings();

public:
  ~Preferences();

  static Preferences* instance();

  // �x�W�G�̕\����̕�����
  BezierActivePrecision getBezierActivePrecision() {
    return m_bezierActivePrecision;
  }
  void setBezierActivePrecision(BezierActivePrecision prec) {
    m_bezierActivePrecision = prec;
  }
  // Reshape Tool �̉E�N���b�N�R�}���h�uLock Positions�v��臒l
  int getLockThreshold() { return m_lockThreshold; }
  void setLockThreshold(int thres) { m_lockThreshold = thres; }

  bool isShowSelectedShapesOnly() { return m_showSelectedShapesOnly; }
  void setShowSelectedShapesOnly(bool on) { m_showSelectedShapesOnly = on; }

  QString language() const { return m_language; }
  void setLanguage(QString lang) { m_language = lang; }
};

#endif