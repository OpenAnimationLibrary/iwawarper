//----------------------------------------------------
// TimeLineData
// �^�C�����C�����̃R�s�[�^�y�[�X�g�Ŏg���f�[�^
//----------------------------------------------------
#ifndef TIMELINEDATA_H
#define TIMELINEDATA_H

#include "iwmimedata.h"

#include <QList>
#include <QString>

class IwLayer;

class TimeLineData : public IwMimeData {
public:
  QList<QPair<QString, QString>> m_frameData;

  TimeLineData();
  TimeLineData(const TimeLineData *src);

  TimeLineData *clone() const { return new TimeLineData(this); }

  // �f�[�^�̃t���[������Ԃ�
  int getFrameLength() { return m_frameData.size(); }

  // ���C������f�[�^���i�[���� (�R�s�[)
  void setFrameData(IwLayer *layer, int start, int end,
                    bool rightFrameIncluded);
  // �f�[�^���烌�C���̎w�肳�ꂽindex�ɓ���� (�}���y�[�X�g)
  void getFrameData(IwLayer *layer, int frameIndex) const;
};

#endif