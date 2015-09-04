#include "ui_ProjectInfoWidgetForm.h"
#include "ProjectInfoWidget.h"
#include "Project.h"

ProjectInfoWidget::ProjectInfoWidget(QWidget* parent) :
  QWidget(parent),
  m_proj(0),
  m_ui(new Ui::ProjectInfoWidgetForm)
{
  m_ui->setupUi(this);
  m_ui->m_description->setEnabled(false);
}

ProjectInfoWidget::~ProjectInfoWidget()
{
}

void ProjectInfoWidget::setProject(ProjectMetadata* project)
{
  m_proj = project;

  if(m_proj != 0)
  {
    m_ui->m_author->setText(m_proj->m_author);
    m_ui->m_title->setText(m_proj->m_title);
    m_ui->m_location->setText(m_proj->m_fileName);
  }
}

void ProjectInfoWidget::updateProjectInfo()
{
  if(m_proj != 0)
  {
    m_proj->m_title = m_ui->m_title->text();
    m_proj->m_author = m_ui->m_author->text();
    m_proj->m_changed = true;
  }
}
