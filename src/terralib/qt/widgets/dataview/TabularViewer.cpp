#include "TabularViewer.h"

#include "DataSetModel.h"
#include "HLDelegateDecorator.h"
#include <terralib/qt/widgets/utils/ColorPickerToolButton.h>

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
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

//! STL include files
#include <vector>

////! Forward declarations
QMenu* makeGroupColorMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QMenu* mainMnu);

QMenu* makePromoteMenu(te::qt::widgets::TabularViewer* viewer, QMenu* parent);

QMenu* makeRemovePromoteMenu(te::qt::widgets::TabularViewer* viewer, QMenu* parent);

void makeHeaderMenu(QMenu*& mnu, QAction*& col, te::qt::widgets::TabularViewer* viewer, QObject* filter);

QMenu* makeDataMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QAction*& rset_hl, QAction*& prmAll, 
  QAction*& reset_prmAll, QAction*& prm, QAction*& reset_prm);

QMenu* makeHiddenMenu(QHeaderView* view, QMenu* parent);

std::vector<size_t> getGrpsHLighted(te::qt::widgets::TabularViewer* viewer);

QMenu* makeResetHLMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QMenu* parent);

bool isAllPromoted(te::qt::widgets::TabularViewer* viewer);

std::vector<size_t> getPKeysPositions(te::da::DataSet* dset);

void updatePromotion(te::map::PromoTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, const std::set<size_t>& promoGrps);

void addPromoted(te::map::AbstractTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, std::set<size_t>& promoGrps, const size_t& grp);

void removePromoted(te::map::AbstractTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, std::set<size_t>& promoGrps, const size_t& grp);

void resetPromotion(te::map::AbstractTable* tbl);

void promoteAll(te::map::AbstractTable* tbl, te::qt::widgets::HLDelegateDecorator* dec, std::set<size_t>& promoGrps);

void resetHeaderOrder(QHeaderView* hdr);

void setHeaderHidden(QHeaderView* hdr, const bool& status);

void resetHeader(QHeaderView* hdr);

te::qt::widgets::HLDelegateDecorator* makeDefaultDelegate();

std::vector< std::set<std::string> > updateHighlightGroups(const std::set<std::string>& ids, const int& hlGroup, te::qt::widgets::HLDelegateDecorator* dec);

std::vector< std::set<std::string> > cleanHighlight(const int& hlGroup, te::qt::widgets::HLDelegateDecorator* dec);



//! DataViewPopupFilter definition
//! ----------------------------------------------------------------

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class DataViewPopupFilter
        \brief Receives the te::qt::widgets::TabularViewer objects events and handles if they are context menu events or mouse clicked. 
        \details This class receives events of context menu and mouse clicks and reacts to them. Popup menus are presented in context
        case, if required, on header or data. Mouse left button clicked adds the pointed row to the "Pointed" group. 
        Another task is to highlight objects that are clicked (with left mouse button).
        \sa te::map::PromoTable, te::qt::widgets::TabularViewer
      */
      class DataViewPopupFilter : public QObject
      {
        Q_OBJECT

      public:
        
        /*!
          \brief Contructor.
          \param viewier The te::qt::qwidgets::TabularViewer to be observed.
        */
        DataViewPopupFilter(TabularViewer* viewer) :
        QObject(viewer),
        m_viewer(viewer),
        m_col(-1),
        m_hiddenCols(0),
        m_setPrm(0),
        m_resetPrm(0),
        m_rsetHL(0),
        m_data_menu(0),
        m_head_menu(0)
        {
          m_viewer->horizontalHeader()->installEventFilter(this);
          m_viewer->viewport()->installEventFilter(this);

          setupMenus();

          m_viewer->connect(this, SIGNAL(hideColumn(int)), SLOT(hideColumn(int)));
          m_viewer->connect(this, SIGNAL(showColumn(int)), SLOT(showColumn(int)));
          m_viewer->connect(this, SIGNAL(updateColor(const int&, const QColor&)), SLOT(setHighlightColor(const int&, const QColor&)));
          m_viewer->connect(this, SIGNAL(highlightObjects(const std::vector<int>&)), SLOT(pointObjects(const std::vector<int>&)));
          m_viewer->connect(this, SIGNAL(promote(const int&)), SLOT(addPromoteHighlight(const int&)));
          m_viewer->connect(this, SIGNAL(resetGroupPromotion(const int&)), SLOT(resetPromoteHighlight(const int&)));
        }

        /*!
          \brief Destructor.
        */
        ~DataViewPopupFilter()
        {
        }

        /*!
          \brief Construct menus.
          \details Re-construct the header and data menus. This method MUST be called when item delegate was updated.
        */
        void setupMenus()
        {
          if(m_head_menu != 0)
          {
            m_head_menu->clear();
            delete m_head_menu;
          }
        
          if(m_data_menu != 0)
          {
            m_data_menu->clear();
            delete m_data_menu;
          }

          makeHeaderMenu(m_head_menu, m_colGroup, m_viewer, this);
          m_data_menu = makeDataMenu(m_viewer, this, m_rset_all, m_prm_all, m_reset_prm_all, m_prmGroup, m_resetPrmGroup);
        }

        /*!
          \brief Reimplementation of QObject method.
          \details Three events are handled:
          <ol>
            <li>Header context menu required.</li>
            <li>Data context menu required.</li>
            <li>Point data (mouse left button clicked on a row).</li>
          </ol>
        */
        bool eventFilter(QObject* watched, QEvent* event)
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

                  m_setPrm = makePromoteMenu(m_viewer, m_data_menu);

                  if(m_setPrm != 0)
                  {
                    m_data_menu->insertMenu(m_prmGroup, m_setPrm);
                    connect(m_setPrm, SIGNAL(triggered(QAction*)), SLOT(promote(QAction*)));
                  }

                  //! Remove promoted menu.
                  if(m_resetPrm != 0)
                    delete m_resetPrm;

                  m_resetPrm = makeRemovePromoteMenu(m_viewer, m_data_menu);

                  if(m_resetPrm != 0)
                  {
                    m_data_menu->insertMenu(m_resetPrmGroup, m_resetPrm);
                    connect (m_resetPrm, SIGNAL(triggered(QAction*)), SLOT(resetPromote(QAction*)));
                  }

                  //! Remove highlights menu.
                  if(m_rsetHL != 0)
                    delete m_rsetHL;

                  m_rsetHL = makeResetHLMenu(m_viewer, this, m_data_menu);

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
                    emit highlightObjects(std::vector<int>(1, row));

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

                  m_hiddenCols = makeHiddenMenu(hdr, m_head_menu);

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

        /*!
          \brief Returns the pointer to the header menu.
          \return The popup menu of the header.
        */
        QMenu* getHeaderMenu()
        {
          return m_head_menu;
        }

        /*!
          \brief Returns the pointer to the data menu.
          \return The popup menu of data.
        */
        QMenu* getDataMenu()
        {
          return m_data_menu;
        }

      protected slots:

        /*!
          \name Popup actions.
          \details Actions executed by the popup menus.
        */
        //@{

        /*!
          \brief Hides the column that where selected.
        */
        void hideCurrentColumn()
        {
          if(m_col != -1)
          {
            emit hideColumn(m_col);
            m_col = -1;
          }
        }

        /*!
          \brief Shows the selected column.
          \param act The QAction triggered. It carries the position of column to be showed.
        */
        void showColumn(QAction* act)
        {
          int col = act->data().toInt();
          emit showColumn(col);
        }

        /*!
          \brief Promotes a group of highlighted itens.
          \param act The QAction triggered. It carries the group of highlighted to be promoted.
          \note Promotion has additional costs. See documentations to learn details about that.
        */
        void promote(QAction* act)
        {
          int grp = act->data().toInt();
          emit promote(grp);
        }

        /*!
          \brief Resets the promotion of some group.
          \param act The QAction triggered. It carries the group of highlighted to reset the promotion.
        */
        void resetPromote(QAction* act)
        {
          int grp = act->data().toInt();
          emit resetGroupPromotion(grp);
        }

        /*!
          \brief Removes objects references of some group of highlighted.
          \param act The QAction triggered. It carries the group of highlighted to be cleared.
        */
        void clearGroup(QAction* act)
        {
          int grp = act->data().toInt();
          emit resetHighlight(grp);
        }

        /*!
          \brief Updates the color of the group.
          \param c New color of the group.
        */
        void colorChanged(const QColor& c)
        {
          ColorPickerToolButton* obj = (ColorPickerToolButton*) QObject::sender();
          emit updateColor(obj->defaultAction()->data().toInt(), c);
        }
        //@}

      signals:

        /*!
          \name Signals.
          \brief Signals emmited by popup actions.
          \note Columns positions are ALWAYS logical. For details about logical an visual positions see QHeaderView documentation.
        */
        //@{

        /*!
          \brief Emitted to hide column \a column.
          \param column Logical column position.
        */
        void hideColumn(int column);

        /*!
          \brief Emitted to show column \a column.
          \param column Logical column position.
        */
        void showColumn(int column);

        /*!
          \brief Emitted to promote only a group.
        */
        void promote(const int&);

        /*!
          \brief Emitted to reset all promotion.
        */
        void resetPromotion();

        /*!
          \brief Emitted to reset only promotion of a group.
        */
        void resetGroupPromotion(const int&);

        /*!
          \brief Emitted to add rows to the pointed group.
        */
        void highlightObjects(const std::vector<int>&);

        /*!
          \brief Emitted to update color of a group.
        */
        void updateColor(const int&, const QColor&);

        /*!
          \brief Emitted to remove only objects of a group.
        */
        void resetHighlight(const int&);

        /*!
          \brief Emitted remove objects of all highlight groups.
        */
        void resetAll();
        //@}

      protected:

        TabularViewer* m_viewer;    //!< Pointer to viewer that is being observed.

        QMenu* m_head_menu;         //!< Header menu.                     
        QMenu* m_data_menu;         //!< Data menu.
        QMenu* m_hiddenCols;        //!< Hide columns menu.
        QMenu* m_resetPrm;          //!< Reset promoted groups menu.
        QMenu* m_setPrm;            //!< Set promoted groups menu.
        QMenu* m_rsetHL;            //!< Reset group highlights menu.
        
        QAction* m_colGroup;        //!< Separator of columns menu.
        QAction* m_prmGroup;        //!< Separator of promote menu.
        QAction* m_resetPrmGroup;   //!< Separator of reset promote menu.
        QAction* m_prm_all;         //!< Promote all action.
        QAction* m_reset_prm_all;   //!< Reset all promotion action.
        QAction* m_rset_all;        //!< Reset all highlighted action.

        int m_col;                  //!< Visual column position.
      };
    }
  }
}

//! end DataViewPopupFilter definition
//! ----------------------------------------------------------------


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
        QAbstractItemDelegate* del = makeDefaultDelegate();
        QAbstractItemDelegate* del_old = QTableView::itemDelegate();

        del->setParent(this);

        QTableView::setItemDelegate(del);
        QTableView::horizontalHeader()->setMovable(true);
        QTableView::setSelectionMode(QAbstractItemView::NoSelection);

        m_menu_filter = new DataViewPopupFilter(this);

        if(del_old != 0)
          delete del_old;
      }

      TabularViewer::~TabularViewer()
      {
      }

      void TabularViewer::updatePopupMenus()
      {
        m_menu_filter->setupMenus();
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
        HighlightDelegate* del = dynamic_cast<HighlightDelegate*>(QTableView::itemDelegate());

        if(del != 0)
        {
          del->clearSelection();
          del->setPKeysColumns(pkeys);
        }

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

      void TabularViewer::pointObjects(const std::vector<int>& rows)
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

        setHighlightObjects(Point_Items, ids);
      }

      void TabularViewer::resetHighlights()
      {
        HighlightDelegate* del = dynamic_cast<HighlightDelegate*>(QTableView::itemDelegate());

        if(del != 0)
          del->clearSelection();

        resetPromotion(m_table);

        QTableView::viewport()->update();
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
        promoteAll(m_table, dynamic_cast<HLDelegateDecorator*>(QTableView::itemDelegate()), m_promotedGroups);

        QTableView::viewport()->update();
        QTableView::scrollToTop();
      }

      void TabularViewer::resetPromote()
      {
        m_promotedGroups.clear();
        resetPromotion(m_table);

        QTableView::viewport()->update();
      }

      void TabularViewer::setHighlightObjects(const int& g, const std::set<std::string>& ids)
      {
        HLDelegateDecorator* del = dynamic_cast<HLDelegateDecorator*>(QTableView::itemDelegate());

        std::vector< std::set<std::string> > hls = updateHighlightGroups(ids, g, del);

        if(del != 0)
          for(size_t i=0; i<hls.size(); i++)
            del->setClassSelection(i, hls[i]);

        updatePromotion(dynamic_cast<te::map::PromoTable*>(m_table), del, m_promotedGroups);

        QTableView::viewport()->update();
      }

      void TabularViewer::clearHighlighted(const int& g)
      {
        HLDelegateDecorator* del = dynamic_cast<HLDelegateDecorator*>(QTableView::itemDelegate());
        std::vector< std::set<std::string> > hls = cleanHighlight(g, del);

        if(del != 0)
          for(size_t i=0; i<hls.size(); i++)
            del->setClassSelection(i, hls[i]);

        QTableView::viewport()->update();
      }

      void TabularViewer::setHighlightColor(const int& g, const QColor& c)
      {
        HLDelegateDecorator* del = dynamic_cast<HLDelegateDecorator*>(QTableView::itemDelegate());

        if(del != 0)
          del->setClassColor(g, c);

        QTableView::viewport()->update();
      }

      void TabularViewer::resetPromoteHighlight(const int& g)
      {
        removePromoted(m_table, dynamic_cast<HLDelegateDecorator*>(QTableView::itemDelegate()), m_promotedGroups, (size_t)g);
        QTableView::viewport()->update();
      }

      void TabularViewer::addPromoteHighlight(const int& g)
      {
        setPromoteEnabled(true);
        addPromoted(m_table, dynamic_cast<HLDelegateDecorator*>(QTableView::itemDelegate()), m_promotedGroups, g);
        QTableView::scrollToTop();
      }

      QMenu* TabularViewer::getMenu(const MenuType& type)
      {
        return (type == Header) ? m_menu_filter->getHeaderMenu() : m_menu_filter->getDataMenu();
      }

      void TabularViewer::resetColumns()
      {
        resetHeader(QTableView::horizontalHeader());
      }
    }
  }
}

