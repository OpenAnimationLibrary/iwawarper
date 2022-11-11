//-----------------------------------------------------------------------------
// IwToolHandle
// ���݂̃c�[�����Ǘ�����
//-----------------------------------------------------------------------------

#include "iwtoolhandle.h"

#include "iwtool.h"

IwToolHandle::IwToolHandle() : m_tool(0), m_toolName("") {}

//-----------------------------------------------------------------------------
// �J�����g�c�[�����擾
//-----------------------------------------------------------------------------
IwTool* IwToolHandle::getTool() const { return m_tool; }

//-----------------------------------------------------------------------------
// �J�����g�c�[�����Z�b�g
//-----------------------------------------------------------------------------
void IwToolHandle::setTool(QString name) {
  m_toolName   = name;
  IwTool* tool = IwTool::getTool(m_toolName);
  if (tool == m_tool) return;

  if (m_tool) m_tool->onDeactivate();

  m_tool = tool;

  if (m_tool) {
    m_tool->onActivate();
    emit toolSwitched();
  }
}
