#include "DataViewPopupFilter.h"

#include "TabularViewer.h"
#include "HLDelegateDecorator.h"

#include <terralib/qt/widgets/utils/ColorPickerToolButton.h>

//! Qt include files
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QWidgetAction>

QMenu* makeGroupColorMenu(te::qt::widgets::TabularViewer* viewer, QMenu* mainMnu)
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
    btn->setColor(dec->getDecorated(i)->getHighlightColor());
    btn->setText(dec->getDecorated(i)->getGroupName());

    QWidgetAction* act = new QWidgetAction(mnu);
    act->setDefaultWidget(btn);
    mnu->addAction(act);

    switch(i)
    {
      case te::qt::widgets::TabularViewer::Point_Items:
        viewer->connect(btn, SIGNAL(colorChanged(const QColor&)), SLOT(setPointedObjectsColor(const QColor&)));
      break;

      case te::qt::widgets::TabularViewer::Query_Items:
        viewer->connect(btn, SIGNAL(colorChanged(const QColor&)), SLOT(setQueriedObjectsColor(const QColor&)));
      break;

      case te::qt::widgets::TabularViewer::Query_and_Point_Items:
        viewer->connect(btn, SIGNAL(colorChanged(const QColor&)), SLOT(setPointedAndQueriedObjectsColor(const QColor&)));
      break;

      default:
      break;
    };

    mainMnu->connect(btn, SIGNAL(colorChanged(const QColor&)), SLOT(close()));
  }

  return mnu;
}

QMenu* makePromoteMenu(te::qt::widgets::TabularViewer* viewer)
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

  QMenu* mnu = new QMenu(QObject::tr("Promote"), viewer);

  for(size_t i=0; i<nGrps; i++)
    if(grps.find(i) == grps.end())
    {
      QAction* act = new QAction(mnu);
      QString gName = dec->getDecorated(i)->getGroupName();
      act->setText(gName);
      act->setToolTip(QObject::tr("Promote the ")+gName + QObject::tr(" objects."));
      act->setData(QVariant((int)i));
      mnu->addAction(act);
    }

  return mnu;
}

QMenu* makeRemovePromoteMenu(te::qt::widgets::TabularViewer* viewer)
{
  std::set<size_t> grps = viewer->getPromotedGroups();

  if(grps.empty())
    return 0;

  te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(viewer->itemDelegate());

  if(dec == 0)
    return 0;

  QMenu* mnu = new QMenu(QObject::tr("Reset promotion"), viewer);

  std::set<size_t>::iterator it;

  for(it=grps.begin(); it!=grps.end(); ++it)
  {
    QAction* act = new QAction(mnu);
    QString gName = dec->getDecorated(*it)->getGroupName();
    act->setText(gName);
    act->setToolTip(QObject::tr("Removes the promotion of the ")+gName + QObject::tr(" objects."));
    act->setData(QVariant((int)*it));
    mnu->addAction(act);
  }

  return mnu;
}

void makeHeaderMenu(QMenu*& mnu, QAction*& col, te::qt::widgets::TabularViewer* viewer, te::qt::widgets::DataViewPopupFilter* filter)
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

QMenu* makeDataMenu(te::qt::widgets::TabularViewer* viewer, QAction*& rset_hl, QAction*& prmAll, QAction*& reset_prmAll, QAction*& prm, QAction*& reset_prm)
{
  QMenu* mnu = new QMenu(viewer);

  //! Promote actions
  QAction* act = new QAction(mnu);
  act->setText(QObject::tr("Promote all"));
  act->setToolTip(QObject::tr("Promote all objects in all groups of highlighted objects."));
  mnu->addAction(act);
  viewer->connect(act, SIGNAL(triggered()), SLOT(promoteHighlighted()));

  prmAll = act;
  prm = mnu->addSeparator();

  //! Reset promote actions
  act = new QAction(mnu);
  act->setText(QObject::tr("Reset all promotion"));
  act->setToolTip(QObject::tr("Clear all promotions."));
  mnu->addAction(act);
  viewer->connect(act, SIGNAL(triggered()), SLOT(resetPromote()));

  reset_prmAll = act;
  reset_prm = mnu->addSeparator();

  mnu->addMenu(makeGroupColorMenu(viewer, mnu));

  mnu->addSeparator();

  //! Reset highlights actions
  act = new QAction(mnu);
  act->setText(QObject::tr("Clear all highlighted"));
  act->setToolTip(QObject::tr("Clear all groups of highlighted objects."));
  mnu->addAction(act);
  viewer->connect(act, SIGNAL(triggered()), SLOT(resetHighlights()));

  rset_hl = act;

  return mnu;
}

QMenu* makeHiddenMenu(QHeaderView* view)
{
  if(!view->sectionsHidden())
    return 0;

  QMenu* mnu = new QMenu(QObject::tr("Show column"));

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

std::vector<size_t> getGrpsHLighted(te::qt::widgets::TabularViewer* viewer)
{
  std::vector<size_t> grps;
  te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*>(viewer->itemDelegate());

  if(dec != 0)
  {
    size_t nC = dec->getNumberOfClasses();

    if(nC > 0)
      for(size_t i=0; i<nC; i++)
        if(!dec->getDecorated(i)->getHighlightKeys().empty())
          grps.push_back(i);
  }

  return grps;
}

QMenu* makeResetHLMenu(te::qt::widgets::TabularViewer* viewer, te::qt::widgets::DataViewPopupFilter* filter)
{
  std::vector<size_t> hls = getGrpsHLighted(viewer);
  
  if(hls.empty())
    return 0;

  QMenu* mnu = new QMenu(QObject::tr("Clear group"));

  for(size_t i=0; i<hls.size(); i++)
  {
    QString gName = ((te::qt::widgets::HLDelegateDecorator*)viewer->itemDelegate())->getDecorated(hls[i])->getGroupName();
    QAction* act = new QAction(gName, mnu);
    act->setToolTip(QObject::tr("Removes the objects from the group of ")+gName+QObject::tr(" objects."));
    act->setData(QVariant((int)hls[i]));
    mnu->addAction(act);
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

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      DataViewPopupFilter::DataViewPopupFilter(TabularViewer* viewer) :
      QObject(viewer),
      m_viewer(viewer),
      m_col(-1),
      m_hiddenCols(0),
      m_setPrm(0),
      m_resetPrm(0),
      m_rsetHL(0)
      {
        m_viewer->horizontalHeader()->installEventFilter(this);
        m_viewer->viewport()->installEventFilter(this);

        makeHeaderMenu(m_head_menu, m_colGroup, m_viewer, this);
        m_data_menu = makeDataMenu(m_viewer, m_rset_all, m_prm_all, m_reset_prm_all, m_prmGroup, m_resetPrmGroup);

        m_viewer->connect(this, SIGNAL(hideColumn(int)), SLOT(hideColumn(int)));
        m_viewer->connect(this, SIGNAL(showColumn(int)), SLOT(showColumn(int)));

        m_viewer->connect(this, SIGNAL(promotePointed()), SLOT(promotePointed()));
        m_viewer->connect(this, SIGNAL(promoteQueried()), SLOT(promoteQueried()));
        m_viewer->connect(this, SIGNAL(promotePointedAndQueried()), SLOT(promotePointedAndQueried()));

        m_viewer->connect(this, SIGNAL(resetPointedPromotion()), SLOT(resetPointedPromote()));
        m_viewer->connect(this, SIGNAL(resetQueriedPromotion()), SLOT(resetQueriedPromote()));
        m_viewer->connect(this, SIGNAL(resetPointedAndQueriedPromotion()), SLOT(resetPointedAndQueriedPromote()));

        m_viewer->connect(this, SIGNAL(resetPointed()), SLOT(resetPointed()));
        m_viewer->connect(this, SIGNAL(resetQueried()), SLOT(resetQueried()));
        m_viewer->connect(this, SIGNAL(resetPointedAndQueried()), SLOT(resetPointedAndQueried()));

        m_viewer->connect(this, SIGNAL(addPointed(const std::vector<int>&)), SLOT(addPointedObjects(const std::vector<int>&)));
      }

      DataViewPopupFilter::~DataViewPopupFilter()
      {
      }

      bool DataViewPopupFilter::eventFilter(QObject* watched, QEvent* event)
      {
        QWidget* vport = m_viewer->viewport();
        QHeaderView* hdr = m_viewer->horizontalHeader();

        if(watched == vport)
        {
          switch(event->type())
          {
            case QEvent::ContextMenu:
              {
                m_prm_all->setEnabled(!isAllPromoted(m_viewer));
                m_reset_prm_all->setEnabled(!m_viewer->getPromotedGroups().empty());
                m_rset_all->setEnabled(!getGrpsHLighted(m_viewer).empty());

                //! Promote menus
                if(m_setPrm != 0)
                  delete m_setPrm;

                m_setPrm = makePromoteMenu(m_viewer);

                if(m_setPrm != 0)
                {
                  m_data_menu->insertMenu(m_prmGroup, m_setPrm);
                  connect(m_setPrm, SIGNAL(triggered(QAction*)), SLOT(promote(QAction*)));
                }

                //! Remove promoted menu.
                if(m_resetPrm != 0)
                  delete m_resetPrm;

                m_resetPrm = makeRemovePromoteMenu(m_viewer);

                if(m_resetPrm != 0)
                {
                  m_data_menu->insertMenu(m_resetPrmGroup, m_resetPrm);
                  connect (m_resetPrm, SIGNAL(triggered(QAction*)), SLOT(resetPromote(QAction*)));
                }

                //! Remove highlights menu.
                if(m_rsetHL != 0)
                  delete m_rsetHL;

                m_rsetHL = makeResetHLMenu(m_viewer, this);

                if(m_rsetHL != 0)
                  m_data_menu->addMenu(m_rsetHL);

                QContextMenuEvent* evt = static_cast<QContextMenuEvent*>(event);
                m_data_menu->popup(evt->globalPos());

                return true;
              }
            break;

            case QEvent::MouseButtonPress:
              {
                QMouseEvent* evt = static_cast<QMouseEvent*>(event);

                if(evt->button() == Qt::LeftButton)
                {
                  if(m_viewer->model()==0 || m_viewer->model()->rowCount() <= 0)
                    return false;

                  int row = m_viewer->rowAt(evt->pos().y());
                  emit addPointed(std::vector<int>(1, row));

                  return true;
                }
              }
            break;

            default:
              return QObject::eventFilter(watched, event);
            break;
          };
        }
        else if(watched == hdr)
        {
          switch(event->type())
          {
            case QEvent::ContextMenu:
              {
                QContextMenuEvent* evt = static_cast<QContextMenuEvent*>(event);
                QPoint pos = evt->globalPos();
                m_col = hdr->logicalIndex(hdr->visualIndexAt(evt->pos().x()));
            
                //! Hidden popup menu.
                if(m_hiddenCols != 0)
                  delete m_hiddenCols;

                m_hiddenCols = makeHiddenMenu(hdr);

                if(m_hiddenCols != 0)
                {
                  m_head_menu->insertMenu(m_colGroup, m_hiddenCols);
                  connect (m_hiddenCols, SIGNAL(triggered(QAction*)), SLOT(showColumn(QAction*)));
                }

                m_head_menu->popup(pos);

                return true;
              }
            break;

            default:
              return QObject::eventFilter(watched, event);
            break;
          };
        }

        return QObject::eventFilter(watched, event);
      }

      QMenu* DataViewPopupFilter::getHeaderMenu()
      {
        return m_head_menu;
      }

      QMenu* DataViewPopupFilter::getDataMenu()
      {
        return m_data_menu;
      }

      void DataViewPopupFilter::hideCurrentColumn()
      {
        if(m_col != -1)
        {
          emit hideColumn(m_col);
          m_col = -1;
        }
      }

      void DataViewPopupFilter::showColumn(QAction* act)
      {
        int col = act->data().toInt();
        emit showColumn(col);
      }

      void DataViewPopupFilter::promote(QAction* act)
      {
        int grp = act->data().toInt();

        switch(grp)
        {
          case te::qt::widgets::TabularViewer::Point_Items:
            emit promotePointed();
          break;

          case te::qt::widgets::TabularViewer::Query_Items:
            emit promoteQueried();
          break;

          case te::qt::widgets::TabularViewer::Query_and_Point_Items:
            emit promotePointedAndQueried();
          break;

          default:
          break;
        };
      }

      void DataViewPopupFilter::resetPromote(QAction* act)
      {
        int grp = act->data().toInt();

        switch(grp)
        {
          case te::qt::widgets::TabularViewer::Point_Items:
            emit resetPointedPromotion();
          break;

          case te::qt::widgets::TabularViewer::Query_Items:
            emit resetQueriedPromotion();
          break;

          case te::qt::widgets::TabularViewer::Query_and_Point_Items:
            emit resetPointedAndQueriedPromotion();
          break;

          default:
          break;
        };
      }

      void DataViewPopupFilter::clearGroup(QAction* act)
      {
        int grp = act->data().toInt();

        switch(grp)
        {
          case te::qt::widgets::TabularViewer::Point_Items:
            emit resetPointed();
          break;

          case te::qt::widgets::TabularViewer::Query_Items:
            emit resetQueried();
          break;

          case te::qt::widgets::TabularViewer::Query_and_Point_Items:
            emit resetPointedAndQueried();
          break;

          default:
          break;
        };
      }
    }
  }
}