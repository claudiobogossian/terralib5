#include "Styler.h"

// TerraLib
#include <terralib/maptools/AbstractLayer.h>
#include <terralib/qt/widgets/se/StyleDockWidget.h>

// Qt
#include <QPushButton>
#include <QGridLayout>

Styler::Styler(te::map::AbstractLayer* l, QWidget* parent) :
QDialog(parent)
{
  initDialog();

  m_explorer->setStyle(l->getStyle(), l);
}

void Styler::initDialog()
{
  setWindowTitle(tr("Change Layer Style"));

  QPushButton* btn = new QPushButton(tr("Close"), this);
  QHBoxLayout* hL = new QHBoxLayout; 

  hL->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
  hL->addWidget(btn);

  m_explorer = new te::qt::widgets::StyleDockWidget(this);

  QVBoxLayout* vL = new QVBoxLayout;

  vL->addWidget(m_explorer);
  vL->addLayout(hL);

  QGridLayout* grd = new QGridLayout(this);
  grd->addLayout(vL, 0, 0);

  connect(btn, SIGNAL(released()), SLOT(close()));
}
