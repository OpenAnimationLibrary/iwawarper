//---------------------------------------------------
// IwTimeLineSelection
// �^�C�����C�����̑f�ރt���[���̑I��͈͂̃N���X
//---------------------------------------------------

#include "iwtimelineselection.h"

#include <iostream>

#include <QClipboard>
#include <QApplication>

#include "iwapp.h"
#include "iwlayerhandle.h"
#include "iwprojecthandle.h"
#include "timelinedata.h"
#include "iwundomanager.h"
#include "iwimagecache.h"
#include "iwproject.h"
#include "viewsettings.h"

#include "menubarcommand.h"
#include "menubarcommandids.h"
#include "iocommand.h"

#include "shapepairdata.h"
#include "iwshapepairselection.h"
#include "iwtrianglecache.h"

//===================================================

IwTimeLineSelection::IwTimeLineSelection()
    : m_start(0), m_end(-1), m_rightFrameIncluded(false) {}

//-----------------------------------------------------------------------------

IwTimeLineSelection* IwTimeLineSelection::instance() {
  static IwTimeLineSelection _instance;
  return &_instance;
}

//---------------------------------------------------
IwTimeLineSelection::~IwTimeLineSelection() {}
//---------------------------------------------------
void IwTimeLineSelection::enableCommands() {
  enableCommand(this, MI_Copy, &IwTimeLineSelection::copyFrames);
  enableCommand(this, MI_Paste, &IwTimeLineSelection::pasteFrames);
  enableCommand(this, MI_Delete, &IwTimeLineSelection::deleteFrames);
  enableCommand(this, MI_Cut, &IwTimeLineSelection::cutFrames);

  enableCommand(this, MI_InsertEmpty, &IwTimeLineSelection::insertEmptyFrame);

  // �t�@�C���p�X�̍����ւ�
  enableCommand(this, MI_ReplaceImages, &IwTimeLineSelection::replaceImages);
  enableCommand(this, MI_ReloadImages, &IwTimeLineSelection::reloadImages);
}
//---------------------------------------------------
bool IwTimeLineSelection::isEmpty() const {
  return (m_start > m_end) ? true : false;
}
//---------------------------------------------------
// �I���̉���
//---------------------------------------------------
void IwTimeLineSelection::selectNone() {
  m_start              = 0;
  m_end                = -1;
  m_rightFrameIncluded = false;
}
//---------------------------------------------------
// �����t���[���̑I�� �I�����ς������true�A�ς��Ȃ����false��Ԃ�
//---------------------------------------------------
bool IwTimeLineSelection::selectFrames(int start, int end,
                                       bool includeRightFrame) {
  if (m_start == start && m_end == end &&
      m_rightFrameIncluded == includeRightFrame)
    return false;

  m_start              = start;
  m_end                = end;
  m_rightFrameIncluded = includeRightFrame;
  return true;
}
//---------------------------------------------------
// �P�̃t���[���̑I��
//---------------------------------------------------
void IwTimeLineSelection::selectFrame(int frame) {
  m_start              = frame;
  m_end                = frame;
  m_rightFrameIncluded = true;
}
//---------------------------------------------------
// ���E�ЂƂ̑I��
//---------------------------------------------------
void IwTimeLineSelection::selectBorder(int frame) {
  m_start              = frame;
  m_end                = frame;
  m_rightFrameIncluded = false;
}
//---------------------------------------------------
// �t���[�����I������Ă��邩
//---------------------------------------------------
bool IwTimeLineSelection::isFrameSelected(int frame) const {
  // �I��͈͂���Ȃ�false
  if (isEmpty()) return false;
  // start�ȏォ��end�����Ȃ�n�j
  if (m_start <= frame && m_end > frame) return true;
  // frame==end�̏ꍇ�A�܂܂�Ă��邩�ǂ����̓t���O�Ō��܂�
  if (m_end == frame) return m_rightFrameIncluded;
  // ����ӊO�̏ꍇ�͑I��͈͊O
  return false;
}
//---------------------------------------------------
// ���E���I������Ă��邩
//---------------------------------------------------
bool IwTimeLineSelection::isBorderSelected(int frame) const {
  // �I��͈͂���Ȃ�false
  if (isEmpty()) return false;
  // start�ȏォ��end�ȉ��Ȃ�n�j
  if (m_start <= frame && m_end >= frame) return true;
  // ����ӊO�̏ꍇ�͑I��͈͊O
  return false;
}
//---------------------------------------------------
// �I��͈͂𓾂�
//---------------------------------------------------
void IwTimeLineSelection::getRange(int& start, int& end,
                                   bool& rightFrameIncluded) {
  start              = m_start;
  end                = m_end;
  rightFrameIncluded = m_rightFrameIncluded;
}

//---------------------------------------------------
// �R�}���h���ʁF����\�ȃJ�����g���C����Ԃ�
//---------------------------------------------------
IwLayer* IwTimeLineSelection::getAvailableLayer() {
  // ���ꂪ�J�����g�łȂ����return 0
  // �I��͈͂���̏ꍇreturn 0
  if (IwSelection::getCurrent() != this || isEmpty()) return 0;
  // �J�����g���[�����擾����return
  return IwApp::instance()->getCurrentLayer()->getLayer();
}
//---------------------------------------------------
// �R�s�[ (Undo�͖���)
//---------------------------------------------------
void IwTimeLineSelection::copyFrames() {
  // ����\�ȃJ�����g���C�����擾
  IwLayer* layer = getAvailableLayer();
  // �J�����g���C���������ꍇreturn
  if (!layer) return;

  // SequenceData�f�[�^�����A�I��͈̓f�[�^���i�[
  TimeLineData* timeLineData = new TimeLineData();

  timeLineData->setFrameData(layer, m_start, m_end, m_rightFrameIncluded);

  // �N���b�v�{�[�h��SequenceData�f�[�^������
  QClipboard* clipboard = QApplication::clipboard();
  clipboard->setMimeData(timeLineData, QClipboard::Clipboard);
}
//---------------------------------------------------
// �y�[�X�g
//---------------------------------------------------
void IwTimeLineSelection::pasteFrames() {
  // ����\�ȃJ�����g���C�����擾
  IwLayer* layer = getAvailableLayer();
  // �J�����g���C���������ꍇreturn
  if (!layer) return;

  // �N���b�v�{�[�h����f�[�^�����ATimeLineData���ǂ����m�F
  QClipboard* clipboard     = QApplication::clipboard();
  const QMimeData* mimeData = clipboard->mimeData();
  const TimeLineData* timeLineData =
      dynamic_cast<const TimeLineData*>(mimeData);
  // �f�[�^������Ă�����return
  if (!timeLineData) {
    // ShapePair�̓^�C�����C���I�𒆂Ƀy�[�X�g�\�ɂ���
    const ShapePairData* shapePairData =
        dynamic_cast<const ShapePairData*>(mimeData);
    if (shapePairData) {
      IwShapePairSelection::instance()->makeCurrent();
      IwShapePairSelection::instance()->pasteShapes();
    }
    return;
  }

  // �I��͈͂��L�[�v
  // �����ō����sequenceData�́APasteSequenceFrameUndo�̃f�X�g���N�^�ŊJ�������
  TimeLineData* beforeData = new TimeLineData();
  beforeData->setFrameData(layer, m_start, m_end, m_rightFrameIncluded);

  // Undo�ɓo�^ ������redo���Ă΂�A�R�}���h�����s�����
  IwUndoManager::instance()->push(
      new PasteTimeLineFrameUndo(layer, m_start, beforeData, timeLineData));
}
//---------------------------------------------------
// ����
//---------------------------------------------------
void IwTimeLineSelection::deleteFrames() {
  // ����\�ȃJ�����g���C�����擾
  IwLayer* layer = getAvailableLayer();
  // �J�����g���C���������ꍇreturn
  if (!layer) return;

  // SequenceData�f�[�^�����A�I��͈̓f�[�^���i�[
  TimeLineData* beforeData = new TimeLineData();
  beforeData->setFrameData(layer, m_start, m_end, m_rightFrameIncluded);

  // Undo�ɓo�^ ������redo���Ă΂�A�R�}���h�����s�����
  IwUndoManager::instance()->push(
      new DeleteTimeLineFrameUndo(layer, m_start, beforeData, true));
}
//---------------------------------------------------
// �J�b�g
//---------------------------------------------------
void IwTimeLineSelection::cutFrames() {
  std::cout << "cutFrames" << std::endl;

  // ����\�ȃJ�����g���C�����擾
  IwLayer* layer = getAvailableLayer();
  // �J�����g���C���������ꍇreturn
  if (!layer) return;

  // SequenceData�f�[�^�����A�I��͈̓f�[�^���i�[
  TimeLineData* beforeData = new TimeLineData();
  beforeData->setFrameData(layer, m_start, m_end, m_rightFrameIncluded);

  // �N���b�v�{�[�h��SequenceData�f�[�^������
  QClipboard* clipboard = QApplication::clipboard();
  clipboard->setMimeData(beforeData, QClipboard::Clipboard);

  // Undo�ɓo�^ ������redo���Ă΂�A�R�}���h�����s�����
  IwUndoManager::instance()->push(
      new DeleteTimeLineFrameUndo(layer, m_start, beforeData, false));
}

//---------------------------------------------------
// �󂫃Z��������
//---------------------------------------------------
void IwTimeLineSelection::insertEmptyFrame() {
  // ����\�ȃJ�����g���C�����擾
  IwLayer* layer = getAvailableLayer();
  // �J�����g���C���������ꍇreturn
  if (!layer) return;

  // Undo�ɓo�^ ������redo���Ă΂�A�R�}���h�����s�����
  IwUndoManager::instance()->push(new InsertEmptyFrameUndo(layer, m_start));
}

//---------------------------------------------------
//  �f�ނ̍����ւ�
//---------------------------------------------------
void IwTimeLineSelection::replaceImages() {
  // ����\�ȃJ�����g���C�����擾
  IwLayer* layer = getAvailableLayer();
  // �J�����g���C���������ꍇreturn
  if (!layer) return;

  // Undo�����ƁA�R���X�g���N�^���Ō��݂̃f�[�^�͊m�ۂ����
  ReplaceImagesUndo* undo = new ReplaceImagesUndo(layer, m_start, m_end);

  // �t���[���͈͂��w��
  IoCmd::doReplaceImages(layer, m_start, m_end);

  // �����̃f�[�^���m��
  undo->storeImageData(false);

  // Undo�ɓo�^ ������redo���Ă΂�A�R�}���h�����s����邪�A�t���O�ŉ��
  IwUndoManager::instance()->push(undo);
}

//---------------------------------------------------
//  �f�ނ̍ēǂݍ���
//---------------------------------------------------
void IwTimeLineSelection::reloadImages() {
  // ����\�ȃJ�����g���C�����擾
  IwLayer* layer = getAvailableLayer();
  // �J�����g���C���������ꍇreturn
  if (!layer) return;
  ViewSettings* vs =
      IwApp::instance()->getCurrentProject()->getProject()->getViewSettings();

  QStringList pathsToBeReloaded;
  for (int f = m_start; f <= m_end; f++) {
    QString path = layer->getImageFilePath(f);
    if (path.isEmpty() || pathsToBeReloaded.contains(path)) continue;
    pathsToBeReloaded.append(path);
  }
  for (const auto& path : pathsToBeReloaded) {
    if (IwImageCache::instance()->isCached(path))
      IwImageCache::instance()->remove(path);
    vs->releaseTexture(path);
  }

  // invalidate
  for (int s = 0; s < layer->getShapePairCount(); s++) {
    ShapePair* shape = layer->getShapePair(s);
    if (!shape) continue;
    IwTriangleCache::instance()->invalidateCache(m_start, m_end, shape);
  }
  // Viewer���ĕ`�悷�邽�߂̃V�O�i�����o��
  IwApp::instance()->getCurrentProject()->notifyViewSettingsChanged();
}

//===================================================
//---------------------------------------------------
// �ȉ��AUndo�R�}���h��o�^
//---------------------------------------------------
//---------------------------------------------------
// �y�[�X�g
//---------------------------------------------------
PasteTimeLineFrameUndo::PasteTimeLineFrameUndo(IwLayer* layer, int startIndex,
                                               TimeLineData* before,
                                               const TimeLineData* after)
    : m_layer(layer)
    , m_startIndex(startIndex)
    , m_beforeData(before)
    , m_afterData(new TimeLineData(after)) {
  setText(QObject::tr("Paste Timeline Frames"));
}

PasteTimeLineFrameUndo::~PasteTimeLineFrameUndo() {
  delete m_beforeData;
  delete m_afterData;
}

void PasteTimeLineFrameUndo::undo() {
  // startIndex����afterData�̖���������
  // �������閇��
  int amount = m_afterData->getFrameLength();
  m_layer->deletePaths(m_startIndex, amount);
  // beforeData�̑}��
  m_beforeData->getFrameData(m_layer, m_startIndex);
  // SequenceChanged���G�~�b�g
  IwApp::instance()->getCurrentLayer()->notifyLayerChanged(m_layer);
}

void PasteTimeLineFrameUndo::redo() {
  //---�I��͈͂�����
  // �������閇��
  int amount = m_beforeData->getFrameLength();
  // ����
  m_layer->deletePaths(m_startIndex, amount);
  // �R�s�[���ꂽSequenceData��}��
  m_afterData->getFrameData(m_layer, m_startIndex);
  // SequenceChanged���G�~�b�g
  IwApp::instance()->getCurrentLayer()->notifyLayerChanged(m_layer);
}

//---------------------------------------------------
// �J�b�g/����
//---------------------------------------------------
DeleteTimeLineFrameUndo::DeleteTimeLineFrameUndo(IwLayer* layer, int startIndex,
                                                 TimeLineData* before,
                                                 bool isDelete)
    : m_layer(layer), m_startIndex(startIndex), m_beforeData(before) {
  if (isDelete)
    setText(QObject::tr("Delete Timeline Frames"));
  else
    setText(QObject::tr("Cut Timeline Frames"));
}

DeleteTimeLineFrameUndo::~DeleteTimeLineFrameUndo() { delete m_beforeData; }

void DeleteTimeLineFrameUndo::undo() {
  // �󂫃Z�����폜
  int amount = m_beforeData->getFrameLength();
  m_layer->deletePaths(m_startIndex, amount);
  // beforeData�̑}��
  m_beforeData->getFrameData(m_layer, m_startIndex);
  // SequenceChanged���G�~�b�g
  IwApp::instance()->getCurrentLayer()->notifyLayerChanged(m_layer);
  // IwApp::instance()->getCurrentSequence()->notifySequenceChanged(m_startIndex);
}

void DeleteTimeLineFrameUndo::redo() {
  //---�͈͂�����
  // �������閇��
  int amount = m_beforeData->getFrameLength();
  // ���� 140313 ��Z���ɂ���
  for (int f = 0; f < amount; f++)
    m_layer->replacePath(m_startIndex + f, QPair<int, QString>(0, QString()));
  // m_layer->deletePaths(m_startIndex,amount);

  // SequenceChanged���G�~�b�g
  IwApp::instance()->getCurrentLayer()->notifyLayerChanged(m_layer);
  // IwApp::instance()->getCurrentSequence()->notifySequenceChanged(m_startIndex);
}

//---------------------------------------------------
// �󂫃Z��������
//---------------------------------------------------

InsertEmptyFrameUndo::InsertEmptyFrameUndo(IwLayer* layer, int index)
    : m_layer(layer), m_index(index) {}

void InsertEmptyFrameUndo::undo() {
  m_layer->deletePaths(m_index, 1);
  IwApp::instance()->getCurrentLayer()->notifyLayerChanged(m_layer);
}

void InsertEmptyFrameUndo::redo() {
  m_layer->insertPath(m_index, QPair<int, QString>(0, ""));
  IwApp::instance()->getCurrentLayer()->notifyLayerChanged(m_layer);
}

//---------------------------------------------------
//  �f�ނ̍����ւ���Undo
//---------------------------------------------------
ReplaceImagesUndo::ReplaceImagesUndo(IwLayer* layer, int from, int to)
    : m_layer(layer), m_from(from), m_to(to), m_firstFlag(true) {
  // ����O�̃t�@�C���p�X�����擾
  storeImageData(true);
}

void ReplaceImagesUndo::undo() {
  // Before�̃f�[�^�ɖ߂�
  for (int f = m_from, i = 0; f <= m_to; f++, i++)
    m_layer->replacePath(f, m_beforeData.at(i));

  IwApp::instance()->getCurrentLayer()->notifyLayerChanged(m_layer);
}

void ReplaceImagesUndo::redo() {
  // �ŏ��̑���͔�΂�
  if (m_firstFlag) {
    m_firstFlag = false;
    return;
  }
  // After�̃f�[�^�ɖ߂�
  for (int f = m_from, i = 0; f <= m_to; f++, i++)
    m_layer->replacePath(f, m_afterData.at(i));

  IwApp::instance()->getCurrentLayer()->notifyLayerChanged(m_layer);
}

// �t�@�C���p�X�����擾����
void ReplaceImagesUndo::storeImageData(bool isBefore) {
  for (int f = m_from; f <= m_to; f++) {
    if (isBefore)
      m_beforeData.append(m_layer->getPathData(f));
    else
      m_afterData.append(m_layer->getPathData(f));
  }
}