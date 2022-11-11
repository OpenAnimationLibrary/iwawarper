//------------------------------------------
// ReshapeToolContextMenu
// Reshape Tool �̉E�N���b�N���j���[�p�N���X
//------------------------------------------

#ifndef RESHAPETOOLCONTEXTMENU_H
#define RESHAPETOOLCONTEXTMENU_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QPair>

#include <QUndoCommand>

#include "shapepair.h"

class IwShapePairSelection;
class IwProject;
class QMouseEvent;
class QAction;
class QMenu;

enum RESHAPE_MENU_MODE {
  // Selection�Ɋ֌W�Ȃ��A�J�[�\�����̃|�C���g�����𑀍�
  CurrentPointMode,
  // �J�[�\�����̃V�F�C�v�̒��́A�I�����ꂽ�|�C���g�����𑀍�
  CurrentShapeMode,
  // Selection�őI�����ꂽ�V�F�C�v�^�|�C���g�𑀍�
  SelectedShapeMode
};

class ReshapeToolContextMenu : public QObject  // singleton
{
  Q_OBJECT

  RESHAPE_MENU_MODE m_mode;
  IwShapePairSelection* m_selection;

  // ���݂�Viewer�́A1�s�N�Z��������̋���
  QPointF m_onePixelLength;

  // �E�N���b�N�����Ƃ��̃J�[�\�����̃A�C�e�����
  OneShape m_shape;
  int m_pointIndex, m_handleId;

  // ���̃��j���[�̑���ŉe����^����|�C���g
  QList<QPair<OneShape, int>> m_effectedPoints;

  // ActivePoint���܂ރV�F�C�v�̃��X�g
  QList<OneShape> m_activePointShapes;

  // �A�N�V�����ꗗ
  QAction *m_lockPointsAct, *m_cuspAct, *m_linearAct, *m_smoothAct,
      *m_centerAct, *m_tweenPointAct, *m_reverseShapesAct;

  // ���̃��j���[�̑���ŉe����^����|�C���g���X�V
  void updateEffectedPoints();
  // �A�N�V�����̗L���^������؂�ւ���
  void enableActions();
  // �L���^�����̔��f
  //  bool canBreakShape();
  bool canLockPoints();
  bool canModifyHandles();  // Cusp, Linear, Smooth�p
  bool canCenter();

  // �n���h������֌W���ЂƂɂ܂Ƃ߂�
  enum ModifyHandleActId { Cusp, Linear, Smooth, Center };
  void doModifyHandle(ModifyHandleActId actId);

  // doCenter�̎���
  void doCenter_Imp(OneShape shape, int index, BezierPointList& bpList);
  ReshapeToolContextMenu();

public:
  static ReshapeToolContextMenu* instance();

  void init(IwShapePairSelection* selection, QPointF onePixelLength);
  void openMenu(const QMouseEvent* e, OneShape shape, int pointIndex,
                int handleId, QMenu& menu);

public slots:
  // �R�}���h�̎���
  void doLockPoints();
  void doCusp();
  void doLinear();
  void doSmooth();
  void doCenter();
  void doTween();
  void doReverse();
};

//---------------------------------------------------
// �ȉ��AUndo�R�}���h
//---------------------------------------------------
// ReshapeContextMenuUndo
//---------------------------------------------------
class ReshapeContextMenuUndo : public QUndoCommand {
  IwProject* m_project;

  QList<OneShape> m_shapes;

  // �V�F�C�v���Ƃ́A�ҏW���ꂽ�t���[�����Ƃ̌��̌`��̃��X�g
  QList<QMap<int, BezierPointList>> m_beforePoints;
  // �V�F�C�v���Ƃ́A�ҏW���ꂽ�t���[�����Ƃ̑����̌`��̃��X�g
  QList<QMap<int, BezierPointList>> m_afterPoints;
  // ���̃V�F�C�v���L�[�t���[�����������ǂ���
  QList<bool> m_wasKeyFrame;
  // ������s�����t���[��
  int m_frame;
  // redo������Ȃ��悤�ɂ���t���O
  int m_firstFlag;

public:
  ReshapeContextMenuUndo(QList<OneShape>& shapes,
                         QList<QMap<int, BezierPointList>>& beforePoints,
                         QList<QMap<int, BezierPointList>>& afterPoints,
                         QList<bool>& wasKeyFrame, int frame,
                         IwProject* project);
  void undo();
  void redo();
};

#endif