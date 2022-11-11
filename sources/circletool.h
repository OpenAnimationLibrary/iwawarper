//--------------------------------------------------------
// Circle Tool
// �~�`�`��c�[��
// �쐬��͂��̃V�F�C�v���J�����g�ɂȂ�
// �J�����g�V�F�C�v�̕ό`���ł���iTransformTool�Ɠ����@�\�j
//--------------------------------------------------------
#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include "iwtool.h"
#include <QPointF>
#include <QRectF>
#include <QUndoCommand>

class IwProject;
class IwShape;

class ShapePair;
class IwLayer;

class CircleTool : public IwTool {
  bool m_startDefined;
  QPointF m_startPos;
  QPointF m_endPos;

public:
  CircleTool();

  bool leftButtonDown(const QPointF&, const QMouseEvent*);
  bool leftButtonDrag(const QPointF&, const QMouseEvent*);
  bool leftButtonUp(const QPointF&, const QMouseEvent*);
  void leftButtonDoubleClick(const QPointF&, const QMouseEvent*);
  void draw();

  int getCursorId(const QMouseEvent*) override;
};

//---------------------------------------------------
// �ȉ��AUndo�R�}���h��o�^
//---------------------------------------------------

class CreateCircleShapeUndo : public QUndoCommand {
  IwProject* m_project;
  int m_index;
  IwShape* m_shape;

  ShapePair* m_shapePair;
  IwLayer* m_layer;

public:
  CreateCircleShapeUndo(QRectF& rect, IwProject* prj, IwLayer* layer);
  ~CreateCircleShapeUndo();

  void undo();
  void redo();
};

#endif