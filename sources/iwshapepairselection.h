//---------------------------------------------------
// IwShapePairSelection
// �V�F�C�v�y�A�̑I���̃N���X
//---------------------------------------------------

#ifndef IWSHAPEPAIRSELECTION_H
#define IWSHAPEPAIRSELECTION_H

#include "iwselection.h"
#include "shapepair.h"

#include <QList>
#include <QPair>
#include <QSet>
#include <QUndoCommand>

class IwLayer;

struct ShapeInfo {
  int index;
  ShapePair* shapePair;
  IwLayer* layer;
};

class IwShapePairSelection : public IwSelection  // singleton
{
  Q_OBJECT

  // int == 0�FFrom�`��Aint == 1�FTo�`��
  QList<OneShape> m_shapes;

  // CorrespondenceTool�Ŏg�p
  QPair<OneShape, int> m_activeCorrPoint;

  // Reshape�c�[���Ŏg�p
  // �I���|�C���gID�̃��X�g
  // [ShapeId+1][FromTo][PointId] 0
  //				��		��
  //			�@0����1	�R���@�@
  QSet<int> m_activePoints;

  IwShapePairSelection();

public:
  static IwShapePairSelection* instance();
  ~IwShapePairSelection();

  void enableCommands();
  bool isEmpty() const;
  // �I���̉���
  void selectNone();

  // ����܂ł̑I�����������āA�V�F�C�v���P�I��
  void selectOneShape(OneShape shape);
  // �I���V�F�C�v��ǉ�����
  void addShape(OneShape shape);
  // �I���V�F�C�v����������
  int removeShape(OneShape shape);
  // FromTo�w��őI��������
  bool removeFromToShapes(int fromTo);

  // �I���V�F�C�v�̌�
  int getCount() { return m_shapes.size(); }
  // �V�F�C�v���擾����
  OneShape getShape(int index);
  // �V�F�C�v�̃��X�g���擾����
  QList<OneShape> getShapes() { return m_shapes; }

  // �����V�F�C�v���I��͈͂Ɋ܂܂�Ă�����true
  bool isSelected(OneShape shape) const;
  bool isSelected(ShapePair* shapePair) const;

  // �I���V�F�C�v�̌���Ԃ�
  int getShapesCount() { return m_shapes.size(); }

  // �A�N�e�B�u�Ή��_�̓��o��
  QPair<OneShape, int> getActiveCorrPoint() { return m_activeCorrPoint; }
  void setActiveCorrPoint(OneShape shape, int index) {
    m_activeCorrPoint = QPair<OneShape, int>(shape, index);
  }
  bool isActiveCorrPoint(OneShape shape, int index) {
    return (m_activeCorrPoint.first == shape) &&
           (m_activeCorrPoint.second == index);
  }

  //---- Reshape�c�[���Ŏg�p
  // �|�C���g���I������Ă��邩�ǂ���
  bool isPointActive(OneShape, int index);
  // �|�C���g��I������
  void activatePoint(int name);
  void activatePoint(OneShape, int index);
  // �|�C���g�I������������
  void deactivatePoint(int name);
  void deactivatePoint(OneShape, int index);
  // �I���|�C���g�̃��X�g��Ԃ�
  QSet<int> getActivePointSet() { return m_activePoints; }
  // ���̃��X�g��
  QList<QPair<OneShape, int>> getActivePointList();
  // �I���|�C���g��S�ĉ�������
  void deactivateAllPoints();

  //-- �ȉ��A���̑I���Ŏg����R�}���h�Q
  // �R�s�[
  void copyShapes();
  // �y�[�X�g
  void pasteShapes();
  // �J�b�g
  void cutShapes();
  // ����
  void deleteShapes();
  void onDeleteCorrPoint();

  // �\������Shape��S�I��
  void selectAllShapes();

  // �I���V�F�C�v�̃G�N�X�|�[�g
  void exportShapes();

  // �I���V�F�C�v�̃��b�N�؂�ւ�
  void toggleLocks();

  // �I���V�F�C�v�̃s���؂�ւ�
  void togglePins();

  // �I���V�F�C�v�̕\����\���؂�ւ�
  void toggleVisibility();

  // �V�F�C�v�^�O�؂�ւ�
  void setShapeTag(int tagId, bool on);
};

//---------------------------------------------------
// �ȉ��AUndo�R�}���h��o�^
//---------------------------------------------------

//---------------------------------------------------
// �y�[�X�g��Undo
//---------------------------------------------------

class PasteShapePairUndo : public QUndoCommand {
  QList<ShapePair*> m_shapes;
  IwProject* m_project;
  IwLayer* m_layer;

  // redo������Ȃ��悤�ɂ���t���O
  int m_firstFlag;

public:
  PasteShapePairUndo(QList<ShapePair*>& shapes, IwProject* project,
                     IwLayer* layer);
  ~PasteShapePairUndo();

  void undo();
  void redo();
};

//---------------------------------------------------
// �J�b�g/������Undo
//---------------------------------------------------

class DeleteShapePairUndo : public QUndoCommand {
  QList<ShapeInfo> m_shapes;
  // QMap<int,IwShape*> m_shapes;
  IwProject* m_project;

  // redo������Ȃ��悤�ɂ���t���O
  int m_firstFlag;

public:
  DeleteShapePairUndo(QList<ShapeInfo>& shapes, IwProject* project);
  ~DeleteShapePairUndo();

  // �v�AJoin�̂Ȃ��Ȃ���
  void undo();
  void redo();
};

//---------------------------------------------------
// ���b�N�؂�ւ���Undo
//---------------------------------------------------

class LockShapesUndo : public QUndoCommand {
  QList<QPair<OneShape, bool>> m_shape_status;
  IwProject* m_project;

  bool m_doLock;

public:
  LockShapesUndo(QList<OneShape>& shapes, IwProject* project, bool doLock);
  void lockShapes(bool isUndo);
  void undo();
  void redo();
};

//---------------------------------------------------
// �s���؂�ւ���Undo
//---------------------------------------------------

class PinShapesUndo : public QUndoCommand {
  QList<QPair<OneShape, bool>> m_shape_status;
  IwProject* m_project;

  bool m_doPin;

public:
  PinShapesUndo(QList<OneShape>& shapes, IwProject* project, bool doPin);
  void pinShapes(bool isUndo);
  void undo();
  void redo();
};

//---------------------------------------------------
// �\���A��\���؂�ւ���Undo
//---------------------------------------------------

class SetVisibleShapesUndo : public QUndoCommand {
  QList<QPair<ShapePair*, bool>> m_shapePair_status;
  IwProject* m_project;

  bool m_setVisible;

public:
  SetVisibleShapesUndo(QList<ShapePair*>& shapes, IwProject* project,
                       bool setVisible);
  void setVisibleShapes(bool isUndo);
  void undo();
  void redo();
};

//---------------------------------------------------
// �V�F�C�v�^�O�؂�ւ���Undo
//---------------------------------------------------

class SetShapeTagUndo : public QUndoCommand {
  QList<QPair<OneShape, bool>> m_shape_status;
  IwProject* m_project;

  int m_tagId;
  bool m_on;

public:
  SetShapeTagUndo(QList<OneShape>& shapes, IwProject* project, int tagId,
                  bool on);
  void setTag(bool isUndo);
  void undo();
  void redo();
};
#endif