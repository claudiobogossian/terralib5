#include "TabularViewer.h"

#include <terralib/common/Exception.h>
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/dataset/PrimaryKey.h>
#include <terralib/maptools/PromoTable.h>
#include <terralib/qt/widgets/utils/ColorPickerToolButton.h>
#include "HLDelegateDecorator.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QWidgetAction>
#include <QHeaderView>

#include <set>
#include <vector>

QMenu* makeResetHLMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QMenu* parent, std::vector<QAction*>& rst_hl_grp);

QMenu* makeGroupColorMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QMenu* mainMnu, std::vector<te::qt::widgets::ColorPickerToolButton*>& colors)
{
  te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(viewer->itemDelegate());

  if(dec == 0)
    return 0;

  size_t nGrps = dec->getNumberOfClasses();

  if(nGrps == 0)
    return 0;

  QMenu* mnu = new QMenu(QObject::tr("Set group color"), mainMnu);

  for(size_t i=0; i<nGrps; i++)
  {
    te::qt::widgets::ColorPickerToolButton* btn = new te::qt::widgets::ColorPickerToolButton(mnu);
    te::qt::widgets::HighlightDelegate* hl = dec->getDecorated(i);
    colors.push_back(btn);

    btn->setColor(hl->getHighlightColor());

    QWidgetAction* act = new QWidgetAction(mnu);
    act->setDefaultWidget(btn);
    mnu->addAction(act);
    btn->setDefaultAction(act);
    act->setText(hl->getGroupName());

    act->setData(QVariant((int)i));

    filter->connect(btn, SIGNAL(colorChanged(const QColor&)), SLOT(colorChanged(const QColor&)));
    mainMnu->connect(btn, SIGNAL(colorChanged(const QColor&)), SLOT(close()));
  }

  return mnu;
}

QMenu* makePromoteMenu(te::qt::widgets::TabularViewer* viewer, QMenu* parent, std::vector<QAction*>& acts)
{
  te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(viewer->itemDelegate());

  if(dec == 0)
    return 0;

  size_t nGrps = dec->getNumberOfClasses();

  if(nGrps == 0)
    return 0;

  std::set<size_t> grps = viewer->getPromotedGroups();

  if(nGrps == grps.size())
    return 0;

  QMenu* mnu = new QMenu(QObject::tr("Promote"), parent);

  for(size_t i=0; i<nGrps; i++)
  {
    QAction* act = new QAction(mnu);
    QString gName = dec->getDecorated(i)->getGroupName();
    act->setText(gName);
    act->setToolTip(QObject::tr("Promote the ")+gName + QObject::tr(" objects."));
    act->setData(QVariant((int)i));
    mnu->addAction(act);

    acts.push_back(act);
  }

  return mnu;
}

QMenu* makeRemovePromoteMenu(te::qt::widgets::TabularViewer* viewer, QMenu* parent, std::vector<QAction*>& acts)
{
  std::set<size_t> grps = viewer->getPromotedGroups();

  te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(viewer->itemDelegate());

  if(dec == 0)
    return 0;

  QMenu* mnu = new QMenu(QObject::tr("Reset promotion"), parent);

  for(size_t i=0; i < dec->getNumberOfClasses(); i++)
  {
    QAction* act = new QAction(mnu);
    QString gName = dec->getDecorated(i)->getGroupName();
    act->setText(gName);
    act->setToolTip(QObject::tr("Removes the promotion of the ")+gName + QObject::tr(" objects."));
    act->setData(QVariant((int)i));
    mnu->addAction(act);

    acts.push_back(act);
  }

  return mnu;
}

void makeHeaderMenu(QMenu*& mnu, QAction*& col, te::qt::widgets::TabularViewer* viewer, QObject* filter)
{
  mnu = new QMenu(viewer);

  //! Hide column action definition.
  QAction* act = new QAction(mnu);
  act->setText(QObject::tr("Hide column"));
  act->setToolTip(QObject::tr("Hides the selected column."));
  mnu->addAction(act);
  filter->connect(act, SIGNAL(triggered()), SLOT(hideCurrentColumn()));

  //! Reset columns action definition.
  act = new QAction(mnu);
  act->setText(QObject::tr("Reset columns"));
  act->setToolTip(QObject::tr("Turns all columns visible and changes to its original position."));
  mnu->addAction(act);
  viewer->connect(act, SIGNAL(triggered()), SLOT(resetColumns()));
  col = act;
}

QMenu* makeDataMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QAction*& rst_hl_all, QAction*& prm_all, QAction*& rst_prm_all, std::vector<QAction*>& prm, 
  std::vector<QAction*>& reset_prm, std::vector<QAction*>& reset_hl, std::vector<te::qt::widgets::ColorPickerToolButton*>& colors)
{
  QMenu* mnu = new QMenu(viewer);
  QAction* act = 0;

  //! Promote actions
  act = new QAction(mnu);
  act->setText(QObject::tr("Promote all"));
  act->setToolTip(QObject::tr("Promote all objects in all groups of highlighted objects."));
  mnu->addAction(act);
  viewer->connect(act, SIGNAL(triggered()), SLOT(promoteHighlighted()));
  prm_all = act;

  QMenu* prm_mnu = makePromoteMenu(viewer, mnu, prm);
  filter->connect(prm_mnu, SIGNAL(triggered(QAction*)), SLOT(promote(QAction*)));

  mnu->addMenu(prm_mnu);

  mnu->addSeparator()->setParent(mnu);

  //! Reset promote actions
  act = new QAction(mnu);
  act->setText(QObject::tr("Reset all promotion"));
  act->setToolTip(QObject::tr("Clear all promotions."));
  mnu->addAction(act);
  viewer->connect(act, SIGNAL(triggered()), SLOT(resetPromote()));

  rst_prm_all = act;

  QMenu* rst_prm_mnu = makeRemovePromoteMenu(viewer, mnu, reset_prm);
  filter->connect (rst_prm_mnu, SIGNAL(triggered(QAction*)), SLOT(resetPromote(QAction*)));

  mnu->addMenu(rst_prm_mnu);

  mnu->addSeparator()->setParent(mnu);

  mnu->addMenu(makeGroupColorMenu(viewer, filter, mnu, colors));

  mnu->addSeparator()->setParent(mnu);

  //! Reset highlights actions
  act = new QAction(mnu);
  act->setText(QObject::tr("Clear all highlighted"));
  act->setToolTip(QObject::tr("Clear all groups of highlighted objects."));
  mnu->addAction(act);
  viewer->connect(act, SIGNAL(triggered()), SLOT(resetHighlights()));

  rst_hl_all = act;

  QMenu* rst_hl_mnu = makeResetHLMenu(viewer, filter, mnu, reset_hl);

  mnu->addMenu(rst_hl_mnu);

  return mnu;
}

QMenu* makeHiddenMenu(QHeaderView* view, QMenu* parent)
{
  if(!view->sectionsHidden())
    return 0;

  QMenu* mnu = new QMenu(QObject::tr("Show column"), parent);

  for(int i=0; i<view->count(); i++)
    if(view->isSectionHidden(i))
    {
      QString cName = view->model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
      QAction* act = new QAction(mnu);

      act->setText(cName);
      act->setData(QVariant(i));
      act->setToolTip(QObject::tr("Turns column \"") + cName + "\" visible.");

      mnu->addAction(act);
    }

  return mnu;
}

std::set<size_t> getGrpsHLighted(te::qt::widgets::TabularViewer* viewer)
{
  std::set<size_t> grps;
  te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(viewer->itemDelegate());

  if(dec != 0)
  {
    size_t nC = dec->getNumberOfClasses();

    if(nC > 0)
      for(size_t i=0; i<nC; i++)
        if(!dec->getDecorated(i)->getHighlightKeys().empty())
          grps.insert(i);
  }

  return grps;
}

QMenu* makeResetHLMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QMenu* parent, std::vector<QAction*>& rst_hl_grp)
{
  std::set<size_t> hls = getGrpsHLighted(viewer);

  te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*> (viewer->itemDelegate());

  QMenu* mnu = new QMenu(QObject::tr("Clear group"), parent);

  for(size_t i=0; i<dec->getNumberOfClasses(); i++)
  {
    QString gName = dec->getDecorated(i)->getGroupName();
    QAction* act = new QAction(gName, mnu);
    act->setToolTip(QObject::tr("Removes the objects from the group of ")+gName+QObject::tr(" objects."));
    act->setData(QVariant((int)i));
    mnu->addAction(act);
    rst_hl_grp.push_back(act);
  }

  filter->connect(mnu, SIGNAL(triggered(QAction*)), SLOT(clearGroup(QAction*)));

  return mnu;
}

bool isAllPromoted(te::qt::widgets::TabularViewer* viewer)
{
  te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*> (viewer->itemDelegate());

  if(dec == 0)
    return false;

  return (viewer->getPromotedGroups().size() == dec->getNumberOfClasses());
}

std::vector<size_t> getPKeysPositions(te::da::DataSet* dset)
{
  std::vector<size_t> res;

  if(dset != 0)
  {
    te::da::DataSetType* type = dset->getType();
    te::da::PrimaryKey* pk = type->getPrimaryKey();

    if(pk != 0)
    {
      std::vector<te::dt::Property*> ps = pk->getProperties();
      std::vector<te::dt::Property*>::iterator it;

      for(it = ps.begin(); it != ps.end(); ++it)
        res.push_back(type->getPropertyPosition(*it));
    }
  }

  return res;
}

void updatePromotion(te::map::PromoTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, const std::set<size_t>& promoGrps)
{
  if(tbl == 0)
    return;

  std::vector<std::string> ids; 

  if(dec != 0)
  {
    std::set<size_t>::const_iterator it;
    std::set<std::string>::iterator sit;
    std::set<std::string> h_ids;

    for(it = promoGrps.begin(); it != promoGrps.end(); ++it)
    {
      h_ids = dec->getDecorated(*it)->getHighlightKeys();

      for(sit=h_ids.begin(); sit!=h_ids.end(); ++sit)
        ids.push_back(*sit);
    }
  }

  tbl->promoteRows(ids);
}

void addPromoted(te::map::AbstractTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, std::set<size_t>& promoGrps, const size_t& grp)
{
  if(dec == 0)
    return;

  te::map::PromoTable* tblAux = dynamic_cast<te::map::PromoTable*>(tbl);

  if(tblAux == 0)
    return;

  if(grp >= dec->getNumberOfClasses())
    throw te::common::Exception(QObject::tr("Resquested group for promotion out of group boundaries.").toStdString());

  if(promoGrps.find(grp) == promoGrps.end())
    promoGrps.insert(grp);

  updatePromotion(tblAux, dec, promoGrps);
}

void removePromoted(te::map::AbstractTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, std::set<size_t>& promoGrps, const size_t& grp)
{
  if(dec == 0)
    return;

  te::map::PromoTable* tblAux = dynamic_cast<te::map::PromoTable*>(tbl);

  if(tblAux == 0)
    return;

  if(grp >= dec->getNumberOfClasses())
    throw te::common::Exception(QObject::tr("Resquested group for promotion out of group boundaries.").toStdString());

  if(promoGrps.find(grp) == promoGrps.end())
    return;

  promoGrps.erase(grp);

  updatePromotion(tblAux, dec, promoGrps);
}

void resetPromotion(te::map::AbstractTable* tbl)
{
  te::map::PromoTable* aux = dynamic_cast<te::map::PromoTable*>(tbl);

  if(aux != 0)
    updatePromotion(aux, 0, std::set<size_t>());
}

void promoteAll(te::map::AbstractTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, std::set<size_t>& promoGrps)
{
  if(dec == 0)
    return;

  te::map::PromoTable* aux = dynamic_cast<te::map::PromoTable*>(tbl);

  if(aux != 0)
  {
    promoGrps.clear();
    for(size_t i=0; i<dec->getNumberOfClasses(); i++)
      promoGrps.insert(i);

    updatePromotion(aux, dec, promoGrps);
  }
}

void resetHeaderOrder(QHeaderView* hdr)
{
  int nCols = hdr->count();

  for(int i=0; i<nCols; i++)
  {
    int visCol = hdr->visualIndex(i);

    if(visCol != i)
      hdr->moveSection(visCol, i);
  }
}

void setHeaderHidden(QHeaderView* hdr, const bool& status)
{
  int nCols = hdr->count();

  for(int i=0; i<nCols; i++)
    hdr->setSectionHidden(i, status);
}

void resetHeader(QHeaderView* hdr)
{
  setHeaderHidden(hdr, false);
  resetHeaderOrder(hdr);
}

te::qt::widgets::HLDelegateDecorator* makeDefaultDelegate()
{
  te::qt::widgets::HLDelegateDecorator* del = (te::qt::widgets::HLDelegateDecorator*) te::qt::widgets::HLDelegateDecorator::getDelegate(3);
  del->getDecorated(te::qt::widgets::TabularViewer::Point_Items)->setGroupName(QObject::tr("Pointed"));
  del->getDecorated(te::qt::widgets::TabularViewer::Query_Items)->setGroupName(QObject::tr("Queried"));
  del->getDecorated(te::qt::widgets::TabularViewer::Query_and_Point_Items)->setGroupName(QObject::tr("Queried and pointed"));

  del->setClassColor(te::qt::widgets::TabularViewer::Point_Items, QColor(255, 0, 0));
  del->setClassColor(te::qt::widgets::TabularViewer::Query_Items, QColor(0, 255, 0));
  del->setClassColor(te::qt::widgets::TabularViewer::Query_and_Point_Items, QColor(0, 0, 255));

  return del;
}

void updateGrp(std::vector< std::set<std::string> >& hlGroups, int grpIn, int grpOut, std::string id)
{
  if(grpOut != -1)
    hlGroups[grpOut].erase(id);

  hlGroups[grpIn].insert(id);
}

std::vector< std::set<std::string> > updateHighlightGroups(const std::set<std::string>& ids, const int& hlGroup, te::qt::widgets::HLDelegateDecorator* dec)
{
  if(dec == 0)
    return std::vector< std::set<std::string> >();

  std::vector< std::set<std::string> > hlGroups;
  size_t nClasses = dec->getNumberOfClasses();
  std::set<std::string>::const_iterator it;

  for(size_t i=0; i<nClasses; i++)
    hlGroups.push_back(dec->getDecorated(i)->getHighlightKeys());

  for(it = ids.begin(); it!=ids.end(); ++it)
  {
    int grp = dec->getGroupPosition(*it);

    switch(hlGroup)
    {
      case te::qt::widgets::TabularViewer::Point_Items:
        switch(grp)
        {
          case te::qt::widgets::TabularViewer::Point_Items:
            hlGroups[grp].erase(*it);
          break;

          case te::qt::widgets::TabularViewer::Query_Items:
            hlGroups[grp].erase(*it);
            hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].insert(*it);
          break;

          case te::qt::widgets::TabularViewer::Query_and_Point_Items:
            hlGroups[grp].erase(*it);
            hlGroups[te::qt::widgets::TabularViewer::Query_Items].insert(*it);
          break;

          default:
            updateGrp(hlGroups, te::qt::widgets::TabularViewer::Point_Items, grp, *it); 
          break;
        };
      break;

      case te::qt::widgets::TabularViewer::Query_Items:
        switch(grp)
        {
          case te::qt::widgets::TabularViewer::Point_Items:
            hlGroups[grp].erase(*it);
            hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].insert(*it);
          break;

          case te::qt::widgets::TabularViewer::Query_Items:
          case te::qt::widgets::TabularViewer::Query_and_Point_Items:
          break;

          default:
            updateGrp(hlGroups, te::qt::widgets::TabularViewer::Query_Items, grp, *it); 
          break;
        };
      break;

      default:
        updateGrp(hlGroups, hlGroup, grp, *it); 
      break;
    }
  }
   
  return hlGroups;
}

std::vector< std::set<std::string> > cleanHighlight(const int& hlGroup, te::qt::widgets::HLDelegateDecorator* dec)
{
  if(dec == 0)
    return std::vector< std::set<std::string> >();

  std::vector< std::set<std::string> > hlGroups;
  size_t nClasses = dec->getNumberOfClasses();

  for(size_t i=0; i<nClasses; i++)
    hlGroups.push_back(dec->getDecorated(i)->getHighlightKeys());

  switch(hlGroup)
  {
    case te::qt::widgets::TabularViewer::Point_Items:
      hlGroups[te::qt::widgets::TabularViewer::Point_Items].clear();
      if(!hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].empty())
        hlGroups[te::qt::widgets::TabularViewer::Query_Items].insert(
        hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].begin(), hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].end());
      hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].clear();
    break;

    case te::qt::widgets::TabularViewer::Query_Items:
      hlGroups[te::qt::widgets::TabularViewer::Query_Items].clear();
      if(!hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].empty())
        hlGroups[te::qt::widgets::TabularViewer::Point_Items].insert(
        hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].begin(), hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].end());
      hlGroups[te::qt::widgets::TabularViewer::Query_and_Point_Items].clear();
    break;

    default:
      hlGroups[hlGroup].clear();
    break;
  };

  return hlGroups;
}
