//------------------------------------------------
// IwTimeLineKeySelection
// TimeLineWindow ���̃L�[�t���[���̑I��
//------------------------------------------------
#ifndef IWTIMELINEKEYSELECTION_H
#define IWTIMELINEKEYSELECTION_H

#include "iwselection.h"

#include "shapepair.h"

#include <QList>
#include <QUndoCommand>

class IwTimeLineKeySelection : public IwSelection {
  OneShape m_shape;
  QList<int> m_selectedFrames;
  bool m_isFormKeySelected;

  int m_rangeSelectionStartPos;

  IwTimeLineKeySelection();

public:
  static IwTimeLineKeySelection* instance();

  void enableCommands();
  bool isEmpty() const;

  // ����܂ł̑I�����������āA���݂̃V�F�C�v��؂�ւ���B
  void setShape(OneShape shape, bool isForm);
  OneShape getShape() { return m_shape; }
  bool isFormKeySelected() { return m_isFormKeySelected; }

  // �P�ɑI���t���[���̒ǉ�
  void selectFrame(int selectedFrame);
  // �w��t���[����I������O��
  void unselectFrame(int frame);
  // �I���̉���
  void selectNone();

  // �t���[���͈͑I��
  void doRangeSelect(int selectedFrame, bool ctrlPressed);

  // �I���J�E���g
  int selectionCount();
  // �t���[���𓾂�
  int getFrame(int index);

  // ���̃t���[�����I������Ă��邩
  bool isFrameSelected(int frame);

  // �R�s�[
  void copyKeyFrames();
  // �y�[�X�g
  void pasteKeyFrames();
  // �J�b�g
  void cutKeyFrames();

  // �I���Z�����L�[�t���[���ɂ���
  void setKey();
  // �I���Z���̃L�[�t���[������������
  void removeKey();
  // ��Ԃ�0.5(���`)�ɖ߂�
  void resetInterpolation();

  void setRangeSelectionStartPos(int pos = -1) {
    m_rangeSelectionStartPos = pos;
  }
};

//---------------------------------------------------
// �ȉ��AUndo�R�}���h
//---------------------------------------------------

class TimeLineKeyEditUndo : public QUndoCommand {
  IwProject* m_project;

  struct KeyData {
    // �`��f�[�^
    QMap<int, BezierPointList> formData;
    // Correspondence(�Ή��_)�f�[�^
    QMap<int, CorrPointList> corrData;
  };

  OneShape m_shape;
  // �S�ẴL�[�t���[���f�[�^��ۑ�
  KeyData m_beforeKeys;
  KeyData m_afterKeys;
  // �ړ����[�h�i�K�v�ȃL�[�����ێ�����j
  bool m_isFormKeyEdited;
  // KeyFrameEditor::KEYMODE m_keyMode;

  // redo������Ȃ��悤�ɂ���t���O
  int m_firstFlag;

public:
  TimeLineKeyEditUndo(OneShape& shape, bool isForm, IwProject* project);

  ~TimeLineKeyEditUndo() {}

  void storeKeyData(bool before);

  void undo();
  void redo();
};

//------------------------------------------------
// �I���Z�����L�[�t���[���ɂ��� ��Undo
//------------------------------------------------
class SetTimeLineKeyUndo : public QUndoCommand {
public:
  // �L�[�t���[���ɂ����t���[���̃��X�g
  struct SetKeyData {
    OneShape shape;
    QMap<int, BezierPointList> formKeyFrames;
    QMap<int, CorrPointList> corrKeyFrames;
  };

private:
  IwProject* m_project;
  // QList<SetKeyData> m_setKeyData;
  SetKeyData m_setKeyData;

public:
  SetTimeLineKeyUndo(SetKeyData& setKeyData, IwProject* project);
  void undo();
  void redo();
};

//------------------------------------------------
// �I���Z���̃L�[�t���[������������ ��Undo
//------------------------------------------------
class UnsetTimeLineKeyUndo : public QUndoCommand {
public:
  // �L�[�����������t���[���̃��X�g
  struct UnsetKeyData {
    OneShape shape;
    QMap<int, BezierPointList> formKeyFrames;
    QMap<int, CorrPointList> corrKeyFrames;
  };

private:
  IwProject* m_project;
  UnsetKeyData m_unsetKeyData;

public:
  UnsetTimeLineKeyUndo(UnsetKeyData& unsetKeyData, IwProject* project);
  void undo();
  void redo();
};

//------------------------------------------------
// ��Ԓl��0.5�Ƀ��Z�b�g ��Undo
//------------------------------------------------
class ResetInterpolationUndo : public QUndoCommand {
  OneShape m_shape;
  bool m_isForm;
  IwProject* m_project;

  QMap<int, double> m_interp_before;

public:
  ResetInterpolationUndo(OneShape& shape, bool isForm,
                         QMap<int, double> interps, IwProject* project);
  void undo();
  void redo();
};
#endif