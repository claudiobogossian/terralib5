#include "TabularViewerEx.h"
#include "SelectPKey.h"
#include "HighlightedInfo.h"
#include "StarDelegate.h"

//! TerraLib include files
#include <terralib/qt/widgets/utils/FileChooser.h>
#include <terralib/qt/widgets/dataview/TabularViewer.h>
#include <terralib/Config.h>

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
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDir>

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
  boost::ptr_vector<std::string> datasets;

  cloader->getDataSets(datasets);

// retrieve the dataset by its name
  te::da::DataSet* dataset = transactor->getDataSet(datasets[0]);

  delete cloader;

// release the transactor: you are the owner
  delete transactor;

  return dataset;
}

//! Function for initialize the TabularViewerEx dialog.
void initDialog(TabularViewerEx* wd, QTabWidget* tab, te::qt::widgets::FileChooser*& fc, te::qt::widgets::TabularViewer*& tv, SelectPKey*& pkeySel)
{
  QMenuBar* mbar= new QMenuBar(wd);
  QMenu* file_mnu = new QMenu(QObject::tr("File"), mbar);
  QAction* act = new QAction(QObject::tr("Change icons theme..."), file_mnu);

  file_mnu->addAction(act);
  mbar->addMenu(file_mnu);
  wd->setMenuBar(mbar);

  QWidget* wid = new QWidget(wd);
  pkeySel = new SelectPKey(wd);

  fc = new te::qt::widgets::FileChooser(wd);
  tv = new te::qt::widgets::TabularViewer(wd);
  QPushButton* btn = new QPushButton(QObject::tr("Update viewer"), wd);
  QVBoxLayout* vlay = new QVBoxLayout;
  QGridLayout* grdLay = new QGridLayout(wid);

  //! [Adding custom delegate decorator]
  te::qt::widgets::HighlightDelegate* del = (te::qt::widgets::HighlightDelegate*)tv->itemDelegate();

  // Decorate the clone of delegate
  StarDelegate* st = new StarDelegate(del->clone());

  // Setting up color
  st->setHighlightColor(QColor(255, 255, 0));

  //Setting up the new delegate
  st->setParent(tv);
  tv->setItemDelegate(st);

  // Update menus
  tv->updatePopupMenus();

  delete del;
  //! [Adding custom delegate decorator]

  vlay->addWidget(fc);
  vlay->addWidget(btn);
  vlay->addWidget(tv);

  grdLay->addLayout(vlay, 0, 0, 1, 1);

  tab->addTab(wid, QObject::tr("Data viewer"));
  tab->addTab(pkeySel, QObject::tr("Primary keys"));
  tab->addTab(new HighlightedInfo(tv, wd), QObject::tr("Query by primary key"));

  tab->setTabEnabled(1, false);
  tab->setTabEnabled(2, false);

  wd->setCentralWidget(tab);

  //! Connecting slots
  wd->connect(btn, SIGNAL(pressed()), SLOT(updateViewer()));
  tv->connect(pkeySel, SIGNAL(pkeysChanged(const std::vector<size_t>&)), SLOT(setPrimaryKeys(const std::vector<size_t>&)));
  wd->connect(pkeySel, SIGNAL(pkeysChanged(const std::vector<size_t>&)), SLOT(pkeysChanged(const std::vector<size_t>&)));
  wd->connect(act, SIGNAL(triggered()), SLOT(changeThemeName()));
}

TabularViewerEx::TabularViewerEx(QWidget* parent) :
QMainWindow(parent),
m_dsrc(0),
m_tab(new QTabWidget(this))
{
  initDialog(this, m_tab, m_fchooser, m_viewer, m_pkey_sel);

  QMainWindow::setWindowTitle(tr("Shapefile tabular viewer"));

  QString filter(tr("Shape Files (*.shp *.SHP)"));

  //! Adding filter for shape files
  m_fchooser->setFilterPattern(filter);
}

TabularViewerEx::~TabularViewerEx()
{
  delete m_dsrc;
}

void TabularViewerEx::updateViewer()
{
  QString fname = m_fchooser->getSelectedResource();

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

  m_dsrc = getDataSource(fname.toUtf8().data());

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
  if(!ids.empty())
    m_tab->setTabEnabled(2, true);
}

void TabularViewerEx::setThemeName(const QString& tName)
{
  QIcon::setThemeName(tName);
  QMainWindow::update();
}

void TabularViewerEx::changeThemeName()
{
  bool ok;
  QString text = QInputDialog::getText(this, tr("Enter the name of the new icon theme."), tr("Icon theme name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);

  if (ok && !text.isEmpty())
   setThemeName(text);
}
