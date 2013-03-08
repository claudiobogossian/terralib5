#include "ui_ProjectInfo.h"
#include "ProjectInfo.h"
#include "Project.h"

te::qt::af::ProjectInfo::ProjectInfo(QWidget* parent) :
QWidget(parent),
  m_ui(new Ui::ProjectInfo),
  m_proj(0)
{
  m_ui->setupUi(this);
}

te::qt::af::ProjectInfo::~ProjectInfo()
{
}

void te::qt::af::ProjectInfo::setProject(Project* project)
{
  m_proj = project;

  if(m_proj != 0)
  {
    m_ui->m_author->setText(project->getAuthor().c_str());
    m_ui->m_title->setText(project->getTitle().c_str());

    QString desc("This project contains %1 layers");
    desc = desc.arg(QString::number((int)m_proj->getLayers().size()));

    m_ui->m_description->setText(desc);
  }
}

void te::qt::af::ProjectInfo::updateProjectInfo()
{
  if(m_proj != 0)
  {
    m_proj->setTitle(m_ui->m_title->text().toStdString());
    m_proj->setAuthor(m_ui->m_author->text().toStdString());
  }
}
