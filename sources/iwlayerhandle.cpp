//---------------------------------------------------
// IwLayerHandle
//---------------------------------------------------

#include "iwlayerhandle.h"

IwLayerHandle::IwLayerHandle() : m_layer(0) {}

IwLayer* IwLayerHandle::getLayer() const { return m_layer; }

//---------------------------------------------------
// ���C�����{���ɐ؂�ւ�����ꍇtrue��Ԃ�
//---------------------------------------------------
bool IwLayerHandle::setLayer(IwLayer* layer) {
  if (m_layer == layer) return false;
  m_layer = layer;
  if (m_layer) emit layerSwitched();
  return true;
}
