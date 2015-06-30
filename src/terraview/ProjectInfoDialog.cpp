//#include "ProjectInfoDialog.h"
//#include "ProjectInfoWidget.h"

//// Qt
//#include <QPushButton>
//#include <QHBoxLayout>
//#include <QVBoxLayout>
//#include <QGridLayout>

//te::qt::af::ProjectInfoDialog::ProjectInfoDialog(QWidget* parent) :
//QDialog(parent)
//{
//  QDialog::setWindowTitle(tr("Project Properties"));
//  QDialog::setWindowModality(Qt::ApplicationModal);

//  //Buttons
//  QPushButton* okButton = new QPushButton(tr("&Ok"), this);
//  QPushButton* cancelButton = new QPushButton(tr("&Cancel"), this);
//  QPushButton* helpButton = new QPushButton(tr("&Help"), this);

//  QHBoxLayout* hlay = new QHBoxLayout;

//  hlay->addWidget(helpButton);
//  hlay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
//  hlay->addWidget(okButton);
//  hlay->addWidget(cancelButton);
//  // --------------------------------------------------------------------

//  // Project info
//  m_info = new ProjectInfoWidget(this);

//  QVBoxLayout* vlay = new QVBoxLayout;

//  vlay->addWidget(m_info);
//  vlay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
//  vlay->addLayout(hlay);

//  QGridLayout* grd = new QGridLayout(this);
//  grd->addLayout(vlay, 0, 0, 1, 1);
//  // --------------------------------------------------------------------

//  // Signal/Slot Connections
//  connect(okButton, SIGNAL(pressed()), SLOT(onOkPushButtonPressed()));
//  connect(cancelButton, SIGNAL(pressed()), SLOT(reject()));
//  connect(helpButton, SIGNAL(pressed()), SLOT(onHelpPushButtonPressed()));
//}

//te::qt::af::ProjectInfoDialog::~ProjectInfoDialog()
//{
//}

//void te::qt::af::ProjectInfoDialog::setProject(Project* proj)
//{
//  m_info->setProject(proj);
//}

//void te::qt::af::ProjectInfoDialog::onOkPushButtonPressed()
//{
//  m_info->updateProjectInfo();
//  QDialog::accept();
//}

//void te::qt::af::ProjectInfoDialog::onHelpPushButtonPressed()
//{
//}
