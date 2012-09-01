#include "HighlightedInfo.h"

//! TerraLib include files
#include <terralib/qt/widgets/dataview/TabularViewer.h>
#include <terralib/qt/widgets/dataview/HLDelegateDecorator.h>

//! Qt include files
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QAction>

std::set<std::string> getIdsList(const QString& ids)
{
  std::set<std::string> idList;
  QStringList iL = ids.split(";");
  QStringList::iterator it;

  for(it=iL.begin(); it!=iL.end(); ++it)
    idList.insert((*it).toStdString());

  return idList;
}

QGroupBox* getGroupBox(HighlightedInfo* dlg, te::qt::widgets::TabularViewer* v, QLineEdit* ids, te::qt::widgets::HLDelegateDecorator* dec, int grp)
{
  if(dec == 0)
    return 0;

  te::qt::widgets::HighlightDelegate* d = dec->getDecorated(grp);

  QGroupBox* grpBox = new QGroupBox(dlg);
  QLabel* lbl = new QLabel(grpBox);
  ids->setParent(grpBox);
  QToolButton* btn = new QToolButton(grpBox);
  QAction* act = new QAction(btn);
  btn->setDefaultAction(act);

  QHBoxLayout* h_lay = new QHBoxLayout(grpBox);
  h_lay->addWidget(lbl);
  h_lay->addWidget(ids);
  h_lay->addWidget(btn);

  QString gName = d->getGroupName();

  grpBox->setTitle(gName + QObject::tr(" data configuration"));
  lbl->setText(gName + QObject::tr(" ids: "));
  act->setText(QObject::tr("Update ")+gName);
  act->setData(QVariant(grp));

  dlg->connect(btn, SIGNAL(triggered(QAction*)), SLOT(addHighlight(QAction*)));
  v->connect(dlg, SIGNAL(updateHighlight(const int&, const std::set<std::string>&)), SLOT(setHighlightObjects(const int&, const std::set<std::string>&)));

  return grpBox;
}

void makeDialog(HighlightedInfo* dlg, te::qt::widgets::TabularViewer* v, QLineEdit* qids, QLineEdit* sids)
{
//  QGroupBox* pBox = getGroupBox(dlg, v, hl_ids, HighlightedInfo::pointed);
  QGroupBox* qBox = getGroupBox(dlg, v, qids, dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(v->itemDelegate()), te::qt::widgets::TabularViewer::Query_Items);
  QGroupBox* sBox = getGroupBox(dlg, v, sids, dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(v->itemDelegate()), 3);
//  QGroupBox* pqBox = getGroupBox(dlg, v, hl_ids, HighlightedInfo::pointed_and_queried);
  QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  QVBoxLayout* v_lay = new QVBoxLayout;
//  v_lay->addWidget(pBox);
  v_lay->addWidget(qBox);
  v_lay->addWidget(sBox);
  v_lay->addItem(spacer);

  QGridLayout* grd = new QGridLayout(dlg);
  grd->addLayout(v_lay, 0, 0, 1, 1);
}

HighlightedInfo::HighlightedInfo(te::qt::widgets::TabularViewer* v, QWidget* parent) :
QWidget(parent)
{
  m_queried = new QLineEdit(this);
  m_starred = new QLineEdit(this);

  makeDialog(this, v, m_queried, m_starred);
}

void HighlightedInfo::addQueriedClicked()
{
  QString txt = m_queried->text();

  if(!txt.isEmpty())
    emit queryItems(getIdsList(txt));
}

void HighlightedInfo::addHighlight(QAction* action)
{
  int act = action->data().toInt();

  QString txt = (act == te::qt::widgets::TabularViewer::Query_Items) ? m_queried->text() : m_starred->text();

  if(!txt.isEmpty())
    emit updateHighlight(act, getIdsList(txt));
}
