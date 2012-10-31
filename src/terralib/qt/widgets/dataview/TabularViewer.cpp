#include "TabularViewer.h"

#include "DataSetModel.h"
#include "HLDelegateDecorator.h"
#include <terralib/qt/widgets/utils/ColorPickerToolButton.h>

//TerraLib include files
#include <terralib/maptools/PromoTable.h>
#include <terralib/maptools/DataSetTable.h>
#include <terralib/common/Exception.h>
#include <terralib/geometry/Geometry.h>
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/dataset/PrimaryKey.h>

//! Qt include files
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QStylePainter>

//! STL include files
#include <vector>

//! Forward declarations
QMenu* makeGroupColorMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QMenu* mainMnu, std::vector<te::qt::widgets::ColorPickerToolButton*>& colors);

QMenu* makePromoteMenu(te::qt::widgets::TabularViewer* viewer, QMenu* parent, std::vector<QAction*> acts);

QMenu* makeRemovePromoteMenu(te::qt::widgets::TabularViewer* viewer, QMenu* parent, std::vector<QAction*>& acts);

void makeHeaderMenu(QMenu*& mnu, QAction*& col, te::qt::widgets::TabularViewer* viewer, QObject* filter);

QMenu* makeDataMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QAction*& rst_hl_all, QAction*& prm_all, QAction*& rst_prm_all, std::vector<QAction*>& prm, 
  std::vector<QAction*>& reset_prm, std::vector<QAction*>& reset_hl, std::vector<te::qt::widgets::ColorPickerToolButton*>& colors);

QMenu* makeHiddenMenu(QHeaderView* view, QMenu* parent);

std::set<size_t> getGrpsHLighted(te::qt::widgets::TabularViewer* viewer);

QMenu* makeResetHLMenu(te::qt::widgets::TabularViewer* viewer, QObject* filter, QMenu* parent, std::vector<QAction*>& rst_hl_grp);

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
        DataViewPopupFilter(TabularViewer* viewer, const bool& promotion=false) :
        QObject(viewer),
        m_viewer(viewer),
        m_col(-1),
        m_data_menu(0),
        m_head_menu(0),
        m_prm_all(0),
        m_reset_prm_all(0),
        m_show_cols(0),
        m_reset_hl(0),
        m_hiddenCols(0),
        m_colGroup(0),
        m_promotion(promotion)
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
          m_viewer->connect(this, SIGNAL(resetHighlight(const int&)), SLOT(clearHighlighted(const int&)));
        }

        /*!
          \brief Destructor.
        */
        ~DataViewPopupFilter()
        {
        }

        void setPromotionEnabled(const bool& prm)
        {
          m_promotion = prm;

          //setupMenus();
        }

        /*!
          \brief Construct menus.
          \details Re-construct the header and data menus. This method MUST be called when item delegate was updated.
        */
        void setupMenus()
        {
          if(m_head_menu != 0)
            m_head_menu->clear();
        
          if(m_data_menu != 0)
            m_data_menu->clear();

          delete m_data_menu;
          delete m_head_menu;

          m_prm_grp.clear();
          m_reset_prm_grp.clear();
          m_reset_hl_grp.clear();

          makeHeaderMenu(m_head_menu, m_show_cols, m_viewer, this);
          m_data_menu = makeDataMenu(m_viewer, this, m_reset_hl, m_prm_all, m_reset_prm_all, m_prm_grp, m_reset_prm_grp, m_reset_hl_grp, m_colors);
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
                  QContextMenuEvent* evt = static_cast<QContextMenuEvent*>(event);

                  checkMenuEnabling();

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

        /*!
          \brief Enable / disable menus based on highlighted objects.
        */
        void checkMenuEnabling()
        {
          te::qt::widgets::HLDelegateDecorator* dec = dynamic_cast<te::qt::widgets::HLDelegateDecorator*> (m_viewer->itemDelegate());
          size_t cls = dec->getNumberOfClasses();
          std::set<size_t> prm = m_viewer->getPromotedGroups();
          std::set<size_t> hl = getGrpsHLighted(m_viewer);

          m_prm_all->setEnabled(m_promotion && (prm.size() != cls) && !hl.empty());
          m_reset_prm_all->setEnabled(m_promotion && !prm.empty());
          m_reset_hl->setEnabled(!hl.empty());

          ((QMenu*) (*m_prm_grp.begin())->parent())->setEnabled(m_promotion && !hl.empty());
          ((QMenu*) (*m_reset_prm_grp.begin())->parent())->setEnabled(m_promotion && !prm.empty());

          ((QMenu*) (*m_reset_hl_grp.begin())->parent())->setEnabled(!hl.empty());

          std::set<size_t> grps = m_viewer->getPromotedGroups();

          if(dec != 0)
            for(size_t i=0; i<cls; i++)
            {
              if(m_promotion)
              {
                m_prm_grp[i]->setEnabled(grps.find(i) == grps.end() && (hl.find(i) != hl.end()));
                m_reset_prm_grp[i]->setEnabled(grps.find(i) != grps.end());
              }

              m_reset_hl_grp[i]->setEnabled(hl.find(i) != hl.end());
            }
        }

        void setColor(const int& g, const QColor& c)
        {
          if(g >= (int)m_colors.size())
            throw te::common::Exception(tr("Group idenitifier out of colors boundaries.").toStdString());
          
          m_colors[g]->setColor(c);
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

        QAction* m_prm_all;         //!< Promote all action.
        QAction* m_reset_prm_all;   //!< Reset all promotion action.
        QAction* m_reset_hl;        //!< Reset all highlighted action.

        std::vector<QAction*> m_prm_grp;        //!< Promote group actions.
        std::vector<QAction*> m_reset_prm_grp;  //!< Reset promoted groups actions.
        std::vector<QAction*> m_reset_hl_grp;   //!< Reset highlighted groups actions.
        std::vector<ColorPickerToolButton*> m_colors; //!< Buttons to set up colors.

        QAction* m_show_cols;       //!< Show hidden columns

        QMenu* m_hiddenCols;        //!< Hide columns menu.
        QAction* m_colGroup;        //!< Separator of columns menu.

        int m_col;                  //!< Visual column position.
        bool m_promotion;           //!< Promotion can be enable or not.
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

        QAbstractButton* btn = QTableView::findChild<QAbstractButton*>();

        if(btn != 0)
        {
          QIcon icon = QIcon::fromTheme("key");
          btn->setIcon(icon.pixmap(btn->size())); 
        }
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

      bool TabularViewer::eventFilter(QObject* o, QEvent* e)
      {
        if (e->type() == QEvent::Paint) 
        {
          QAbstractButton* btn = qobject_cast<QAbstractButton*>(o);

          if (btn != 0) 
          {
 // paint by hand (borrowed from QTableCornerButton)
            QStyleOptionHeader opt;
            opt.init(btn);
            QStyle::State styleState = QStyle::State_None;

            if (btn->isEnabled())
              styleState |= QStyle::State_Enabled;
            if (btn->isActiveWindow())
              styleState |= QStyle::State_Active;
            if (btn->isDown())
              styleState |= QStyle::State_Sunken;

            opt.state = styleState;
            opt.rect = btn->rect();

            opt.icon = btn->icon(); // this line is the only difference to QTableCornerButton
            opt.position = QStyleOptionHeader::OnlyOneSection;
            QStylePainter painter(btn);
            painter.drawControl(QStyle::CE_Header, opt);
            return true; // eat event
          }
        }

        return false;      
      }

      void TabularViewer::showData(te::da::DataSet* dset)
      {
        //Making PromoTable
        te::map::DataSetTable* tbl = new te::map::DataSetTable(dset);
        te::map::PromoTable* tbl2 = new te::map::PromoTable(tbl);

        tbl2->resetTable();

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
        QTableView::scrollToTop();

        //Reseting params
        delete m;
        delete m_model;
        m_model = model;
        m_table = table;
        m_promotedGroups.clear();

        setPrimaryKeys(m_table->getPKeysColumns());

        resetColumns();

        QAbstractButton* btn = QTableView::findChild<QAbstractButton*>();

        if(btn != 0)
        {
          if(m_table->getPKeysColumns().empty())
          {
            btn->installEventFilter(this);
            m_menu_filter->setPromotionEnabled(false);
          }
          else
          {
            btn->removeEventFilter(this);
            m_menu_filter->setPromotionEnabled(false);
          }
        }
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

      void TabularViewer::setHighlightColor(const int& g, const QColor& c, const bool& emitSignal)
      {
        HLDelegateDecorator* del = dynamic_cast<HLDelegateDecorator*>(QTableView::itemDelegate());

        if(del != 0)
          del->setClassColor(g, c);

        m_menu_filter->setColor(g, c);

        QTableView::viewport()->update();

        if(emitSignal)
          emit groupColorChanged(g, c);
      }

      void TabularViewer::pointObjects(const std::vector<int>& rows)
      {
        std::vector<int>::const_iterator it;
        std::set<std::string> ids;
        std::map<std::string, te::gm::Geometry*> geoms;

        for(it=rows.begin(); it!=rows.end(); ++it)
        {
          QModelIndex idx = m_model->index(*it, 0);

          if(!idx.isValid())
            throw te::common::Exception(tr("Invalid index of row.").toStdString());

          std::string id = m_model->data(idx, DataSetModel::PKEY).toString().toStdString();
          ids.insert(id);
        }

        setHighlightObjects(Point_Items, ids);

        HLDelegateDecorator* del = dynamic_cast<HLDelegateDecorator*>(QTableView::itemDelegate());

        if(del != 0)
        {
          std::set<std::string> hl = del->getDecorated(Point_Items)->getHighlightKeys();
          std::set<std::string>::iterator hit;

          for(hit = hl.begin(); hit != hl.end(); ++hit)
          {
            int row = m_table->map2Row(*hit);
            QModelIndex idx = m_model->index(row, 0);

            QVariant v = m_model->data(idx, DataSetModel::GEOMETRY);
            QVariant pkey = m_model->data(idx, DataSetModel::PKEY);

            te::gm::Geometry* geom = static_cast<te::gm::Geometry*>(v.value<void*>());
            if(geom != 0)
              geoms[pkey.toString().toStdString()] = geom;
          }

//          if(!geoms.empty())
          emit pointObjects(geoms);
        }
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
        if(m_table->getPKeysColumns().empty())
          return;

        if(enable) 
          m_table->preprocessPKeys();
        else
        {
          m_table->cleanPreprocessPKeys();
          resetPromotion(m_table);

          QTableView::viewport()->update();
        }

        m_menu_filter->setPromotionEnabled(enable);
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

