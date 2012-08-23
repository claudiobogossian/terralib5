#include "HighlightedInfo.h"

//! TerraLib include files
#include <terralib/qt/widgets/utils/ColorPickerToolButton.h>
#include <terralib/qt/widgets/dataview/TabularViewer.h>

//! Qt include files
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>

std::set<std::string> getIdsList(const QString& ids)
{
  std::set<std::string> idList;
  QStringList iL = ids.split(";");
  QStringList::iterator it;

  for(it=iL.begin(); it!=iL.end(); ++it)
    idList.insert((*it).toStdString());

  return idList;
}

QGroupBox* getGroupBox(HighlightedInfo* dlg, te::qt::widgets::TabularViewer* v, QLineEdit* ids, const int& grp)
{
  QGroupBox* grpBox = new QGroupBox(dlg);
  QLabel* lbl = new QLabel(grpBox);
  ids->setParent(grpBox);
  QPushButton* btn = new QPushButton(grpBox);
  //QPushButton* btn2 = new QPushButton(QObject::tr("Promote"), grpBox);
  //QPushButton* btn3 = new QPushButton(QObject::tr("Reset promote"), grpBox);
  //QPushButton* btn4 = new QPushButton(QObject::tr("Reset highlight"), grpBox);
//  te::qt::widgets::ColorPickerToolButton* cbtn = new te::qt::widgets::ColorPickerToolButton(grpBox);

  QHBoxLayout* h_lay = new QHBoxLayout(grpBox);
  h_lay->addWidget(lbl);
  h_lay->addWidget(ids);
  h_lay->addWidget(btn);
  //h_lay->addWidget(btn2);
  //h_lay->addWidget(btn3);
  //h_lay->addWidget(btn4);
//  h_lay->addWidget(cbtn);

  switch(grp)
  {
    case te::qt::widgets::TabularViewer::Point_Items:
      grpBox->setTitle(QObject::tr("Pointed data configuration"));
      lbl->setText(QObject::tr("Pointed ids: "));
      btn->setText(QObject::tr("Update pointed"));
      dlg->connect(btn, SIGNAL(clicked()), SLOT(addPointedClicked()));
//      v->connect(cbtn, SIGNAL(colorChanged(const QColor&)), SLOT(setPointedObjectsColor(const QColor&)));
      v->connect(dlg, SIGNAL(pointItems(const std::set<std::string>&)), SLOT(addPointedObjects(const std::set<std::string>&)));
      //v->connect(btn2, SIGNAL(clicked()), SLOT(promotePointed()));
      //v->connect(btn3, SIGNAL(clicked()), SLOT(resetPointedPromote()));
      //v->connect(btn4, SIGNAL(clicked()), SLOT(resetPointed()));
    break;

    case te::qt::widgets::TabularViewer::Query_Items:
      grpBox->setTitle(QObject::tr("Queried data configuration"));
      lbl->setText(QObject::tr("Queried ids: "));
      btn->setText(QObject::tr("Update"));
      dlg->connect(btn, SIGNAL(clicked()), SLOT(addQueriedClicked()));
      v->connect(dlg, SIGNAL(queryItems(const std::set<std::string>&)), SLOT(addQueriedObjects(const std::set<std::string>&)));
      //v->connect(btn2, SIGNAL(clicked()), SLOT(promoteQueried()));
      //v->connect(btn3, SIGNAL(clicked()), SLOT(resetQueriedPromote()));
      //v->connect(btn4, SIGNAL(clicked()), SLOT(resetQueried()));
    break;

    case te::qt::widgets::TabularViewer::Query_and_Point_Items:
      grpBox->setTitle(QObject::tr("Pointed / queried data configuration"));
      lbl->setText(QObject::tr("Pointed / queried ids: "));
      btn->setText(QObject::tr("Update pointed / queried"));
      dlg->connect(btn, SIGNAL(clicked()), SLOT(addPointedAndQueriedClicked()));
//      v->connect(cbtn, SIGNAL(colorChanged(const QColor&)), SLOT(setPointedAndQueriedObjectsColor(const QColor&)));
      v->connect(dlg, SIGNAL(pointAndQueryItems(const std::set<std::string>&)), SLOT(addPointedAndQueriedObjects(const std::set<std::string>&)));
      //v->connect(btn2, SIGNAL(clicked()), SLOT(promotePointedAndQueried()));
      //v->connect(btn3, SIGNAL(clicked()), SLOT(resetPointedAndQueriedPromote()));
      //v->connect(btn4, SIGNAL(clicked()), SLOT(resetPointedAndQueried()));
    break;
  };

  return grpBox;
}

void makeDialog(HighlightedInfo* dlg, te::qt::widgets::TabularViewer* v, QLineEdit* ids)
{
//  QGroupBox* pBox = getGroupBox(dlg, v, hl_ids, HighlightedInfo::pointed);
  QGroupBox* qBox = getGroupBox(dlg, v, ids, te::qt::widgets::TabularViewer::Query_Items);
//  QGroupBox* pqBox = getGroupBox(dlg, v, hl_ids, HighlightedInfo::pointed_and_queried);
  QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  QVBoxLayout* v_lay = new QVBoxLayout;
//  v_lay->addWidget(pBox);
  v_lay->addWidget(qBox);
//  v_lay->addWidget(pqBox);
  v_lay->addItem(spacer);

  QGridLayout* grd = new QGridLayout(dlg);
  grd->addLayout(v_lay, 0, 0, 1, 1);
}

HighlightedInfo::HighlightedInfo(te::qt::widgets::TabularViewer* v, QWidget* parent) :
QWidget(parent)
{
  m_queried = new QLineEdit;
  makeDialog(this, v, m_queried);
}

void HighlightedInfo::addQueriedClicked()
{
  QString txt = m_queried->text();

  if(!txt.isEmpty())
    emit queryItems(getIdsList(txt));
}
