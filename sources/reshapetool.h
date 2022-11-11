//--------------------------------------------------------
// Reshape Tool
// �R���g���[���|�C���g�ҏW�c�[��
//--------------------------------------------------------
#ifndef RESHAPETOOL_H
#define RESHAPETOOL_H

#include "iwtool.h"
#include "shapepair.h"

#include <QUndoCommand>

class IwShapePairSelection;
class PointDragTool;
class ReshapeToolContextMenu;
class IwLayer;

class ReshapeTool : public IwTool {
  IwShapePairSelection* m_selection;
  PointDragTool* m_dragTool;

  // �|�C���g��Rect�I��
  bool m_isRubberBanding;
  QPointF m_rubberStartPos;
  QPointF m_currentMousePos;

  bool isRubberBandValid();
  // �N���b�N�ʒu����A����ΏۂƂȂ�V�F�C�v�^�n���h���𓾂�
  OneShape getClickedShapeAndIndex(int& pointIndex, int& handleId,
                                   const QMouseEvent* e);

public:
  ReshapeTool();
  ~ReshapeTool();

  bool leftButtonDown(const QPointF&, const QMouseEvent*);
  bool leftButtonDrag(const QPointF&, const QMouseEvent*);
  bool leftButtonUp(const QPointF&, const QMouseEvent*);
  void leftButtonDoubleClick(const QPointF&, const QMouseEvent*);

  // �E�N���b�N���j���[
  bool rightButtonDown(const QPointF&, const QMouseEvent*, bool& canOpenMenu,
                       QMenu& menu);
  // bool rightButtonDown(const QPointF &, const QMouseEvent*);

  bool keyDown(int key, bool ctrl, bool shift, bool alt);

  // �A�N�e�B�u�ȃV�F�C�v�ɃR���g���[���|�C���g��`��
  // �I���|�C���g�͐F��ς��A�n���h�����`��
  void draw();
  int getCursorId(const QMouseEvent*);

  // �R���g���[���|�C���g��`�悷��B�n���h�����t���邩�ǂ����������Ō��߂�
  //  PenTool�ł��g�������̂ŁAstatic������
  static void drawControlPoint(
      OneShape shape, BezierPointList& pointList, int cpIndex, bool drawHandles,
      QPointF& onePix,
      int specialShapeIndex =
          0,  // PenTool�Ŏg���BProject�ɑ����Ă��Ȃ��V�F�C�v�Ɏg��
      bool fillPoint = false);
  // static void drawControlPoint(IwShape* shape,
  //							BezierPointList&
  // pointList, 							int cpIndex,
  // bool
  // drawHandles, 							QPointF &
  // onePix, int specialShapeIndex =
  // 0);//PenTool�Ŏg���BProject�ɑ����Ă��Ȃ��V�F�C�v�Ɏg��

  // �n���h���p�̉~��`��
  //  PenTool�ł��g�������̂ŁAstatic������
  static void drawCircle();

  // �A�N�e�B�u�ȃV�F�C�v��Alt�N���b�N�����ꍇ�A�R���g���[���|�C���g��ǉ�����
  void addControlPoint(OneShape shape, const QPointF& pos);
  // void addControlPoint(IwShape* shape, const QPointF & pos);

  bool setSpecialShapeColor(OneShape) override;
  bool setSpecialGridColor(int gId, bool isVertical) override;

protected:
  // Ctrl+���L�[��0.25�s�N�Z�������ړ�������
  void arrowKeyMove(QPointF& delta);
};

//---------------------------------------------------
// �ȉ��AUndo�R�}���h��o�^
//---------------------------------------------------

class AddControlPointUndo : public QUndoCommand {
  IwProject* m_project;
  IwLayer* m_layer;
  OneShape m_shape;
  // IwShape*	m_shape;

  // ���̌`��f�[�^
  QMap<int, BezierPointList> m_beforeFormData[2];
  // ���̑Ή��_�f�[�^
  QMap<int, CorrPointList> m_beforeCorrData[2];
  // �����̌`��f�[�^
  QMap<int, BezierPointList> m_afterFormData[2];
  // �����̑Ή��_�f�[�^
  QMap<int, CorrPointList> m_afterCorrData[2];

  // redo������Ȃ��悤�ɂ���t���O
  int m_firstFlag;

public:
  AddControlPointUndo(QMap<int, BezierPointList>& beforeFormDataFrom,
                      QMap<int, CorrPointList>& beforeCorrDataFrom,
                      QMap<int, BezierPointList>& beforeFormDataTo,
                      QMap<int, CorrPointList>& beforeCorrDataTo,
                      OneShape shape, IwProject* project, IwLayer* layer);
  // AddControlPointUndo(	QMap<int, BezierPointList> & beforeFormData,
  //						QMap<int, CorrPointList> &
  // beforeCorrData, 						IwShape* shape,
  // IwProject* project);

  void setAfterData(QMap<int, BezierPointList>& afterFormDataFrom,
                    QMap<int, CorrPointList>& afterCorrDataFrom,
                    QMap<int, BezierPointList>& afterFormDataTo,
                    QMap<int, CorrPointList>& afterCorrDataTo);
  void undo();
  void redo();
};

#endif