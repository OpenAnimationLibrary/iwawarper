//---------------------------------------------------
// IwLayerHandle
// �V�[�N�G���X�ɑ���A���C���ŊǗ�����
//---------------------------------------------------

#ifndef IWLAYERHANDLE_H
#define IWLAYERHANDLE_H

#include <QObject>

#include "iwlayer.h"

class IwLayerHandle : public QObject {
  Q_OBJECT

  IwLayer* m_layer;

public:
  IwLayerHandle();

  IwLayer* getLayer() const;

  // ���C�����{���ɐ؂�ւ�����ꍇtrue��Ԃ�
  bool setLayer(IwLayer* layer);

  // ���C���̕ύX���G�~�b�g����
  void notifyLayerChanged(IwLayer* lay) { emit layerChanged(lay); }

signals:
  // �J�����g���C�����؂�ւ����
  void layerSwitched();
  // ���C�����ύX���ꂽ
  void layerChanged(IwLayer*);
};

#endif