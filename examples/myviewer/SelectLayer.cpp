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

  QGroupBox* b1 = new QGroupBox(this);
  QLabel* label1 = new QLabel("Data Source Type Name:", b1);
  m_dataSourceTypeComboBox = new QComboBox(b1);
  m_dataSourceTypeComboBox->setMinimumWidth(100);
  QHBoxLayout* layout1 = new QHBoxLayout(b1);
  layout1->addWidget(label1);
  layout1->addWidget(m_dataSourceTypeComboBox);
  layout1->addStretch();
  vlayout->addWidget(b1);

  QGroupBox* b2 = new QGroupBox(this);
  QLabel* label2 = new QLabel("Connection String:", b2);
  m_connectionStringLineEdit = new QLineEdit(b2);
  //QPushButton* editedButton = new QPushButton("CR", b2);
  QHBoxLayout* layout2 = new QHBoxLayout(b2);
  layout2->addWidget(label2);
  layout2->addWidget(m_connectionStringLineEdit);
  //layout2->addWidget(editedButton);
  vlayout->addWidget(b2);

  QGroupBox* b3 = new QGroupBox(this);
  QLabel* label3 = new QLabel("Layer Name:", b3);
  m_layerNameComboBox = new QComboBox(b3);
  m_layerNameComboBox->setMinimumWidth(200);
  QHBoxLayout* layout3 = new QHBoxLayout(b3);
  layout3->addWidget(label3);
  layout3->addWidget(m_layerNameComboBox);
  layout3->addStretch();
  vlayout->addWidget(b3);

  QGroupBox* b4 = new QGroupBox(this);
  QLabel* label4 = new QLabel("Title Name:", b4);
  m_titleNameLineEdit = new QLineEdit(b4);
  m_titleNameLineEdit->setMinimumWidth(100);
  QHBoxLayout* layout4 = new QHBoxLayout(b4);
  layout4->addWidget(label4);
  layout4->addWidget(m_titleNameLineEdit);
  layout4->addStretch();
  vlayout->addWidget(b4);

  QGroupBox* b5 = new QGroupBox(this);
  m_okPushButton = new QPushButton("Ok", b5);
  m_cancelPushButton = new QPushButton("Cancel", b5);
  QHBoxLayout* layout5 = new QHBoxLayout(b5);
  layout5->addStretch();
  layout5->addWidget(m_okPushButton);
  layout5->addStretch();
  layout5->addWidget(m_cancelPushButton);
  layout5->addStretch();
  vlayout->addWidget(b5);

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

//    QString s = ds->getConnectionStr().c_str();
//    m_connectionStringLineEdit->setText(s);
    m_connectionStringLineEdit->setText("host=atlas.dpi.inpe.br&port=5432&dbname=terralib4&user=postgres&password=sitim110&connect_timeout=20&MaxPoolSize=15");

    te::da::DataSourceCatalog* catalog = ds->getCatalog();
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();
    boost::ptr_vector<std::string> names;
    loader->getDataSets(names);
    int size = names.size();
    QStringList items;
    for(int i = 0; i < size; ++i)
    {
      std::string& id = (names[i]);
      QString s = id.c_str();
      m_layerNameComboBox->addItem(s);
    }
  }
}

void SelectLayer::dataSourceChangedSlot(int)
{
  m_connectionStringLineEdit->clear();
  m_layerNameComboBox->clear();
    m_titleNameLineEdit->clear();
}

void SelectLayer::connectionStringEditedSlot()
{
  if(m_connectionStringLineEdit->text().isEmpty() || m_connectionWithError.isEmpty() == false)
  {
    m_connectionStringLineEdit->setText(m_connectionWithError);
    m_connectionWithError.clear();
    return;
  }

  try
  {
    m_layerNameComboBox->clear();

    std::string dstype = m_dataSourceTypeComboBox->currentText().toStdString();
    te::da::DataSource* ds = te::da::DataSourceFactory::make(dstype);
    m_connectionStringLineEdit->setText(m_connectionStringLineEdit->text().remove(QChar(' ')));
    std::string cs = m_connectionStringLineEdit->text().toStdString();
    m_connectionWithError = m_connectionStringLineEdit->text();
    std::string dsInfo;
    //if(dstype == "GDAL")
    //  dsInfo = "URI=" + cs;
    //else
    //dsInfo = "connection_string=" + cs;
    if(dstype == "GDAL")
    {
      int p = cs.find("host=");
      if(p != std::string::npos)
        cs.replace(p, 5, "URI=");
    }
    dsInfo = cs;
    ds->open(dsInfo);
    
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();
    boost::ptr_vector<std::string> names;
    loader->getDataSets(names);
    int size = names.size();
    QStringList items;
    for(int i = 0; i < size; ++i)
    {
      std::string& id = (names[i]);
      QString s = id.c_str();
      m_layerNameComboBox->addItem(s);
    }

    delete loader;
    delete transactor;
    delete ds;
    //m_connectionWithError.clear();
  }
  catch(const te::common::Exception& e)
  {
    // O catch faz chamar este slot (connectionStringEditedSlot()) novamente
    m_connectionStringLineEdit->clear();
    QMessageBox::information(this, tr("Connection String Error"), tr(e.what()));
  }
}

void SelectLayer::okSlot()
{
  if(m_layerNameComboBox->count() == 0)
    return;

  if(m_connectionWithError.isEmpty() == false && m_titleNameLineEdit->text().isEmpty())
  {
    m_connectionWithError.clear();
    return;
  }

  if(m_titleNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Title Name is empty"), tr("Type Title Name and Continue"));
    return;
  }
  accept();
  hide();
}

void SelectLayer::cancelSlot()
{
  reject();
  hide();
}
