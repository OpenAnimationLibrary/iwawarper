//--------------------------------------------------------
// Join Tool
// �ڑ��c�[���B�V�F�C�v�Q�����[�t�B���O�̃y�A�ɂ���
//--------------------------------------------------------

#include "jointool.h"

#include "iwapp.h"
#include "iwprojecthandle.h"
#include "iwproject.h"
#include "sceneviewer.h"
#include "iwshape.h"
#include "viewsettings.h"
#include "iwundomanager.h"

#include <QMouseEvent>
#include <QMessageBox>
#include <iostream>

JoinTool::JoinTool()
    : IwTool("T_Join"), m_overShape(0), m_srcShape(0), m_dstShape(0) {}

//--------------------------------------------------------

JoinTool::~JoinTool() {}

//--------------------------------------------------------

bool JoinTool::leftButtonDown(const QPointF& pos, const QMouseEvent*) {
  // �}�E�X��ɃV�F�C�v��������΃X���[
  if (!m_overShape) return false;

  // ���ɑΉ��_�����V�F�C�v���N���b�N�����ꍇ�A
  //  Join��؂邩�ǂ����𕷂��_�C�A���O���o��
  if (m_overShape->getPartner()) {
    QMessageBox msgBox;
    msgBox.setText("Unjoin the Selected Shape?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
      // Unjoin����������ɓ����
      // Undo�ɓo�^ ������redo���Ă΂�A�R�}���h�����s�����
      IwUndoManager::instance()->push(new UnjoinShapeUndo(m_overShape));
      return true;
      break;
    case QMessageBox::Cancel:
    default:
      return false;
      break;
    }
    // std::cout<<"ret = "<<ret<<std::endl;
  }

  // �N���b�N������A��������X�^�[�g
  m_startPos   = pos;
  m_currentPos = pos;
  m_srcShape   = m_overShape;

  return true;
}

//--------------------------------------------------------

bool JoinTool::leftButtonDrag(const QPointF& pos, const QMouseEvent* e) {
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (m_srcShape) {
    m_currentPos = pos;

    // ������DstShape���E���邩�`�F�b�N
    int name = m_viewer->pick(e->pos());
    // �}�E�X�I�[�o�[���Ă���V�F�C�v���L��ꍇ�A�i�[����
    IwShape* shape = project->getShapeFromName(name);
    if (shape) {
      // DstShape���蓾������F
      // 1, �܂�Join���肪���Ȃ�
      // 2, �Ή��_�̌�������
      // 3, isClosed �������i���Ă��铯�m�A�J���Ă��铯�m�j
      if (!shape->getPartner() &&
          (m_srcShape->getCorrPointAmount() == shape->getCorrPointAmount()) &&
          (m_srcShape->isClosed() == shape->isClosed())) {
        // dstShape������
        m_dstShape = shape;
      } else {
        m_dstShape = 0;
      }
    }
    // �}�E�X�I�[�o�[���Ă���V�F�C�v��������΁AdstShape������
    else {
      m_dstShape = 0;
    }

    // �}�E�X�������Ă��鎞�_�ŕ\���͍X�V����
    return true;
  }
  return false;
}

//--------------------------------------------------------

bool JoinTool::leftButtonUp(const QPointF&, const QMouseEvent*) {
  if (m_srcShape && m_dstShape) {
    // Join����ōs������
    // 1, ���݂���m_partner�ɑ���̃V�F�C�v�̃|�C���^������
    // 2, Dst���̃V�F�C�v��ShapeTypeId��ς���
    // 3, �Ή��_�Ԃ̕��������Ⴄ�ꍇ�A�������ɍ��킹��

    IwProject* project = IwApp::instance()->getCurrentProject()->getProject();

    // Undo�ɓo�^ ������redo���Ă΂�A�R�}���h�����s�����
    IwUndoManager::instance()->push(
        new JoinShapeUndo(m_srcShape, m_dstShape, project));
  }

  if (m_srcShape) m_srcShape = 0;
  if (m_dstShape) m_dstShape = 0;

  return false;
}

//--------------------------------------------------------

bool JoinTool::mouseMove(const QPointF& pos, const QMouseEvent* e) {
  if (!m_viewer) return false;

  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return false;

  int name = m_viewer->pick(e->pos());

  // �}�E�X�I�[�o�[���Ă���V�F�C�v���L��ꍇ�A�i�[����
  IwShape* shape = project->getShapeFromName(name);
  // �V�F�C�v���N���b�N�����ꍇ
  if (shape) {
    if (shape == m_overShape)
      return false;
    else {
      m_overShape = shape;
      return true;
    }
  } else {
    if (m_overShape == 0)
      return false;
    else {
      m_overShape = 0;
      return true;
    }
  }
}

//--------------------------------------------------------

void JoinTool::draw() {
  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  // MouseOver���Ă�Ƃ��́A�}�E�X���̃V�F�C�v�̑Ή��_��\���B
  // ���ɐڑ����Ă���V�F�C�v���L��ꍇ�́A����Ƃ̃����N�Ԃ���\��
  if (m_overShape) {
    // ���ɐڑ����Ă���V�F�C�v�̏ꍇ
    // ���A���̑��肪�������[���ɂ���Ƃ�
    ShapeRollId rollId = project->getViewSettings()->getShapeDisplayMode();
    if (m_overShape->getPartner() &&
        ((rollId == SHAPE_ROLL_AB) ||
         (m_overShape->getPartner()->getRollId() == rollId))) {
      drawJoinLine(m_overShape, m_overShape->getPartner());
      drawCorrLine(m_overShape->getPartner());
    }

    // std::cout<<"you have overShape"<<std::endl;
    drawCorrLine(m_overShape);
  }

  if (!m_srcShape) return;

  // �V�F�C�v���N���b�N���h���b�O���Ă���Ƃ��́A���̃V�F�C�v�̑Ή��_�ƁA
  // �h���b�O�̃X�^�[�g�_���獡�̃}�E�X�ʒu�܂ł̒�����`��B
  if (m_srcShape != m_overShape) {
    drawCorrLine(m_srcShape);
  }

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(m_startPos.x(), m_startPos.y(), 0.0);
  glVertex3f(m_currentPos.x(), m_currentPos.y(), 0.0);
  glEnd();

  if (!m_dstShape) return;

  // ����ɁA�}�E�X���ɃV�F�C�v���L��ꍇ���A�ڑ��\�ȏꍇ�́A
  // ���̃V�F�C�v�̑Ή��_�A����сA�Ή��_���m�����񂾃v���r���[����\������B
  if (m_dstShape != m_overShape) {
    drawCorrLine(m_dstShape);
  }

  // �Ή��_���m���q���v���r���[����`�悷��
  drawJoinPreviewLine();
}

//--------------------------------------------------------

int JoinTool::getCursorId(const QMouseEvent* e) {
  return ToolCursor::ArrowCursor;
}

//--------------------------------------------------------
// �Ή��_���m���q���v���r���[����`�悷��
//--------------------------------------------------------
void JoinTool::drawJoinPreviewLine() {
  // 2�V�F�C�v���������return(�ی�)
  if (!m_srcShape || !m_dstShape || m_srcShape == m_dstShape) return;

  IwProject* project = IwApp::instance()->getCurrentProject()->getProject();
  if (!project) return;
  // �\���t���[���𓾂�
  int frame = project->getViewFrame();

  // �Ή��_�̍��W���X�g�𓾂�
  QList<QPointF> corrPoints1 = m_srcShape->getCorrPointPositions(frame);
  QList<QPointF> corrPoints2 = m_dstShape->getCorrPointPositions(frame);

  glColor3f(1.0, 1.0, 1.0);

  glBegin(GL_LINES);

  for (int p = 0; p < corrPoints1.size(); p++) {
    QPointF corr1P = corrPoints1.at(p);
    QPointF corr2P = corrPoints2.at(p);

    glVertex3f(corr1P.x(), corr1P.y(), 0.0f);
    glVertex3f(corr2P.x(), corr2P.y(), 0.0f);

    glPopMatrix();
  }

  glEnd();
}

JoinTool joinTool;

//---------------------------------------------------
// �ȉ��AUndo�R�}���h��o�^
//---------------------------------------------------
JoinShapeUndo::JoinShapeUndo(IwShape* srcShape, IwShape* dstShape,
                             IwProject* prj)
    : m_project(prj), m_srcShape(srcShape), m_dstShape(dstShape) {
  // �Ή��_�̐��x������Ă���
  m_srcPrec = m_srcShape->getEdgeDensity();
  m_dstPrec = m_dstShape->getEdgeDensity();

  m_srcWasInAB = (m_srcShape->getRollId() == SHAPE_ROLL_AB);
  m_dstWasInAB = (m_dstShape->getRollId() == SHAPE_ROLL_AB);
}

JoinShapeUndo::~JoinShapeUndo() {}

void JoinShapeUndo::undo() {
  // Undo����
  // 1, m_partner������
  m_srcShape->setPartner(0);
  m_dstShape->setPartner(0);

  // 2, Dst���̃V�F�C�v��ShapeTypeId�����ɖ߂�
  m_dstShape->setShapeType(SourceType);

  // 3, �Ή��_�Ԃ̕����������ɖ߂�
  m_dstShape->setEdgeDensity(m_dstPrec);
  m_srcShape->setEdgeDensity(m_srcPrec);

  // 4, �V�F�C�v�̑����郍�[�����ȑOAB���[���������ꍇ�A���[����
  if (m_srcWasInAB) m_srcShape->setRollId(SHAPE_ROLL_AB);
  if (m_dstWasInAB) m_dstShape->setRollId(SHAPE_ROLL_AB);

  // �������ꂪ�J�����g�Ȃ�A�V�O�i�����G�~�b�g
  if (m_project->isCurrent())
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
}

void JoinShapeUndo::redo() {
  // Join����ōs������
  // 1, ���݂���m_partner�ɑ���̃V�F�C�v�̃|�C���^������
  m_srcShape->setPartner(m_dstShape);
  m_dstShape->setPartner(m_srcShape);

  // 2, Dst���̃V�F�C�v��ShapeTypeId��ς���
  m_dstShape->setShapeType(DestType);

  // 3, �Ή��_�Ԃ̕��������Ⴄ�ꍇ�A�������ɍ��킹��
  if (m_srcPrec != m_dstPrec) {
    if (m_srcPrec > m_dstPrec)
      m_dstShape->setEdgeDensity(m_srcPrec);
    else
      m_srcShape->setEdgeDensity(m_dstPrec);
  }

  // 4, �V�F�C�v�̑����郍�[����AB���[���������ꍇ�AA�܂���B�Ɉړ�����
  // ����AB�̏ꍇ�ASRC��A,DST��B�Ƃ���
  if (m_srcWasInAB && m_dstWasInAB) {
    m_srcShape->setRollId(SHAPE_ROLL_A);
    m_dstShape->setRollId(SHAPE_ROLL_B);
  }
  // ����ȊO�̏ꍇ�AAB���[���ɑ�����V�F�C�v���A����Ƃ͕ʂ̃��[���Ɉړ�������
  else if (m_srcWasInAB)
    m_srcShape->setRollId((m_dstShape->getRollId() == SHAPE_ROLL_A)
                              ? SHAPE_ROLL_B
                              : SHAPE_ROLL_A);
  else if (m_dstWasInAB)
    m_dstShape->setRollId((m_srcShape->getRollId() == SHAPE_ROLL_A)
                              ? SHAPE_ROLL_B
                              : SHAPE_ROLL_A);

  // �������ꂪ�J�����g�Ȃ�A�V�O�i�����G�~�b�g
  if (m_project->isCurrent())
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
}

//---------------------------------------------------

UnjoinShapeUndo::UnjoinShapeUndo(IwShape* shape)
    : m_project(IwApp::instance()->getCurrentProject()->getProject()) {
  switch (shape->getShapeType()) {
  case SourceType:
    m_srcShape = shape;
    m_dstShape = shape->getPartner();
    break;
  case DestType:
  default:
    m_srcShape = shape->getPartner();
    m_dstShape = shape;
    break;
  }
}

UnjoinShapeUndo::~UnjoinShapeUndo() {}

void UnjoinShapeUndo::undo() {
  // Join����ōs������
  // 1, ���݂���m_partner�ɑ���̃V�F�C�v�̃|�C���^������
  m_srcShape->setPartner(m_dstShape);
  m_dstShape->setPartner(m_srcShape);

  // 2, Dst���̃V�F�C�v��ShapeTypeId��ς���
  m_dstShape->setShapeType(DestType);

  // 3, �Ή��_�Ԃ̕��������Ⴄ�ꍇ�A�������ɍ��킹��
  int srcPrec = m_srcShape->getEdgeDensity();
  int dstPrec = m_dstShape->getEdgeDensity();
  if (srcPrec != dstPrec) {
    if (srcPrec > dstPrec)
      m_dstShape->setEdgeDensity(srcPrec);
    else
      m_srcShape->setEdgeDensity(dstPrec);
  }

  // �������ꂪ�J�����g�Ȃ�A�V�O�i�����G�~�b�g
  if (m_project->isCurrent())
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
}

void UnjoinShapeUndo::redo() {
  // 1, m_partner������
  m_srcShape->setPartner(0);
  m_dstShape->setPartner(0);

  // 2, Dst���̃V�F�C�v��ShapeTypeId�����ɖ߂�
  m_dstShape->setShapeType(SourceType);

  // �������ꂪ�J�����g�Ȃ�A�V�O�i�����G�~�b�g
  if (m_project->isCurrent())
    IwApp::instance()->getCurrentProject()->notifyProjectChanged();
}