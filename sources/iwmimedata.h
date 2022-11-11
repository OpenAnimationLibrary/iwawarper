//-----------------------------------------------------------------------------
// IwMimedata
// �N���b�v�{�[�h�ɓ����f�[�^�B
// Undo�ɑΉ������邽�߂Ƀf�[�^��Clone�@�\��ǉ����Ă���
//-----------------------------------------------------------------------------
#ifndef IWMIMEDATA_H
#define IWMIMEDATA_H

#include <QMimeData>

class IwMimeData : public QMimeData {
public:
  IwMimeData();
  virtual ~IwMimeData();

  virtual IwMimeData* clone() const = 0;
  virtual void releaseData() {}
};

QMimeData* cloneData(const QMimeData* data);

#endif
