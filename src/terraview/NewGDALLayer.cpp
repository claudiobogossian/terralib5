#include "NewGDALLayer.h"

//! Terralib include files
#include <terralib/qt/widgets/utils/FileChooser.h>
#include <terralib/qt/widgets/srs/SRSManagerDialog.h>
#include <terralib/maptools/RasterLayer.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceCatalogLoader.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/raster.h>
#include <terralib/se.h>

#include <terralib/maptools/RasterLayerRenderer.h>

//! Qt include files
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QCheckBox>

NewGDALLayer::NewGDALLayer(QWidget* parent) :
QDialog(parent)
{
  makeDialog();

  setWindowTitle(tr("Add an GDAL layer"));
}

NewGDALLayer::~NewGDALLayer()
{
}

te::map::RasterLayer* NewGDALLayer::getNewLayer()
{
  return m_layer.release();
}

std::string NewGDALLayer::getDataSetRasterName(te::da::DataSource* ds, te::gm::Envelope*& extent) 
{
// get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = ds->getTransactor();

// from transactor, take a catalog loader to find out the datasets stored in the data source
  te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();
  cloader->loadCatalog();

// now retrieve the name of the datasets
  boost::ptr_vector<std::string> datasets;

  cloader->getDataSets(datasets);

  assert(!datasets.empty());

  std::string datasetName = datasets[0];

  // Gets DataSet Type
  te::da::DataSetType* dt = cloader->getDataSetType(datasets[0]);

  te::da::DataSet* dataSet = transactor->getDataSet(datasetName);

  te::rst::Raster* raster = dataSet->getRaster();

  extent = raster->getExtent();

  delete dt;

  delete cloader;

  delete transactor;

  return datasetName;
}

void NewGDALLayer::makeDialog()
{
  te::qt::widgets::FileChooser* fc = new te::qt::widgets::FileChooser(this);
  fc->setFilterPattern(tr("Image Files (*.tif *.TIF)"));

  connect(fc, SIGNAL(resourceSelected(QString)), this, SLOT(onFileSelected(QString)));

  QPushButton* okBtn = new QPushButton(tr("&Ok"), this);
  QPushButton* canBtn = new QPushButton(tr("&Cancel"), this);

  QPushButton* sridBtn = new QPushButton(tr("&SRID..."), this);
  m_sridLnEdt = new QLineEdit(this);
  m_sridLnEdt->setEnabled(false);

  QCheckBox* noValueChkBox = new QCheckBox(tr("Set No Value"), this);
  m_noValueLnEdt = new QLineEdit("0", this);
  m_noValueLnEdt->setEnabled(false);

  connect(noValueChkBox, SIGNAL(clicked(bool)), m_noValueLnEdt, SLOT(setEnabled(bool)));

  QHBoxLayout* hNoValueLay = new QHBoxLayout;
  hNoValueLay->addWidget(noValueChkBox);
  hNoValueLay->addWidget(m_noValueLnEdt);

  QHBoxLayout* hSRIDLay = new QHBoxLayout;
  hSRIDLay->addWidget(sridBtn);
  hSRIDLay->addWidget(m_sridLnEdt);

  QHBoxLayout* hLay = new QHBoxLayout;
  hLay->addWidget(okBtn);
  hLay->addWidget(canBtn);
  hLay->insertSpacing(0, 10);

  QVBoxLayout* vLay = new QVBoxLayout;
  vLay->addWidget(fc);
  vLay->addLayout(hNoValueLay);
  vLay->addLayout(hSRIDLay);
  vLay->addLayout(hLay);
  vLay->insertSpacing(1, 10);

  QGridLayout* grid = new QGridLayout(this);
  grid->addLayout(vLay, 0, 0, 1, 1);

  connect(okBtn, SIGNAL(pressed()), SLOT(onOkPushButtonClicked()));
  connect(canBtn, SIGNAL(pressed()), SLOT(reject()));
  connect(sridBtn, SIGNAL(pressed()), SLOT(showProjDlg()));
}

void NewGDALLayer::onFileSelected(QString s)
{
  QString f = s;
  QFileInfo info(f);

  if(!info.exists() || !info.isFile())
    QMessageBox::information(this, tr("Fail to open image file"), tr("Selected resource is not valid."));
  else
  {
    te::da::DataSourceManager& man = te::da::DataSourceManager::getInstance();
    QString id("ds_");
    id += QString::number(man.size());
    std::map<std::string, std::string> connInfo;
    connInfo["SOURCE"] = f.toLatin1().data();  

    te::da::DataSourcePtr ds = man.get(id.toLatin1().data(), "GDAL", connInfo);

    if(!ds->isOpened())
      ds->open(connInfo);
      
    if(ds->isValid() && ds->isOpened())
    {
      te::gm::Envelope* extent;

      std::string dset = getDataSetRasterName(ds.get(), extent);

      m_layer.reset(new te::map::RasterLayer(id.toLatin1().data(), info.baseName().toLatin1().data()));

      m_layer->setDataSource(ds.get());
      m_layer->setDataSetName(dset);

      m_layer->setRenderer(new te::map::RasterLayerRenderer());
      m_layer->setExtent(extent);

      //set srid information in interface
      QString sridStr;
      sridStr.setNum(m_layer->getSRID());

      m_sridLnEdt->setText(sridStr);
    }
  }
}

void NewGDALLayer::setNoValue(QString s)
{
  if(s.isEmpty())
    return;

  bool ok = false;

  double val = s.toDouble(&ok);

  if(ok)
  {
    if(m_layer.get())
    {
      te::rst::Raster* raster = m_layer->getRaster();

      for(size_t t = 0; t < raster->getNumberOfBands(); ++t)
      {
        raster->getBand(t)->getProperty()->m_noDataValue = val;
      }
    }
  }
}

void NewGDALLayer::showProjDlg()
{
  te::qt::widgets::SRSManagerDialog* srsManDialog = new te::qt::widgets::SRSManagerDialog();
  
  if(srsManDialog->exec() == QDialog::Accepted)
  {
    const std::pair<int, std::string>& srid = srsManDialog->getSelectedSRS();
    srid.first;

    QString sridStr;
    sridStr.setNum(srid.first);

    m_sridLnEdt->setText(sridStr);

    //set the new srid in raster layer
    if(m_layer.get())
    {
      m_layer->setSRID(srid.first);
    }
  }
}

void NewGDALLayer::onOkPushButtonClicked()
{
  //set no value
  if(m_noValueLnEdt->isEnabled())
  {
    setNoValue(m_noValueLnEdt->text());
  }

  accept();
}
