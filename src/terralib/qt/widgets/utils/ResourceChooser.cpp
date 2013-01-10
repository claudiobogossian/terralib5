#include "ResourceChooser.h"

#include "FileChooser.h"

//! Qt include files
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>
#include <QDialog>

namespace te_qt = te::qt::widgets;

te_qt::ResourceChooser::ResourceChooser(QWidget* parent) :
QWidget(parent)
{
  m_fchooser = new FileChooser(this);
  QRadioButton* f = new QRadioButton(tr("File"), this);
  QRadioButton* d = new QRadioButton(tr("Directory"), this);
  QButtonGroup* grp = new QButtonGroup(this);
  QHBoxLayout* hlay = new QHBoxLayout;
  QVBoxLayout* vlay = new QVBoxLayout;

  hlay->addWidget(m_fchooser);
  vlay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
  hlay->addWidget(f);
  hlay->addWidget(d);

  vlay->addLayout(hlay);
  vlay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

  QGridLayout* grd = new QGridLayout(this);
  grd->addLayout(vlay, 0, 0, 1, 1);

  grp->addButton(f, FileChooser::FILE);
  grp->addButton(d, FileChooser::FOLDER);

  f->setChecked(true);

  connect(grp, SIGNAL(buttonClicked(int)), SLOT(setResource(int)));
}

te_qt::ResourceChooser::~ResourceChooser()
{
}

QString te_qt::ResourceChooser::getSelectedResource()
{
  return m_fchooser->getSelectedResource();
}

void te_qt::ResourceChooser::setFilterPattern(const QString& filter)
{
  m_fchooser->setFilterPattern(filter);
}

QString te_qt::ResourceChooser::getResource(const QString& initialPath, const QString& filter, QWidget* parent)
{
  QDialog dlg(parent);
  ResourceChooser* rsc = new ResourceChooser(&dlg);
  QPushButton* okBtn = new QPushButton(tr("&Ok"), &dlg);
  QPushButton* cancelBtn = new QPushButton(tr("&Cancel"), &dlg);
  QPushButton* helpBtn = new QPushButton(tr("&Help"), &dlg);
  QHBoxLayout* hlay = new QHBoxLayout;
  QVBoxLayout* vlay = new QVBoxLayout;
  QGridLayout* grd = new QGridLayout(&dlg);

  hlay->addWidget(helpBtn);
  hlay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
  hlay->addWidget(okBtn);
  hlay->addWidget(cancelBtn);

  vlay->addWidget(rsc);
  vlay->addLayout(hlay);
  grd->addLayout(vlay, 0, 0, 1, 1);

  rsc->m_fchooser->setInitialPath(initialPath);
  rsc->setFilterPattern(filter);

  dlg.setWindowTitle(tr("Choose a xml file or a directory containing xml plug-in files"));

  dlg.connect(okBtn, SIGNAL(pressed()), SLOT(accept()));
  dlg.connect(cancelBtn, SIGNAL(pressed()), SLOT(reject()));
//  dlg.connect(helpBtn, SIGNAL(), SLOT());

  if(dlg.exec() == QDialog::Accepted)
    return rsc->getSelectedResource();

  return "";
}

void te_qt::ResourceChooser::setResource(int id)
{
  m_fchooser->setResourceType((FileChooser::ResourceType)id);
}
