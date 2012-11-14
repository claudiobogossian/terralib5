#include "FrozenLayersSelection.h"
#include "MyLayer.h"

#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidgetItem>

FrozenLayersSelection::FrozenLayersSelection(std::list<te::map::AbstractLayer*> list, std::set<MyLayer*> lset, QWidget* parent, Qt::WindowFlags f) :
  QDialog(parent, f),
  m_list(list)
{
  setWindowTitle("Selectable layers on the map display");

  QVBoxLayout* layout = new QVBoxLayout(this);

  QSplitter* splitter = new QSplitter(this);

  QGroupBox* gb = new QGroupBox(splitter);
  gb->setMaximumWidth(90);
  QVBoxLayout* layoutb = new QVBoxLayout(gb);

  QPushButton* checkAllPushButton = new QPushButton("Check All", gb);
  QPushButton* uncheckAllPushButton = new QPushButton("Uncheck All", gb);
  QPushButton* togglePushButton = new QPushButton("Togle", gb);

  layoutb->addStretch();
  layoutb->addWidget(checkAllPushButton);
  layoutb->addStretch();
  layoutb->addWidget(uncheckAllPushButton);
  layoutb->addStretch();
  layoutb->addWidget(togglePushButton);
  layoutb->addStretch();

  std::string name, fname;
  int rows = list.size();
  int cols = 2;

  m_table = new QTableWidget(rows, cols, splitter);
  QTableWidgetItem* hitem = new QTableWidgetItem("");
  m_table->setHorizontalHeaderItem(0, hitem);
  hitem = new QTableWidgetItem("Layer Title");
  m_table->setHorizontalHeaderItem(1, hitem);

  std::list<te::map::AbstractLayer*>::iterator lit;
  std::set<MyLayer*>::iterator sit;
  int i;
  for(i = 0, lit = m_list.begin(); lit != m_list.end(); ++i, ++lit)
  {
    name = (*lit)->getTitle();
    QCheckBox* cb = new QCheckBox(m_table);
    m_table->setCellWidget(i, 0, cb);
    QTableWidgetItem* item = new QTableWidgetItem(name.c_str());
    m_table->setItem(i, 1, item);

    for(sit = lset.begin(); sit != lset.end(); ++sit)
    {
      fname = (*sit)->getTitle();
      if(name == fname)
        break;
    }
    if(sit == lset.end())
      cb->setCheckState(Qt::Checked);
  }
  m_table->adjustSize();
  m_table->resizeColumnsToContents();

  layout->addWidget(splitter);

  QGroupBox* gb1 = new QGroupBox(this);
  gb1->setMaximumHeight(50);

  QHBoxLayout* hlayout = new QHBoxLayout(gb1);
  QPushButton* okPushButton = new QPushButton("Ok", gb1);
  hlayout->addStretch();
  hlayout->addWidget(okPushButton);

  QPushButton* cancelPushButton = new QPushButton("Cancel", gb1);
  hlayout->addStretch();
  hlayout->addWidget(cancelPushButton);
  hlayout->addStretch();

  layout->addWidget(gb1);

  setLayout(layout);

  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okSlot()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));
  connect(checkAllPushButton, SIGNAL(clicked()), this, SLOT(checkAllSlot()));
  connect(uncheckAllPushButton, SIGNAL(clicked()), this, SLOT(uncheckAllSlot()));
  connect(togglePushButton, SIGNAL(clicked()), this, SLOT(toggleSlot()));
}

FrozenLayersSelection::~FrozenLayersSelection()
{
}

void FrozenLayersSelection::checkAllSlot()
{
  std::list<te::map::AbstractLayer*>::iterator lit;
  int i;
  for(i = 0, lit = m_list.begin(); lit != m_list.end(); ++i, ++lit)
  {
    QCheckBox* cb = (QCheckBox*) m_table->cellWidget(i, 0);
    if(cb->checkState() == Qt::Unchecked)
      cb->setCheckState(Qt::Checked);
  }
}

void FrozenLayersSelection::uncheckAllSlot()
{
  std::list<te::map::AbstractLayer*>::iterator lit;
  int i;
  for(i = 0, lit = m_list.begin(); lit != m_list.end(); ++i, ++lit)
  {
    QCheckBox* cb = (QCheckBox*) m_table->cellWidget(i, 0);
    if(cb->checkState() == Qt::Checked)
      cb->setCheckState(Qt::Unchecked);
  }
}

void FrozenLayersSelection::toggleSlot()
{
  std::list<te::map::AbstractLayer*>::iterator lit;
  int i;
  for(i = 0, lit = m_list.begin(); lit != m_list.end(); ++i, ++lit)
  {
    QCheckBox* cb = (QCheckBox*) m_table->cellWidget(i, 0);
    if(cb->checkState() == Qt::Unchecked)
      cb->setCheckState(Qt::Checked);
    else
      cb->setCheckState(Qt::Unchecked);
  }
}

void FrozenLayersSelection::okSlot()
{
  std::list<te::map::AbstractLayer*>::iterator lit;
  int i;
  for(i = 0, lit = m_list.begin(); lit != m_list.end(); ++i, ++lit)
  {
    MyLayer* layer = (MyLayer*)(*lit);
    QCheckBox* cb = (QCheckBox*) m_table->cellWidget(i, 0);
    if(cb->checkState() == Qt::Unchecked)
      m_layerSet.insert(layer);
  }

  accept();
  hide();
}

void FrozenLayersSelection::cancelSlot()
{
  reject();
  hide();
}

