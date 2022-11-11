//---------------------------------------------------
// ShapePairData
// ShapePair�̃R�s�[�^�y�[�X�g�ň����f�[�^
//---------------------------------------------------

#include "shapepairdata.h"

#include "iwapp.h"
#include "iwprojecthandle.h"
#include "iwlayerhandle.h"
#include "iwproject.h"
#include "iwlayer.h"
#include "shapepair.h"
#include "viewsettings.h"

//----------------------------------------------------

ShapePairData::~ShapePairData() {
  // ���ׂ�delete
  for (int s = 0; s < m_data.size(); s++) {
    delete m_data.at(s);
  }
}

//----------------------------------------------------
// �I��͈͂���f�[�^���i�[����
//----------------------------------------------------
void ShapePairData::setShapePairData(QList<ShapePair*>& shapes) {
  // �I���V�F�C�v�����ꂼ��clone����B
  for (int s = 0; s < shapes.size(); s++) {
    ShapePair* shape       = shapes.at(s);
    ShapePair* clonedShape = shape->clone();
    m_data.push_back(clonedShape);
  }
}

//----------------------------------------------------
// �f�[�^���y�[�X�g����
//----------------------------------------------------
void ShapePairData::getShapePairData(IwLayer* layer,
                                     QList<ShapePair*>& shapes) const {
  // �v���W�F�N�g�������return
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;

  for (int s = 0; s < m_data.size(); s++) {
    ShapePair* clonedShape = m_data.at(s)->clone();
    layer->addShapePair(clonedShape);
    shapes.append(clonedShape);
  }
}