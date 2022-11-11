#pragma once
#ifndef KEYDRAGTOOL_H
#define KEYDRAGTOOL_H

#include "shapepair.h"

#include <QSet>
#include <QUndoCommand>

class KeyDragTool {  // singleton

  OneShape m_shape;
  QList<int> m_selectedFrames;
  bool m_isFormKeySelected;

  int m_dragStart, m_dragCurrent;
  KeyDragTool();

public:
  static KeyDragTool* instance();

  bool isCurrentRow(OneShape, bool);
  bool isDragDestination(int);

  bool isDragging();
  // ���݂̃L�[�I����o�^�AisDragging�I��
  void onClick(int frame);
  // �h���b�O����V�t�g����
  void onMove(int frame);
  // �h���b�O�悪�����Ă���΁AUndo�쐬���ď���
  void onRelease();
};

//------------------------------------------------------
// �L�[�Ԃ̕�ԃn���h���̃h���b�O
class InterpHandleDragTool {  // singleton
  OneShape m_shape;
  bool m_isForm;
  int m_key, m_nextKey;
  double m_frameOffset;
  double m_interpStart;
  InterpHandleDragTool();

public:
  static InterpHandleDragTool* instance();
  bool isDragging();

  void onClick(OneShape shape, bool isForm, double frameOffset, int key,
               int nextKey);
  // �h���b�O����V�t�g����
  void onMove(double mouseFramePos, bool doSnap);
  // �h���b�O�悪�����Ă���΁AUndo�쐬���ď���
  void onRelease();

  // �����擾
  void getInfo(OneShape& shape, bool& isForm, int& key, int& nextKey);
};

//------------------------------------------------------

class InterpHandleEditUndo : public QUndoCommand {
  IwProject* m_project;

  OneShape m_shape;
  bool m_isForm;
  int m_key;
  double m_before, m_after;

  // redo������Ȃ��悤�ɂ���t���O
  int m_firstFlag;

public:
  InterpHandleEditUndo(OneShape& shape, bool isForm, int key, double before,
                       double after, IwProject* project);

  ~InterpHandleEditUndo() {}

  void undo();
  void redo();
};
#endif