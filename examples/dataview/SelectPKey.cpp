#include "SelectPKey.h"  

#include <ui_SelectPKey.h>

//TerraLib include files
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/datatype/Property.h>

//Qt include files
#include <QCheckBox>


void updateChecks(QTableWidget* tab, const bool& status)
{
  int nrows = tab->rowCount();

  for(int i=0; i<nrows; i++)
    ((QCheckBox*)tab->cellWidget(i, 1))->setChecked(status);
}

SelectPKey::SelectPKey(QWidget* parent) :
QWidget(parent),
  m_ui(new Ui::SelectPKey)
{
  m_ui->setupUi(this);
}

SelectPKey::~SelectPKey()
{
  delete m_ui;
}

void SelectPKey::on_m_selectAllPushButton_clicked()
{
  updateChecks(m_ui->m_columnsTable, true);
}

void SelectPKey::on_m_unselectAllPushButton_clicked()
{
  updateChecks(m_ui->m_columnsTable, false);
}

void SelectPKey::on_m_updateKeysPushButton_clicked()
{
  int nrows = m_ui->m_columnsTable->rowCount();
  std::vector<size_t> pkeys;

  for(int i=0; i<nrows; i++)
    if(((QCheckBox*)m_ui->m_columnsTable->cellWidget((int)i, 1))->isChecked())
      pkeys.push_back(i);

  if(!pkeys.empty())
    emit pkeysChanged(pkeys);
}

void SelectPKey::updateColumns(te::da::DataSet* dset)
{
  m_ui->m_columnsTable->clearContents();
  m_ui->m_columnsTable->setRowCount(0);

  te::da::DataSetType* type = dset->getType();
  std::vector<te::dt::Property*> props = type->getProperties();
  std::vector<te::dt::Property*>::iterator it;

  for(it=props.begin(); it!= props.end(); ++it)
  {
    te::dt::Property* prop = *it;

    if(prop->getType() != te::dt::GEOMETRY_TYPE)
    {
      int nRows = m_ui->m_columnsTable->rowCount();
      m_ui->m_columnsTable->insertRow(nRows);

      m_ui->m_columnsTable->setItem(nRows, 0, new QTableWidgetItem(prop->getName().c_str()));
      m_ui->m_columnsTable->setCellWidget(nRows, 1, new QCheckBox(tr("primary key"), this));
    }
  }
}
