//----------------------------------------------------
// KeyFrameData
// KeyFrameEditor���̃R�s�[/�y�[�X�g�ň����f�[�^
//----------------------------------------------------
#ifndef KEYFRAMEDATA_H
#define KEYFRAMEDATA_H

#include "iwmimedata.h"
#include "keyframeeditor.h"
#include "keycontainer.h"

#include <QPoint>
#include <QList>

struct OneShape;

class KeyFrameData : public IwMimeData {
  struct CellData {
    QPoint pos;                       // �I��͈̓o�E���f�B���O�̍��ォ��̍��W
    KeyFrameEditor::KEYMODE keymode;  // �R�s�[���̃L�[���[�h
    bool shapeWasClosed;              // �R�s�[���A�V�F�C�v�����Ă������ǂ���
    int bpCount;                      // �R�s�[���̃R���g���[���|�C���g��
    int cpCount;                      // �R�s�[���̑Ή��_��
    BezierPointList bpList;           // �`��L�[�t���[�����
    CorrPointList cpList;             // �Ή��_�L�[�t���[�����
    double b_interp;                  // �`��L�[�t���[���̕�ԏ��
    double c_interp;                  // �Ή��_�L�[�t���[���̕�ԏ��
  };

public:
  QList<CellData> m_data;

  KeyFrameData();
  KeyFrameData(const KeyFrameData* src);

  KeyFrameData* clone() const { return new KeyFrameData(this); }

  // �I��͈͂���f�[�^���i�[����
  void setKeyFrameData(OneShape& shape, QList<int>& selectedFrames,
                       KeyFrameEditor::KEYMODE keymode);

  // �w��Z���Ƀf�[�^���㏑���y�[�X�g����
  void getKeyFrameData(OneShape& shape, QList<int>& selectedCells,
                       KeyFrameEditor::KEYMODE keymode) const;
};

#endif