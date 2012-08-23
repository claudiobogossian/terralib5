#include "TabularViewer.h"

#include "DataSetModel.h"
#include "HLDelegateDecorator.h"
#include "DataViewPopupFilter.h"

//TerraLib MapTools include files
#include <terralib/maptools/PromoTable.h>
#include <terralib/maptools/DataSetTable.h>
#include <terralib/common/Exception.h>

//TerraLib DataSet include files
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/dataset/PrimaryKey.h>

//! Qt include files
#include <QHeaderView>

//! Function for getting the primary keys positions.
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
  std::set<size_t>::const_iterator it;
  std::set<std::string>::iterator sit;
  std::set<std::string> h_ids;
  std::vector<std::string> ids; 

  for(it = promoGrps.begin(); it != promoGrps.end(); ++it)
  {
    h_ids = dec->getDecorated(*it)->getHighlightKeys();

    for(sit=h_ids.begin(); sit!=h_ids.end(); ++sit)
      ids.push_back(*sit);
  }

  tbl->promoteRows(ids);
}

void addPromoted(te::map::AbstractTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, std::set<size_t>& promoGrps, const size_t& grp)
{
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

std::vector< std::set<std::string> > updateHighlightGroups(const std::set<std::string>& ids, const int& hlGroup, te::qt::widgets::HLDelegateDecorator* dec)
{
  std::vector< std::set<std::string> > hlGroups;
  size_t nClasses = dec->getNumberOfClasses();
  std::set<std::string>::const_iterator it;

  for(size_t i=0; i<nClasses; i++)
    hlGroups.push_back(dec->getDecorated(i)->getHighlightKeys());

  for(it = ids.begin(); it!=ids.end(); ++it)
  {
    size_t grp = dec->getGroupPosition(*it);

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
            hlGroups[te::qt::widgets::TabularViewer::Point_Items].insert(*it);
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
            hlGroups[te::qt::widgets::TabularViewer::Query_Items].insert(*it);
          break;
        };
      break;
    }
  }
   
  return hlGroups;
}

std::vector< std::set<std::string> > cleanHighlight(const int& hlGroup, te::qt::widgets::HLDelegateDecorator* dec)
{
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

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      TabularViewer::TabularViewer(QWidget* parent) :
      QTableView(parent)
      {
        m_model = 0;
        m_table = 0;
        m_delegate = makeDefaultDelegate();

        m_delegate->setParent(this);

        QTableView::setItemDelegate(m_delegate);
        QTableView::horizontalHeader()->setMovable(true);
        QTableView::setSelectionMode(QAbstractItemView::NoSelection);

        m_menu_filter = new DataViewPopupFilter(this);
      }

      TabularViewer::~TabularViewer()
      {
      }

      std::set<size_t> TabularViewer::getPromotedGroups() const
      {
        return m_promotedGroups;
      }

      void TabularViewer::showData(te::da::DataSet* dset)
      {
        //Making PromoTable
        te::map::DataSetTable* tbl = new te::map::DataSetTable(dset);
        te::map::PromoTable* tbl2 = new te::map::PromoTable(tbl);

        //Finding primary keys
        std::vector<size_t> pks = getPKeysPositions(dset);
        tbl2->setPKeysColumns(pks);

        showData(tbl2);
      }

      void TabularViewer::showData(te::map::AbstractTable* table)
      {
        table->setGeoColumnsVisible(false);
        te::qt::widgets::DataSetModel* model = new te::qt::widgets::DataSetModel(table, this);

        QItemSelectionModel* m = QTableView::selectionModel();
        QTableView::setModel(model);
        QTableView::setSortingEnabled(false);

        //Reseting params
        delete m;
        delete m_model;
        m_model = model;
        m_table = table;
        m_promotedGroups.clear();

        setPrimaryKeys(m_table->getPKeysColumns());

        resetColumns();
      }

      void TabularViewer::setPrimaryKeys(const std::vector<size_t>& pkeys)
      {
        m_delegate->clearSelection();

        m_delegate->setPKeysColumns(pkeys);
        m_table->setPKeysColumns(pkeys);
        m_table->cleanPreprocessPKeys();

        resetPromotion(m_table);

        std::vector<size_t>::const_iterator it;
        QHeaderView* hdr = QTableView::horizontalHeader();

        for(it=pkeys.begin(); it!=pkeys.end(); ++it)
         hdr->resizeSection((int)*it, hdr->sectionSizeHint((int)*it));
      }

      std::vector<size_t> TabularViewer::getPrimaryKeys() const
      {
        return m_table->getPKeysColumns();
      }

      void TabularViewer::setPointedObjectsColor(const QColor& c)
      {
        setHighlightColor(Point_Items, c);
      }

      void TabularViewer::addPointedObjects(const std::set<std::string>& objIds)
      {
        setHighlightObjects(Point_Items, objIds);
      }

      void TabularViewer::addPointedObjects(const std::vector<int>& rows)
      {
        std::vector<int>::const_iterator it;
        std::set<std::string> ids;

        for(it=rows.begin(); it!=rows.end(); ++it)
        {
          QModelIndex idx = m_model->index(*it, 0);

          if(!idx.isValid())
            throw te::common::Exception(tr("Invalid index of row.").toLatin1().data());

          ids.insert(m_model->data(idx, DataSetModel::PKEY).toString().toLatin1().data());
        }

        addPointedObjects(ids);
      }

      void TabularViewer::setQueriedObjectsColor(const QColor& c)
      {
        setHighlightColor(Query_Items, c);
      }

      void TabularViewer::addQueriedObjects(const std::set<std::string>& objIds)
      {
        setHighlightObjects(Query_Items, objIds);
      }

      void TabularViewer::setPointedAndQueriedObjectsColor(const QColor& c)
      {
        setHighlightColor(Query_and_Point_Items, c);
      }

      void TabularViewer::resetHighlights()
      {
        m_delegate->clearSelection();
        resetPromotion(m_table);

        QTableView::viewport()->update();
      }

      void TabularViewer::resetPointed()
      {
        clearHighlighted(Point_Items);
      }

      void TabularViewer::resetQueried()
      {
        clearHighlighted(Query_Items);
      }

      void TabularViewer::resetPointedAndQueried()
      {
        clearHighlighted(Query_and_Point_Items);
      }

      void TabularViewer::setPromoteEnabled(const bool& enable)
      {
        if(enable) 
          m_table->preprocessPKeys();
        else
        {
          m_table->cleanPreprocessPKeys();
          resetPromotion(m_table);

          QTableView::viewport()->update();
        }
      }

      void TabularViewer::promoteHighlighted()
      {
        setPromoteEnabled(true);
        promoteAll(m_table, m_delegate, m_promotedGroups);

        QTableView::viewport()->update();
        QTableView::scrollToTop();
      }

      void TabularViewer::promotePointed()
      {
        addPromoteHighlight(Point_Items);
      }

      void TabularViewer::promoteQueried()
      {
        addPromoteHighlight(Query_Items);
      }

      void TabularViewer::promotePointedAndQueried()
      {
        addPromoteHighlight(Query_and_Point_Items);
      }

      void TabularViewer::resetPromote()
      {
        m_promotedGroups.clear();
        resetPromotion(m_table);

        QTableView::viewport()->update();
      }

      void TabularViewer::resetPointedPromote()
      {
        resetPromoteHighlight(Point_Items);
      }

      void TabularViewer::resetQueriedPromote()
      {
        resetPromoteHighlight(Query_Items);
      }

      void TabularViewer::resetPointedAndQueriedPromote()
      {
        resetPromoteHighlight(Query_and_Point_Items);
      }

      QMenu* TabularViewer::getMenu(const MenuType& type)
      {
        return (type == Header) ? m_menu_filter->getHeaderMenu() : m_menu_filter->getDataMenu();
      }

      void TabularViewer::resetColumns()
      {
        resetHeader(QTableView::horizontalHeader());
      }

      void TabularViewer::setHighlightObjects(const HLGroups& g, const std::set<std::string>& ids)
      {
        std::vector< std::set<std::string> > hls = updateHighlightGroups(ids, g, m_delegate);

        for(size_t i=0; i<hls.size(); i++)
          m_delegate->setClassSelection(i, hls[i]);

        if(m_promotedGroups.find(g) != m_promotedGroups.end() && !ids.empty())
          addPromoteHighlight(g);

        QTableView::viewport()->update();
      }

      void TabularViewer::clearHighlighted(const HLGroups& g)
      {
        std::vector< std::set<std::string> > hls = cleanHighlight(g, m_delegate);

        for(size_t i=0; i<hls.size(); i++)
          m_delegate->setClassSelection(i, hls[i]);

        QTableView::viewport()->update();
      }

      void TabularViewer::setHighlightColor(const HLGroups& g, const QColor& c)
      {
        m_delegate->setClassColor(g, c);

        QTableView::viewport()->update();
      }

      void TabularViewer::resetPromoteHighlight(const HLGroups& g)
      {
        removePromoted(m_table, m_delegate, m_promotedGroups, (size_t)g);
        QTableView::viewport()->update();
      }

      void TabularViewer::addPromoteHighlight(const HLGroups& g)
      {
        setPromoteEnabled(true);
        addPromoted(m_table, m_delegate, m_promotedGroups, g);
        QTableView::scrollToTop();
      }
    }
  }
}
