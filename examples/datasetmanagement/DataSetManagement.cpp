

// Qt
#include <QtGui>
#include <QComboBox>
#include <QMessageBox>

// TerraLib
#include "terralib/dataaccess/dataset/DataSetType.h"
#include "terralib/dataaccess/datasource/DataSource.h"
#include "terralib/dataaccess/datasource/DataSourceFactory.h"
//#include "terralib/dataaccess/datasource/DataSourceCatalogLoader.h"
#include "terralib/dataaccess/datasource/DataSourceTransactor.h"
#include "terralib/dataaccess/dataset/Index.h"

//#include "terralib/postgis/DataSetTypePersistence.h"

#include "terralib/qt/widgets/dataset/AddIndex.h"
#include "terralib/qt/widgets/dataset/CreateDataSet.h"

// Application
#include "DataSetManagement.h"


DataSetManagement::DataSetManagement(QWidget* parent)
  : QMainWindow(parent)
{
  setupUi(this);

  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "terralib4";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252";     // "LATIN1";
 
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");
  ds->setConnectionInfo(connInfo);
  ds->open();
  m_ds = ds.get();

  m_dataSourceName = connInfo["PG_DB_NAME"];

  // Connect signal/slots
  connect(createDataSetPushButton, SIGNAL(clicked()), this, SLOT(createDataSetPushButtonClicked()));
  connect(removeDataSetPushButton, SIGNAL(clicked()), this, SLOT(removeDataSetPushButtonClicked()));
  connect(renameDataSetPushButton, SIGNAL(clicked()), this, SLOT(renameDataSetPushButtonClicked()));
  connect(addPrimaryKeyPushButton, SIGNAL(clicked()), this, SLOT(addPrimaryKeyPushButtonClicked()));
  connect(removePrimaryKeyPushButton, SIGNAL(clicked()), this, SLOT(removePrimaryKeyPushButtonClicked()));
  connect(addForeignKeyPushButton, SIGNAL(clicked()), this, SLOT(addForeignKeyPushButtonClicked()));
  connect(removeForeignKeyPushButton, SIGNAL(clicked()), this, SLOT(removeForeignKeyPushButtonClicked()));
  connect(addUniqueKeyPushButton, SIGNAL(clicked()), this, SLOT(addUniqueKeyPushButtonClicked()));
  connect(removeUniqueKeyPushButton, SIGNAL(clicked()), this, SLOT(removeUniqueKeyPushButtonClicked()));
  connect(addCheckConstraintPushButton, SIGNAL(clicked()), this, SLOT(addCheckConstraintPushButtonClicked()));
  connect(removeCheckConstraintPushButton, SIGNAL(clicked()), this, SLOT(removeCheckConstraintPushButtonClicked()));
  connect(addIndexPushButton, SIGNAL(clicked()), this, SLOT(addIndexPushButtonClicked()));
  connect(removeIndexPushButton, SIGNAL(clicked()), this, SLOT(removeIndexPushButtonClicked()));
  connect(addSequencePushButton, SIGNAL(clicked()), this, SLOT(addSequencePushButtonClicked()));
  connect(removeSequencePushButton, SIGNAL(clicked()), this, SLOT(removeSequencePushButtonClicked()));

  connect(quitPushButton, SIGNAL(clicked()), this, SLOT(close()));
}

void DataSetManagement::createDataSetPushButtonClicked()
{
  // Get the dataset to be created
  te::da::DataSetType* dataSetType = 0;   // dataset to be created

  te::qt::widgets::CreateDataSet* createDataSetDialog = new te::qt::widgets::CreateDataSet(m_ds);

  if(createDataSetDialog->exec() != QDialog::Accepted)
  {
    delete createDataSetDialog;
    return;
  }

  dataSetType = createDataSetDialog->getDataSetType();

  // Create data set
  std::auto_ptr<te::da::DataSourceTransactor> t = m_ds->getTransactor();
  //std::auto_ptr<te::da::DataSetTypePersistence* persistence = t->getDataSetTypePersistence();

  try
  {
    std::map<std::string, std::string> options;
    t->createDataSet(dataSetType, options);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(this, tr("Operation of Creating a Data Set Failed"), e.what());
    delete createDataSetDialog;
    return;
  }

  QMessageBox::information(this, tr("Operation of Creating a Data Set"),
    tr("The data set \"%1\" was created successfully in the data source \"%2\"!")
      .arg(dataSetType->getName().c_str()).arg(m_dataSourceName.c_str()));


  delete createDataSetDialog;
}

void DataSetManagement::removeDataSetPushButtonClicked()
{

}

void DataSetManagement::renameDataSetPushButtonClicked()
{

}


void DataSetManagement::addPrimaryKeyPushButtonClicked()
{

}

void DataSetManagement::removePrimaryKeyPushButtonClicked()
{

}


void DataSetManagement::addForeignKeyPushButtonClicked()
{

}

void DataSetManagement::removeForeignKeyPushButtonClicked()
{

}


void DataSetManagement::addUniqueKeyPushButtonClicked()
{

}

void DataSetManagement::removeUniqueKeyPushButtonClicked()
{

}


void DataSetManagement::addCheckConstraintPushButtonClicked()
{

}

void DataSetManagement::removeCheckConstraintPushButtonClicked()
{

}


void DataSetManagement::addIndexPushButtonClicked()
{
  te::da::Index* index = 0;         // Index to be created
  te::da::DataSetType* dataSetType; // The data set type associated to the index

  te::qt::widgets::AddIndex* addIndexDialog = new te::qt::widgets::AddIndex(m_ds);

  if(addIndexDialog->exec() != QDialog::Accepted)
  {
    delete addIndexDialog;
    return;
  }

  // Get the data set type
  dataSetType = addIndexDialog->getDataSetType();

  // Get the index
  index = addIndexDialog->getIndex();

  // Add the index to the data set type
  std::auto_ptr<te::da::DataSourceTransactor> t = m_ds->getTransactor();

  try
  {
    std::map<std::string, std::string> options;
    t->addIndex(dataSetType->getName(), index,options);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(this, tr("Operation of Adding an Index Failed"), e.what());
    delete addIndexDialog;
    return;
  }

  QMessageBox::information(this, tr("Operation of Adding an Index"),
    tr("The index \"%1\" was added successfully to the data set \"%2\"!")
      .arg(index->getName().c_str()).arg(dataSetType->getName().c_str()));


  delete addIndexDialog;
}

void DataSetManagement::removeIndexPushButtonClicked()
{

}


void DataSetManagement::addSequencePushButtonClicked()
{

}

void DataSetManagement::removeSequencePushButtonClicked()
{

}


void DataSetManagement::closeEvent(QCloseEvent* /*e*/)
{
  if(m_ds)
  {
    m_ds->close();
    delete m_ds;
  }
}

