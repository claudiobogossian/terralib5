#include "DataSetTableView.h"
#include "DataSetTableModel.h"
#include "HighlightDelegate.h"
#include "DataSetTableVerticalHeader.h"

// TerraLib include files
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"

// Qt
#include <QtGui/QHeaderView>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QCursor>

// STL
#include <vector>

bool IsGeometryColumn(te::da::DataSet* dset, const int& col)
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

  int sz = hView->count();

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
      act->setToolTip(QObject::tr("Turns column \"") + cName + "\" visible.");

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
      m_vportMenu(0)
    {
      m_view->horizontalHeader()->installEventFilter(this);
      m_view->verticalHeader()->installEventFilter(this);
      m_view->viewport()->installEventFilter(this);

      m_view->connect(this, SIGNAL(hideColumn(const int&)), SLOT(hideColumn(const int&)));
      m_view->connect(this, SIGNAL(showColumn(const int&)), SLOT(showColumn(const int&)));
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

            QContextMenuEvent* evt = static_cast<QContextMenuEvent*>(event);
            QPoint pos = evt->globalPos();

            m_columnPressed = hHdr->logicalIndex(hHdr->visualIndexAt(evt->pos().x()));

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

            // Signal / Slot connections
            connect (act, SIGNAL(triggered()), SLOT(hideColumn()));
            connect (hMnu, SIGNAL(triggered(QAction*)), SLOT(showColumn(QAction*)));
            m_view->connect (act2, SIGNAL(triggered()), SLOT(showAllColumns()));
            m_view->connect (act3, SIGNAL(triggered()), SLOT(resetColumnsOrder()));

            m_hMenu->popup(pos);
          }
          else if(watched == vHdr)
          {
          }
          else if(watched == vport)
          {
            delete m_vportMenu;

            QContextMenuEvent* evt = static_cast<QContextMenuEvent*>(event);
            QPoint pos = evt->globalPos();

            m_vportMenu = new QMenu;

            QAction* act = new QAction(m_vportMenu);
            act->setText(tr("Promote"));
            act->setToolTip(tr("Reorder rows"));
            m_vportMenu->addAction(act);

            m_view->connect(act, SIGNAL(triggered()), SLOT(promote()));

            m_vportMenu->popup(pos);
          }
        }
        break;

        //case QEvent::MouseButtonPress:
        //  {
        //    QMouseEvent* evt = static_cast<QMouseEvent*>(event);

        //    if(evt->button() == Qt::LeftButton && watched == vport)
        //    {
        //      int row = m_view->rowAt(evt->pos().y());

        //      if(evt->modifiers() & Qt::ShiftModifier)
        //      {
        //        emit selectObjects(m_initRow, row);

        //        return true;
        //      }

        //      m_initRow = row;

        //      bool add = evt->modifiers() & Qt::ControlModifier;

        //      emit selectObject(row, add);

        //      return true;
        //    }
        //  }
        //break;

        //case QEvent::MouseButtonDblClick:
        //{
        //  QMouseEvent* evt = static_cast<QMouseEvent*>(event);

        //  QModelIndex idx = m_view->indexAt(evt->pos());

        //  m_view->edit(idx);

        //  return true;
        //}
        //break;
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

  protected slots:

    void hideColumn()
    {
      emit hideColumn(m_columnPressed);
    }

    void showColumn(QAction* act)
    {
      int column = act->data().toInt();

      emit showColumn(column);
    }

  signals:

    void hideColumn(const int&);

    void showColumn(const int&);

    void selectObject(const int&, const bool&);

    void selectObjects(const int& initRow, const int& finalRow);

    void promote();

  protected:

    te::qt::widgets::DataSetTableView* m_view;
    QMenu* m_hMenu;
    QMenu* m_vMenu;
    QMenu* m_vportMenu;
    te::da::DataSet* m_dset;

    int m_columnPressed;
};

te::qt::widgets::DataSetTableView::DataSetTableView(QWidget* parent) :
QTableView(parent)
{
  m_model = new DataSetTableModel(this);
  setModel(m_model);

  horizontalHeader()->setMovable(true);
  setVerticalHeader(new DataSetTableVerticalHeader(this));

  setSelectionMode(QAbstractItemView::MultiSelection);

  m_popupFilter = new TablePopupFilter(this);

  m_delegate = new HighlightDelegate(this);

  m_delegate->setColor(Qt::green);

  setItemDelegate(m_delegate);

  connect(verticalHeader(), SIGNAL(selectedRow(const int&, const bool&)), SLOT(highlightRow(const int&, const bool&)));
  connect(verticalHeader(), SIGNAL(selectedRows(const int&, const int&)), SLOT(highlightRows(const int&, const int&)));
}

te::qt::widgets::DataSetTableView::~DataSetTableView()
{
}

void te::qt::widgets::DataSetTableView::setDataSet(te::da::DataSet* dset)
{
  m_model->setDataSet(dset);

  std::vector<int> geoCols;
  std::vector<int>::iterator it;
  GetGeometryColumnsPositions(dset, geoCols);

  for(it = geoCols.begin(); it != geoCols.end(); ++it)
    hideColumn(*it);

  m_popupFilter->setDataSet(dset);
  m_delegate->setDataSet(dset);

  dset->moveFirst();
}

void te::qt::widgets::DataSetTableView::setLayerSchema(const te::da::DataSetType* schema)
{
  te::da::ObjectIdSet* objs = 0;

  te::da::GetEmptyOIDSet(schema, objs);

  m_delegate->setObjectIdSet(objs);

  m_model->setPkeysColumns(objs->getPropertyPos());
}

void te::qt::widgets::DataSetTableView::hideColumn(const int& column)
{
  horizontalHeader()->hideSection(column);
}

void te::qt::widgets::DataSetTableView::showColumn(const int& column)
{
  horizontalHeader()->showSection(column);
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
  if(!add)
    m_delegate->clearSelected();

  m_delegate->addObject(row);

  viewport()->repaint();
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

  m_delegate->addObjects(ini, final);

  viewport()->repaint();
}

void te::qt::widgets::DataSetTableView::promote()
{
  QCursor cursor(Qt::WaitCursor);

  std::vector<te::da::ObjectId*> oids = m_delegate->getSelected();
  m_model->promote(oids);

  m_delegate->setPromoter(m_model->getPromoter());

  viewport()->repaint();
}
