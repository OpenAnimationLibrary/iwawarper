//---------------------------------------------------
// ShapePairData
// ShapePair�̃R�s�[�^�y�[�X�g�ň����f�[�^
//---------------------------------------------------

#ifndef SHAPEPAIRDATA_H
#define SHAPEPAIRDATA_H

#include "iwmimedata.h"

#include <QList>
#include <QMap>

class ShapePair;
class IwLayer;

class ShapePairData : public IwMimeData {
public:
  QList<ShapePair*> m_data;

  ShapePairData() {}
  ShapePairData(const ShapePairData* src) { m_data = src->m_data; }
  ~ShapePairData();

  ShapePairData* clone() const { return new ShapePairData(this); }

  // �I��͈͂���f�[�^���i�[����
  void setShapePairData(QList<ShapePair*>& shapes);

  // �f�[�^���y�[�X�g����
  void getShapePairData(IwLayer* layer, QList<ShapePair*>& shapes) const;
};

#endif