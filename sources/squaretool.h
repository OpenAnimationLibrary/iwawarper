//--------------------------------------------------------
// Square Tool
// ��`�`��c�[��
// �쐬��͂��̃V�F�C�v���J�����g�ɂȂ�
// �J�����g�V�F�C�v�̕ό`���ł���iTransformTool�Ɠ����@�\�j
//--------------------------------------------------------
#ifndef SQUARETOOL_H
#define SQUARETOOL_H

#include "iwtool.h"
#include <QPointF>
#include <QRectF>
#include <QUndoCommand>

class IwProject;
class IwShape;

class ShapePair;
class IwLayer;

class SquareTool : public IwTool {
  bool m_startDefined;
  QPointF m_startPos;
  QPointF m_endPos;

public:
  SquareTool();

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

class CreateSquareShapeUndo : public QUndoCommand {
  IwProject* m_project;
  int m_index;
  IwShape* m_shape;

  ShapePair* m_shapePair;
  IwLayer* m_layer;

public:
  CreateSquareShapeUndo(QRectF& rect, IwProject* prj, IwLayer* layer);
  ~CreateSquareShapeUndo();

  void undo();
  void redo();
};

#endif