#include "SelectLayer.h"

//TerraLib
#include <terralib/dataaccess.h>

//QT
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QAction>
#include <QMessageBox>

SelectLayer::SelectLayer(te::da::DataSource* ds, QWidget* parent, Qt::WindowFlags f) : 
  QDialog(parent, f)
{
  setWindowTitle("Add Layer");
  QVBoxLayout* vlayout = new QVBoxLayout(this);
  QLabel* label1 = new QLabel("Data Source Type Name:", this);
  m_dataSourceTypeComboBox = new QComboBox(this);
  m_dataSourceTypeComboBox->setMinimumWidth(100);
  QHBoxLayout* layout1 = new QHBoxLayout(this);
  layout1->addWidget(label1);
  layout1->addWidget(m_dataSourceTypeComboBox);
  layout1->addStretch();
  vlayout->addLayout(layout1);

  QLabel* label2 = new QLabel("Connection String:", this);
  m_connectionStringLineEdit = new QLineEdit(this);
  QPushButton* editedButton = new QPushButton("CR", this);
  QHBoxLayout* layout2 = new QHBoxLayout(this);
  layout2->addWidget(label2);
  layout2->addWidget(m_connectionStringLineEdit);
  layout2->addWidget(editedButton);
  vlayout->addLayout(layout2);

  QLabel* label3 = new QLabel("Layer Name:", this);
  m_layerNameComboBox = new QComboBox(this);
  m_layerNameComboBox->setMinimumWidth(100);
  QHBoxLayout* layout3 = new QHBoxLayout(this);
  layout3->addWidget(label3);
  layout3->addWidget(m_layerNameComboBox);
  layout3->addStretch();
  vlayout->addLayout(layout3);

  QGroupBox* gb = new QGroupBox(this);
  m_okPushButton = new QPushButton("Ok", gb);
  m_cancelPushButton = new QPushButton("Cancel", gb);
  QHBoxLayout* layout4 = new QHBoxLayout(gb);
  layout4->addStretch();
  layout4->addWidget(m_okPushButton);
  layout4->addStretch();
  layout4->addWidget(m_cancelPushButton);
  layout4->addStretch();
  vlayout->addWidget(gb);

  setLayout(vlayout);

  QObject::connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(okSlot()));
  QObject::connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));
  QObject::connect(m_dataSourceTypeComboBox, SIGNAL(activated (int)), this, SLOT(dataSourceChangedSlot(int)));
  QObject::connect(m_connectionStringLineEdit, SIGNAL(editingFinished()), this, SLOT(connectionStringEditedSlot()));

  populeWidgets(ds);

  show();
  setFixedHeight(height());
}

SelectLayer::~SelectLayer()
{
  delete m_dataSourceTypeComboBox;
  delete m_connectionStringLineEdit;
  delete m_layerNameComboBox;
  delete m_okPushButton;
  delete m_cancelPushButton;
}

void SelectLayer::populeWidgets(te::da::DataSource* ds)
{
  m_dataSourceTypeComboBox->addItem("POSTGIS");
  m_dataSourceTypeComboBox->addItem("SQLITE");
  m_dataSourceTypeComboBox->addItem("WFS");
  m_dataSourceTypeComboBox->addItem("WMS");
  m_dataSourceTypeComboBox->addItem("MYSQL");
  m_dataSourceTypeComboBox->addItem("ORACLE_SPATIAL");
  m_dataSourceTypeComboBox->addItem("SHP");
  m_dataSourceTypeComboBox->addItem("MICROSOFT_ACCESS");
  m_dataSourceTypeComboBox->addItem("OGR");
  m_dataSourceTypeComboBox->addItem("GDAL");
  m_dataSourceTypeComboBox->addItem("FDO");
  if(ds)
  {
    QString type = ds->getType().c_str();
    int ind = m_dataSourceTypeComboBox->findText(type);
    m_dataSourceTypeComboBox->setCurrentIndex(ind);

    QString s = ds->getConnectionStr().c_str();
    m_connectionStringLineEdit->setText(s);

    te::da::DataSourceCatalog* catalog = ds->getCatalog();
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();
    std::vector<std::string*> names;
    loader->getDataSets(names);
    int size = names.size();
    QStringList items;
    for(int i = 0; i < size; ++i)
    {
      std::string& id = (*names[i]);
      QString s = id.c_str();
      m_layerNameComboBox->addItem(s);
    }
  }
}

void SelectLayer::dataSourceChangedSlot(int)
{
  m_connectionStringLineEdit->clear();
  m_layerNameComboBox->clear();
}

void SelectLayer::connectionStringEditedSlot()
{
  if(m_connectionStringLineEdit->text().isEmpty())
    return;

  try
  {
    m_layerNameComboBox->clear();

    std::string dstype = m_dataSourceTypeComboBox->currentText().toStdString();
    te::da::DataSource* ds = te::da::DataSourceFactory::make(dstype);
    std::string cs = m_connectionStringLineEdit->text().toStdString();

    std::string dsInfo;
    if(dstype == "GDAL")
      dsInfo = "URI=" + cs;
    else
    dsInfo = "connection_string=" + cs;
    ds->open(dsInfo);

    te::da::DataSourceCatalog* catalog = ds->getCatalog();
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();
    std::vector<std::string*> names;
    loader->getDataSets(names);
    int size = names.size();
    QStringList items;
    for(int i = 0; i < size; ++i)
    {
      std::string& id = (*names[i]);
      QString s = id.c_str();
      m_layerNameComboBox->addItem(s);
    }

    delete loader;
    delete transactor;
    delete ds;
  }
  catch(const te::common::Exception& e)
  {
    QMessageBox::information(this, tr("Connection String Error"), tr(e.what()));
    return;
  }
}

void SelectLayer::okSlot()
{
  accept();
  hide();
}

void SelectLayer::cancelSlot()
{
  reject();
  hide();
}
