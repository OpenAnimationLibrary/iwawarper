//--------------------------------------------------------
// Join Tool
// �ڑ��c�[���B�V�F�C�v�Q�����[�t�B���O�̃y�A�ɂ���
//--------------------------------------------------------
#ifndef JOINTOOL_H
#define JOINTOOL_H

#include "iwtool.h"

#include <QPointF>
#include <QUndoCommand>

class IwProject;
class IwShape;

class JoinTool : public IwTool {
  // ���݃}�E�X���I�[�o�[���Ă���V�F�C�v
  IwShape* m_overShape;

  IwShape* m_srcShape;
  IwShape* m_dstShape;

  QPointF m_startPos;
  QPointF m_currentPos;

public:
  JoinTool();
  ~JoinTool();

  bool leftButtonDown(const QPointF&, const QMouseEvent*);
  bool leftButtonDrag(const QPointF&, const QMouseEvent*);
  bool leftButtonUp(const QPointF&, const QMouseEvent*);

  bool mouseMove(const QPointF&, const QMouseEvent*);

  void draw();

  int getCursorId(const QMouseEvent* e);

  // �Ή��_���m���q���v���r���[����`�悷��
  void drawJoinPreviewLine();
};

//---------------------------------------------------
// �ȉ��AUndo�R�}���h��o�^
//---------------------------------------------------

class JoinShapeUndo : public QUndoCommand {
  IwProject* m_project;
  IwShape* m_srcShape;
  IwShape* m_dstShape;

  int m_srcPrec;
  int m_dstPrec;

  // AB���[���ɑ����Ă����Ƃ�true
  bool m_srcWasInAB;
  bool m_dstWasInAB;

public:
  JoinShapeUndo(IwShape*, IwShape*, IwProject*);
  ~JoinShapeUndo();

  void undo();
  void redo();
};

class UnjoinShapeUndo : public QUndoCommand {
  IwProject* m_project;
  IwShape* m_srcShape;
  IwShape* m_dstShape;

public:
  UnjoinShapeUndo(IwShape*);
  ~UnjoinShapeUndo();

  void undo();
  void redo();
};
#endif