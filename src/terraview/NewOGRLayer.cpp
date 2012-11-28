#include "NewOGRLayer.h"

//! Terralib include files
#include <terralib/qt/widgets/utils/FileChooser.h>
#include <terralib/qt/widgets/srs/SRSManagerDialog.h>
#include <terralib/maptools/Layer.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceCatalogLoader.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/se.h>

#include <terralib/maptools/LayerRenderer.h>

//! Qt include files
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileInfo>
#include <QMessageBox>

NewOGRLayer::NewOGRLayer(QWidget* parent) :
QDialog(parent)
{
  makeDialog();

  setWindowTitle(tr("Add an OGR layer"));
}

NewOGRLayer::~NewOGRLayer()
{
}

te::map::Layer* NewOGRLayer::getNewLayer()
{
  return m_layer.release();
}

std::string NewOGRLayer::getDataSetName(te::da::DataSource* ds, te::gm::Envelope*& box) 
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

  // Gets DataSet Type
  te::da::DataSetType* dt = cloader->getDataSetType(datasets[0]);

  // Default geometry property
  te::gm::GeometryProperty* geomProperty = dt->getDefaultGeomProperty();

  std::string datasetName = datasets[0];

//  te::da::DataSet* dataset = transactor->getDataSet(datasetName);

  box = cloader->getExtent(geomProperty);

  delete dt;

  delete cloader;

  delete transactor;

  return datasetName;
}

void NewOGRLayer::makeDialog()
{
  te::qt::widgets::FileChooser* fc = new te::qt::widgets::FileChooser(this);
  fc->setFilterPattern(tr("Shape Files (*.shp *.SHP)"));

  connect(fc, SIGNAL(resourceSelected(QString)), this, SLOT(onFileSelected(QString)));

  QPushButton* ok_btn = new QPushButton(tr("&Ok"), this);
  QPushButton* can_btn = new QPushButton(tr("&Cancel"), this);

  QPushButton* sridBtn = new QPushButton(tr("&SRID..."), this);
  m_sridLnEdt = new QLineEdit(this);
  m_sridLnEdt->setEnabled(false);

  QHBoxLayout* hSRIDLay = new QHBoxLayout;
  hSRIDLay->addWidget(sridBtn);
  hSRIDLay->addWidget(m_sridLnEdt);

  QHBoxLayout* hLay = new QHBoxLayout;
  hLay->addWidget(ok_btn);
  hLay->addWidget(can_btn);
  hLay->insertSpacing(0, 10);

  QVBoxLayout* vLay = new QVBoxLayout;
  vLay->addWidget(fc);
  vLay->addLayout(hSRIDLay);
  vLay->addLayout(hLay);
  vLay->insertSpacing(1, 10);

  QGridLayout* grid = new QGridLayout(this);
  grid->addLayout(vLay, 0, 0, 1, 1);

  connect(ok_btn, SIGNAL(pressed()), SLOT(accept()));
  connect(can_btn, SIGNAL(pressed()), SLOT(reject()));
  connect(sridBtn, SIGNAL(pressed()), SLOT(showProjDlg()));
}

void NewOGRLayer::onFileSelected(QString s)
{
  QString f = s;
  QFileInfo info(f);

  if(!info.exists() || !info.isFile())
    QMessageBox::information(this, tr("Fail to open shape file"), tr("Selected resource is not valid."));
  else
  {
    te::da::DataSourceManager& man = te::da::DataSourceManager::getInstance();
    QString id("ds_");
    id += QString::number(man.size());
    std::map<std::string, std::string> connInfo;
    connInfo["SOURCE"] = f.toLatin1().data();  

    te::da::DataSourcePtr ds = man.get(id.toLatin1().data(), "OGR", connInfo);

    if(!ds->isOpened())
      ds->open(connInfo);
      
    if(ds->isValid() && ds->isOpened())
    {
      te::gm::Envelope* env = 0;

      std::string dset = getDataSetName(ds.get(), env);

      m_layer.reset(new te::map::Layer(id.toLatin1().data(), info.baseName().toLatin1().data()));

      m_layer->setDataSetName(dset);
      m_layer->setDataSource(ds.get());

      m_layer->setRenderer(new te::map::LayerRenderer());
      m_layer->setExtent(env);
    }
  }
}

void NewOGRLayer::showProjDlg()
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

