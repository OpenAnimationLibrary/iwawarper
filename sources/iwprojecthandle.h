//---------------------------------------------------
// IwProjectHandle
// �v���W�F�N�g���̃J�����g���Ǘ�����B
//---------------------------------------------------
#ifndef IWPROJECTHANDLE_H
#define IWPROJECTHANDLE_H

#include <QObject>
#include <QMap>

class IwProject;
class IwRenderInstance;  // ����

class IwProjectHandle : public QObject {
  Q_OBJECT

  IwProject* m_project;
  bool m_dirtyFlag;

  QMap<int, unsigned int> m_runningInstances;

public:
  IwProjectHandle();
  ~IwProjectHandle();

  IwProject* getProject() const;
  void setProject(IwProject* project);

  // ���ݕ\�����̃t���[�����ς������Ă�
  void notifyViewFrameChanged() { emit viewFrameChanged(); }
  // project���؂�ւ������Ă�
  void notifyProjectSwitched() { emit projectSwitched(); }
  // ViewSettings���ς������Ă�
  void notifyViewSettingsChanged() { emit viewSettingsChanged(); }

  void notifyShapeTagSettingsChanged() {
    emit shapeTagSettingsChanged();
    setDirty();
  }
  // project�̓��e���ς������Ă�
  void notifyProjectChanged(bool emitShapeTreeChanged = false) {
    emit projectChanged();
    if (emitShapeTreeChanged) emit shapeTreeChanged();
    setDirty();
  }
  // �O���[�v�֌W��������ꂽ�Ƃ��Ă�
  void notifyGroupChanged() { emit groupChanged(); }
  // GroupEditor��CurveEditor�̂��Ƃ�̂��߁B
  // ���ݑI�𒆂�Group�̃C���f�b�N�X���ς������emit����
  void notifyCurrentGroupIndexChanged(int index) {
    emit currentGroupIndexChanged(index);
  }

  //---�����_�����O�֌W
  void notifyPreviewCacheInvalidated() { emit previewCacheInvalidated(); }

  void setDirty(bool dirty = true);
  bool isDirty() { return m_dirtyFlag; }

  void suspendRender(int frame);
  bool isRunningPreview(int frame);

  // �V�F�C�v�֌W
  void notifyShapeTreeChanged() { emit shapeTreeChanged(); }

protected slots:
  // ���[�����ҏW���ꂽ�Ƃ��A���݂̃v���W�F�N�g������΁A
  // multiFrame�����X�V���� IwApp�ŃR�l�N�g���Ă���
  void onSequenceChanged();
  void onPreviewRenderStarted(int frame, unsigned int taskId);
  void onPreviewRenderFinished(int frame, unsigned int taskId);

signals:
  // �J�����g�t���[�����ς�����Ƃ�
  void viewFrameChanged();
  // project���؂�ւ�����ꍇ
  void projectSwitched();
  // ����ȊO�A���ł�ViewSettings���ς������Ă΂��
  void viewSettingsChanged();
  // project�̓��e���ς�����ꍇ(���[�N�G���A�̃T�C�Y���ς�����Ƃ��Ƃ�)
  void projectChanged();

  void shapeTreeChanged();

  // �O���[�v�֌W��������ꂽ�Ƃ�
  void groupChanged();

  // GroupEditor��CurveEditor�̂��Ƃ�̂��߁B
  // ���ݑI�𒆂�Group�̃C���f�b�N�X���ς������emit����
  void currentGroupIndexChanged(int);

  //---�����_�����O�֌W
  // �v���r���[����
  // void previewRenderCompleted();
  void previewRenderStarted(int frame);
  void previewRenderFinished(int frame);
  // �L���b�V����Invalidate���ꂽ
  void previewCacheInvalidated();

  void dirtyFlagChanged();

  // �V�F�C�v�^�O�ݒ�̍X�V
  void shapeTagSettingsChanged();
};

#endif