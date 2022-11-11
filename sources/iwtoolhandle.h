//-----------------------------------------------------------------------------
// IwToolHandle
// �c�[���̃J�����g���Ǘ�����
//-----------------------------------------------------------------------------
#ifndef IWTOOLHANDLE_H
#define IWTOOLHANDLE_H

#include <QObject>
#include <QString>

class IwTool;

class IwToolHandle : public QObject {
  Q_OBJECT

  IwTool* m_tool;
  QString m_toolName;

public:
  IwToolHandle();

  // �J�����g�c�[�����擾
  IwTool* getTool() const;
  // �J�����g�c�[�����Z�b�g
  void setTool(QString name);

signals:
  // �c�[�����؂�ւ������Ă΂��
  void toolSwitched();
};

#endif