#include "AddColumnDialog.h"
#include "DataSetTableModel.h"
#include "DataSetTableVerticalHeader.h"
#include "DataSetTableHorizontalHeader.h"
#include "DataSetTableView.h"
#include "HighlightDelegate.h"
#include "Promoter.h"
#include "RenameColumnDialog.h"
#include "RetypeColumnDialog.h"
#include "AlterDataDialog.h"

// TerraLib include files
#include "../charts/HistogramDataWidget.h"
#include "../charts/Utils.h"
#include "../utils/ScopedCursor.h"
#include "../Config.h"
#include "../Exception.h"
#include "../../../common/Exception.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/dataset/DataSetTypeCapabilities.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../dataaccess/query/DataSetName.h"
#include "../../../dataaccess/query/Field.h"
#include "../../../dataaccess/query/From.h"
#include "../../../dataaccess/query/In.h"
#include "../../../dataaccess/query/Literal.h"
#include "../../../dataaccess/query/Or.h"
#include "../../../dataaccess/query/OrderBy.h"
#include "../../../dataaccess/query/OrderByItem.h"
#include "../../../dataaccess/query/Select.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../maptools/DataSetAdapterLayer.h"
#include "../../../maptools/QueryLayer.h"
#include "../../../statistics/qt/StatisticsDialog.h"
#include "../../../st/maptools/ObservationDataSetLayer.h"
#include "../../../st/maptools/TimeSeriesDataSetLayer.h"
#include "../../../st/maptools/TrajectoryDataSetLayer.h"

// Qt
#include <QBoxLayout>
#include <QContextMenuEvent>
#include <QCursor>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QSpinBox>


// STL
#include <vector>
#include <memory>

bool IsGeometryColumn(te::da::DataSet* dset, const size_t& col)
{
  return dset->getPropertyDataType(col) == te::dt::GEOMETRY_TYPE;
}

void GetGeometryColumnsPositions(te::da::DataSet* dset, std::vector<int>& cols)
{
  size_t nProps = dset->getNumProperties();

  cols.clear();

  for(size_t i=0; i<nProps; i++)
    if(IsGeometryColumn(dset, i))
      cols.push_back((int)i);
}

std::vector<int> GetHiddenSections(QHeaderView* hView, te::da::DataSet* dset)
{
  std::vector<int> res;

  int sz = dset->getNumProperties();

  if(sz > 0)
  {
    for (int i=0; i<sz; i++)
      if(hView->isSectionHidden(i) && !IsGeometryColumn(dset, i))
        res.push_back(i);
  }

  return res;
}

QMenu* GetHiddenColumnsMenu(QHeaderView* hView, te::da::DataSet* dset, QMenu* hMnu)
{
  std::vector<int> hSecs = GetHiddenSections(hView, dset);
  std::vector<int>::iterator it;

  QMenu* mnu = new QMenu(hMnu);
  mnu->setTitle(QObject::tr("Show hidden column"));

  if(hSecs.empty())
    mnu->setEnabled(false);
  else
    for(it=hSecs.begin(); it!=hSecs.end(); ++it)
    {
      QString cName = hView->model()->headerData(*it, Qt::Horizontal, Qt::DisplayRole).toString();
      QAction* act = new QAction(mnu);

      act->setText(cName);
      act->setData(QVariant(*it));

      QString tt = QObject::tr("Turns column \"%1\" visible.").arg(cName);

      act->setToolTip(tt);

      mnu->addAction(act);
    }

  return mnu;
}

QAction* GetShowAllMenu(QHeaderView* hView, te::da::DataSet* dset, QMenu* hMnu)
{
  QAction* act = new QAction(hMnu);
  act->setText(QObject::tr("Show all columns"));
  act->setEnabled(!GetHiddenSections(hView, dset).empty());

  return act;
}

te::da::DataSourcePtr GetDataSource(const te::map::AbstractLayer* layer)
{
  // Getting data source, if it is available
  te::da::DataSourcePtr ds;

  if(layer->getType() == "DATASETLAYER")
  {
    const te::map::DataSetLayer* l = dynamic_cast<const te::map::DataSetLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }
  else if(layer->getType() == "DATASETADAPTERLAYER")
  {
    const te::map::DataSetAdapterLayer* l = dynamic_cast<const te::map::DataSetAdapterLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }
  else if(layer->getType() == "QUERYLAYER")
  {
    const te::map::QueryLayer* l = dynamic_cast<const te::map::QueryLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }
  else if(layer->getType() == "OBSERVATIONDATASETLAYER")
  {
    const te::st::ObservationDataSetLayer* l = dynamic_cast<const te::st::ObservationDataSetLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }
  else if(layer->getType() == "TIMESERIESDATASETLAYER")
  {
    const te::st::TimeSeriesDataSetLayer* l = dynamic_cast<const te::st::TimeSeriesDataSetLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }
  else if(layer->getType() == "TRAJECTORYDATASETLAYER")
  {
    const te::st::TrajectoryDataSetLayer* l = dynamic_cast<const te::st::TrajectoryDataSetLayer*>(layer);

    if(l != 0)
      ds = te::da::DataSourceManager::getInstance().find(l->getDataSourceId());
  }
  return ds;
}

te::da::DataSetTypeCapabilities* GetCapabilities(const te::map::AbstractLayer* layer)
{
    // Getting data source capabilities, if it is available
    te::da::DataSourcePtr ds = GetDataSource(layer);

    if(ds.get() != 0)
    {
      const te::map::DataSetLayer* dl = dynamic_cast<const te::map::DataSetLayer*>(layer);

      if(dl != 0)
        return ds->getCapabilities(dl->getDataSetName()).release();
    }

    return 0;
}

std::auto_ptr<te::da::Select> GetSelectExpression(const std::string& datasetName, const std::vector<std::string>& colsNames, const bool& asc)
{
  te::da::Fields fields;

  fields.push_back(std::auto_ptr<te::da::Field>(new te::da::Field("*")));

  te::da::From from;

  from.push_back(std::auto_ptr<te::da::DataSetName>(new te::da::DataSetName(datasetName)));

  te::da::OrderBy order_by;

  for(size_t i=0; i<colsNames.size(); i++)
    order_by.push_back(std::auto_ptr<te::da::OrderByItem>(new te::da::OrderByItem(colsNames[i], (asc) ? te::da::ASC : te::da::DESC)));

  return std::auto_ptr<te::da::Select>(new te::da::Select(fields, from, order_by));
}

std::auto_ptr<te::da::Select> GetSelectExpression(const std::string& tableName, const te::da::DataSet* set, const std::vector<int>& cols, const bool& asc)
{
  std::vector<std::string> colsNames;

  for(size_t i = 0; i < cols.size(); ++i)
    colsNames.push_back(set->getPropertyName((size_t)cols[i]));

  return GetSelectExpression(tableName, colsNames, asc);
}

std::auto_ptr<te::da::DataSet> GetDataSet(const te::map::AbstractLayer* layer, const te::da::DataSet* set, const std::vector<int>& cols, const bool& asc)
{
  std::auto_ptr<te::da::Select> query = GetSelectExpression(layer->getSchema()->getName(), set, cols, asc);

  try
  {
    if(query.get() == 0)
      throw std::string("Fail to generate query.");

    te::da::DataSourcePtr dsc = GetDataSource(layer);

    if(dsc.get() == 0)
      throw std::string("Fail to get data source.");

    return dsc->getTransactor()->query(query.get(), te::common::RANDOM);
  }
  catch(...)
  {
    return std::auto_ptr<te::da::DataSet>();
  }
}

std::auto_ptr<te::da::DataSet> GetDataSet(const te::map::AbstractLayer* layer, const std::vector<std::string>& colsNames, const bool& asc)
{
  std::auto_ptr<te::da::Select> query;
  
  if(layer->getType() == "DATASETLAYER")
  {
    const te::map::DataSetLayer* l = dynamic_cast<const te::map::DataSetLayer*>(layer);

    if(l != 0)
      query = GetSelectExpression(layer->getSchema()->getName(), colsNames, asc);
  }
  else if(layer->getType() == "DATASETADAPTERLAYER")
  {
    const te::map::DataSetAdapterLayer* l = dynamic_cast<const te::map::DataSetAdapterLayer*>(layer);

    if(l != 0)
      query = GetSelectExpression(layer->getSchema()->getName(), colsNames, asc);
  }
  else if(layer->getType() == "QUERYLAYER")
  {
    const te::map::QueryLayer* l = dynamic_cast<const te::map::QueryLayer*>(layer);

    if(l != 0)
      query.reset(new te::da::Select(l->getQuery()));
  }
  else if(layer->getType() == "OBSERVATIONDATASETLAYER")
  {
    const te::st::ObservationDataSetLayer* l = dynamic_cast<const te::st::ObservationDataSetLayer*>(layer);

    if(l != 0)
      query = GetSelectExpression(layer->getSchema()->getName(), colsNames, asc);
  }
  else if(layer->getType() == "TIMESERIESDATASETLAYER")
  {
    const te::st::TimeSeriesDataSetLayer* l = dynamic_cast<const te::st::TimeSeriesDataSetLayer*>(layer);

    if(l != 0)
      query = GetSelectExpression(layer->getSchema()->getName(), colsNames, asc);
  }
  else if(layer->getType() == "TRAJECTORYDATASETLAYER")
  {
    const te::st::TrajectoryDataSetLayer* l = dynamic_cast<const te::st::TrajectoryDataSetLayer*>(layer);

    if(l != 0)
      query = GetSelectExpression(layer->getSchema()->getName(), colsNames, asc);
  }

  try
  {
    if(query.get() == 0)
      throw std::string("Fail to generate query.");

    te::da::DataSourcePtr dsc = GetDataSource(layer);

    if(dsc.get() == 0)
      throw std::string("Fail to get data source.");

    return dsc->getTransactor()->query(query.get(), te::common::RANDOM);
  }
  catch(...)
  {
    return std::auto_ptr<te::da::DataSet>();
  }
}

std::vector<QString> GetColumnsNames(te::da::DataSet* dset)
{
  std::vector<QString> res;

  if(dset != 0)
  {
    size_t n = dset->getNumProperties();

    for(size_t i=0; i<n; i++)
      if(!IsGeometryColumn(dset, i))
        res.push_back(dset->getPropertyName(i).c_str());
  }

  return res;
}

std::auto_ptr<te::gm::Envelope> GetExtent(te::da::DataSet* dset, te::qt::widgets::Promoter* p, const int& rowPosition)
{
  // Getting last select object bounding rectangle
  std::vector<int> geoCols;

  GetGeometryColumnsPositions(dset, geoCols);

  if(!geoCols.empty())
  {
    int rpos = p->getLogicalRow(rowPosition);

    dset->move(rpos);

    size_t pos = (size_t)geoCols[0];

    std::auto_ptr<te::gm::Geometry> g = dset->getGeometry(pos);

    return std::auto_ptr<te::gm::Envelope> (new te::gm::Envelope(*g->getMBR()));
  }

  return std::auto_ptr<te::gm::Envelope>();
}

bool IsPrimaryKey(const int& col, te::qt::widgets::DataSetTableView* view)
{
  te::qt::widgets::HighlightDelegate* d = dynamic_cast<te::qt::widgets::HighlightDelegate*>(view->itemDelegate());

  if(d == 0)
    return false;

  const te::da::ObjectIdSet* objs = d->getSelected();

  if(objs == 0)
    return false;

  std::vector<size_t> cols = objs->getPropertyPos();
  std::vector<size_t>::iterator it;

  for(it = cols.begin(); it != cols.end(); ++it)
    if((size_t)col == *it)
      return true;

  return false;
}

void HideGeometryColumns(te::da::DataSet* dset, te::qt::widgets::DataSetTableView* view)
{
  if(dset != 0)
  {
    std::vector<int> geoCols;
    std::vector<int>::iterator it;
    GetGeometryColumnsPositions(dset, geoCols);

    for(it = geoCols.begin(); it != geoCols.end(); ++it)
      view->hideColumn(*it);
  }
}

void HideTsVectorColumn(te::da::DataSet* dset, te::qt::widgets::DataSetTableView* view)
{
  if(dset != 0)
  {
    size_t nProps = dset->getNumProperties();
   
    for(std::size_t i = 0; i < nProps; ++i)
      if(dset->getPropertyName(i) == "tsvector")
        view->hideColumn(i);
  }
}

/*!
  \class Filter for popup menus
*/
class TablePopupFilter : public QObject
{
    Q_OBJECT

  public:

    /*!
      \brief Contructor.
    */
    TablePopupFilter(te::qt::widgets::DataSetTableView* view) :
      QObject(view),
      m_view(view),
      m_hMenu(0),
      m_vMenu(0),
      m_vportMenu(0),
      m_dset(0),
      m_showOidsColumns(false),
      m_enabled(true),
      m_autoScrollEnabled(false),
      m_promotionEnabled(false),
      m_isOGR(false)
    {
      m_view->horizontalHeader()->installEventFilter(this);
      m_view->verticalHeader()->installEventFilter(this);
      m_view->viewport()->installEventFilter(this);

      m_view->connect(this, SIGNAL(createHistogram(const int&)), SLOT(createHistogram(const int&)));
      m_view->connect(this, SIGNAL(createNormalDistribution(const int&)), SLOT(createNormalDistribution(const int&)));
      m_view->connect(this, SIGNAL(hideColumn(const int&)), SLOT(hideColumn(const int&)));
      m_view->connect(this, SIGNAL(showColumn(const int&)), SLOT(showColumn(const int&)));
      m_view->connect(this, SIGNAL(removeColumn(const int&)), SLOT(removeColumn(const int&)));
      m_view->connect(this, SIGNAL(enableAutoScroll(const bool&)), SLOT(setAutoScrollEnabled(const bool&)));
      m_view->connect(this, SIGNAL(sortData(const bool&)), SLOT(sortByColumns(const bool&)));
      m_view->connect(this, SIGNAL(renameColumn(const int&)), SLOT(renameColumn(const int&)));
      m_view->connect(this, SIGNAL(retypeColumn(const int&)), SLOT(retypeColumn(const int&)));
      m_view->connect(this, SIGNAL(changeColumnData(const int&)), SLOT(changeColumnData(const int&)));
      m_view->connect(this, SIGNAL(enablePromotion(const bool&)), SLOT(setPromotionEnabled(const bool&)));
      m_view->connect(this, SIGNAL(saveEditions()), SLOT(saveEditions()));
    }

    /*!
      \brief Destructor.
    */
    ~TablePopupFilter()
    {
      delete m_hMenu;
      delete m_vMenu;
      delete m_vportMenu;
    }

    bool eventFilter(QObject* watched, QEvent* event)
    {
      if(!m_enabled)
        return QObject::eventFilter(watched, event);

      QWidget* vport = m_view->viewport();
      QHeaderView* hHdr = m_view->horizontalHeader();
      QHeaderView* vHdr = m_view->verticalHeader();

      switch(event->type())
      {
        case QEvent::ContextMenu:
        {
          if(watched == hHdr)
          {
            delete m_hMenu;
            m_hMenu = 0;

            QContextMenuEvent* evt = static_cast<QContextMenuEvent*>(event);
            QPoint pos = evt->globalPos();

            m_columnPressed = hHdr->logicalIndex(hHdr->visualIndexAt(evt->pos().x()));
            bool isPKey = IsPrimaryKey(m_columnPressed, m_view);

            if(m_columnPressed > 0 && !isPKey)
            {
              m_hMenu = new QMenu;

              QAction* act = new QAction(m_hMenu);
              act->setText(tr("Hide column"));
              act->setToolTip(tr("Hides the selected column."));

              m_hMenu->addAction(act);

              QMenu* hMnu = GetHiddenColumnsMenu(hHdr, m_dset, m_hMenu);

              if(m_columnPressed == -1)
                act->setEnabled(false);

              m_hMenu->addAction(hMnu->menuAction());

              QAction* act2 = GetShowAllMenu(hHdr, m_dset, m_hMenu);
              m_hMenu->addAction(act2);

              QAction* act3 = new QAction(m_hMenu);
              act3->setText(tr("Reset columns order"));
              act3->setToolTip(tr("Put all columns in the original order."));
              m_hMenu->addAction(act3);

              m_hMenu->addSeparator();

              QAction* act5 = new QAction(m_hMenu);
              act5->setText(tr("Sort data ASC"));
              act5->setToolTip(tr("Sort data in ascendent order using selected columns."));
              m_hMenu->addAction(act5);

              QAction* act9 = new QAction(m_hMenu);
              act9->setText(tr("Sort data DESC"));
              act9->setToolTip(tr("Sort data in descendent order using selected columns."));
              m_hMenu->addAction(act9);

              m_hMenu->addSeparator();

              QAction* act4 = new QAction(m_hMenu);
              act4->setText(tr("Histogram"));
              act4->setToolTip(tr("Creates a new histogram based on the data of the selected colunm."));
              m_hMenu->addAction(act4);

              QAction* act10 = new QAction(m_hMenu);
              act10->setText(tr("Normal Probability"));
              act10->setToolTip(tr("Show a chart that displays the normal probability curve."));
              m_hMenu->addAction(act10);

              QAction* act6 = new QAction(m_hMenu);
              act6->setText(tr("Statistics"));
              act6->setToolTip(tr("Show the statistics summary of the selected colunm."));
              m_hMenu->addAction(act6);

              m_hMenu->addSeparator();

              // Signal / Slot connections
              connect(act, SIGNAL(triggered()), SLOT(hideColumn()));
              connect(hMnu, SIGNAL(triggered(QAction*)), SLOT(showColumn(QAction*)));
              
              connect(act4, SIGNAL(triggered()), SLOT(createHistogram()));

              m_view->connect(act2, SIGNAL(triggered()), SLOT(showAllColumns()));
              m_view->connect(act3, SIGNAL(triggered()), SLOT(resetColumnsOrder()));
              
            
              connect(act6, SIGNAL(triggered()), SLOT(showStatistics()));
              connect(act5, SIGNAL(triggered()), SLOT(sortDataAsc()));
              connect(act9, SIGNAL(triggered()), SLOT(sortDataDesc()));
              connect(act10, SIGNAL(triggered()), SLOT(createNormalDistribution()));

              if(m_caps.get())
              {
                QAction* act7 = new QAction(m_hMenu);
                act7->setText(tr("Add column"));
                act7->setToolTip(tr("Adds a column to the table."));
                m_hMenu->addAction(act7);
                act7->setEnabled(m_caps->supportsAddColumn());

                QAction* act8 = new QAction(m_hMenu);
                act8->setText(tr("Remove column"));
                act8->setToolTip(tr("Removes a column from the table."));
                m_hMenu->addAction(act8);
                act8->setEnabled(m_caps->supportsRemoveColumn());

                QAction* act10 = new QAction(m_hMenu);
                act10->setText(tr("Rename column"));
                act10->setToolTip(tr("Renames a column of the table."));
                m_hMenu->addAction(act10);

                QAction* act11 = new QAction(m_hMenu);
                act11->setText(tr("Change column type"));
                act11->setToolTip(tr("Changes the type of a column of the table."));
                act11->setEnabled(!m_isOGR);
                m_hMenu->addAction(act11);

                QAction* act12 = new QAction(m_hMenu);
                act12->setText(tr("Change column data"));
                act12->setToolTip(tr("Changes the data of a column of the table."));
                act12->setEnabled(!m_isOGR);
                m_hMenu->addAction(act12);

                QAction* act13 = new QAction(m_hMenu);
                act13->setText(tr("Save editions"));
                act13->setToolTip(tr("Save pendent editions to layer."));
                act13->setEnabled(m_view->hasEditions());
                m_hMenu->addAction(act13);

                m_view->connect(act7, SIGNAL(triggered()), SLOT(addColumn()));

                connect(act8, SIGNAL(triggered()), SLOT(removeColumn()));
                connect (act10, SIGNAL(triggered()), SLOT(renameColumn()));
                connect (act11, SIGNAL(triggered()), SLOT(retypeColumn()));
                connect (act12, SIGNAL(triggered()), SLOT(changeColumnData()));
                connect (act13, SIGNAL(triggered()), SIGNAL(saveEditions()));
              }

              m_hMenu->popup(pos);
            }
          }
          else if(watched == vport)
          {
          }
          else if(watched == vHdr)
          {
            delete m_vMenu;

            QContextMenuEvent* evt = static_cast<QContextMenuEvent*>(event);
            QPoint pos = evt->globalPos();

            m_vMenu = new QMenu;
            QAction* act = new QAction(m_vMenu);
            act->setText(tr("Enable auto scroll"));
            act->setToolTip(tr("Goes to the selected row."));

            act->setCheckable(true);
            act->setChecked(m_autoScrollEnabled);

            m_vMenu->addAction(act);

            connect(act, SIGNAL(triggered()), SLOT(setAutoScrollEnabled()));

            act = new QAction(m_vMenu);
            act->setText(tr("Enable promotion"));
            act->setToolTip(tr("Enables promotion of selected rows."));

            act->setCheckable(true);
            act->setChecked(m_promotionEnabled);

            m_vMenu->addSeparator();

            m_vMenu->addAction(act);

            connect(act, SIGNAL(triggered()), SLOT(enablePromotion()));

            m_vMenu->popup(pos);
          }
        }
        break;

        default:
        break;
      }

      return QObject::eventFilter(watched, event);
    }

    void setDataSet(te::da::DataSet* dset)
    {
      m_dset = dset;
    }

    std::vector<int> getHiddenColumns()
    {
      return GetHiddenSections(m_view->horizontalHeader(), m_dset);
    }

    void setEnabled(const bool& enabled)
    {
      m_enabled = enabled;
    }

    void setDataSetTypeCapabilities(te::da::DataSetTypeCapabilities* caps)
    {
      m_caps.reset(caps);
    }

    void setPromotionEnabled(const bool& enabled)
    {
      m_promotionEnabled = enabled;
    }

    void setIsOGR(const bool& isOGR)
    {
      m_isOGR = isOGR;
    }

  protected slots:

    void createHistogram()
    {
      emit createHistogram(m_columnPressed);
    }

    void createNormalDistribution()
    {
      emit createNormalDistribution(m_columnPressed);
    }

    void hideColumn()
    {
      emit hideColumn(m_columnPressed);
    }

    void showColumn(QAction* act)
    {
      int column = act->data().toInt();

      emit showColumn(column);
    }

    void showStatistics()
    {
      te::stat::StatisticsDialog statisticDialog;
      std::string prop = m_dset->getPropertyName(m_columnPressed);
      statisticDialog.setStatistics(m_dset, prop);
      statisticDialog.exec();
    }

    void removeColumn()
    {
      emit removeColumn(m_columnPressed);
    }

    void setAutoScrollEnabled()
    {
      m_autoScrollEnabled = !m_autoScrollEnabled;

      emit enableAutoScroll(m_autoScrollEnabled);
    }

    void sortDataAsc()
    {
      emit sortData(true);
    }

    void sortDataDesc()
    {
      emit sortData(false);
    }

    void renameColumn()
    {
      emit renameColumn(m_columnPressed);
    }

    void retypeColumn()
    {
      emit retypeColumn(m_columnPressed);
    }

    void changeColumnData()
    {
      emit changeColumnData(m_columnPressed);
    }

    void enablePromotion()
    {
      m_promotionEnabled = !m_promotionEnabled;

      emit enablePromotion(m_promotionEnabled);
    }

  signals:

    void createHistogram(const int&);

    void createNormalDistribution(const int&);

    void hideColumn(const int&);

    void showColumn(const int&);

    void renameColumn(const int&);

    void retypeColumn(const int&);

    void changeColumnData(const int&);

    void selectObject(const int&, const bool&);

    void selectObjects(const int&, const int&);

    void enablePromotion(const bool&);

    void removeColumn(const int&);

    void enableAutoScroll(const bool&);

    void sortData(const bool&);

    void saveEditions();

  protected:

    te::qt::widgets::DataSetTableView* m_view;
    QMenu* m_hMenu;
    QMenu* m_vMenu;
    QMenu* m_vportMenu;
    te::da::DataSet* m_dset;
    std::auto_ptr<te::da::DataSetTypeCapabilities> m_caps;
    bool m_showOidsColumns;
    bool m_enabled;
    int m_columnPressed;
    bool m_autoScrollEnabled;
    bool m_promotionEnabled;
    bool m_isOGR;
};

te::qt::widgets::DataSetTableView::DataSetTableView(QWidget* parent) :
  QTableView(parent),
  m_layer(0),
  m_autoScrollEnabled(false),
  m_doScroll(true),
  m_promotionEnabled(false),
  m_dset(0),
  m_orderAsc(true)
{
  m_model = new DataSetTableModel(this);

  setModel(m_model);

  setVerticalHeader(new DataSetTableVerticalHeader(this));
  setHorizontalHeader(new DataSetTableHorizontalHeader(this));

#if QT_VERSION >= 0x050000
  horizontalHeader()->setSectionsMovable(true);
#else
  horizontalHeader()->setMovable(true);
#endif

  setSelectionMode(QAbstractItemView::MultiSelection);
  setSelectionBehavior(QAbstractItemView::SelectColumns);

  m_popupFilter = new TablePopupFilter(this);

  m_delegate = new HighlightDelegate(this);

  m_delegate->setPromoter(m_model->getPromoter());

  m_delegate->setColor(Qt::green);

  setItemDelegate(m_delegate);

  connect(verticalHeader(), SIGNAL(selectedRow(const int&, const bool&)), SLOT(highlightRow(const int&, const bool&)));
  connect(verticalHeader(), SIGNAL(selectedRows(const int&, const int&)), SLOT(highlightRows(const int&, const int&)));
}

te::qt::widgets::DataSetTableView::~DataSetTableView()
{
  if (m_model->hasEditions())
  {
    QMessageBox msgBox(this);
    msgBox.setText("There are unsaved changes on table.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
    msgBox.setDefaultButton(QMessageBox::Save);
    
    int ret = msgBox.exec();
    
    if(ret == QMessageBox::Save)
      saveEditions();
  }
}

void te::qt::widgets::DataSetTableView::setDragDrop(bool b)
{
  DataSetTableHorizontalHeader* hheader = static_cast<DataSetTableHorizontalHeader*>(horizontalHeader());
  hheader->setDragDrop(true);
}

bool te::qt::widgets::DataSetTableView::getDragDrop()
{
  DataSetTableHorizontalHeader* hheader = static_cast<DataSetTableHorizontalHeader*>(horizontalHeader());
  return hheader->getDragDrop();
}

void te::qt::widgets::DataSetTableView::setAcceptDrop(bool b)
{
  DataSetTableHorizontalHeader* hheader = static_cast<DataSetTableHorizontalHeader*>(horizontalHeader());
  hheader->setAcceptDrop(true);
}

bool te::qt::widgets::DataSetTableView::getAcceptDrop()
{
  DataSetTableHorizontalHeader* hheader = static_cast<DataSetTableHorizontalHeader*>(horizontalHeader());
  return hheader->getAcceptDrop();
}

void te::qt::widgets::DataSetTableView::setLayer(te::map::AbstractLayer* layer, const bool& clearEditor)
{
  ScopedCursor cursor(Qt::WaitCursor);

  m_layer = layer;
  DataSetTableHorizontalHeader* hheader = static_cast<DataSetTableHorizontalHeader*>(horizontalHeader());
  hheader->setLayer(m_layer);

  std::auto_ptr<te::map::LayerSchema> sch = m_layer->getSchema();

  if (m_orderby.empty())
  {
    std::vector<te::dt::Property*> psps;
    
    if(sch->getPrimaryKey())
      psps = sch->getPrimaryKey()->getProperties();
    else
      psps = sch->getProperties();

    std::vector<te::dt::Property*>::iterator it;

    for(it = psps.begin(); it != psps.end(); ++it)
      m_orderby.push_back((*it)->getName());
  }

  te::da::DataSourcePtr dsc = GetDataSource(m_layer);

  setDataSet(GetDataSet(m_layer, m_orderby, m_orderAsc).release(), dsc->getEncoding(), clearEditor);
  setLayerSchema(sch.get());

  te::da::DataSetTypeCapabilities* caps = GetCapabilities(m_layer);

  m_popupFilter->setDataSetTypeCapabilities(caps);

  if(caps)
    m_model->setEditable(caps->supportsDataEdition());

  if(dsc.get() != 0)
  {
    bool isOGR = (dsc->getType().compare("OGR") == 0);
    setSelectionMode(isOGR ? SingleSelection : MultiSelection);
    setSelectionBehavior(isOGR ? QAbstractItemView::SelectColumns : QAbstractItemView::SelectItems);
    m_popupFilter->setIsOGR(isOGR);
  }

  highlightOIds(m_layer->getSelected());
}

void te::qt::widgets::DataSetTableView::setDataSet(te::da::DataSet* dset, te::common::CharEncoding enc, const bool& clearEditor)
{
  reset();

  m_encoding = enc;

  m_model->setDataSet(dset, enc, clearEditor);
  DataSetTableHorizontalHeader* hheader = static_cast<DataSetTableHorizontalHeader*>(horizontalHeader());
  hheader->setDataSet(dset);

  HideGeometryColumns(dset, this);
  HideTsVectorColumn(dset, this);

  m_popupFilter->setDataSet(dset);
  m_delegate->setDataSet(dset);
  m_dset = dset;
}

void te::qt::widgets::DataSetTableView::setLayerSchema(const te::da::DataSetType* schema)
{
  te::da::ObjectIdSet* objs = 0;

  te::da::GetEmptyOIDSet(schema, objs);

  std::auto_ptr<te::da::ObjectIdSet> objs_ptr(objs);

  m_delegate->setObjectIdSet(objs_ptr.get());

  m_model->setPkeysColumns(objs_ptr->getPropertyPos());

//  m_model->getPromoter()->preProcessKeys(m_dset, objs_ptr->getPropertyPos());
}

void te::qt::widgets::DataSetTableView::highlightOIds(const te::da::ObjectIdSet* oids)
{
  if(oids == 0)
    return;
  
  m_delegate->setObjectIdSet(oids);

  if(m_autoScrollEnabled && oids != 0 && oids->size() > 0)
  {
    size_t row = m_model->getPromoter()->map2Row(*oids->begin());

    scrollTo(m_model->index((int)row, 0));
  }

  if(m_promotionEnabled)
    promote(m_doScroll);

  viewport()->repaint();
}

void te::qt::widgets::DataSetTableView::setHighlightColor(const QColor& color)
{
  m_delegate->setColor(color);

  repaint();
}

bool te::qt::widgets::DataSetTableView::hasEditions() const
{
  return m_model->hasEditions();
}

void te::qt::widgets::DataSetTableView::createHistogram(const int& column)
{
  QDialog* dialog = new QDialog(this);
  dialog->setFixedSize(160, 75);

  const te::map::LayerSchema* schema = m_layer->getSchema().release();

  te::da::DataSet* dataset = m_layer->getData().release();
  te::da::DataSetType* dataType = (te::da::DataSetType*) schema;

  // Histogram data Widget
  te::qt::widgets::HistogramDataWidget* histogramWidget = new te::qt::widgets::HistogramDataWidget(dataset, dataType, dialog);
  histogramWidget->setHistogramProperty(column);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(dialog);
  layout->addWidget(histogramWidget);

  QDialogButtonBox* bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
  connect(bbox, SIGNAL(accepted()), dialog, SLOT(accept()));
  connect(bbox, SIGNAL(rejected()), dialog, SLOT(reject()));

  layout->addWidget(bbox);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  int res = dialog->exec();
  if (res == QDialog::Accepted)
    emit createChartDisplay(te::qt::widgets::createHistogramDisplay(dataset,dataType, column, histogramWidget->getHistogram()));

  delete dialog;
}

void te::qt::widgets::DataSetTableView::createNormalDistribution(const int& column)
{
  int propType = m_layer->getData()->getPropertyDataType(column);
  if(propType >= te::dt::INT16_TYPE && propType <= te::dt::NUMERIC_TYPE)
  {
    emit createChartDisplay(te::qt::widgets::createNormalDistribution(m_layer->getData().get(), column));
  }
  else
  {
    QString msgErr(tr("This operation is not available for this type of data"));
    QString msgTitle(tr("TNormal Probability"));
    QMessageBox::warning(this, msgTitle, msgErr);
  }
}

void te::qt::widgets::DataSetTableView::hideColumn(const int& column)
{
  horizontalHeader()->hideSection(column);
}

void te::qt::widgets::DataSetTableView::showColumn(const int& column)
{
  horizontalHeader()->showSection(column);
}

void te::qt::widgets::DataSetTableView::renameColumn(const int& column)
{
  RenameColumnDialog dlg(parentWidget());

  dlg.setOldColumnName(QString::fromStdString(m_dset->getPropertyName(column)));

  if(dlg.exec() == QDialog::Accepted)
  {
    std::string oldName = dlg.getOldName().toStdString();
    std::string newName = dlg.getNewName().toStdString();
    te::da::DataSourcePtr dsrc = GetDataSource(m_layer);

    if(dsrc.get() == 0)
      throw Exception(tr("Fail to get data source.").toStdString());

    if(!dsrc->isPropertyNameValid(newName))
    {
      QMessageBox::warning(this, tr("TerraView"), tr("Invalid column name. Choose another."));
      return;
    }

    try
    {
      dsrc->renameProperty(m_layer->getSchema()->getName(), oldName, newName);
    }
    catch (const te::common::Exception& e)
    {
      QMessageBox::information(this, tr("Rename Column"), tr("The column could not be renamed: ") + e.what());
    }

    m_layer->setOutOfDate();

    setLayer(m_layer);
  }
}

void te::qt::widgets::DataSetTableView::retypeColumn(const int& column)
{
  RetypeColumnDialog dlg(parentWidget());

  std::auto_ptr<te::da::DataSetType> schema = m_layer->getSchema();
  te::dt::Property* prp;
  std::string dsetName = schema->getName();
  std::string columnName;

  prp = schema->getProperty(column);

  if(prp == 0)
    throw Exception(tr("Fail to get property of the dataset.").toStdString());

  columnName = prp->getName();

  dlg.setTableName(dsetName.c_str());
  dlg.setColumnName(columnName.c_str());
  dlg.setType(prp->getType());

  if(dlg.exec() == QDialog::Accepted)
  {
    te::da::DataSourcePtr dsrc = GetDataSource(m_layer);

    if(dsrc.get() == 0)
      throw Exception(tr("Fail to get data source.").toStdString());

    setDataSet(0, te::common::LATIN1);

    dsrc->changePropertyDefinition(dsetName, columnName, dlg.getProperty().release());

    m_layer->setOutOfDate();

    setLayer(m_layer);
  }
}

void te::qt::widgets::DataSetTableView::changeColumnData(const int& column)
{
  if(m_dset == 0 || m_layer == 0)
    return;

  std::auto_ptr<te::da::DataSetType> schema = m_layer->getSchema();
  te::da::PrimaryKey* pk = schema->getPrimaryKey();
  std::string dsetName = schema->getName();
  te::dt::Property* prop = schema->getProperty(column);
  std::string columnName = prop->getName();
  std::vector<QString> cols = GetColumnsNames(m_dset);

  std::map<std::string, int> pkColumnsType;

  std::vector<te::dt::Property*> pkProps = pk->getProperties();

  for (size_t i = 0; i < pkProps.size(); i++)
  {
    pkColumnsType[pkProps[i]->getName()] = pkProps[i]->getType();
  }

  AlterDataDialog dlg(parentWidget());
  dlg.setSelectedColumn(columnName.c_str());
  dlg.setDataColumns(cols);
  if (prop->getType() == te::dt::STRING_TYPE)
    dlg.setHelpLabelText(tr("This is a string column, use single quotes"));

  if(dlg.exec() == QDialog::Accepted)
  {
    te::da::DataSourcePtr dsrc = GetDataSource(m_layer);

    if(dsrc.get() == 0)
      throw Exception(tr("Fail to get data source.").toStdString());

    std::string sql = "UPDATE " + dsetName + " SET " + columnName + " = " + dlg.getExpression().toStdString();

    try
    {
      //TODO: create class Update at dataaccess/query to do this operations
      //Obs: not working when the dataset primary key has more than two properties
      if (!dlg.alterAllData())
      {
        const te::da::ObjectIdSet* objSet =  m_layer->getSelected();

        std::vector<std::string> pkCols =  objSet->getPropertyNames();

        te::da::Expression* exp = objSet->getExpressionByInClause();

        te::da::In* inExp = dynamic_cast<te::da::In*>(exp);
        te::da::Or* orExp = dynamic_cast<te::da::Or*>(exp);
      
        te::da::In* orFirstInExp = 0;
        te::da::In* orSecondInExp = 0;

        std::string orFirstInExpStr = " IN (";
        std::string orSecondInExpStr = " IN (";

        if (orExp) // more than one properties at primary key
        {
          orFirstInExp = dynamic_cast<te::da::In*>(orExp->getFirst());
          orSecondInExp = dynamic_cast<te::da::In*>(orExp->getSecond());

          for (std::size_t i = 0; i < orFirstInExp->getNumArgs(); ++i)
          {
            te::da::Expression* a1 = orFirstInExp->getArg(i);

            te::da::Literal* l1 = dynamic_cast<te::da::Literal*>(a1);

            std::string inStr = l1->getValue()->toString();

            if (pkColumnsType[objSet->getPropertyNames()[0]] == te::dt::STRING_TYPE)
              inStr = "'" + inStr + "'";

            if (i == orFirstInExp->getNumArgs() - 1)
              orFirstInExpStr += inStr + ")";
            else
              orFirstInExpStr += inStr + ",";
          }

          for (std::size_t i = 0; i < orSecondInExp->getNumArgs(); ++i)
          {
            te::da::Expression* a1 = orSecondInExp->getArg(i);

            te::da::Literal* l1 = dynamic_cast<te::da::Literal*>(a1);

            std::string inStr = l1->getValue()->toString();

            if (pkColumnsType[objSet->getPropertyNames()[1]] == te::dt::STRING_TYPE)
              inStr = "'" + inStr + "'";

            if (i == orSecondInExp->getNumArgs() - 1)
              orSecondInExpStr += inStr + ")";
            else
              orSecondInExpStr += inStr + ",";
          }

          sql += " WHERE " + objSet->getPropertyNames()[0] + orFirstInExpStr + " AND " + objSet->getPropertyNames()[1] + orSecondInExpStr;

        }
        else if (inExp)
        {

          std::string inExpStr = " IN (";

          for (std::size_t i = 0; i < inExp->getNumArgs(); ++i)
          {
            te::da::Expression* a1 = inExp->getArg(i);

            te::da::Literal* l1 = dynamic_cast<te::da::Literal*>(a1);          

            std::string inStr = l1->getValue()->toString();

            if (pkColumnsType[objSet->getPropertyNames()[0]] == te::dt::STRING_TYPE)
              inStr = "'" + inStr + "'";

            if (i == inExp->getNumArgs() - 1)
            {
              inExpStr += inStr + ")";
            }
            else
            {
              inExpStr += inStr + ",";
            }
          }

          sql += " WHERE " + objSet->getPropertyNames()[0] + inExpStr;
        }      

      }

      // Ignore \n
      std::replace(sql.begin(), sql.end(), '\n', ' ');

      dsrc->execute(sql);

      m_layer->setOutOfDate();

      setLayer(m_layer);
    }
    catch(te::common::Exception& e)
    {
      QMessageBox::information(this, tr("Updating data failure"), e.what());
    }
    catch(...)
    {
      QMessageBox::information(this, tr("Updating data failure"), tr("Data source operation fail for unknown reason."));
    }
  }
}

void te::qt::widgets::DataSetTableView::showAllColumns()
{
  std::vector<int> hCols = m_popupFilter->getHiddenColumns();
  std::vector<int>::iterator it;

  for (it=hCols.begin(); it!=hCols.end(); ++it)
    horizontalHeader()->showSection(*it);
}

void te::qt::widgets::DataSetTableView::resetColumnsOrder()
{
  QHeaderView* hdr = horizontalHeader();

  int nCols = hdr->count();

  for(int i=0; i<nCols; i++)
  {
    int visCol = hdr->visualIndex(i);

    if(visCol != i)
      hdr->moveSection(visCol, i);
  }
}

void te::qt::widgets::DataSetTableView::highlightRow(const int& row, const bool& add)
{
  te::da::ObjectIdSet* oids = m_model->getObjectIdSet(row, row);

  if(add)
  {
    te::da::ObjectId* oid = *oids->begin();

    if(m_delegate->getSelected()->contains(oid))
    {
      emit deselectOIds(oids);

      delete oids;

      m_delegate->setObjectIdSet(m_layer->getSelected());

      if(m_promotionEnabled)
        promote();

      viewport()->repaint();

      return;
    }
  }

  m_doScroll = false;

  emit selectOIds(oids, add, GetExtent(m_dset, m_model->getPromoter(), row).get());

  if(m_promotionEnabled)
    promote();

  viewport()->repaint();

  m_doScroll = true;
}

void te::qt::widgets::DataSetTableView::highlightRows(const int& initRow, const int& finalRow)
{
  int ini,
      final;

  if(initRow < finalRow)
  {
    ini = initRow;
    final = finalRow;
  }
  else
  {
    ini = finalRow;
    final = initRow;
  }

  te::da::ObjectIdSet* oids = m_model->getObjectIdSet(ini, final);

  m_doScroll = false;

  emit selectOIds(oids, true, GetExtent(m_dset, m_model->getPromoter(), final).get());

  if(m_promotionEnabled)
    promote();

  viewport()->repaint();

  m_doScroll = false;
}

void te::qt::widgets::DataSetTableView::promote(const bool& scroll)
{
  ScopedCursor cursor(Qt::WaitCursor);

  setEnabled(false);
  m_model->setEnabled(false);
  m_popupFilter->setEnabled(false);

  const te::da::ObjectIdSet* oids = m_delegate->getSelected();
  m_model->promote(oids);

  m_popupFilter->setEnabled(true);
  m_model->setEnabled(true);
  setEnabled(true);

  viewport()->repaint();

  if(scroll)
    scrollToTop();
}

void te::qt::widgets::DataSetTableView::sortByColumns(const bool& asc)
{
  //*********************
  // Sort by query
  //*********************
  try
  {
    ScopedCursor cursor(Qt::WaitCursor);

    std::vector<int> selCols;

    m_orderby.clear();

    int nCols = model()->columnCount();

    for(int i=0; i<nCols; i++)
    {
      int logRow = horizontalHeader()->logicalIndex(i);
      if(selectionModel()->isColumnSelected(logRow, QModelIndex()))
      {
        m_orderby.push_back(m_dset->getPropertyName((size_t)logRow));
        selCols.push_back(logRow);
      }
    }

    if(selCols.empty())
      return;

    std::auto_ptr<te::da::DataSet> dset = GetDataSet(m_layer, m_orderby, asc);

    if(dset.get() == 0)
      throw te::common::Exception(tr("Sort operation not supported by the source of data.").toStdString());

    setDataSet(dset.release(), m_encoding);

    viewport()->repaint();

    setUpdatesEnabled(false);

    m_model->getPromoter()->preProcessKeys(m_dset, m_delegate->getSelected()->getPropertyPos());

    setUpdatesEnabled(true);

    if(m_promotionEnabled)
      promote();
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::information(this, tr("Sorting columns failure"), tr("Could not sort data: ") + e.what());
  }
}

void te::qt::widgets::DataSetTableView::setOIdsColumnsVisible(const bool& visible)
{
  m_model->showOIdsVisible(visible);

  horizontalHeader()->viewport()->repaint();
}

void te::qt::widgets::DataSetTableView::addColumn()
{
  if(m_layer == 0)
    return;

  try
  {
    AddColumnDialog dlg(parentWidget());

    std::auto_ptr<te::da::DataSetType> ds_t = m_layer->getSchema();

    std::string dsName = ds_t->getName();
    size_t n_prop = ds_t->getProperties().size();
    dlg.setTableName(dsName);

    if(dlg.exec() == QDialog::Accepted)
    {
      te::da::DataSourcePtr ds = GetDataSource(m_layer);

      if(ds.get() == 0)
        throw te::common::Exception(tr("Fail to get data source of the layer.").toStdString());

      std::auto_ptr<te::dt::Property> p(dlg.getNewProperty());

      if(p->getName().empty())
        throw te::common::Exception(tr("Name must not be empty.").toStdString());

      if(!ds->isPropertyNameValid(p->getName()))
        throw te::common::Exception(tr("The property name is invalid.").toStdString());

      if(ds->propertyExists(dsName, p->getName()))
        throw te::common::Exception(tr("There already exists a property with this name.").toStdString());

      ds->addProperty(dsName, p.get());

      if(ds->getType().compare("OGR") == 0)
        m_model->insertColumns(((int)n_prop-1), 0);

      m_layer->setOutOfDate();

      setLayer(m_layer, false);
    }
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::information(this, tr("Creating column failure"), tr("The column could not be created: ") + e.what());
  }
}

void te::qt::widgets::DataSetTableView::removeColumn(const int& column)
{
  try
  {
    if(QMessageBox::question(this, tr("Remove column"), tr("Are you sure you want to remove this column?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
      te::da::DataSourcePtr ds = GetDataSource(m_layer);

      if(ds.get() == 0)
        throw te::common::Exception(tr("Fail to get data source of the layer.").toStdString());

      std::auto_ptr<te::da::DataSetType> ds_t = m_layer->getSchema();
      std::string dsName = ds_t->getName();

      std::string pName = ds_t->getProperty(column)->getName();

      ds->dropProperty(dsName, pName);

      m_model->removeColumns(column, 0);

      m_layer->setOutOfDate();

      setLayer(m_layer, false);
    }
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::information(this, tr("Removing column failure"), tr("The column could not be removed: ") + e.what());
  }
}

void te::qt::widgets::DataSetTableView::setAutoScrollEnabled(const bool& enable)
{
  m_autoScrollEnabled = enable;
}

void te::qt::widgets::DataSetTableView::setPromotionEnabled(const bool &enable)
{
  m_promotionEnabled = enable;

  if(m_promotionEnabled)
    promote();

  m_popupFilter->setPromotionEnabled(m_promotionEnabled);
}

void te::qt::widgets::DataSetTableView::removeSelection(const int& initRow, const int& finalRow)
{
  QItemSelection toRemove;

  for(int i=initRow; i<=finalRow; i++)
  {
    QModelIndexList idx = selectionModel()->selectedColumns(i);

    if(!idx.empty())
      toRemove.select(idx.first(), idx.last());
  }

  selectionModel()->select(toRemove, QItemSelectionModel::Deselect);
}

void te::qt::widgets::DataSetTableView::saveEditions()
{
  try
  {
    ScopedCursor c(Qt::WaitCursor);

    std::vector< std::set<int> > ed;

    std::auto_ptr<te::map::LayerSchema> sch = m_layer->getSchema();

    std::auto_ptr<te::da::DataSet> md = m_model->getEditions(sch.get(), ed);

    te::da::DataSourcePtr ds = GetDataSource(m_layer);

    if(ds.get() == 0)
      throw te::common::Exception(tr("Fail to get data source from layer").toStdString());

    te::da::ObjectIdSet* objs = 0;

    te::da::GetEmptyOIDSet(sch.get(), objs);

    std::auto_ptr<te::da::ObjectIdSet> objs1(objs);

    ds->update(sch->getName(), md.get(), ed, objs1->getPropertyPos());

    setLayer(m_layer);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::warning(this, tr("Save edition failure"), e.what());
  }
}

#include "DataSetTableView.moc"

