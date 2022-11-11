//-----------------------------------------------------------------------------
// IwSelection
// �I������肭�肷��N���X
//-----------------------------------------------------------------------------
#include "iwselection.h"

#include "iwselectionhandle.h"

#include "iwapp.h"
#include "iwprojecthandle.h"

#include <QMenu>

IwSelection::IwSelection() {}

//-----------------------------------------------------------------------------

IwSelection::~IwSelection() {}

//-----------------------------------------------------------------------------
// �J�����g�̑I��������ɂ���
void IwSelection::makeCurrent() {
  IwSelectionHandle::getCurrent()->setSelection(this);
}

//-----------------------------------------------------------------------------
// �J�����g�̑I�������ꂾ�����ꍇ�A�I������������
void IwSelection::makeNotCurrent() {
  IwSelectionHandle *sh = IwSelectionHandle::getCurrent();
  if (sh->getSelection() == this) sh->setSelection(0);
}

//-----------------------------------------------------------------------------
// �J�����g�̑I����Ԃ��B���������҂��͊֌W�Ȃ��B
IwSelection *IwSelection::getCurrent() {
  return IwSelectionHandle::getCurrent()->getSelection();
}

//-----------------------------------------------------------------------------
// �����̑I�����J�����g�Ɋi�[�B���������҂��͊֌W�Ȃ��B
void IwSelection::setCurrent(IwSelection *selection) {
  IwSelectionHandle::getCurrent()->setSelection(selection);
}

//-----------------------------------------------------------------------------
// CommandId�Ŏw�肵���R�}���h��L���ɂ���
void IwSelection::enableCommand(CommandId cmdId,
                                CommandHandlerInterface *handler) {
  IwSelectionHandle::getCurrent()->enableCommand(cmdId, handler);
}

void IwSelection::addMenuAction(QMenu *menu, CommandId cmdId) {
  menu->addAction(CommandManager::instance()->getAction(cmdId));
}

//-----------------------------------------------------------------------------
// �v���W�F�N�g���؂�ւ������A�I������������
//-----------------------------------------------------------------------------
void IwSelection::onProjectSwitched() { selectNone(); }