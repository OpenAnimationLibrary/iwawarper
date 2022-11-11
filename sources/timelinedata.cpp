//----------------------------------------------------
// TimeLineData
// �^�C�����C�����̃R�s�[�^�y�[�X�g�Ŏg���f�[�^
//----------------------------------------------------

#include "timelinedata.h"

#include "iwlayer.h"

TimeLineData::TimeLineData() {}

//----------------------------------------------------

TimeLineData::TimeLineData(const TimeLineData *src) {
  m_frameData = src->m_frameData;
}

//----------------------------------------------------
// Sequence����f�[�^���i�[���� (�R�s�[)
//----------------------------------------------------
void TimeLineData::setFrameData(IwLayer *layer, int start, int end,
                                bool rightFrameIncluded) {
  // ���[���������Ȃ�������return
  if (!layer) return;

  // �O�̂��߁A�f�[�^����łȂ�������N���A
  if (!m_frameData.isEmpty()) m_frameData.clear();

  // �E�̃t���[�����܂܂��ꍇ��lastFrame���P����
  int lastFrame = (rightFrameIncluded) ? end + 1 : end;
  // �e�t���[�����i�[����
  for (int f = start; f < lastFrame; f++) {
    m_frameData.append(QPair<QString, QString>(layer->getParentFolderName(f),
                                               layer->getImageFileName(f)));
  }

  setText("TimeLineData");
}

//----------------------------------------------------
// �f�[�^����Sequence�̎w�肳�ꂽindex�ɓ���� (�}���y�[�X�g)
//----------------------------------------------------
void TimeLineData::getFrameData(IwLayer *layer, int frameIndex) const {
  // ���[���������Ȃ�������return
  if (!layer) return;

  // �f�[�^����Ȃ�return
  if (m_frameData.isEmpty()) return;

  // �f�[�^�̊e�t���[�������[���ɑ}������
  for (int f = 0; f < m_frameData.size(); f++) {
    // �t�H���_�p�X�𓾂�
    QString folderPath = m_frameData.at(f).first;
    // �t�@�C���p�X�𓾂�
    QString filePath = m_frameData.at(f).second;

    // �t�H���_�C���f�b�N�X�𓾂�
    int folderIndex = layer->getFolderPathIndex(folderPath);

    layer->insertPath(frameIndex, QPair<int, QString>(folderIndex, filePath));

    // �}�����鋫�E�C���f�b�N�X��1�i�߂�
    frameIndex++;
  }
}