#include "TabularViewerEx.h"
#include "SelectPKey.h"
#include "HighlightedInfo.h"

//! TerraLib include files
#include <terralib/qt/widgets/utils/FileChooser.h>
#include <terralib/qt/widgets/dataview/TabularViewer.h>
#include <terralib/Config.h>
#include <terralib/plugin.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceCatalogLoader.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/common/STLUtils.h>

//! Qt include files
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QTabWidget>
#include <QLabel>
#include <QGroupBox>

te::da::DataSource* getDataSource(const std::string& src)
{
    te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");

    bool res = ds->isOpened();  // expect false;

    std::map<std::string, std::string> connInfo;
    connInfo["SOURCE"] = src ;  
    ds->open(connInfo);
    res = ds->isOpened();  // expect true
    res = ds->isValid();   // expect true

    return (res) ? ds : 0;
}

te::da::DataSet* getDataSet(te::da::DataSource* ds) 
{
// get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = ds->getTransactor();

// from transactor, take a catalog loader to find out the datasets stored in the data source
  te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();

// now retrieve the name of the datasets
  std::vector<std::string*> datasets;

  cloader->getDataSets(datasets);

  const std::string* datasetName = *datasets.begin();

// retrieve the dataset by its name
  te::da::DataSet* dataset = transactor->getDataSet(*datasetName);

  delete cloader;

// release the transactor: you are the owner
  delete transactor;

  return dataset;
}

void loadModules()
{
  try
  {
    {
      te::plugin::PluginInfo info;

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      info.m_type = "dll";
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
      info.m_type = "s.o.";
#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
      info.m_type = "dylib";      
#else
  #error "Platform not supported yet"
#endif

      info.m_name = "OGR DataSource Driver";
      info.m_description = "This data source driver supports...";

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
      info.m_mainFile = "terralib_ogr.dll";
#else
      info.m_mainFile = "terralib_ogr_d.dll";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX
#ifdef NDEBUG
      info.m_mainFile = "libterralib_ogr.so";
#else
      info.m_mainFile = "libterralib_ogr_d.so";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
#ifdef NDEBUG
      info.m_mainFile = "libterralib_ogr.dylib";
#else
      info.m_mainFile = "libterralib_ogr.dylib";
#endif
#endif

      te::plugin::PluginManager::getInstance().loadPlugin(info);
    }
  }
  catch(const te::common::Exception& e)
  {
    throw e;
  }
}

//! Function for initialize the TabularViewerEx dialog.
void initDialog(TabularViewerEx* wd, QTabWidget* tab, te::qt::widgets::FileChooser*& fc, te::qt::widgets::TabularViewer*& tv, SelectPKey*& pkeySel)
{
  QWidget* wid = new QWidget(wd);
  pkeySel = new SelectPKey(wd);

  fc = new te::qt::widgets::FileChooser(wd);
  tv = new te::qt::widgets::TabularViewer(wd);
  QPushButton* btn = new QPushButton(QObject::tr("Update viewer"), wd);
//  QPushButton* btn2 = new QPushButton(QObject::tr("Promote highlighted"), wd);
//  QPushButton* btn3 = new QPushButton(QObject::tr("Remove promotion"), wd);
//  QPushButton* btn4 = new QPushButton(QObject::tr("Reset highlighted"), wd);

  QVBoxLayout* vlay = new QVBoxLayout;
//  QHBoxLayout* hlay = new QHBoxLayout;
  QGridLayout* grdLay = new QGridLayout(wid);

//  hlay->addWidget(btn);
 // hlay->addWidget(btn2);
 // hlay->addWidget(btn3);
 // hlay->addWidget(btn4);

  vlay->addWidget(fc);
  vlay->addWidget(btn);
  vlay->addWidget(tv);

  grdLay->addLayout(vlay, 0, 0, 1, 1);

  QGridLayout* grdLay2 = new QGridLayout(wd);

  grdLay2->addWidget(tab, 0, 0, 1, 1);

  tab->addTab(wid, QObject::tr("Data viewer"));
  tab->addTab(pkeySel, QObject::tr("Primary keys"));
  tab->addTab(new HighlightedInfo(tv, wd), QObject::tr("Query by primary key"));

  tab->setTabEnabled(1, false);
  tab->setTabEnabled(2, false);

  //! Connecting slots
  tv->connect(pkeySel, SIGNAL(pkeysChanged(const std::vector<size_t>&)), SLOT(setPrimaryKeys(const std::vector<size_t>&)));
//  tv->connect(btn2, SIGNAL(clicked()), SLOT(promoteHighlighted()));
//  tv->connect(btn3, SIGNAL(clicked()), SLOT(resetPromote()));
//  tv->connect(btn4, SIGNAL(clicked()), SLOT(resetHighlights()));

  wd->connect(btn, SIGNAL(pressed()), SLOT(updateViewer()));
  wd->connect(pkeySel, SIGNAL(pkeysChanged(const std::vector<size_t>&)), SLOT(pkeysChanged(const std::vector<size_t>&)));
}

TabularViewerEx::TabularViewerEx(QWidget* parent) :
QDialog(parent),
m_dsrc(0),
m_tab(new QTabWidget(this))
{
  loadModules();

  initDialog(this, m_tab, m_fchooser, m_viewer, m_pkey_sel);

  QDialog::setWindowTitle(tr("Shapefile tabular viewer"));

  //! Adding filter for shape files
  m_fchooser->setFilterPattern(tr("Shape Files (*.shp *.SHP)"));
}

TabularViewerEx::~TabularViewerEx()
{
  delete m_dsrc;
}

void TabularViewerEx::updateViewer()
{
  QString fname = m_fchooser->getSelectedFile();

  if(fname.isEmpty())
  {
    QMessageBox::warning(this, tr("Example warning"), tr("There's no file selected."));
    return;
  }

  if(m_dsrc != 0)
  {
    m_dsrc->close();
    delete m_dsrc;
  }

  m_dsrc = getDataSource(fname.toLatin1().data());

  if(m_dsrc != 0)
  {
    te::da::DataSet* ds = getDataSet(m_dsrc);
    m_viewer->showData(ds);
    m_pkey_sel->updateColumns(ds);

    m_tab->setTabEnabled(1, true);
    m_tab->setTabEnabled(2, false);
  }
}

void TabularViewerEx::pkeysChanged(const std::vector<size_t>& ids)
{
  bool idsEmpty = ids.empty();

  if(!ids.empty())
    m_tab->setTabEnabled(2, true);
}
