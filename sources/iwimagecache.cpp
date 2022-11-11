//---------------------------------------------------
// IwImageCache
// OpenToonz�̃L���b�V���@�\���g���ĉ摜�L���b�V��������C���^�t�F�[�X
//---------------------------------------------------

#include "iwimagecache.h"

IwImageCache::IwImageCache() {}

IwImageCache *IwImageCache::instance() {
  static IwImageCache _instance;
  return &_instance;
}

//---------------------------------------------------
// �i�[�F�p�X��Id�ɂ���TImageP���i�[ ���łɂ���ꍇ�͏㏑��
//---------------------------------------------------
void IwImageCache::add(const QString &id, const TImageP &img) {
  if (isCached(id)) remove(id);
  TImageCache::instance()->add(id, img);
}

//---------------------------------------------------
// ���o���F�p�X��Id�ɂ���TImageP��Ԃ�
//---------------------------------------------------
TImageP IwImageCache::get(const QString &id) const {
  return TImageCache::instance()->get(id, true);
}

//---------------------------------------------------
// �L���b�V������Ă��邩�ǂ���
//---------------------------------------------------
bool IwImageCache::isCached(const QString &id) const {
  return TImageCache::instance()->isCached(id.toStdString());
}

//---------------------------------------------------
// �L���b�V��������
//---------------------------------------------------
void IwImageCache::remove(const QString &id) {
  TImageCache::instance()->remove(id);
}

void IwImageCache::clear() { TImageCache::instance()->clear(); }
