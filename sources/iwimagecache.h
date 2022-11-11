//---------------------------------------------------
// IwImageCache
// DV�̃L���b�V���@�\���g���ĉ摜�L���b�V��������C���^�t�F�[�X
//---------------------------------------------------
#ifndef IWIMAGECACHE_H
#define IWIMAGECACHE_H

#include <QString>
#include "timagecache.h"

class IwImageCache  // singleton
{
  IwImageCache();

public:
  static IwImageCache *instance();

  // �i�[�F�p�X��Id�ɂ���TImageP���i�[
  void add(const QString &id, const TImageP &img);
  // ���o���F�p�X��Id�ɂ���TImageP��Ԃ�
  TImageP get(const QString &id) const;
  // �L���b�V������Ă��邩�ǂ���
  bool isCached(const QString &id) const;
  // �L���b�V��������
  void remove(const QString &id);
  // �S����
  void clear();
};

#endif