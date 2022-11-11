//---------------------------------
// Shape Options Dialog
// �I�𒆂̃V�F�C�v�̃v���p�e�B��\������_�C�A���O
// ���[�_���ł͂Ȃ�
//---------------------------------
#ifndef SHAPEOPTIONSDIALOG_H
#define SHAPEOPTIONSDIALOG_H

#include "iwdialog.h"

#include <QUndoCommand>
#include <QList>

#include "shapepair.h"

// class IwShape;
class QSlider;
class QLineEdit;
class IwSelection;
class IwProject;

class ShapeOptionsDialog : public IwDialog {
  Q_OBJECT

  // ���ݑI������Ă���V�F�C�v
  QList<OneShape> m_selectedShapes;

  // �Ή��_�̖��x�X���C�_
  QSlider* m_edgeDensitySlider;
  QLineEdit* m_edgeDensityEdit;

public:
  ShapeOptionsDialog();

  void setDensity(int value);

protected:
  void showEvent(QShowEvent*);
  void hideEvent(QHideEvent*);

protected slots:

  void onSelectionSwitched(IwSelection*, IwSelection*);
  void onSelectionChanged(IwSelection*);

  void onEdgeDensitySliderMoved(int val);
  void onEdgeDensityEditEdited();
};

//-------------------------------------
// �ȉ��AUndo�R�}���h
//-------------------------------------

class ChangeEdgeDensityUndo : public QUndoCommand {
public:
  struct EdgeDensityInfo {
    OneShape shape;
    int beforeED;
  };

private:
  IwProject* m_project;
  QList<EdgeDensityInfo> m_info;
  int m_afterED;

public:
  ChangeEdgeDensityUndo(QList<EdgeDensityInfo>& info, int afterED,
                        IwProject* project);

  void undo();
  void redo();
};

#endif