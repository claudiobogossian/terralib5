//#include "ui_ProjectInfoWidgetForm.h"
//#include "ProjectInfoWidget.h"
//#include "Project.h"

//te::qt::af::ProjectInfoWidget::ProjectInfoWidget(QWidget* parent) :
//  QWidget(parent),
//  m_proj(0),
//  m_ui(new Ui::ProjectInfoWidgetForm)
//{
//  m_ui->setupUi(this);
//}

//te::qt::af::ProjectInfoWidget::~ProjectInfoWidget()
//{
//}

//void te::qt::af::ProjectInfoWidget::setProject(Project* project)
//{
//  m_proj = project;

//  if(m_proj != 0)
//  {
//    m_ui->m_author->setText(project->getAuthor().c_str());
//    m_ui->m_title->setText(project->getTitle().c_str());
//    m_ui->m_location->setText(project->getFileName().c_str());
//  }
//}

//void te::qt::af::ProjectInfoWidget::updateProjectInfo()
//{
//  if(m_proj != 0)
//  {
//    m_proj->setTitle(m_ui->m_title->text().toStdString());
//    m_proj->setAuthor(m_ui->m_author->text().toStdString());
//  }
//}
