

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

//#include "terralib/postgis/DataSetTypePersistence.h"

#include "terralib/qt/widgets/property/AddProperty.h"
#include "terralib/qt/widgets/property/RenameProperty.h"
#include "terralib/qt/widgets/property/RemoveProperty.h"
#include "terralib/qt/widgets/property/UpdateProperty.h"
#include "terralib/qt/widgets/dataset/CreateDataSet.h"

// Application
#include "PropertyManagement.h"

//PropertyManagement* PropertyManagement::propertyManagementWindow = 0;

PropertyManagement::PropertyManagement(QWidget* parent)
  : QMainWindow(parent)
{
  setupUi(this);

  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br";
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "terralib4";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 

  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");
  ds->setConnectionInfo(connInfo);
  ds->open();
  m_ds = ds.get();

  m_dataSourceName = connInfo["PG_DB_NAME"];

  // Connect signal/slots
  connect(addPropertyPushButton, SIGNAL(clicked()), this, SLOT(addPropertyPushButtonClicked()));
  connect(removePropertyPushButton, SIGNAL(clicked()), this, SLOT(removePropertyPushButtonClicked()));
  connect(renamePropertyPushButton, SIGNAL(clicked()), this, SLOT(renamePropertyPushButtonClicked()));
  connect(updatePropertyPushButton, SIGNAL(clicked()), this, SLOT(updatePropertyPushButtonClicked()));
  connect(changePropertyDataPushButton, SIGNAL(clicked()), this, SLOT(changePropertyDataPushButtonClicked()));
  connect(quitPushButton, SIGNAL(clicked()), this, SLOT(close()));
}


void PropertyManagement::closeEvent(QCloseEvent* /*e*/)
{
  if(m_ds)
  {
    m_ds->close();
    delete m_ds;
  }
}

void PropertyManagement::addPropertyPushButtonClicked()
{
  te::dt::Property* p = 0;                  // property to be built
  te::da::DataSetType* propertyParent = 0;  // parent of the property to be built

  te::qt::widgets::AddProperty* addPropertyDialog = new te::qt::widgets::AddProperty(m_ds);

  if(addPropertyDialog->exec() != QDialog::Accepted)
  {
    delete addPropertyDialog;
    return;
  }

  // Add the property to its parent
  p = addPropertyDialog->getProperty();
  propertyParent = addPropertyDialog->getPropertyParent();

  te::da::DataSourceTransactor* t = m_ds->getTransactor();
  //te::da::DataSetTypePersistence* persistence = t->getDataSetTypePersistence();

  try
  {
    t->add(propertyParent, p);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(this, tr("Operation of Adding Property Failed"), e.what());
    delete t;
    delete addPropertyDialog;
    return;
  }

  QString propertyName = p->getName().c_str();
  QString propertyParentName = propertyParent->getName().c_str();

  QMessageBox::information(this, tr("Operation of Adding Property"),
    tr("The property \"%1\" was added to the data set \"%2\" successfully!").arg(propertyName).arg(propertyParentName));

  delete t;

  delete addPropertyDialog;
}

void PropertyManagement::removePropertyPushButtonClicked()
{
  te::dt::Property* p = 0;                  // property to be removed
  te::da::DataSetType* propertyParent = 0;  // parent of the property to be removed

  te::qt::widgets::RemoveProperty* removePropertyDialog = new te::qt::widgets::RemoveProperty(m_ds);

  if(removePropertyDialog->exec() != QDialog::Accepted)
  {
    delete removePropertyDialog;
    return;
  }

  // Remove the property
  p = removePropertyDialog->getProperty();
  propertyParent = removePropertyDialog->getPropertyParent();

  te::da::DataSourceTransactor* t = m_ds->getTransactor();
  te::da::DataSetTypePersistence* persistence = t->getDataSetTypePersistence();

  QString propertyName = p->getName().c_str();
  QString propertyParentName = propertyParent->getName().c_str();

  try
  {
    persistence->drop(p);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(this, tr("Operation of Removing Property Failed"), e.what());
    delete t;
    delete removePropertyDialog;
    return;
  }

  QMessageBox::information(this, tr("Operation of Removing Property"),
    tr("The property \"%1\" was removed from the data set \"%2\" successfully!").arg(propertyName).arg(propertyParentName));

  delete t;

  delete removePropertyDialog;
}

void PropertyManagement::renamePropertyPushButtonClicked()
{
  te::dt::Property* p = 0;                  // property to be removed
  te::da::DataSetType* propertyParent = 0;  // parent of the property to be removed

  te::qt::widgets::RenameProperty* renamePropertyDialog = new te::qt::widgets::RenameProperty(m_ds);

  if(renamePropertyDialog->exec() != QDialog::Accepted)
  {
    delete renamePropertyDialog;
    return;
  }

  // Change property name
  p = renamePropertyDialog->getProperty();
  propertyParent = renamePropertyDialog->getPropertyParent();
    
  te::da::DataSourceTransactor* t = m_ds->getTransactor();
  te::da::DataSetTypePersistence* persistence = t->getDataSetTypePersistence();

  QString oldPropertyName = renamePropertyDialog->getOldPropertyName().c_str();

  try
  {
    persistence->rename(p, renamePropertyDialog->getNewPropertyName());
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(this, tr("Operation of Changing Property Name Failed"), e.what());
    delete t;
    delete renamePropertyDialog;
    return;
  }

  QString newPropertyName = p->getName().c_str();
  QString propertyParentName = propertyParent->getName().c_str();

  QMessageBox::information(this, tr("Operation of Changing Property Name"),
    tr("The property \"%1\" from the data set \"%2\" was renamed to \"%3\" successfully!")
      .arg(oldPropertyName).arg(propertyParentName).arg(newPropertyName));

  delete t;

  delete renamePropertyDialog;
}

void PropertyManagement::updatePropertyPushButtonClicked()
{
  // Get the property to be updated and the replacing property
  te::dt::Property* oldProperty = 0;      // property to be updated
  te::dt::Property* updatedProperty = 0;  // updated property

  te::qt::widgets::UpdateProperty* updatePropertyDialog = new te::qt::widgets::UpdateProperty(m_ds);

  if(updatePropertyDialog->exec() != QDialog::Accepted)
  {
    delete updatePropertyDialog;
    return;
  }

  oldProperty = updatePropertyDialog->getOldProperty();
  updatedProperty = updatePropertyDialog->getUpdatedProperty();

  // Update property
  te::da::DataSourceTransactor* t = m_ds->getTransactor();
  te::da::DataSetTypePersistence* persistence = t->getDataSetTypePersistence();

  try
  {
    persistence->update(oldProperty, updatedProperty);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::critical(this, tr("Operation of Updating Property Failed"), e.what());
    delete t;
    delete updatePropertyDialog;
    return;
  }

  QMessageBox::information(this, tr("Operation of Updating Property"),
    tr("The property \"%1\" from the data set \"%2\" was updated successfully!")
      .arg(oldProperty->getName().c_str()).arg(oldProperty->getParent()->getName().c_str()));

  delete t;

  delete updatePropertyDialog;
}

void PropertyManagement::changePropertyDataPushButtonClicked()
{

}
