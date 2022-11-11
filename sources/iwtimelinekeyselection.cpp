//------------------------------------------------
// IwTimeLineKeySelection
// TimeLineWindow ���̃L�[�t���[���̑I��
//------------------------------------------------

#include "iwtimelinekeyselection.h"

#include "iwapp.h"
#include "iwprojecthandle.h"

#include "keyframedata.h"
#include "menubarcommandids.h"
#include "keyframeeditor.h"
#include "iwproject.h"
#include "iwundomanager.h"

#include "iwshapepairselection.h"
#include "iwtrianglecache.h"

#include <QClipboard>
#include <QApplication>

IwTimeLineKeySelection::IwTimeLineKeySelection()
    : m_shape(0, 0), m_rangeSelectionStartPos(-1) {}

//-----------------------------------------------------------------------------

IwTimeLineKeySelection* IwTimeLineKeySelection::instance() {
  static IwTimeLineKeySelection _instance;
  return &_instance;
}

//------------------------------------------------

void IwTimeLineKeySelection::enableCommands() {
  enableCommand(this, MI_Copy, &IwTimeLineKeySelection::copyKeyFrames);
  enableCommand(this, MI_Paste, &IwTimeLineKeySelection::pasteKeyFrames);
  enableCommand(this, MI_Cut, &IwTimeLineKeySelection::cutKeyFrames);
  enableCommand(this, MI_Delete, &IwTimeLineKeySelection::removeKey);

  enableCommand(this, MI_Key, &IwTimeLineKeySelection::setKey);
  enableCommand(this, MI_Unkey, &IwTimeLineKeySelection::removeKey);
  enableCommand(this, MI_ResetInterpolation,
                &IwTimeLineKeySelection::resetInterpolation);
}

//------------------------------------------------

bool IwTimeLineKeySelection::isEmpty() const {
  return m_shape.shapePairP == 0 || m_selectedFrames.isEmpty();
}

//------------------------------------------------
// ����܂ł̑I�����������āA���݂̃V�F�C�v��؂�ւ���B
//------------------------------------------------

void IwTimeLineKeySelection::setShape(OneShape shape, bool isForm) {
  if (shape == m_shape && isForm == m_isFormKeySelected) return;

  // �ȉ��A�V�K�L�[�t���[���̍s�ɐ؂�ւ�����ꍇ
  selectNone();
  m_shape                  = shape;
  m_isFormKeySelected      = isForm;
  m_rangeSelectionStartPos = -1;
}

//------------------------------------------------
// �P�ɑI���t���[���̒ǉ�
//------------------------------------------------

void IwTimeLineKeySelection::selectFrame(int selectedFrame) {
  if (!m_selectedFrames.contains(selectedFrame))
    m_selectedFrames.push_back(selectedFrame);
}

//------------------------------------------------
// �w��Z����I������O��
//------------------------------------------------

void IwTimeLineKeySelection::unselectFrame(int frame) {
  m_selectedFrames.removeAll(frame);
}

//------------------------------------------------
// �I���̉���
//------------------------------------------------

void IwTimeLineKeySelection::selectNone() { m_selectedFrames.clear(); }

//------------------------------------------------
// �t���[���͈͑I��
//------------------------------------------------
void IwTimeLineKeySelection::doRangeSelect(int selectedFrame,
                                           bool ctrlPressed) {
  if (m_rangeSelectionStartPos == -1) m_rangeSelectionStartPos = selectedFrame;

  int from = (selectedFrame <= m_rangeSelectionStartPos)
                 ? selectedFrame
                 : m_rangeSelectionStartPos;
  int to   = (selectedFrame <= m_rangeSelectionStartPos)
                 ? m_rangeSelectionStartPos
                 : selectedFrame;

  if (!ctrlPressed) selectNone();
  for (int f = from; f <= to; f++) selectFrame(f);
}

//------------------------------------------------
// �I���J�E���g
//------------------------------------------------

int IwTimeLineKeySelection::selectionCount() { return m_selectedFrames.size(); }

//------------------------------------------------
// �Z���𓾂�
//------------------------------------------------

int IwTimeLineKeySelection::getFrame(int index) {
  if (index >= m_selectedFrames.size()) return -1;

  return m_selectedFrames.at(index);
}

//------------------------------------------------
// ���̃Z�����I������Ă��邩
//------------------------------------------------

bool IwTimeLineKeySelection::isFrameSelected(int frame) {
  return m_selectedFrames.contains(frame);
}

//------------------------------------------------
// �R�s�[ (Undo�͖���)
//------------------------------------------------

void IwTimeLineKeySelection::copyKeyFrames() {
  // �Z���I��͈͂��������return
  if (m_selectedFrames.isEmpty()) return;

  // KeyFrameData�����A�f�[�^���i�[
  KeyFrameData* data = new KeyFrameData();

  data->setKeyFrameData(m_shape, m_selectedFrames,
                        (m_isFormKeySelected) ? KeyFrameEditor::KeyMode_Form
                                              : KeyFrameEditor::KeyMode_Corr);

  // �N���b�v�{�[�h�Ƀf�[�^������
  QClipboard* clipboard = QApplication::clipboard();
  clipboard->setMimeData(data, QClipboard::Clipboard);
}

//------------------------------------------------
// �y�[�X�g
//------------------------------------------------

void IwTimeLineKeySelection::pasteKeyFrames() {
  // �v���W�F�N�g�������return
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;

  // �V�F�C�v�I�����������return
  if (!m_shape.shapePairP) return;
  // �Z���I��͈͂��������return
  if (m_selectedFrames.isEmpty()) return;

  // �N���b�v�{�[�h����f�[�^�����AKeyFrameData���ǂ����m�F
  QClipboard* clipboard     = QApplication::clipboard();
  const QMimeData* mimeData = clipboard->mimeData();
  const KeyFrameData* keyFrameData =
      dynamic_cast<const KeyFrameData*>(mimeData);
  // �f�[�^������Ă�����return
  if (!keyFrameData) return;

  // �����j�F�y�[�X�g���쎞�̂��ׂĂ̑I���V�F�C�v�̏����L�[�v����B
  // �y�[�X�g����O�̃V�F�C�v�����L�[�v���Ă���
  TimeLineKeyEditUndo* undo =
      new TimeLineKeyEditUndo(m_shape, m_isFormKeySelected, project);

  keyFrameData->getKeyFrameData(m_shape, m_selectedFrames,
                                (m_isFormKeySelected)
                                    ? KeyFrameEditor::KeyMode_Form
                                    : KeyFrameEditor::KeyMode_Corr);

  // �y�[�X�g�����̃V�F�C�v����Undo�Ɋi�[
  undo->storeKeyData(false);
  IwUndoManager::instance()->push(undo);

  // �X�V�V�O�i�����G�~�b�g
  IwApp::instance()->getCurrentProject()->notifyProjectChanged();
}

//------------------------------------------------
// �J�b�g
//------------------------------------------------
void IwTimeLineKeySelection::cutKeyFrames() {
  // �v���W�F�N�g�������return
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;

  // �V�F�C�v�I�����������return
  if (!m_shape.shapePairP) return;
  // �t���[���I��͈͂��������return
  if (m_selectedFrames.isEmpty()) return;

  // �R�s�[���āA������Z���͏���
  copyKeyFrames();

  // ���j�F�J�b�g���쎞�̂��ׂĂ̑I���V�F�C�v�̏����L�[�v����B
  // �J�b�g����O�̃V�F�C�v�����L�[�v���Ă���
  TimeLineKeyEditUndo* undo =
      new TimeLineKeyEditUndo(m_shape, m_isFormKeySelected, project);
  int firstFormKeyFrame = -1;
  int firstCorrKeyFrame = -1;
  // �e�Z��������
  for (int f = 0; f < m_selectedFrames.size(); f++) {
    // ��ڂ̃L�[�t���[���̓X�L�b�v
    // �`��L�[
    if (m_isFormKeySelected) {
      // �L�[�̂Ƃ�
      if (m_shape.shapePairP->isFormKey(m_selectedFrames.at(f),
                                        m_shape.fromTo)) {
        // ��ڂ͏������ɃC���f�b�N�X���L�[�v���Ă���
        if (firstFormKeyFrame == -1) firstFormKeyFrame = m_selectedFrames.at(f);
        // ����ȊO�̃L�[�t���[���͏���
        else {
          m_shape.shapePairP->removeFormKey(m_selectedFrames.at(f),
                                            m_shape.fromTo);
          // �ύX�����t���[����invalidate
          int start, end;
          m_shape.shapePairP->getFormKeyRange(
              start, end, m_selectedFrames.at(f), m_shape.fromTo);
          IwTriangleCache::instance()->invalidateCache(
              start, end, project->getParentShape(m_shape.shapePairP));
        }
      }
    }
    // �Ή��_�L�[
    else {
      // �L�[�̂Ƃ�
      if (m_shape.shapePairP->isCorrKey(m_selectedFrames.at(f),
                                        m_shape.fromTo)) {
        // ��ڂ͏������ɃC���f�b�N�X���L�[�v���Ă���
        if (firstCorrKeyFrame == -1) firstCorrKeyFrame = m_selectedFrames.at(f);
        // ����ȊO�̃L�[�t���[���͏���
        else {
          m_shape.shapePairP->removeCorrKey(m_selectedFrames.at(f),
                                            m_shape.fromTo);
          // �ύX�����t���[����invalidate
          int start, end;
          m_shape.shapePairP->getCorrKeyRange(
              start, end, m_selectedFrames.at(f), m_shape.fromTo);
          IwTriangleCache::instance()->invalidateCache(
              start, end, project->getParentShape(m_shape.shapePairP));
        }
      }
    }

    f++;
  }

  // �V�F�C�v�̃L�[���c��P�ɂȂ���������Ȃ�
  // �`��L�[
  if (m_isFormKeySelected &&
      firstFormKeyFrame != -1 &&  // �擪�L�[���������Ă���
      m_shape.shapePairP->getFormKeyFrameAmount(m_shape.fromTo) !=
          1)  // �c��L�[���Q�ȏ�
  {
    m_shape.shapePairP->removeFormKey(firstFormKeyFrame, m_shape.fromTo);
    // �ύX�����t���[����invalidate
    int start, end;
    m_shape.shapePairP->getFormKeyRange(start, end, firstFormKeyFrame,
                                        m_shape.fromTo);
    IwTriangleCache::instance()->invalidateCache(
        start, end, project->getParentShape(m_shape.shapePairP));
  }

  // �Ή��_�L�[
  if (!m_isFormKeySelected &&
      firstCorrKeyFrame != -1 &&  // �擪�L�[���������Ă���
      m_shape.shapePairP->getCorrKeyFrameAmount(m_shape.fromTo) !=
          1)  // �c��L�[���Q�ȏ�
  {
    m_shape.shapePairP->removeCorrKey(firstCorrKeyFrame, m_shape.fromTo);
    // �ύX�����t���[����invalidate
    int start, end;
    m_shape.shapePairP->getCorrKeyRange(start, end, firstFormKeyFrame,
                                        m_shape.fromTo);
    IwTriangleCache::instance()->invalidateCache(
        start, end, project->getParentShape(m_shape.shapePairP));
  }

  // �J�b�g�����̃V�F�C�v����Undo�Ɋi�[
  undo->storeKeyData(false);
  IwUndoManager::instance()->push(undo);

  // �X�V�V�O�i�����G�~�b�g
  IwApp::instance()->getCurrentProject()->notifyProjectChanged();
}

//------------------------------------------------
// �I���Z�����L�[�t���[���ɂ���
//------------------------------------------------
void IwTimeLineKeySelection::setKey() {
  // �v���W�F�N�g�������return
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;

  // �V�F�C�v�I�����������return
  if (!m_shape.shapePairP) return;
  // �Z���I��͈͂��������return
  if (m_selectedFrames.isEmpty()) return;

  // �����L�[��ǉ������ꍇ�̃t���O
  bool somethingChanged = false;

  // Undo�Ɋi�[����f�[�^
  // QList<SetTimeLineKeyUndo::SetKeyData> setKeyDataList;
  SetTimeLineKeyUndo::SetKeyData setKeyData;
  setKeyData.shape = m_shape;
  setKeyData.corrKeyFrames.clear();
  setKeyData.formKeyFrames.clear();

  // �e�Z��������
  for (int f = 0; f < m_selectedFrames.size(); f++) {
    int tmpFrame = m_selectedFrames.at(f);

    // �`��L�[
    //  �L�[�łȂ��Ƃ��̂ݒǉ�
    if (m_isFormKeySelected &&
        !m_shape.shapePairP->isFormKey(tmpFrame, m_shape.fromTo)) {
      setKeyData.formKeyFrames[tmpFrame] =
          m_shape.shapePairP->getBezierPointList(tmpFrame, m_shape.fromTo);
      somethingChanged = true;
    }

    // �Ή��_�L�[
    //  �L�[�łȂ��Ƃ��̂ݒǉ�
    if (!m_isFormKeySelected &&
        !m_shape.shapePairP->isCorrKey(tmpFrame, m_shape.fromTo)) {
      setKeyData.corrKeyFrames[tmpFrame] =
          m_shape.shapePairP->getCorrPointList(tmpFrame, m_shape.fromTo);
      somethingChanged = true;
    }
  }
  if (!somethingChanged) return;

  // Undo�ǉ� redo���Ă΂�A�L�[���Z�b�g�����
  IwUndoManager::instance()->push(new SetTimeLineKeyUndo(setKeyData, project));
}

//------------------------------------------------
// �I���Z���̃L�[�t���[������������
//------------------------------------------------
void IwTimeLineKeySelection::removeKey() {
  // �v���W�F�N�g�������return
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;

  // �V�F�C�v�I�����������return
  if (!m_shape.shapePairP) return;
  // �Z���I��͈͂��������return
  if (m_selectedFrames.isEmpty()) return;

  // �����L�[��ǉ������ꍇ�̃t���O
  bool somethingChanged = false;

  // Undo�Ɋi�[����f�[�^
  UnsetTimeLineKeyUndo::UnsetKeyData unsetKeyData;
  unsetKeyData.shape = m_shape;

  // �e�Z��������
  for (int f = 0; f < m_selectedFrames.size(); f++) {
    int tmpFrame = m_selectedFrames.at(f);

    // �`��L�[
    //  �L�[�̂Ƃ��̂ݒǉ�
    if (m_isFormKeySelected &&
        m_shape.shapePairP->isFormKey(tmpFrame, m_shape.fromTo)) {
      unsetKeyData.formKeyFrames[tmpFrame] =
          m_shape.shapePairP->getBezierPointList(tmpFrame, m_shape.fromTo);
      somethingChanged = true;
    }

    // �Ή��_�L�[
    //  �L�[�̂Ƃ��̂ݒǉ�
    if (!m_isFormKeySelected &&
        m_shape.shapePairP->isCorrKey(tmpFrame, m_shape.fromTo)) {
      unsetKeyData.corrKeyFrames[tmpFrame] =
          m_shape.shapePairP->getCorrPointList(tmpFrame, m_shape.fromTo);
      somethingChanged = true;
    }
  }

  if (!somethingChanged) return;

  // Undo�ǉ� redo���Ă΂�A�L�[���Z�b�g�����
  IwUndoManager::instance()->push(
      new UnsetTimeLineKeyUndo(unsetKeyData, project));

  // �X�V�V�O�i�����G�~�b�g
  IwApp::instance()->getCurrentProject()->notifyProjectChanged();
}

//---------------------------------------------------
// ��Ԃ�0.5(���`)�ɖ߂�
//---------------------------------------------------
void IwTimeLineKeySelection::resetInterpolation() {
  if (m_selectedFrames.isEmpty()) return;
  // �v���W�F�N�g�������return
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;
  QMap<int, double> interps;
  QList<int> frames = m_selectedFrames;
  std::sort(frames.begin(), frames.end());
  for (auto frame : frames) {
    // �`��L�[�̏ꍇ
    if (m_isFormKeySelected &&
        m_shape.shapePairP->isFormKey(frame, m_shape.fromTo)) {
      double interp =
          m_shape.shapePairP->getBezierInterpolation(frame, m_shape.fromTo);
      if (interp != 0.5) interps.insert(frame, interp);
    }
    if (!m_isFormKeySelected &&
        m_shape.shapePairP->isCorrKey(frame, m_shape.fromTo)) {
      double interp =
          m_shape.shapePairP->getCorrInterpolation(frame, m_shape.fromTo);
      if (interp != 0.5) interps.insert(frame, interp);
    }
  }
  // �ŏ��̃t���[�����L�[�t���[���łȂ������ꍇ�A���̃Z�O�����g�̃L�[�ɂ��Ă����肷��
  int firstFrame = frames.at(0);
  if (m_isFormKeySelected &&
      !m_shape.shapePairP->isFormKey(firstFrame, m_shape.fromTo)) {
    int belongingKey =
        m_shape.shapePairP->belongingFormKey(firstFrame, m_shape.fromTo);
    if (belongingKey != -1) {
      double interp = m_shape.shapePairP->getBezierInterpolation(
          belongingKey, m_shape.fromTo);
      if (interp != 0.5) interps.insert(belongingKey, interp);
    }
  } else if (!m_isFormKeySelected &&
             !m_shape.shapePairP->isCorrKey(firstFrame, m_shape.fromTo)) {
    int belongingKey =
        m_shape.shapePairP->belongingCorrKey(firstFrame, m_shape.fromTo);
    if (belongingKey != -1) {
      double interp = m_shape.shapePairP->getCorrInterpolation(belongingKey,
                                                               m_shape.fromTo);
      if (interp != 0.5) interps.insert(belongingKey, interp);
    }
  }

  if (interps.isEmpty()) return;

  // Undo�ǉ� redo���Ă΂�A��Ԓl��0.5�Ƀ��Z�b�g�����
  IwUndoManager::instance()->push(new ResetInterpolationUndo(
      m_shape, m_isFormKeySelected, interps, project));

  // �X�V�V�O�i�����G�~�b�g
  IwApp::instance()->getCurrentProject()->notifyProjectChanged();
}

//---------------------------------------------------
// �ȉ��AUndo�R�}���h
//---------------------------------------------------

TimeLineKeyEditUndo::TimeLineKeyEditUndo(OneShape& shape, bool isForm,
                                         IwProject* project)
    : m_project(project)
    , m_shape(shape)
    , m_isFormKeyEdited(isForm)
    , m_firstFlag(true) {
  // ����O�̃L�[�����i�[
  storeKeyData(true);
}

//---------------------------------------------------

void TimeLineKeyEditUndo::storeKeyData(bool before) {
  KeyData keyData;

  // �`��L�[
  if (m_isFormKeyEdited)
    keyData.formData = m_shape.shapePairP->getFormData(m_shape.fromTo);

  // �Ή��_�L�[
  else
    keyData.corrData = m_shape.shapePairP->getCorrData(m_shape.fromTo);

  // �L�[�����i�[
  if (before)
    m_beforeKeys = keyData;
  else
    m_afterKeys = keyData;
}

//---------------------------------------------------

void TimeLineKeyEditUndo::undo() {
  // �`��L�[
  if (m_isFormKeyEdited)
    m_shape.shapePairP->setFormData(m_beforeKeys.formData, m_shape.fromTo);

  // �Ή��_�L�[
  else
    m_shape.shapePairP->setCorrData(m_beforeKeys.corrData, m_shape.fromTo);
  //}

  // �J�����g�v���W�F�N�g�Ȃ�
  if (m_project->isCurrent()) {
    // �e�������V�F�C�v��I������
    IwShapePairSelection::instance()->selectOneShape(m_shape);
    IwShapePairSelection::instance()->makeCurrent();

    // �X�V�V�O�i�����G�~�b�g
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
    IwTriangleCache::instance()->invalidateShapeCache(
        m_project->getParentShape(m_shape.shapePairP));
  }
}

//---------------------------------------------------

void TimeLineKeyEditUndo::redo() {
  if (m_firstFlag) {
    m_firstFlag = false;
    return;
  }

  // �`��L�[
  if (m_isFormKeyEdited)
    m_shape.shapePairP->setFormData(m_afterKeys.formData, m_shape.fromTo);
  // �Ή��_�L�[
  else
    m_shape.shapePairP->setCorrData(m_afterKeys.corrData, m_shape.fromTo);

  // �J�����g�v���W�F�N�g�Ȃ�
  if (m_project->isCurrent()) {
    // �e�������V�F�C�v��I������
    IwShapePairSelection::instance()->selectOneShape(m_shape);
    IwShapePairSelection::instance()->makeCurrent();
    // �X�V�V�O�i�����G�~�b�g
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
    IwTriangleCache::instance()->invalidateShapeCache(
        m_project->getParentShape(m_shape.shapePairP));
  }
}

//------------------------------------------------
// �I���Z�����L�[�t���[���ɂ��� ��Undo
//------------------------------------------------
SetTimeLineKeyUndo::SetTimeLineKeyUndo(SetKeyData& setKeyData,
                                       IwProject* project)
    : m_setKeyData(setKeyData), m_project(project) {}

// �L�[�t���[������������
void SetTimeLineKeyUndo::undo() {
  // �V�F�C�v
  OneShape shape = m_setKeyData.shape;
  // �`��L�[����ꂽ�t���[��
  for (auto formKey : m_setKeyData.formKeyFrames.keys())
    shape.shapePairP->removeFormKey(formKey, shape.fromTo);
  // �Ή��_�L�[����ꂽ�t���[��
  for (auto corrKey : m_setKeyData.corrKeyFrames.keys())
    shape.shapePairP->removeCorrKey(corrKey, shape.fromTo);

  // ���̃v���W�F�N�g���J�����g�Ȃ�X�V�V�O�i�����G�~�b�g
  if (m_project->isCurrent()) {
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
    IwTriangleCache::instance()->invalidateShapeCache(
        m_project->getParentShape(shape.shapePairP));
  }
}

// �L�[�t���[�����Z�b�g����
void SetTimeLineKeyUndo::redo() {
  // �V�F�C�v
  OneShape shape = m_setKeyData.shape;
  // �`��L�[����ꂽ�t���[��
  QMap<int, BezierPointList>::const_iterator i =
      m_setKeyData.formKeyFrames.constBegin();
  while (i != m_setKeyData.formKeyFrames.constEnd()) {
    shape.shapePairP->setFormKey(i.key(), shape.fromTo, i.value());
    i++;
  }
  // �Ή��_�L�[����ꂽ�t���[��
  QMap<int, CorrPointList>::const_iterator j =
      m_setKeyData.corrKeyFrames.constBegin();
  while (j != m_setKeyData.corrKeyFrames.constEnd()) {
    shape.shapePairP->setCorrKey(j.key(), shape.fromTo, j.value());
    j++;
  }

  // ���̃v���W�F�N�g���J�����g�Ȃ�X�V�V�O�i�����G�~�b�g
  if (m_project->isCurrent()) {
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
    IwTriangleCache::instance()->invalidateShapeCache(
        m_project->getParentShape(shape.shapePairP));
  }
}

//------------------------------------------------
// �I���Z���̃L�[�t���[������������ ��Undo
//------------------------------------------------
UnsetTimeLineKeyUndo::UnsetTimeLineKeyUndo(UnsetKeyData& unsetKeyData,
                                           IwProject* project)
    : m_unsetKeyData(unsetKeyData), m_project(project) {}

// �L�[�t���[�����Z�b�g����
void UnsetTimeLineKeyUndo::undo() {
  // �V�F�C�v
  OneShape shape = m_unsetKeyData.shape;
  // �`��L�[���������t���[��
  QMap<int, BezierPointList> formKey = m_unsetKeyData.formKeyFrames;
  if (!formKey.isEmpty()) {
    QMap<int, BezierPointList>::iterator i = formKey.begin();
    while (i != formKey.end()) {
      shape.shapePairP->setFormKey(i.key(), shape.fromTo, i.value());
      ++i;
    }
  }
  // �Ή��_�L�[���������t���[��
  QMap<int, CorrPointList> corrKey = m_unsetKeyData.corrKeyFrames;
  if (!corrKey.isEmpty()) {
    QMap<int, CorrPointList>::iterator i = corrKey.begin();
    while (i != corrKey.end()) {
      shape.shapePairP->setCorrKey(i.key(), shape.fromTo, i.value());
      ++i;
    }
  }

  // ���̃v���W�F�N�g���J�����g�Ȃ�X�V�V�O�i�����G�~�b�g
  if (m_project->isCurrent()) {
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
    IwTriangleCache::instance()->invalidateShapeCache(
        m_project->getParentShape(shape.shapePairP));
  }
}

// �L�[�t���[������������ (�Ō��1�͏����Ȃ�)
void UnsetTimeLineKeyUndo::redo() {
  // �V�F�C�v
  OneShape shape = m_unsetKeyData.shape;
  // �`��L�[���������t���[��
  QMap<int, BezierPointList> formKey = m_unsetKeyData.formKeyFrames;
  if (!formKey.isEmpty()) {
    QList<int> formKeyFrames = formKey.keys();
    for (int f = 0; f < formKeyFrames.size(); f++)
      shape.shapePairP->removeFormKey(formKeyFrames.at(f),
                                      shape.fromTo);  // (�Ō��1�͏����Ȃ�)
  }
  // �Ή��_�L�[���������t���[��
  QMap<int, CorrPointList> corrKey = m_unsetKeyData.corrKeyFrames;
  if (!corrKey.isEmpty()) {
    QList<int> corrKeyFrames = corrKey.keys();
    for (int f = 0; f < corrKeyFrames.size(); f++)
      shape.shapePairP->removeCorrKey(corrKeyFrames.at(f),
                                      shape.fromTo);  // (�Ō��1�͏����Ȃ�)
  }

  // ���̃v���W�F�N�g���J�����g�Ȃ�X�V�V�O�i�����G�~�b�g
  if (m_project->isCurrent()) {
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
    IwTriangleCache::instance()->invalidateShapeCache(
        m_project->getParentShape(shape.shapePairP));
  }
}

//------------------------------------------------
// ��Ԓl��0.5�Ƀ��Z�b�g ��Undo
//------------------------------------------------
ResetInterpolationUndo::ResetInterpolationUndo(OneShape& shape, bool isForm,
                                               QMap<int, double> interps,
                                               IwProject* project)
    : m_shape(shape)
    , m_isForm(isForm)
    , m_interp_before(interps)
    , m_project(project) {}

// before�̒l�ɂ���
void ResetInterpolationUndo::undo() {
  QMap<int, double>::const_iterator i = m_interp_before.constBegin();
  while (i != m_interp_before.constEnd()) {
    if (m_isForm)
      m_shape.shapePairP->setBezierInterpolation(i.key(), m_shape.fromTo,
                                                 i.value());
    else
      m_shape.shapePairP->setCorrInterpolation(i.key(), m_shape.fromTo,
                                               i.value());
    ++i;
  }
  // ���̃v���W�F�N�g���J�����g�Ȃ�X�V�V�O�i�����G�~�b�g
  if (m_project->isCurrent()) {
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
    IwTriangleCache::instance()->invalidateShapeCache(
        m_project->getParentShape(m_shape.shapePairP));
  }
}

// �S��0.5�ɂ���
void ResetInterpolationUndo::redo() {
  for (auto key : m_interp_before.keys()) {
    if (m_isForm)
      m_shape.shapePairP->setBezierInterpolation(key, m_shape.fromTo, 0.5);
    else
      m_shape.shapePairP->setCorrInterpolation(key, m_shape.fromTo, 0.5);
  }
  // ���̃v���W�F�N�g���J�����g�Ȃ�X�V�V�O�i�����G�~�b�g
  if (m_project->isCurrent()) {
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
    IwTriangleCache::instance()->invalidateShapeCache(
        m_project->getParentShape(m_shape.shapePairP));
  }
}
