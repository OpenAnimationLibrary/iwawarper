//---------------------------------------------------
// IwUndoManager
// QUndoStack���g���č��
//---------------------------------------------------

#include "iwundomanager.h"

IwUndoManager::IwUndoManager() {}

//---------------------------------------------------

IwUndoManager* IwUndoManager::instance() {
  static IwUndoManager _instance;
  return &_instance;
}

//---------------------------------------------------

IwUndoManager::~IwUndoManager() {}