//---------------------------------------------------
// IwTimeLineSelection
// �^�C�����C�����̑f�ރt���[���̑I��͈͂̃N���X
//---------------------------------------------------
#ifndef IWTIMELINESELECTION_H
#define IWTIMELINESELECTION_H

#include "iwselection.h"
#include "iwlayer.h"

#include <QUndoCommand>

class TimeLineData;

class IwTimeLineSelection : public IwSelection  // singleton
{
  // start,end��frame�Ԃ̋��E�͈̔́B
  // ����Ȃ���
  //	�@�@�a�@�@�@�a�@�@�@�a�@�@�@�a�@�@�@�a
  // �@�@�@�a �e�O �a �e�P �a �e�Q �a �e�R �a
  // �@�@�@�a�@�@�@�a�@�@�@�a�@�@�@�a�@�@�@�a
  // �@���E�O�@�@�@�P�@�@�@�Q�@�@�@�R�@�@�@�S
  int m_start;
  int m_end;
  // end�̉E�ׂ̃t���[�����I���Ɋ܂܂�Ă��邩
  bool m_rightFrameIncluded;

  IwTimeLineSelection();

public:
  static IwTimeLineSelection* instance();
  ~IwTimeLineSelection();

  void enableCommands();

  bool isEmpty() const;

  // �����t���[���̑I�� �I�����ς������true�A�ς��Ȃ����false��Ԃ�
  bool selectFrames(int start, int end, bool includeRightFrame);
  // �P�̃t���[���̑I��
  void selectFrame(int frame);
  // ���E�ЂƂ̑I��
  void selectBorder(int frame);
  // �I���̉���
  void selectNone();
  // �I��͈͂𓾂�
  void getRange(int& start, int& end, bool& rightFrameIncluded);

  // �t���[�����I������Ă��邩
  bool isFrameSelected(int frame) const;
  // ���E���I������Ă��邩
  bool isBorderSelected(int frame) const;

  //-- �ȉ��A���̑I���Ŏg����R�}���h�Q
  // �R�}���h���ʁF����\�ȃJ�����g���C����Ԃ�
  IwLayer* getAvailableLayer();
  // �R�s�[
  void copyFrames();
  // �y�[�X�g
  void pasteFrames();
  // ����
  void deleteFrames();
  // �J�b�g
  void cutFrames();
  // �󂫃Z��������
  void insertEmptyFrame();

  // �f�ނ̍����ւ�
  void replaceImages();
  // �ēǂݍ���
  void reloadImages();
};

//---------------------------------------------------
// �ȉ��AUndo�R�}���h��o�^
//---------------------------------------------------

//---------------------------------------------------
// �y�[�X�g
//---------------------------------------------------
class PasteTimeLineFrameUndo : public QUndoCommand {
  IwLayer* m_layer;
  int m_startIndex;
  TimeLineData* m_beforeData;
  TimeLineData* m_afterData;

public:
  PasteTimeLineFrameUndo(IwLayer* layer, int startIndex, TimeLineData* before,
                         const TimeLineData* after);
  ~PasteTimeLineFrameUndo();
  void undo();
  void redo();
};
//---------------------------------------------------
// �J�b�g/����
//---------------------------------------------------
class DeleteTimeLineFrameUndo : public QUndoCommand {
  IwLayer* m_layer;
  int m_startIndex;
  TimeLineData* m_beforeData;

public:
  DeleteTimeLineFrameUndo(IwLayer* layer, int startIndex, TimeLineData* before,
                          bool isDelete);
  ~DeleteTimeLineFrameUndo();
  void undo();
  void redo();
};

//---------------------------------------------------
// �󂫃Z��������
//---------------------------------------------------
class InsertEmptyFrameUndo : public QUndoCommand {
  IwLayer* m_layer;
  int m_index;

public:
  InsertEmptyFrameUndo(IwLayer* layer, int index);
  void undo();
  void redo();
};

//---------------------------------------------------
//  �f�ނ̍����ւ�
//---------------------------------------------------
class ReplaceImagesUndo : public QUndoCommand {
  IwLayer* m_layer;
  int m_from, m_to;
  QList<QPair<int, QString>> m_beforeData, m_afterData;
  bool m_firstFlag;

public:
  ReplaceImagesUndo(IwLayer* layer, int from, int to);
  void undo();
  void redo();
  void storeImageData(bool isBefore);
};

#endif