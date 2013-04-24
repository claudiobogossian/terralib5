#include "DataSetTableView.h"
#include "DataSetTableModel.h"

// TerraLib include files
#include "../../../maptools/AbstractLayer.h"
#include "../../../dataaccess/dataset/DataSet.h"

// Qt
#include <QtGui/QHeaderView>

// STL
#include <vector>

void GetGeometryColumnsPositions(te::da::DataSet* dset, std::vector<int>& cols)
{
  size_t nProps = dset->getNumProperties();

  cols.clear();

  for(size_t i=0; i<nProps; i++)
    if(dset->getPropertyDataType(i) == te::dt::GEOMETRY_TYPE)
      cols.push_back((int)i);
}

te::qt::widgets::DataSetTableView::DataSetTableView(QWidget* parent) :
QTableView(parent)
{
  m_model = new DataSetTableModel(this);
  setModel(m_model);

  horizontalHeader()->setMovable(true);
  setSelectionMode(QAbstractItemView::NoSelection);
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
}
