#include "MyDisplay.h"
#include "MyLayer.h"
#include "MyLayerRenderer.h"
#include "PrintInFile.h"
#include <terralib/common/PlatformUtils.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/raster.h>
#include <terralib/geometry.h>
#include <terralib/qt/widgets/utils/ScopedCursor.h>
#include <terralib/srs/Converter.h>
#include <terralib/srs/Config.h>

#include "STExamples.h"

//QT
#include <QtGui/QInputDialog>
#include <QTimer>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QHBoxLayout>
#include <QIcon>
#include <QMessageBox>
#include <QString>
#include <QMatrix>
#include <QRectF>
#include <QPrintDialog>
#include <QPrinter>
#include <QUrl>
#include <QToolTip>

MyDisplay::MyDisplay(int w, int h, te::map::AbstractLayer* root, QWidget* parent, Qt::WindowFlags f) :
  te::qt::widgets::MapDisplay(QSize(w, h), parent, f),
  m_rootFolderLayer(root),
  m_timeSlider(0)
{
  setObjectName("MyDisplay");

  setAcceptDrops(true);

  m_temporalVectorialDisplayPixmap = new QPixmap(w, h);
  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));

  m_temporalImageDisplayPixmap = new QPixmap(w, h);
  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));

  m_menu = new QMenu(this);

  m_srsAction = new QAction("&Set SRID", m_menu);
  m_menu->addAction(m_srsAction);
  connect(m_srsAction, SIGNAL(triggered()), this, SLOT(setSRIDSlot()));

  m_fitAllLayersAction = new QAction("&Fit All Layers", m_menu);
  m_menu->addAction(m_fitAllLayersAction);
  connect(m_fitAllLayersAction, SIGNAL(triggered()), this, SLOT(fitAllLayersSlot()));

  m_mouseOperationMenu = m_menu->addMenu("Mouse Operation");

  m_mouseZoomInAction = new QAction("Zoom In", m_mouseOperationMenu);
  m_mouseZoomInAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseZoomInAction);
  connect(m_mouseZoomInAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToZoomInSlot()));

  m_mouseZoomOutAction = new QAction("Zoom Out", m_mouseOperationMenu);
  m_mouseZoomOutAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseZoomOutAction);
  connect(m_mouseZoomOutAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToZoomOutSlot()));

  m_mousePanAction = new QAction("Pan", m_mouseOperationMenu);
  m_mousePanAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mousePanAction);
  connect(m_mousePanAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToPanSlot()));

  m_mouseSelectionAction = new QAction("Object Selection", m_mouseOperationMenu);
  m_mouseSelectionAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseSelectionAction);
  connect(m_mouseSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToObjectSelectionSlot()));

  m_mouseAddSelectionAction = new QAction("Object Add Selection", m_mouseOperationMenu);
  m_mouseAddSelectionAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseAddSelectionAction);
  connect(m_mouseAddSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToAddObjectSelectionSlot()));

  m_mouseToggleSelectionAction = new QAction("Object Toggle selection", m_mouseOperationMenu);
  m_mouseToggleSelectionAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseToggleSelectionAction);
  connect(m_mouseToggleSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToToggleObjectSelectionSlot()));

  m_mouseTooltipAction = new QAction("Tooltip", m_mouseOperationMenu);
  m_mouseTooltipAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseTooltipAction);
  connect(m_mouseTooltipAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToTooltipSlot()));

  m_mouseShowCoordAction = new QAction("Show Coordenates", m_mouseOperationMenu);
  m_mouseShowCoordAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseShowCoordAction);
  connect(m_mouseShowCoordAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToShowCoordSlot()));

  QAction* drawAllPointedsAction = new QAction("Fit All &Pointeds", m_menu);
  m_menu->addAction(drawAllPointedsAction);
  connect(drawAllPointedsAction, SIGNAL(triggered()), this, SLOT(drawAllPointedsSlot()));

  QAction* drawAllQueriedsAction = new QAction("Fit All &Querieds", m_menu);
  m_menu->addAction(drawAllQueriedsAction);
  connect(drawAllQueriedsAction, SIGNAL(triggered()), this, SLOT(drawAllQueriedsSlot()));

  QAction* drawAllPointedsAndQueriedsAction = new QAction("Fit &All Pointeds And Querieds", m_menu);
  m_menu->addAction(drawAllPointedsAndQueriedsAction);
  connect(drawAllPointedsAndQueriedsAction, SIGNAL(triggered()), this, SLOT(drawAllPointedsAndQueriedsSlot()));

  QAction* showRootFolderAction = new QAction("Show Root Folder", m_menu);
  m_menu->addAction(showRootFolderAction);
  connect(showRootFolderAction, SIGNAL(triggered()), this, SLOT(showRootFolderSlot()));

  QAction* printAction = new QAction("Print...", m_menu);
  m_menu->addAction(printAction);
  connect(printAction, SIGNAL(triggered()), this, SLOT(printSlot()));

  QAction* printFileAction = new QAction("Print In File...", m_menu);
  m_menu->addAction(printFileAction);
  connect(printFileAction, SIGNAL(triggered()), this, SLOT(printFileSlot()));

  m_mouseHandler = new MouseHandler(this);

  connect(m_mouseHandler, SIGNAL(execZoomArea(QRect)), this, SLOT(execZoomAreaSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execZoomIn(QPoint)), this, SLOT(execZoomInSlot(QPoint)));
  connect(m_mouseHandler, SIGNAL(execZoomOut(QPoint)), this, SLOT(execZoomOutSlot(QPoint)));
  connect(m_mouseHandler, SIGNAL(execPan(QPoint, QPoint)), this, SLOT(execPanSlot(QPoint, QPoint)));
  connect(m_mouseHandler, SIGNAL(execSelection(QRect)), this, SLOT(mouseSelectionSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execAddSelection(QRect)), this, SLOT(mouseAddSelectionSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execToggleSelection(QRect)), this, SLOT(mouseToggleSelectionSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execTooltip(QPoint)), this, SLOT(mouseTooltipSlot(QPoint)));
  connect(m_mouseHandler, SIGNAL(execShowCoord(QPoint)), this, SLOT(mouseShowCoordSlot(QPoint)));

  m_mouseHandler->setCursor();

  setAttribute(Qt::WA_DeleteOnClose);

  sizeHint();

  // put temporal controls

  m_widget = new QWidget(this, f);
  m_widget->resize(w, h);
  m_widget->setWindowOpacity(0.);
  m_layout = new QVBoxLayout(this);
  m_layout->addWidget(m_widget);

  m_timeGroupBox = new QGroupBox(m_widget);
  m_timeGroupBox->setMaximumHeight(40);

  m_playPauseButton = new QPushButton("", m_timeGroupBox); // tem que ser criado antes do TimeSlider
  m_playPauseButton->setMaximumWidth(22);
  m_timeLineEdit = new QLineEdit(m_timeGroupBox); // tem que ser criado antes do TimeSlider
  m_timeLineEdit->setMaximumWidth(120);
  m_timeLineEdit->setMinimumWidth(120);
  m_timeLineEdit->setReadOnly(true);
  m_timeLineEdit->hide();

  m_timeSlider = new TimeSlider(this, m_timeGroupBox);

  m_timeLayout = new QHBoxLayout(m_timeGroupBox);

  QDir dir;
  if(dir.cd(""TE_DATA_EXAMPLE_LOCALE"/data/cursorShapes") == false)
      dir.cd("../../images");
  QIcon stopIcon(dir.absolutePath() + "/stop.png");
  QPushButton* stopButton = new QPushButton(stopIcon, "", m_timeGroupBox);
  stopButton->setMaximumWidth(22);
  m_timeLayout->addWidget(m_playPauseButton);
  m_timeLayout->addWidget(stopButton);
  m_timeLayout->addWidget(m_timeSlider);
  m_timeGroupBox->hide();
  connect(m_playPauseButton, SIGNAL(clicked()), m_timeSlider, SLOT(playPauseSlot()));
  connect(stopButton, SIGNAL(clicked()), m_timeSlider, SLOT(stopSlot()));

  m_timeGroupBox->setContextMenuPolicy(Qt::CustomContextMenu);
  m_timeSliderMenu = new QMenu(m_timeGroupBox);

  QAction* configTemporalPlayAction = new QAction("&Play Config...", m_timeSliderMenu);
  m_timeSliderMenu->addAction(configTemporalPlayAction);
  connect(configTemporalPlayAction, SIGNAL(triggered()), this, SLOT(configTemporalPlaySlot()));

  m_showTimeLineEditAction = new QAction("&Show Current Time...", m_timeSliderMenu);
  m_showTimeLineEditAction->setCheckable(true);
  m_timeSliderMenu->addAction(m_showTimeLineEditAction);
  connect(m_showTimeLineEditAction, SIGNAL(triggered()), this, SLOT(showTimeLineEditSlot()));

  connect(m_timeGroupBox, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(timeSliderContextMenuSlot(const QPoint&)));
  m_mouseHandler->raise();
}

MyDisplay::~MyDisplay()
{
  Q_EMIT closed(this);
  delete m_temporalVectorialDisplayPixmap;
  delete m_temporalImageDisplayPixmap;
  disconnect();
}

void MyDisplay::closeEvent(QCloseEvent *event)
{
  //setAttribute(Qt::WA_DeleteOnClose);
  //Q_EMIT closed(this);
  //event->accept();
}

void MyDisplay::timeSliderContextMenuSlot(const QPoint& pos)
{
  QPoint p = mapToGlobal(m_timeGroupBox->pos() + pos);
  m_timeSliderMenu->exec(p);
}

void MyDisplay::configTemporalPlaySlot()
{
  bool b = m_timeSlider->isEnabled();
  m_timeSlider->setEnabled(true);
  m_timeSlider->configDrawing();
  m_timeSlider->setEnabled(b);
}

void MyDisplay::showTimeLineEditSlot()
{
  if(m_showTimeLineEditAction->isChecked())
  {
    m_timeLayout->removeWidget(m_timeSlider);
    m_timeLineEdit->show();
    m_timeLayout->addWidget(m_timeLineEdit);
    m_timeLayout->addWidget(m_timeSlider);
  }
  else
  {
    m_timeLineEdit->hide();
    m_timeLayout->removeWidget(m_timeLineEdit);
  }
}

void MyDisplay::setTimeSliderIcon(QPixmap* p)
{
  QIcon icon(*p);
  m_playPauseButton->setIcon(icon);
}

void MyDisplay::dragEnterEvent(QDragEnterEvent* e)
{
  const QMimeData* mime = e->mimeData();
  QString s = mime->data("application/layer-explorer").constData();
  if(s.isEmpty())
    return;

  unsigned long long v = s.toULongLong();
  te::map::AbstractLayer* al = (te::map::AbstractLayer*)v;
  te::map::AbstractLayer* laux = al;
  while((te::map::AbstractLayer*)laux->getParent())
    laux = (te::map::AbstractLayer*)laux->getParent();

  if(laux == m_rootFolderLayer)
    e->accept();
  return;

  //const QMimeData* mime = e->mimeData();
  //QString s =	mime->text(); 
  //bool b = mime->hasColor();
  //b = mime->hasHtml();
  //b = mime->hasImage();
  //b = mime->hasText();
  //b = mime->hasUrls();
  //QList<QUrl>	urls = mime->urls();
  //QList<QUrl>::Iterator it;
  //for(it = urls.begin(); it != urls.end(); ++it)
  //{
  //  QUrl url = *it;
  //  QString autority = url.authority();
  //  QString host = url.host();
  //  QString path = url.path();
  //  int port = url.port();
  //  bool isLocalFile = url.isLocalFile();
  //  bool isRelative = url.isRelative();
  //  bool hasfragment = url.hasFragment();
  //}
}

void MyDisplay::dropEvent(QDropEvent* e)
{
  const QMimeData* mime = e->mimeData();
  QString s = mime->data("application/layer-explorer").constData();
  unsigned long long v = s.toULongLong();
  te::map::AbstractLayer* al = (te::map::AbstractLayer*)v;
  changeTree(al);
}

void MyDisplay::setLayerTree(te::map::AbstractLayer* layer)
{
  m_layerTree = layer;
}

te::map::AbstractLayer* MyDisplay::getLayerTree()
{
  return m_layerTree;
}

te::gm::Envelope MyDisplay::getLayerExtent(te::map::AbstractLayer* al)
{
  try
  {
    te::map::Layer* layer = (te::map::Layer*)al;
    if(layer->getExtent() == 0)
    {
      te::da::DataSourceTransactor* transactor = layer->getDataSource()->getTransactor();
      te::da::DataSet* ds = transactor->getDataSet(layer->getId());
      te::gm::Envelope* dsEnv = ds->getExtent();
      layer->setExtent(dsEnv);
      delete ds;
      delete transactor;
    }
    return *(layer->getExtent());
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::changeTree(te::map::AbstractLayer* al)
{
  initTemporal();

  // limpe todos os canvas antes usados 
  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::map::Canvas* c = getCanvas(it->first);
    delete c;
  }
  m_layerCanvasMap.clear();

  // calcule novo SRID e extent
  m_srid = -1;
  m_layerTree = al;
  std::list<te::map::AbstractLayer*> layerList;
  std::list<te::map::AbstractLayer*>::iterator lit;
  if(m_layerTree)
    getLayerList(m_layerTree, layerList);
  showOrHideTimeSlider(layerList);

  m_envelope = te::gm::Envelope();

  for(lit = layerList.begin(); lit != layerList.end(); ++lit)
  {
    te::gm::Envelope env = getLayerExtent(*lit);

    int srid = (*lit)->getSRID();
    if(m_srid <= 0)
      m_srid = srid;
    if(srid != m_srid)
      transform(env, srid, m_srid);

    m_envelope.Union(env);
  }
  setExtent();

  QString wtitle = "Display: ";
  if(m_layerTree)
    wtitle += m_layerTree->getTitle().c_str();
  setWindowTitle(wtitle);

  QWidget* w = (QWidget*)parent();
  while(w)
  {
    if(w->parent())
      w = (QWidget*)w->parent();
    else
    {
      QString mainTitle = "My Main Window - " + wtitle;
      w->setWindowTitle(mainTitle);
      break;
    }
  }

  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));
  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));

  draw();
  if(m_envelope.isValid() == false)
  {
    m_displayPixmap->fill();
    update();
  }
}

void MyDisplay::showRootFolderSlot()
{
  if(m_layerTree == m_rootFolderLayer)
    return;

  m_layerTree = m_rootFolderLayer;
  changeTree(m_layerTree);
}

void MyDisplay::contextMenuEvent(QContextMenuEvent* c)
{
  m_mouseZoomInAction->setChecked(false);
  m_mouseZoomOutAction->setChecked(false);
  m_mousePanAction->setChecked(false);
  m_mouseSelectionAction->setChecked(false);
  m_mouseAddSelectionAction->setChecked(false);
  m_mouseToggleSelectionAction->setChecked(false);
  m_mouseTooltipAction->setChecked(false);
  m_mouseShowCoordAction->setChecked(false);

  MouseHandler::MouseMode mode = m_mouseHandler->getMode();
  if(mode == MouseHandler::ZoomInMode)
    m_mouseZoomInAction->setChecked(true);
  else if(mode == MouseHandler::ZoomOutMode)
    m_mouseZoomOutAction->setChecked(true);
  else if(mode == MouseHandler::PanMode)
    m_mousePanAction->setChecked(true);
  else if(mode == MouseHandler::SelectionMode)
    m_mouseSelectionAction->setChecked(true);
  else if(mode == MouseHandler::AddSelectionMode)
    m_mouseAddSelectionAction->setChecked(true);
  else if(mode == MouseHandler::ToggleSelectionMode)
    m_mouseToggleSelectionAction->setChecked(true);
  else if(mode == MouseHandler::TooltipMode)
    m_mouseTooltipAction->setChecked(true);
  else if(mode == MouseHandler::ShowCoordMode)
    m_mouseShowCoordAction->setChecked(true);

  m_menu->exec(c->globalPos());
}

void MyDisplay::setMouseOperationToObjectSelectionSlot()
{
  m_mouseHandler->setMode(MouseHandler::SelectionMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToAddObjectSelectionSlot()
{
  m_mouseHandler->setMode(MouseHandler::AddSelectionMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToToggleObjectSelectionSlot()
{
  m_mouseHandler->setMode(MouseHandler::ToggleSelectionMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToTooltipSlot()
{
  m_mouseZoomInAction->setChecked(false);
  m_mouseZoomOutAction->setChecked(false);
  m_mousePanAction->setChecked(false);
  m_mouseSelectionAction->setChecked(false);
  m_mouseAddSelectionAction->setChecked(false);
  m_mouseToggleSelectionAction->setChecked(false);
  m_mouseTooltipAction->setChecked(true);

  m_mouseHandler->setMode(MouseHandler::TooltipMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToShowCoordSlot()
{
  m_mouseZoomInAction->setChecked(false);
  m_mouseZoomOutAction->setChecked(false);
  m_mousePanAction->setChecked(false);
  m_mouseSelectionAction->setChecked(false);
  m_mouseAddSelectionAction->setChecked(false);
  m_mouseToggleSelectionAction->setChecked(false);
  m_mouseTooltipAction->setChecked(false);
  m_mouseShowCoordAction->setChecked(true);

  m_mouseHandler->setMode(MouseHandler::ShowCoordMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToZoomInSlot()
{
  m_mouseHandler->setMode(MouseHandler::ZoomInMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToZoomOutSlot()
{
  m_mouseHandler->setMode(MouseHandler::ZoomOutMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToPanSlot()
{
  m_mouseHandler->setMode(MouseHandler::PanMode);
  m_mouseHandler->setCursor();
}


void MyDisplay::selectionChangedSlot(te::map::DataGridOperation* op)
{
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
  draw();
}

void MyDisplay::removeDrawOnlyChanged(te::map::AbstractLayer* al)
{
  m_drawOnlyChanged.erase(al);
}

void MyDisplay::addDrawOnlyChanged(te::map::AbstractLayer* al)
{
  m_drawOnlyChanged.insert(al);
}

// se o layer e' do tipo temporal
// existe apenas um canvas associado ao pai do layer.
te::qt::widgets::Canvas* MyDisplay::getCanvas(te::map::AbstractLayer* layer)
{
  MyLayer* l = (MyLayer*)layer;
  if(m_extent == 0)
  {
    if(layer->getExtent())
      m_extent = new te::gm::Envelope(*(layer->getExtent()));
    else
      return 0;
  }

  if(l->isTemporal())
  {
    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)l->getParent();
    return te::qt::widgets::MapDisplay::getCanvas(parent);
  }
  return te::qt::widgets::MapDisplay::getCanvas(l);
}

// se o layer e' do tipo temporal
// existe apenas um canvas associado ao pai do layer.
void MyDisplay::setCanvas(te::map::AbstractLayer* layer)
{
  MyLayer* l = (MyLayer*)layer;
  if(l->isTemporal())
  {
    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)l->getParent();    
    if(m_layerCanvasMap.find(parent) == m_layerCanvasMap.end())
    {
      te::qt::widgets::Canvas* c = new te::qt::widgets::Canvas(m_displayPixmap->width(), m_displayPixmap->height());
      m_layerCanvasMap[parent] = c;
    }  
  }
  else
  {
    if(m_layerCanvasMap.find(l) == m_layerCanvasMap.end())
    {
      te::qt::widgets::Canvas* c = new te::qt::widgets::Canvas(m_displayPixmap->width(), m_displayPixmap->height());
      m_layerCanvasMap[l] = c;
    }  
  }
}

void MyDisplay::draw()
{
  try
  {
    std::vector<te::map::AbstractLayer*> layers =  m_timeSlider->getLayers();
    std::vector<te::map::AbstractLayer*>::iterator it;
    for(it = layers.begin(); it != layers.end(); ++it)
      clearTemporalCanvas(*it);

    if(m_displayPixmap)
    {
      m_displayPixmap->fill(m_backgroundColor);

      if(m_layerTree) // Use the tree if it exists, otherwise use the list of layers.
      {
        std::list<te::map::AbstractLayer*> layerList;
        getLayerList(m_layerTree, layerList);
        draw(layerList);
      }
      else if(m_layerList.empty() == false)
        draw(m_layerList);
    }
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::draw(std::list<te::map::AbstractLayer*>& layerList)
{
  try
  {
    QPainter painter(m_displayPixmap);
    std::list<te::map::AbstractLayer*>::iterator it;
 
    if(m_srid <= 0 || m_extent == 0)
    {
      if(m_extent == 0)
        m_extent = new te::gm::Envelope();
      for(it = layerList.begin(); it != layerList.end(); ++it)
      {
        te::gm::Envelope env = getLayerExtent(*it);

        int srid = (*it)->getSRID();
        if(m_srid <= 0)
          m_srid = srid;
        if(srid != m_srid)
          transform(env, srid, m_srid);

        m_extent->Union(env);
      }
      if(m_extent->isValid() == false)
      {
        delete m_extent;
        m_extent = 0;
        return;
      }
      m_envelope = *m_extent;
    }
    if(m_srid <= 0)
      return;

    for(it = layerList.begin(); it != layerList.end(); ++it)
    {
      te::qt::widgets::Canvas *c = getCanvas(*it);
      if(c == 0)
      {
        setCanvas(*it);
        c = getCanvas(*it);
        c->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
        c->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
      }
      draw(*it);

      if(m_displayPixmap->size() == c->getPixmap()->size())
        painter.drawPixmap(0, 0, *(c->getPixmap()));
    }
    update();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::draw(te::map::AbstractLayer* al)
{
  try
  {
    te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
    te::qt::widgets::Canvas* canvas = getCanvas(al);
    if(canvas == 0)
      return;

    MyLayer* layer =  (MyLayer*)al;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0 && layer->isTemporal() == false)
    {
      std::string name = layer->getId();
      te::da::DataSource* ds = layer->getDataSource();
      te::da::DataSourceTransactor* t = ds->getTransactor();
      te::da::DataSet* dataSet = t->getDataSet(name);
      te::da::DataSetType* dsType = ds->getCatalog()->getDataSetType(name);

      assert(dataSet);

      if(dsType->getPrimaryKey())
      {
        op = new te::map::DataGridOperation();
        op->init(dsType, dataSet);
        layer->setDataGridOperation(op);
      }
    }

    bool b = false;
    if(m_drawOnlyChanged.find(layer) != m_drawOnlyChanged.end())
      b = true;
    MyLayerRenderer* renderer = new MyLayerRenderer(b);
    layer->setRenderer(renderer);

    if(layer->isTemporal() == false)
      al->draw(canvas, *m_extent, m_srid);

    addDrawOnlyChanged(layer);
  }
  catch(std::exception& e)
  {
    QMessageBox::information(this, tr("Error Drawing..."), tr(e.what()));
    return;
  }
}

void MyDisplay::reorderDrawing(std::vector<te::map::AbstractLayer*> layers)
{
  try
  {
    te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

    m_displayPixmap->fill();
    QPainter painter(m_displayPixmap);

    std::vector<te::map::AbstractLayer*>::reverse_iterator it;
    for(it = layers.rbegin(); it != layers.rend(); ++it)
    {
      MyLayer* layer =  (MyLayer*)(*it);
      te::qt::widgets::Canvas* c = getCanvas(layer);
      if(layer->getVisibility() == te::map::VISIBLE)
      {
        if(c == 0)
        {
          setCanvas(layer);
          c = getCanvas(layer);
          draw(layer);
        }
        painter.drawPixmap(0, 0, *(c->getPixmap()));
      }
    }
  }
  catch(std::exception& e)
  {
    QMessageBox::information(this, tr("Error Drawing..."), tr(e.what()));
    return;
  }
}

te::gm::Envelope MyDisplay::getAllExtent()
{
  try
  {
    if(m_layerList.empty() == false)
      return getAllExtent(m_layerList);

    std::list<te::map::AbstractLayer*> layerList;
    if(m_layerTree)
      getLayerList(m_layerTree, layerList);
    return getAllExtent(layerList);
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

te::gm::Envelope MyDisplay::getAllExtent(std::list<te::map::AbstractLayer*>& layerList)
{
  try
  {
    std::list<te::map::AbstractLayer*>::iterator it;

    te::gm::Envelope extent;
    for(it = layerList.begin(); it != layerList.end(); ++it)
    {
      te::gm::Envelope env = getLayerExtent(*it);

      int srid = (*it)->getSRID();
      if(srid > 0)
      {
        if(m_srid <= 0)
          m_srid = srid;
        if(srid != m_srid)
          transform(env, srid, m_srid);
      }

      extent.Union(env);
    }
    return extent;
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::clearTemporalPixmaps(std::vector<te::map::AbstractLayer*> layers)
{
  std::set<te::map::AbstractLayer*> als;
  te::map::AbstractLayer* al;

  std::vector<te::map::AbstractLayer*>::iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    al = (te::map::AbstractLayer*)(*it)->getParent();
    if(als.find(al) == als.end())
    {
      als.insert(al);
      clearTemporalCanvas(*it);
    }
  }

  int w = m_displayPixmap->width();
  int h = m_displayPixmap->height();

  if(w != m_temporalVectorialDisplayPixmap->width() || h != m_temporalVectorialDisplayPixmap->height())
  {
    delete m_temporalVectorialDisplayPixmap;
    m_temporalVectorialDisplayPixmap = new QPixmap(w, h);
  }
  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));

  if(w != m_temporalImageDisplayPixmap->width() || h != m_temporalImageDisplayPixmap->height())
  {
    delete m_temporalImageDisplayPixmap;
    m_temporalImageDisplayPixmap = new QPixmap(w, h);
  }
  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));
}

void MyDisplay::clearTemporalCanvas(te::map::AbstractLayer* al)
{
  te::qt::widgets::Canvas* canvas = getCanvas(al);
  if(canvas)
    canvas->clear();
}

void MyDisplay::drawTemporalData(te::map::AbstractLayer* layer, std::vector<te::gm::Geometry*>& geoms, bool drawLines)
{
  MyLayer* l = (MyLayer*)layer;
  te::qt::widgets::Canvas* canvas = getCanvas(l);
  if(canvas == 0)
  {
    setCanvas(l);
    canvas = getCanvas(l);
  }
  te::map::AbstractLayer* parent = (te::map::AbstractLayer*)l->getParent();
  std::string pid = parent->getId();

  if(pid == "MovingObjects") // vectorial data
  {
    if(drawLines)
    {
      canvas->setLinePattern(0, 0, 0);
      canvas->setLineWidth(1);
      if(layer->getId() == "40: locations")
        canvas->setLineColor(te::color::RGBAColor(220, 220, 0, 255));
      else
        canvas->setLineColor(te::color::RGBAColor(0, 220, 220, 255));
    }
    else
    {
      canvas->setPointPattern(0, 0, 0);
      canvas->setPointWidth(1);
      if(layer->getId() == "40: locations")
      {
        canvas->setPointColor(te::color::RGBAColor(220 ,0, 0, 255));
      }
      else
      {
        canvas->setPointColor(te::color::RGBAColor(0 , 220, 0, 255));
      }
    }

    std::vector<te::gm::Geometry*>::iterator it = geoms.begin();
    while(it != geoms.end())
    {
      // Verifies the SRID. Case differents, converts coordinates...
      if((*it)->getSRID() > 0)
        (*it)->transform(m_srid);
      canvas->draw(*it);
      ++it;
    }

    if(m_showTimeLineEditAction->isChecked())
    {
      te::dt::TimeInstant* t = m_timeSlider->getCurrentTime();
      if(t)
      {
        QString timeString = t->toString().c_str();
        m_timeLineEdit->setText(timeString);
      }
    }

    QPainter painter(m_temporalVectorialDisplayPixmap);
    painter.drawPixmap(0, 0, *(canvas->getPixmap()));
    update();
  }
  else // raster data
  {
    m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));
    canvas->clear();
    MyLayer* l = (MyLayer*)layer;
    te::da::DataSource* ds = l->getDataSource();
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    std::string name = l->getId();
    te::da::DataSet* dataSet = transactor->getDataSet(name);
    te::rst::Raster* raster = dataSet->getRaster();
    int ncols = raster->getNumberOfColumns();
    int nrows = raster->getNumberOfRows();
    const te::gm::Envelope* env = l->getExtent();
    QRectF recw(env->getLowerLeftX(), env->getLowerLeftY(), env->getWidth(), env->getHeight());
    QMatrix m = canvas->getMatrix();
    QRectF recd = m.mapRect(recw);

    int size = ncols * nrows;
    char *buf = new char[size];
    std::string namePNG = name;
    size_t p = namePNG.find(".tif");
    if(p != std::string::npos)
    {
      namePNG.erase(p, 4);
      namePNG += ".PNG";
    }
    std::string file = TE_DATA_EXAMPLE_LOCALE"/data/png/";
    file += namePNG;

    FILE* fp = fopen(file.c_str(), "rb");
    size_t bsize = fread(buf, sizeof(char), size, fp);
    canvas->drawImage(recd.x(), recd.y(), recd.width(), recd.height(), buf, bsize, te::map::PNG);

    fclose(fp);
    delete []buf;

    delete dataSet;
    QPainter painter(m_temporalImageDisplayPixmap);
    painter.drawPixmap(0, 0, *(canvas->getPixmap()));
    update();
  }

}

void MyDisplay::removeAllTemporalLayers()
{
  m_timeSlider->removeAllLayers();
}

void MyDisplay::addTemporalLayer(te::map::AbstractLayer* layer)
{
  m_timeSlider->addLayer(layer);
}

void MyDisplay::paintEvent(QPaintEvent* e)
{
  if(m_displayPixmap == 0)
    return;

  double w, h, nw, nh, sw, sh;
  QMatrix m;
  QPointF p1, p2;
  te::qt::widgets::Canvas *c = 0;

  if(m_resize == false)
  {
    if(m_repaint)
    {
      QPainter painter(this);
      QRect rec = e->rect();
      painter.drawPixmap(rec, *m_displayPixmap, rec);
    }
  }
  else // fazendo resize do display
  {
    if(m_layerCanvasMap.begin() != m_layerCanvasMap.end())
      c = m_layerCanvasMap.begin()->second;

    if(c == 0)
      return;

    m = c->getMatrix();

    p1 = m.map(QPointF(m_envelope.getLowerLeftX(), m_envelope.getLowerLeftY()));
    p2 = m.map(QPointF(m_envelope.getUpperRightX(), m_envelope.getUpperRightY()));
    m_resizeWRec = QRect(QPoint(p1.x(), p2.y()), QPoint(p2.x(), p1.y()));
    nw = m_resizeWRec.width();
    nh = m_resizeWRec.height();

    QPainter painter(this);
    m_resizeRec = rect();
    w = m_resizeRec.width();
    h = m_resizeRec.height();

    QPixmap pix(w, h);
    pix.fill();
    painter.drawPixmap(0, 0, pix);

    sh = h/m_resizeWRec.height();
    sw = w/m_resizeWRec.width();
    if(sw < sh)
    {
      nw *= sw;
      nh *= sw;
    }
    else if(sh < sw)
    {
      nw *= sh;
      nh *= sh;
    }

    if(m_hAlign == te::map::Center)
    {
      m_resizeRec.setLeft((w - nw) / 2);
      m_resizeRec.setRight(m_resizeRec.left() + nw);
    }
    else if(m_hAlign == te::map::Left)
      m_resizeRec.setRight(nw);
    else if(m_hAlign == te::map::Right)
      m_resizeRec.setLeft(m_resizeRec.width() - nw);

    if(m_vAlign == te::map::Center)
    {
      m_resizeRec.setTop((h - nh) / 2);
      m_resizeRec.setBottom(m_resizeRec.top() + nh);
    }
    else if(m_vAlign == te::map::Top)
      m_resizeRec.setBottom(nh);
    else if(m_vAlign == te::map::Bottom)
      m_resizeRec.setTop(m_resizeRec.height() - nh);

    painter.drawPixmap(m_resizeRec, *m_displayPixmap, m_resizeWRec);
  }

  QRect rec, wrec;

  if(m_resize == false)
  {
    if(m_repaint)
    {
      QPainter painter(this);
      QRect rec = e->rect();
      painter.drawPixmap(rec, *m_temporalVectorialDisplayPixmap, rec);
      painter.drawPixmap(rec, *m_temporalImageDisplayPixmap, rec);
    }
  }
  else // fazendo resize do display com os pixmaps temporais
  {
    if(m_layerCanvasMap.begin() != m_layerCanvasMap.end())
      c = m_layerCanvasMap.begin()->second;

    if(c == 0)
      return;

    QPainter painter(this);
    painter.drawPixmap(m_resizeRec, *m_temporalVectorialDisplayPixmap, m_resizeWRec);
    painter.drawPixmap(m_resizeRec, *m_temporalImageDisplayPixmap, m_resizeWRec);
  }
}

void MyDisplay::resizeEvent(QResizeEvent* e)
{ 
  m_resize = true;
  QWidget::resizeEvent(e);

  if(m_timer)
  {
    m_timer->disconnect();
    delete m_timer;
  }
  m_timer = new QTimer;
  m_timer->setSingleShot(true);
  m_timer->start(500);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(onResizeTimeout()));
}

void MyDisplay::onResizeTimeout()
{
  m_resize = false;
  if(m_displayPixmap)
    delete m_displayPixmap;
  m_displayPixmap = new QPixmap(width(), height());
  m_displayPixmap->fill(QColor(0, 0, 0, 0));
  int w = m_displayPixmap->width();
  int h = m_displayPixmap->height();

  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;

  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::qt::widgets::Canvas *c = it->second;
    c->resize(w, h);
  }
  setExtent();

  draw();

  emit sizeChanged(QSize(w, h));
}


void MyDisplay::setExtent()
{
  m_drawOnlyChanged.clear();
  te::map::MapDisplay::setExtent(m_envelope);
  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::qt::widgets::Canvas* canvas = it->second;
    canvas->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
    canvas->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
    canvas->clear();
  }

  if(m_timeSlider)
    clearTemporalPixmaps(m_timeSlider->getLayers());
}

void MyDisplay::setRepaint(bool s)
{
  m_repaint = s;
}


void MyDisplay::setSRID(const int& srid)
{
  m_drawOnlyChanged.clear();
  if(m_srid == srid)
    return;

  if(m_extent)
  {
    m_envelope = *m_extent;
    if(transform(m_envelope, m_srid, srid) == false)
    {
      QMessageBox::information(this, tr("Projection Fail..."), tr("Fail to change projection!"));
      return;
    }
    setExtent();
  }
  m_srid = srid;
}

void MyDisplay::changeObjectStatus(QRect rec, const std::string& mode)
{
  std::list<te::map::AbstractLayer*> layerList;
  getLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;

    te::qt::widgets::Canvas* canvas = getCanvas(layer);

    te::gm::Point worldPoint(0., 0., m_srid);
    QPointF p(rec.bottomLeft().x(), rec.bottomLeft().y());
    p = canvas->getMatrix().inverted().map(p);
    worldPoint.setX(p.x());
    worldPoint.setY(p.y());

    te::gm::Polygon worldRec(0, te::gm::PolygonType, m_srid);
    QRectF r(rec);
    r = canvas->getMatrix().inverted().mapRect(r);
    // tem que ser ponteiro porque o destrutor de polygon deleta os rings
    te::gm::LinearRing* line = new te::gm::LinearRing(5, te::gm::LineStringType);
    line->setPoint(0, r.bottomLeft().x(), r.topRight().y()); // lower left
    line->setPoint(1, r.bottomRight().x(), r.topRight().y()); // lower rigth
    line->setPoint(2, r.topRight().x(), r.bottomRight().y()); // upper rigth
    line->setPoint(3, r.topLeft().x(), r.bottomLeft().y()); // upper left
    line->setPoint(4, r.bottomLeft().x(), r.topRight().y()); // closing
    worldRec.push_back(line);

    te::gm::Polygon worldRecExpanded(0, te::gm::PolygonType, m_srid);
    QRectF re(0., 0., 9., 9.);
    re.moveCenter(rec.center());
    re = canvas->getMatrix().inverted().mapRect(re);
    // tem que ser ponteiro porque o destrutor de polygon deleta os rings
    te::gm::LinearRing* linee = new te::gm::LinearRing(5, te::gm::LineStringType);
    linee->setPoint(0, re.bottomLeft().x(), re.topRight().y()); // lower left
    linee->setPoint(1, re.bottomRight().x(), re.topRight().y()); // lower rigth
    linee->setPoint(2, re.topRight().x(), re.bottomRight().y()); // upper rigth
    linee->setPoint(3, re.topLeft().x(), re.bottomLeft().y()); // upper left
    linee->setPoint(4, re.bottomLeft().x(), re.topRight().y()); // closing
    worldRecExpanded.push_back(linee);
 
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkv;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
    int gtype = dsType->getDefaultGeomProperty()->getGeometryType();

    std::vector<int> visRows;
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      pkv.clear();
      for (size_t i = 0; i < pkProps.size(); ++i)
        pkv += dataSet->getAsString(pkProps[i]->getName());

      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;
      g->setSRID(layer->getSRID());
      g->transform(m_srid);

      if(gtype == te::gm::PointType || gtype == te::gm::MultiPointType)
      {
        if(rec.width() == 1 && rec.height() == 1)
        {
          if(worldRecExpanded.contains(g))
            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
        }
        else
        {
          if(worldRec.contains(g))
            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
        }
      }
      else if(gtype == te::gm::LineStringType || gtype == te::gm::MultiLineStringType)
      { 
        if(rec.width() == 1 && rec.height() == 1)
        {
          if(g->intersects(&worldRecExpanded))
            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
        }
        else
        {
          if(g->intersects(&worldRec))
            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
        }
      }
      else if(gtype == te::gm::PolygonType || gtype == te::gm::MultiPolygonType)
      {
        if(rec.width() == 1 && rec.height() == 1)
        {
          if(g->contains(&worldPoint))
            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
        }
        else
        {
          if(g->intersects(&worldRec))
            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
        }
      }

      delete g;
    }

    if(visRows.empty())
      continue;

    if(mode == "POINTING")
      op->setRowsAsPointed(visRows);
    else if(mode == "ADDPOINTING")
      op->addRowsToPointed(visRows);
    else if(mode == "TOGGLEPOINTING")
      op->toggleRowsPointingStatus(visRows);
    Q_EMIT selectionChanged(op);
  }
}

void MyDisplay::execZoomAreaSlot(const QRect& rec)
{
  try
  {
    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
    if(canvas == NULL)
      return;

    QPointF pll(rec.left(), rec.bottom());
    QPointF pur(rec.right(), rec.top());

    pll = canvas->getMatrix().inverted().map(pll);
    pur = canvas->getMatrix().inverted().map(pur);
    m_envelope = te::gm::Envelope(pll.x(), pll.y(), pur.x(), pur.y());

    setExtent();

    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::execZoomInSlot(const QPoint& p)
{
  try
  {
    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
    if(canvas == NULL)
      return;

    QRect r(0, 0, canvas->getWidth(), canvas->getHeight());
    r.setWidth(r.width() / 2);
    r.setHeight(r.height() / 2);
    r.moveCenter(p);
    QPointF pll(r.left(), r.bottom());
    QPointF pur(r.right(), r.top());

    pll = canvas->getMatrix().inverted().map(pll);
    pur = canvas->getMatrix().inverted().map(pur);
    m_envelope = te::gm::Envelope(pll.x(), pll.y(), pur.x(), pur.y());

    setExtent();

    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::execZoomOutSlot(const QPoint& p)
{
  try
  {
    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
    if(canvas == NULL)
      return;

    QRect rec(0, 0, canvas->getWidth(), canvas->getHeight());
    rec.setWidth(rec.width() * 2);
    rec.moveCenter(p);
    QPointF pll(rec.left(), rec.bottom());
    QPointF pur(rec.right(), rec.top());

    pll = canvas->getMatrix().inverted().map(pll);
    pur = canvas->getMatrix().inverted().map(pur);
    m_envelope = te::gm::Envelope(pll.x(), pll.y(), pur.x(), pur.y());
    setExtent();

    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::execPanSlot(const QPoint& from, const QPoint& to)
{
  try
  {
    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
    if(canvas == NULL)
      return;
  
    QPoint p = to - from;
    QRect rec(0, 0, canvas->getWidth(), canvas->getHeight());
    QPoint c = rec.center();
    c -= p;
    rec.moveCenter(c);

    QPointF pll(rec.left(), rec.bottom());
    QPointF pur(rec.right(), rec.top());

    pll = canvas->getMatrix().inverted().map(pll);
    pur = canvas->getMatrix().inverted().map(pur);
    m_envelope = te::gm::Envelope(pll.x(), pll.y(), pur.x(), pur.y());

    setExtent();
    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::mouseSelectionSlot(QRect rec)
{
  changeObjectStatus(rec, "POINTING");
}

void MyDisplay::mouseAddSelectionSlot(QRect rec)
{
  changeObjectStatus(rec, "ADDPOINTING");
}

void MyDisplay::mouseToggleSelectionSlot(QRect rec)
{
  changeObjectStatus(rec, "TOGGLEPOINTING");
}

void MyDisplay::mouseTooltipSlot(QPoint p)
{
  std::list<te::map::AbstractLayer*> layerList;
  getLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;

    std::vector<int> tooltipColumns = layer->getTooltipColumns();
    if(tooltipColumns.empty())
      continue;
    
    te::qt::widgets::Canvas* canvas = getCanvas(layer);
    if(canvas == 0)
      continue;

    QRectF mouseRectF(0., 0., 5., 5.);
    mouseRectF.moveCenter(p);     

    //get coordenates in the display projection
    QPointF pw = canvas->getMatrix().inverted().map(QPointF(p));
    te::gm::Point mouseWPos(pw.x(), pw.y(), m_srid);

    mouseRectF = canvas->getMatrix().inverted().mapRect(mouseRectF);
    // bottom and top are inverted in device coordenates
    te::gm::Envelope env(mouseRectF.bottomLeft().x(), mouseRectF.topRight().y(), mouseRectF.topRight().x(), mouseRectF.bottomLeft().y());
    te::gm::Polygon* poly = new te::gm::Polygon(0, te::gm::PolygonType, m_srid);
    te::gm::LinearRing* line = new te::gm::LinearRing(5, te::gm::LineStringType, m_srid);
    line->setPoint(0, env.getLowerLeftX(), env.getLowerLeftY()); // lower left
    line->setPoint(1, env.getUpperRightX(), env.getLowerLeftY()); // lower rigth
    line->setPoint(2, env.getUpperRightX(), env.getUpperRightY()); // upper rigth
    line->setPoint(3, env.getLowerLeftX(), env.getUpperRightY()); // upper left
    line->setPoint(4, env.getLowerLeftX(), env.getLowerLeftY()); // closing

    poly->push_back(line);

    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
    te::gm::GeomType gtype = dsType->getDefaultGeomProperty()->getGeometryType();

    QString val;
    std::vector<QString> values;
    std::vector<int>::iterator it;
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;
      
      g->setSRID(layer->getSRID());
      g->transform(m_srid);

      if(gtype == te::gm::PolygonType || gtype == te::gm::MultiPolygonType)
      {
        if(g->contains(&mouseWPos))
        {
          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
          {
            val = dsType->getProperty(*it)->getName().c_str();
            val += ": ";
            if(dataSet->isNull(*it) == false)
              val += dataSet->getAsString(*it).c_str();
            values.push_back(val);
          }
          delete g;
          break;
        }
      }
      else if(gtype == te::gm::PointType || gtype == te::gm::MultiPointType)
      {
        if(poly->contains(g))
        {
          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
          {
            val = dsType->getProperty(*it)->getName().c_str();
            val += ": ";
            if(dataSet->isNull(*it) == false)
              val += dataSet->getAsString(*it).c_str();
            values.push_back(val);
          }
          delete g;
          break;
        }
      }
      else if(gtype == te::gm::LineStringType || gtype == te::gm::MultiLineStringType)
      {
        if(g->crosses(poly))
        {
          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
          {
            val = dsType->getProperty(*it)->getName().c_str();
            val += ": ";
            if(dataSet->isNull(*it) == false)
              val += dataSet->getAsString(*it).c_str();
            values.push_back(val);
          }
          delete g;
          break;
        }
      }
      delete g;
    }
    delete poly;

    if(values.empty() == false)
    {
      QString s;
      std::vector<QString>::iterator it = values.begin();
      s = *it;
      it++;
      while(it != values.end())
      {
        s += "\n" + *it;
        it++;
      }

      if(s != QToolTip::text())
        QToolTip::showText(mapToGlobal(p), s);
    }
    else
      QToolTip::hideText();
  }
}

void MyDisplay::mouseShowCoordSlot(QPoint p)
{
  QString s, pixx, pixy, dispx, dispy, layerx, layery, srid;

  pixx.setNum(p.x());
  pixy.setNum(p.y());

  s += "display(pixel) x:" + pixx + " y:" + pixy;

  std::list<te::map::AbstractLayer*> layerList;
  getLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;

  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;

    te::qt::widgets::Canvas* canvas = getCanvas(layer);
    if(canvas)
    {
      //get coodenates in the display projection
      QPointF pf = canvas->getMatrix().inverted().map(QPointF(p));
      if(dispx.isEmpty())
      {
        dispx.setNum(pf.x());
        dispy.setNum(pf.y());
        srid.setNum(m_srid);
        s += "\ndisplay(" + srid + ") x:" + dispx + " y:" + dispy;
      }

      te::gm::Point g(pf.x(), pf.y());
      g.setSRID(m_srid);

      //transform to layer projection
      g.transform(layer->getSRID());
      srid.setNum(layer->getSRID());
      layerx.setNum(g.getX());
      layery.setNum(g.getY());
      s += "\n";
      s +=  layer->getId().c_str();
      s += "(" + srid + ") x:" + layerx + " y:" + layery;
    }
  }
  if(dispx.isEmpty() == false)

  QToolTip::showText(mapToGlobal(p), s);
}

void MyDisplay::drawAllPointedsSlot()
{
  te::qt::widgets::Canvas* canvas;
  int status = -1;
  std::list<te::map::AbstractLayer*> layerList;
  getLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  te::gm::Envelope fitEnv;
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;
    canvas = getCanvas(layer);
   
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkv;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();

    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      pkv.clear();
      for (size_t i = 0; i < pkProps.size(); ++i)
        pkv += dataSet->getAsString(pkProps[i]->getName());

      status = op->getDataSetSelectionStatus(pkv);
      if(status == te::map::DataGridOperation::POINTED || status == te::map::DataGridOperation::POINTED_AND_QUERIED)
      {
        te::gm::Envelope env(*g->getMBR());
        if(transform(env, layer->getSRID(), m_srid))
          fitEnv.Union(env);
      }
      delete g;
    }
  }

  if(fitEnv.isValid())
  {
    m_envelope = fitEnv;
    setExtent();
    draw();
  }
  else
  {
    if(status != -1)
    {
      QMessageBox::information(this, tr("Projection Error"), tr("Convertion Error"));
      return;
    }
  }
}

void MyDisplay::drawAllQueriedsSlot()
{
  te::qt::widgets::Canvas* canvas;
  int status;
  std::list<te::map::AbstractLayer*> layerList;
  getLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  te::gm::Envelope fitEnv;
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;
    canvas = getCanvas(layer);
   
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkv;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();

    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      pkv.clear();
      for (size_t i = 0; i < pkProps.size(); ++i)
        pkv += dataSet->getAsString(pkProps[i]->getName());

      status = op->getDataSetSelectionStatus(pkv);
      if(status == te::map::DataGridOperation::QUERIED || status == te::map::DataGridOperation::POINTED_AND_QUERIED)
      {
        te::gm::Envelope env(*g->getMBR());
        if(transform(env, layer->getSRID(), m_srid))
          fitEnv.Union(env);
      }
      delete g;
    }
  }

  if(fitEnv.isValid())
  {
    m_envelope = fitEnv;
    setExtent();
    draw();
  }
  else
  {
    if(status != -1)
    {
      QMessageBox::information(this, tr("Projection Error"), tr("Convertion Error"));
      return;
    }
  }
}

void MyDisplay::drawAllPointedsAndQueriedsSlot()
{
  te::qt::widgets::Canvas* canvas;
  QRectF recF;
  int status;
  std::list<te::map::AbstractLayer*> layerList;
  getLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  te::gm::Envelope fitEnv;
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;
    canvas = getCanvas(layer);
   
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkv;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();

    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      pkv.clear();
      for (size_t i = 0; i < pkProps.size(); ++i)
        pkv += dataSet->getAsString(pkProps[i]->getName());

      status = op->getDataSetSelectionStatus(pkv);
      if(status == te::map::DataGridOperation::POINTED_AND_QUERIED)
      {
        te::gm::Envelope env(*g->getMBR());
        if(transform(env, layer->getSRID(), m_srid))
          fitEnv.Union(env);
      }
      delete g;
    }
  }

  if(fitEnv.isValid())
  {
    m_envelope = fitEnv;
    setExtent();
    draw();
  }
  else
  {
    if(status != -1)
    {
      QMessageBox::information(this, tr("Projection Error"), tr("Convertion Error"));
      return;
    }
  }
}

void MyDisplay::initTemporal()
{
  //parar o desenho dos layers temporais e reinicializar o vector de layers temporais no time Slider
  m_timeSlider->stop();

  m_timeSlider->removeAllLayers();
  std::list<te::map::AbstractLayer*>::iterator lit;
  std::list<te::map::AbstractLayer*> layers;
  getLayerList(m_layerTree, layers);
  for(lit = layers.begin(); lit != layers.end(); ++lit)
  {
    MyLayer* layer = (MyLayer*)(*lit);
    if(layer->isTemporal())
    {
      if(layer->getVisibility() == te::map::VISIBLE)
        m_timeSlider->addLayer(layer);
    }
  }

  showOrHideTimeSlider(layers);
}

void MyDisplay::printSlot()
{
  QPrintDialog w(this);
  
  if(w.exec() ==  QDialog::Rejected)
    return;

  QPrinter* printer = w.printer();

  int fx = printer->physicalDpiX();
  int fy = printer->physicalDpiY();
  printer->setResolution(fx);  // precisa fazer isto para que a impressora tenha a resolucao configurada
                               // na interface. Caso contrario a resolucao sempre sera´ 96 dpi.
  print(printer);
}

void MyDisplay::printFileSlot()
{
  PrintInFile w(this);
  if(w.exec() ==  QDialog::Rejected)
    return;

  QPrinter printer(QPrinter::HighResolution);
  printer.setOutputFileName(w.m_fileLineEdit->text());

  if(w.m_fileFormatComboBox->currentText() == "PDF")
    printer.setOutputFormat(QPrinter::PdfFormat);
  else
    printer.setOutputFormat(QPrinter::PostScriptFormat);

  double width = w.m_paperWidthLineEdit->text().toDouble();
  double height = w.m_paperHeightLineEdit->text().toDouble();
  QSizeF paperSize(width, height);     
  printer.setPaperSize(paperSize, QPrinter::Millimeter);
  double left = w.m_leftMarginSpinBox->text().toDouble();
  double top = w.m_rightMarginSpinBox->text().toDouble();
  double right = w.m_topMarginSpinBox->text().toDouble();
  double bottom = w.m_bottomMarginSpinBox->text().toDouble();
  printer.setPageMargins(left, top, right, bottom, QPrinter::Millimeter);

  int resolution = w.m_resolutionComboBox->currentText().toInt();
  printer.setResolution(resolution);
  int r = printer.resolution();

  if(w.m_paperOrientationComboBox->currentText() == "Portrait")
    printer.setOrientation(QPrinter::Portrait);
  else
    printer.setOrientation(QPrinter::Landscape);

  QString file = w.m_fileLineEdit->text();
  printer.setOutputFileName(file);

  print(&printer);
}

void MyDisplay::print(QPrinter* printer)
{
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
  QSizeF paperSizeMM = printer->paperSize(QPrinter::Millimeter);

  int resolution = printer->resolution();
  QPainter painter(printer); // so pode inicializar painter depois de configurar a resolucao da impressora

  QPrinter::Orientation orientation = printer->orientation();
  int width, height;

  int pwidth = (double)printer->widthMM() * (double)resolution / 25.4;
  int pheight = (double)printer->heightMM() * (double)resolution / 25.4;

  double left, top, right, bottom;
  printer->getPageMargins (&left, &top, &right, &bottom, QPrinter::Millimeter);
  left = (double)left * (double)resolution / 25.4;
  top = (double)top * (double)resolution / 25.4;
  right = (double)right * (double)resolution / 25.4;
  bottom = (double)bottom * (double)resolution / 25.4;

  pwidth -= (left + right);
  pheight -= (top + bottom);

  int mwidth = m_displayPixmap->width();
  int mheight = m_displayPixmap->height();

  double a = (double)mwidth / (double)mheight;
  double b = (double)pwidth / (double)pheight;
  
  if(a < b)
  {
    width = (int)((double)pheight * a + .5);
    height = pheight;
  }
  else
  {
    height = (int)((double)pwidth / a + .5);
    width = pwidth;
  }

  left = (pwidth - width) / 2;
  top = (pheight - height) / 2;

  //unsigned long memAvailable = te::common::GetFreePhysicalMemory();

  int n = 1;
  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(width, height/n);
  while(canvas->getWidth() == 0 || canvas->getHeight() == 0)
  {
    delete canvas;
    ++n;
    canvas = new te::qt::widgets::Canvas(width, height/n);
  }

  if(n == 1)
  {
    te::gm::Envelope e(*m_extent);
    canvas->calcAspectRatio(e.m_llx, e.m_lly, e.m_urx, e.m_ury, m_hAlign, m_vAlign);
    canvas->setWindow(e.m_llx, e.m_lly, e.m_urx, e.m_ury);

    std::list<te::map::AbstractLayer*> layerList;
    std::list<te::map::AbstractLayer*>::iterator it;
    getLayerList(m_layerTree, layerList);

    for(it = layerList.begin(); it != layerList.end(); ++it)
    {
      MyLayer* layer =  (MyLayer*)(*it);
      MyLayerRenderer* renderer = new MyLayerRenderer(false);
      layer->setRenderer(renderer);
      layer->draw(canvas, e, m_srid);

      painter.drawPixmap(left, top, *(canvas->getPixmap()));
    }
  }
  else
  {
    // 4096 x 4096 pixels
    int w = 4096;
    int h = 4096;
    int wn = (int)((double)width / (double)w + 1.);
    n =  (int)((double)height / (double)h + 1.);

    double y1, y2;
    double x1 = m_extent->getLowerLeftX();
    double x2 = m_extent->getUpperRightX();

    delete canvas;
    canvas = new te::qt::widgets::Canvas(w, h);
    QPixmap pix(w, h);
    QPainter painterAux; 
    te::gm::Envelope env(*m_extent);
    double eh = m_extent->getHeight() / (double)n;
    double ew = m_extent->getWidth() / (double)wn;

    y2 = m_extent->getUpperRightY();
    y1 = y2 - eh;
    x1 = m_extent->getLowerLeftX();
    x2 = x1 + ew;
    canvas->setWindow(x1, y1, x2, y2);
    canvas->calcAspectRatio(x1, y1, x2, y2, m_hAlign, m_vAlign);
    double x1ini = x1;
    eh = y2 - y1;
    ew = x2 - x1;
    double wmin = ew / (double)w;
    double hmin = eh / (double)h;

    double d = (m_extent->getUpperRightY() - y1) / (y2 - y1);
    double ssy = h - (int)(d * (double)h + .5);
    d = (m_extent->getLowerLeftX() - x1) / (x2 - x1);
    double ssx = (int)(d * (double)w + .5);

    int yini = top;
    while((y1 + eh) > m_extent->getLowerLeftY())
    {
      int xini = left;
      x1 = x1ini;
      while(x1 < m_extent->getUpperRightX())
      {
        canvas->setWindow(x1, y1, x1+ew, y1+eh);
        env.init(x1, y1, x1+ew, y1+eh);
        pix.fill(QColor(0, 0, 0, 0));
        painterAux.begin(&pix);

        std::list<te::map::AbstractLayer*> layerList;
        std::list<te::map::AbstractLayer*>::iterator it;
        getLayerList(m_layerTree, layerList);

        for(it = layerList.begin(); it != layerList.end(); ++it)
        {
          MyLayer* layer =  (MyLayer*)(*it);
          MyLayerRenderer* renderer = new MyLayerRenderer(false); // clear canvas
          layer->setRenderer(renderer);
          layer->draw(canvas, env, m_srid);

          int sx = 0;
          int sy = 0;
          int dx = w;
          int dy = h;
          if(yini == (int)top)
          {
            sy = ssy;
            dy = 0;
          }
          if(xini == (int)left)
          {
            sx = ssx;
            dx = 0;
          }
          if((x1 + ew) > m_extent->getUpperRightX())
          {
            QPointF pf(m_extent->getUpperRightX(), m_extent->getUpperRightY());
            QPoint p(canvas->getMatrix().map(pf).toPoint());
            dx = p.x();
          }
          if(y1 < m_extent->getLowerLeftY())
          {
            QPointF pf(m_extent->getLowerLeftX(), m_extent->getLowerLeftY());
            QPoint p(canvas->getMatrix().map(pf).toPoint());
            dy = p.y();
          }

          painterAux.drawPixmap(sx, sy, *canvas->getPixmap(), sx, sy, dx, dy);
        }

        painterAux.end();
        painter.drawPixmap(xini, yini, pix);

        xini += w;
        x1 += (ew + wmin);
      }

      yini += h;
      y1 -= (eh + hmin);
    }
  }
  painter.end();
  delete canvas;
}

void MyDisplay::getLayerList(te::map::AbstractLayer* al, std::list<te::map::AbstractLayer*>& layerList)
{
  try
  {
    if(al->getType() == "LAYER")
    {
      if(al->getVisibility() == te::map::VISIBLE)
        layerList.push_front(al);
    }

    te::map::AbstractLayer::iterator it;
    for(it = al->begin(); it != al->end(); ++it)
    {
      te::map::AbstractLayer* t = dynamic_cast<te::map::AbstractLayer*>(*it);
      getLayerList(t, layerList);
    }
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::showOrHideTimeSlider(std::list<te::map::AbstractLayer*>& layerList)
{
  std::list<te::map::AbstractLayer*>::iterator it;
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    if(layer->isTemporal() == true && layer->getVisibility() == te::map::VISIBLE)
    {
      m_timeGroupBox->show();
      m_layout->addWidget(m_timeGroupBox);
      break;
    }
  }

  if(it == layerList.end())
  {
    m_timeGroupBox->hide();
    m_layout->removeWidget(m_timeGroupBox);
  }
}

QWidget* MyDisplay::getWidget()
{
  return m_widget;
}

void MyDisplay::clearTimeLineEdit()
{
  m_timeLineEdit->clear();
}


void MyDisplay::fitAllLayersSlot()
{
  try
  {
    if(m_layerTree)
    {
      std::list<te::map::AbstractLayer*> layerList;
      getLayerList(m_layerTree, layerList);
      fit(layerList);
    }
    else if(m_layerList.empty() == false)
      fit(m_layerList);
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::information(this, tr("Error drawing..."), tr(e.what()));
    return;
  }
}

void MyDisplay::fit(std::list<te::map::AbstractLayer*>& layerList)
{
  try
  {
    m_envelope = te::gm::Envelope();
    std::list<te::map::AbstractLayer*>::iterator it;
    for(it = layerList.begin(); it != layerList.end(); ++it)
    {
      te::gm::Envelope env = getLayerExtent(*it);

      int srid = (*it)->getSRID();
      if(m_srid <= 0)
        m_srid = srid;
      if(srid != m_srid)
      {
        transform(env, srid, m_srid);
        //env.transform(srid, m_srid);
      }
      m_envelope.Union(env);
    }
    setExtent();
    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::setSRIDSlot()
{
  int srid = getSRID();
  std::map<int, QString> sridMap;
  std::map<int, QString>::iterator it;

  sridMap[TE_SRS_SAD69] = "TE_SRS_SAD69";
  sridMap[TE_SRS_CORREGO_ALEGRE] = "TE_SRS_CORREGO_ALEGRE";
  sridMap[TE_SRS_WGS84] = "TE_SRS_WGS84";
  sridMap[TE_SRS_SIRGAS2000] = "TE_SRS_SIRGAS2000";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S";
  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S";
  sridMap[TE_SRS_SAD69_POLYCONIC] = "TE_SRS_SAD69_POLYCONIC";
  sridMap[TE_SRS_SAD69_UTM_ZONE_18N] = "TE_SRS_SAD69_UTM_ZONE_18N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_19N] = "TE_SRS_SAD69_UTM_ZONE_19N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_20N] = "TE_SRS_SAD69_UTM_ZONE_20N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_21N] = "TE_SRS_SAD69_UTM_ZONE_21N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_22N] = "TE_SRS_SAD69_UTM_ZONE_22N";
  sridMap[TE_SRS_SAD69_UTM_ZONE_17S] = "TE_SRS_SAD69_UTM_ZONE_17S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_18S] = "TE_SRS_SAD69_UTM_ZONE_18S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_19S] = "TE_SRS_SAD69_UTM_ZONE_19S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_20S] = "TE_SRS_SAD69_UTM_ZONE_20S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_21S] = "TE_SRS_SAD69_UTM_ZONE_21S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_22S] = "TE_SRS_SAD69_UTM_ZONE_22S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_23S] = "TE_SRS_SAD69_UTM_ZONE_23S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_24S] = "TE_SRS_SAD69_UTM_ZONE_24S";
  sridMap[TE_SRS_SAD69_UTM_ZONE_25S] = "TE_SRS_SAD69_UTM_ZONE_25S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_17N] = "TE_SRS_SIRGAS2000_UTM_ZONE_17N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_18N] = "TE_SRS_SIRGAS2000_UTM_ZONE_18N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_19N] = "TE_SRS_SIRGAS2000_UTM_ZONE_19N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_20N] = "TE_SRS_SIRGAS2000_UTM_ZONE_20N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_21N] = "TE_SRS_SIRGAS2000_UTM_ZONE_21N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_22N] = "TE_SRS_SIRGAS2000_UTM_ZONE_22N";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_17S] = "TE_SRS_SIRGAS2000_UTM_ZONE_17S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_18S] = "TE_SRS_SIRGAS2000_UTM_ZONE_18S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_19S] = "TE_SRS_SIRGAS2000_UTM_ZONE_19S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_20S] = "TE_SRS_SIRGAS2000_UTM_ZONE_20S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_21S] = "TE_SRS_SIRGAS2000_UTM_ZONE_21S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_22S] = "TE_SRS_SIRGAS2000_UTM_ZONE_22S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_23S] = "TE_SRS_SIRGAS2000_UTM_ZONE_23S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_24S] = "TE_SRS_SIRGAS2000_UTM_ZONE_24S";
  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_25S] = "TE_SRS_SIRGAS2000_UTM_ZONE_25S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_18N] = "TE_SRS_WGS84_UTM_ZONE_18N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_19N] = "TE_SRS_WGS84_UTM_ZONE_19N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_20N] = "TE_SRS_WGS84_UTM_ZONE_20N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_21N] = "TE_SRS_WGS84_UTM_ZONE_21N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_22N] = "TE_SRS_WGS84_UTM_ZONE_22N";
  sridMap[TE_SRS_WGS84_UTM_ZONE_17S] = "TE_SRS_WGS84_UTM_ZONE_17S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_18S] = "TE_SRS_WGS84_UTM_ZONE_18S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_19S] = "TE_SRS_WGS84_UTM_ZONE_19S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_20S] = "TE_SRS_WGS84_UTM_ZONE_20S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_21S] = "TE_SRS_WGS84_UTM_ZONE_21S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_22S] = "TE_SRS_WGS84_UTM_ZONE_22S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_23S] = "TE_SRS_WGS84_UTM_ZONE_23S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_24S] = "TE_SRS_WGS84_UTM_ZONE_24S";
  sridMap[TE_SRS_WGS84_UTM_ZONE_25S] = "TE_SRS_WGS84_UTM_ZONE_25S";
  sridMap[TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC] = "TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC";
  sridMap[4291] = "4291";
  sridMap[29177] = "29177";
  sridMap[29178] = "29178";
  sridMap[29179] = "29179";
  sridMap[29180] = "29180";
  sridMap[29181] = "29181";
  sridMap[29182] = "29182";
  sridMap[29183] = "29183";
  sridMap[29184] = "29184";
  sridMap[29185] = "29185";

  QStringList items;
  for(it = sridMap.begin(); it != sridMap.end(); ++it)
  {
    if(it->first == srid)
    {
      items.append(it->second);
      break;
    }
  }

  for(it = sridMap.begin(); it != sridMap.end(); ++it)
  {
    if(it->first != srid)
      items.append(it->second);
  }

  bool ok;
  QString item = QInputDialog::getItem(this, "Config SRID", "SRID:", items, 0, false, &ok);

  if(ok && !items.isEmpty())
  {
    for(it = sridMap.begin(); it != sridMap.end(); ++it)
    {
      if(item == it->second)
      {
        srid = it->first;
        setSRID(srid);
        draw();
        break;
      }
    }
  }
}

bool MyDisplay::transform(te::gm::Envelope& e, int oldsrid, int newsrid)
{
  if(oldsrid == newsrid)
    return true;

  try
  {
    double x1, y1, x2, y2, cx, cy, dx, dy;
  
    std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());

    converter->setSourceSRID(oldsrid);
    converter->setTargetSRID(newsrid);

    cx = e.getLowerLeftX() + e.getWidth() / 2.;
    cy = e.getLowerLeftY() + e.getHeight() / 2.;
    converter->convert(cx, cy);

    x1 = e.getLowerLeftX();
    y1 = e.getLowerLeftY();
    converter->convert(x1, y1);

    x2 = e.getUpperRightX();
    y2 = e.getUpperRightY();
    converter->convert(x2, y2);
    if(std::numeric_limits<double>::has_infinity)
    {
      if(x1 == std::numeric_limits<double>::infinity() || y1 == std::numeric_limits<double>::infinity() ||
         x2 == std::numeric_limits<double>::infinity() || y2 == std::numeric_limits<double>::infinity())
      {
        if(cx == std::numeric_limits<double>::infinity() || cy == std::numeric_limits<double>::infinity())
          return false;
        else
        {
          x1 = y1 = -20000000;
          x2 = y2 = 20000000;
        }
      }
    }

    dx = fabs(x2 - x1) / 2.;
    dy = fabs(y2 - y1) / 2.;

    x1 = cx - dx;
    x2 = cx + dx;
    y1 = cy - dy;
    y2 = cy + dy;

    e.init(x1, y1, x2, y2);
    if(e.getWidth() == 0. || e.getHeight() == 0.)
      return false;
    return true;
  }
  catch(std::exception& e)
  {
    QMessageBox::information(this, tr("Projection convertion error..."), tr(e.what()));
    return false;
  }
}


//#include "MyDisplay.h"
//#include "MyLayer.h"
//#include "MyLayerRenderer.h"
//#include "MyTemporalDisplay.h"
//#include "PrintInFile.h"
//#include <terralib/common/PlatformUtils.h>
//#include <terralib/dataaccess.h>
//#include <terralib/maptools.h>
//#include <terralib/raster.h>
//#include <terralib/geometry.h>
//#include <terralib/qt/widgets/utils/ScopedCursor.h>
//#include <terralib/srs/Converter.h>
//#include <terralib/srs/Config.h>
//
//#include "STExamples.h"
//
////QT
//#include <QtGui/QInputDialog>
//#include <QTimer>
//#include <QPaintEvent>
//#include <QPaintEngine>
//#include <QHBoxLayout>
//#include <QIcon>
//#include <QMessageBox>
//#include <QString>
//#include <QMatrix>
//#include <QRectF>
//#include <QPrintDialog>
//#include <QPrinter>
//#include <QUrl>
//#include <QToolTip>
//#include <QMenu.h>
//
//MyDisplay::MyDisplay(int w, int h, te::map::AbstractLayer* root, QWidget* parent, Qt::WindowFlags f) :
//  te::qt::widgets::MapDisplay(QSize(w, h), parent, f),
//  m_rootFolderLayer(root),
//  m_temporalDisplay(0)
//{
//  setObjectName("MyDisplay");
//
//  setAcceptDrops(true);
//
//  m_temporalVectorialDisplayPixmap = new QPixmap(w, h);
//  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));
//
//  m_temporalImageDisplayPixmap = new QPixmap(w, h);
//  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));
//
//  m_menu = new QMenu(this);
//
//  m_srsAction = new QAction("&Set SRID", m_menu);
//  m_menu->addAction(m_srsAction);
//  connect(m_srsAction, SIGNAL(triggered()), this, SLOT(setSRIDSlot()));
//
//  m_fitAllLayersAction = new QAction("&Fit All Layers", m_menu);
//  m_menu->addAction(m_fitAllLayersAction);
//  connect(m_fitAllLayersAction, SIGNAL(triggered()), this, SLOT(fitAllLayersSlot()));
//
//  m_mouseOperationMenu = m_menu->addMenu("Mouse Operation");
//
//  m_mouseZoomInAction = new QAction("Zoom In", m_mouseOperationMenu);
//  m_mouseZoomInAction->setCheckable(true);
//  m_mouseOperationMenu->addAction(m_mouseZoomInAction);
//  connect(m_mouseZoomInAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToZoomInSlot()));
//
//  m_mouseZoomOutAction = new QAction("Zoom Out", m_mouseOperationMenu);
//  m_mouseZoomOutAction->setCheckable(true);
//  m_mouseOperationMenu->addAction(m_mouseZoomOutAction);
//  connect(m_mouseZoomOutAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToZoomOutSlot()));
//
//  m_mousePanAction = new QAction("Pan", m_mouseOperationMenu);
//  m_mousePanAction->setCheckable(true);
//  m_mouseOperationMenu->addAction(m_mousePanAction);
//  connect(m_mousePanAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToPanSlot()));
//
//  m_mouseSelectionAction = new QAction("Object Selection", m_mouseOperationMenu);
//  m_mouseSelectionAction->setCheckable(true);
//  m_mouseOperationMenu->addAction(m_mouseSelectionAction);
//  connect(m_mouseSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToObjectSelectionSlot()));
//
//  m_mouseAddSelectionAction = new QAction("Object Add Selection", m_mouseOperationMenu);
//  m_mouseAddSelectionAction->setCheckable(true);
//  m_mouseOperationMenu->addAction(m_mouseAddSelectionAction);
//  connect(m_mouseAddSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToAddObjectSelectionSlot()));
//
//  m_mouseToggleSelectionAction = new QAction("Object Toggle selection", m_mouseOperationMenu);
//  m_mouseToggleSelectionAction->setCheckable(true);
//  m_mouseOperationMenu->addAction(m_mouseToggleSelectionAction);
//  connect(m_mouseToggleSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToToggleObjectSelectionSlot()));
//
//  m_mouseTooltipAction = new QAction("Tooltip", m_mouseOperationMenu);
//  m_mouseTooltipAction->setCheckable(true);
//  m_mouseOperationMenu->addAction(m_mouseTooltipAction);
//  connect(m_mouseTooltipAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToTooltipSlot()));
//
//  m_mouseShowCoordAction = new QAction("Show Coordenates", m_mouseOperationMenu);
//  m_mouseShowCoordAction->setCheckable(true);
//  m_mouseOperationMenu->addAction(m_mouseShowCoordAction);
//  connect(m_mouseShowCoordAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToShowCoordSlot()));
//
//  QAction* drawAllPointedsAction = new QAction("Fit All &Pointeds", m_menu);
//  m_menu->addAction(drawAllPointedsAction);
//  connect(drawAllPointedsAction, SIGNAL(triggered()), this, SLOT(drawAllPointedsSlot()));
//
//  QAction* drawAllQueriedsAction = new QAction("Fit All &Querieds", m_menu);
//  m_menu->addAction(drawAllQueriedsAction);
//  connect(drawAllQueriedsAction, SIGNAL(triggered()), this, SLOT(drawAllQueriedsSlot()));
//
//  QAction* drawAllPointedsAndQueriedsAction = new QAction("Fit &All Pointeds And Querieds", m_menu);
//  m_menu->addAction(drawAllPointedsAndQueriedsAction);
//  connect(drawAllPointedsAndQueriedsAction, SIGNAL(triggered()), this, SLOT(drawAllPointedsAndQueriedsSlot()));
//
//  QAction* showRootFolderAction = new QAction("Show Root Folder", m_menu);
//  m_menu->addAction(showRootFolderAction);
//  connect(showRootFolderAction, SIGNAL(triggered()), this, SLOT(showRootFolderSlot()));
//
//  QAction* printAction = new QAction("Print...", m_menu);
//  m_menu->addAction(printAction);
//  connect(printAction, SIGNAL(triggered()), this, SLOT(printSlot()));
//
//  QAction* printFileAction = new QAction("Print In File...", m_menu);
//  m_menu->addAction(printFileAction);
//  connect(printFileAction, SIGNAL(triggered()), this, SLOT(printFileSlot()));
//
//  m_mouseHandler = new MouseHandler(this);
//
//  connect(m_mouseHandler, SIGNAL(execZoomArea(QRect)), this, SLOT(execZoomAreaSlot(QRect)));
//  connect(m_mouseHandler, SIGNAL(execZoomIn(QPoint)), this, SLOT(execZoomInSlot(QPoint)));
//  connect(m_mouseHandler, SIGNAL(execZoomOut(QPoint)), this, SLOT(execZoomOutSlot(QPoint)));
//  connect(m_mouseHandler, SIGNAL(execPan(QPoint, QPoint)), this, SLOT(execPanSlot(QPoint, QPoint)));
//  connect(m_mouseHandler, SIGNAL(execSelection(QRect)), this, SLOT(mouseSelectionSlot(QRect)));
//  connect(m_mouseHandler, SIGNAL(execAddSelection(QRect)), this, SLOT(mouseAddSelectionSlot(QRect)));
//  connect(m_mouseHandler, SIGNAL(execToggleSelection(QRect)), this, SLOT(mouseToggleSelectionSlot(QRect)));
//  connect(m_mouseHandler, SIGNAL(execTooltip(QPoint)), this, SLOT(mouseTooltipSlot(QPoint)));
//  connect(m_mouseHandler, SIGNAL(execShowCoord(QPoint)), this, SLOT(mouseShowCoordSlot(QPoint)));
//
//  m_mouseHandler->setCursor();
//
//  setAttribute(Qt::WA_DeleteOnClose);
//
//  sizeHint();
//}
//
//MyDisplay::~MyDisplay()
//{
//  //Q_EMIT closed(this);
//  delete m_temporalVectorialDisplayPixmap;
//  delete m_temporalImageDisplayPixmap;
//  disconnect();
//}
//
//void MyDisplay::closeEvent(QCloseEvent *event)
//{
//  //setAttribute(Qt::WA_DeleteOnClose);
//  //Q_EMIT closed(this);
//  //event->accept();
//}
//
//void MyDisplay::dragEnterEvent(QDragEnterEvent* e)
//{
//  const QMimeData* mime = e->mimeData();
//  QString s = mime->data("application/layer-explorer").constData();
//  if(s.isEmpty())
//    return;
//
//  unsigned long long v = s.toULongLong();
//  te::map::AbstractLayer* al = (te::map::AbstractLayer*)v;
//  te::map::AbstractLayer* laux = al;
//  while((te::map::AbstractLayer*)laux->getParent())
//    laux = (te::map::AbstractLayer*)laux->getParent();
//
//  if(laux == m_rootFolderLayer)
//    e->accept();
//  return;
//
//  //const QMimeData* mime = e->mimeData();
//  //QString s =	mime->text(); 
//  //bool b = mime->hasColor();
//  //b = mime->hasHtml();
//  //b = mime->hasImage();
//  //b = mime->hasText();
//  //b = mime->hasUrls();
//  //QList<QUrl>	urls = mime->urls();
//  //QList<QUrl>::Iterator it;
//  //for(it = urls.begin(); it != urls.end(); ++it)
//  //{
//  //  QUrl url = *it;
//  //  QString autority = url.authority();
//  //  QString host = url.host();
//  //  QString path = url.path();
//  //  int port = url.port();
//  //  bool isLocalFile = url.isLocalFile();
//  //  bool isRelative = url.isRelative();
//  //  bool hasfragment = url.hasFragment();
//  //}
//}
//
//void MyDisplay::dropEvent(QDropEvent* e)
//{
//  const QMimeData* mime = e->mimeData();
//  QString s = mime->data("application/layer-explorer").constData();
//  unsigned long long v = s.toULongLong();
//  te::map::AbstractLayer* al = (te::map::AbstractLayer*)v;
//  changeTree(al);
//}
//
//void MyDisplay::setLayerTree(te::map::AbstractLayer* layer)
//{
//  m_layerTree = layer;
//}
//
//te::map::AbstractLayer* MyDisplay::getLayerTree()
//{
//  return m_layerTree;
//}
//
//te::gm::Envelope MyDisplay::getLayerExtent(te::map::AbstractLayer* al)
//{
//  try
//  {
//    te::map::Layer* layer = (te::map::Layer*)al;
//    if(layer->getExtent() == 0)
//    {
//      te::da::DataSourceTransactor* transactor = layer->getDataSource()->getTransactor();
//      te::da::DataSet* ds = transactor->getDataSet(layer->getId());
//      te::gm::Envelope* dsEnv = ds->getExtent();
//      layer->setExtent(dsEnv);
//      delete ds;
//      delete transactor;
//    }
//    return *(layer->getExtent());
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::changeTree(te::map::AbstractLayer* al)
//{
//  // limpe todos os canvas antes usados 
//  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
//  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
//  {
//    te::map::Canvas* c = getCanvas(it->first);
//    delete c;
//  }
//  m_layerCanvasMap.clear();
//
//  // calcule novo SRID e extent
//  m_srid = -1;
//  m_layerTree = al;
//  std::list<te::map::AbstractLayer*> layerList;
//  std::list<te::map::AbstractLayer*>::iterator lit;
//  if(m_layerTree)
//    getLayerList(m_layerTree, layerList);
//  if(m_temporalDisplay)
//    m_temporalDisplay->showOrHideTimeSlider(layerList);
//
//  m_envelope = te::gm::Envelope();
//
//  for(lit = layerList.begin(); lit != layerList.end(); ++lit)
//  {
//    te::gm::Envelope env = getLayerExtent(*lit);
//
//    int srid = (*lit)->getSRID();
//    if(m_srid <= 0)
//      m_srid = srid;
//    if(srid != m_srid)
//      transform(env, srid, m_srid);
//
//    m_envelope.Union(env);
//  }
//  setExtent();
//
//  QString wtitle = "Display: ";
//  if(m_layerTree)
//    wtitle += m_layerTree->getTitle().c_str();
//  setWindowTitle(wtitle);
//
//  QWidget* w = (QWidget*)parent();
//  while(w)
//  {
//    if(w->parent())
//      w = (QWidget*)w->parent();
//    else
//    {
//      QString mainTitle = "My Main Window - " + wtitle;
//      w->setWindowTitle(mainTitle);
//      break;
//    }
//  }
//
//  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));
//  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));
//
//  draw();
//  if(m_envelope.isValid() == false)
//  {
//    m_displayPixmap->fill();
//    update();
//  }
//}
//
//void MyDisplay::showRootFolderSlot()
//{
//  if(m_layerTree == m_rootFolderLayer)
//    return;
//
//  m_layerTree = m_rootFolderLayer;
//  changeTree(m_layerTree);
//}
//
//void MyDisplay::contextMenuEvent(QContextMenuEvent* c)
//{
//  m_mouseZoomInAction->setChecked(false);
//  m_mouseZoomOutAction->setChecked(false);
//  m_mousePanAction->setChecked(false);
//  m_mouseSelectionAction->setChecked(false);
//  m_mouseAddSelectionAction->setChecked(false);
//  m_mouseToggleSelectionAction->setChecked(false);
//  m_mouseTooltipAction->setChecked(false);
//  m_mouseShowCoordAction->setChecked(false);
//
//  MouseHandler::MouseMode mode = m_mouseHandler->getMode();
//  if(mode == MouseHandler::ZoomInMode)
//    m_mouseZoomInAction->setChecked(true);
//  else if(mode == MouseHandler::ZoomOutMode)
//    m_mouseZoomOutAction->setChecked(true);
//  else if(mode == MouseHandler::PanMode)
//    m_mousePanAction->setChecked(true);
//  else if(mode == MouseHandler::SelectionMode)
//    m_mouseSelectionAction->setChecked(true);
//  else if(mode == MouseHandler::AddSelectionMode)
//    m_mouseAddSelectionAction->setChecked(true);
//  else if(mode == MouseHandler::ToggleSelectionMode)
//    m_mouseToggleSelectionAction->setChecked(true);
//  else if(mode == MouseHandler::TooltipMode)
//    m_mouseTooltipAction->setChecked(true);
//  else if(mode == MouseHandler::ShowCoordMode)
//    m_mouseShowCoordAction->setChecked(true);
//
//  m_menu->exec(c->globalPos());
//}
//
//void MyDisplay::setMouseOperationToObjectSelectionSlot()
//{
//  m_mouseHandler->setMode(MouseHandler::SelectionMode);
//  m_mouseHandler->setCursor();
//}
//
//void MyDisplay::setMouseOperationToAddObjectSelectionSlot()
//{
//  m_mouseHandler->setMode(MouseHandler::AddSelectionMode);
//  m_mouseHandler->setCursor();
//}
//
//void MyDisplay::setMouseOperationToToggleObjectSelectionSlot()
//{
//  m_mouseHandler->setMode(MouseHandler::ToggleSelectionMode);
//  m_mouseHandler->setCursor();
//}
//
//void MyDisplay::setMouseOperationToTooltipSlot()
//{
//  m_mouseZoomInAction->setChecked(false);
//  m_mouseZoomOutAction->setChecked(false);
//  m_mousePanAction->setChecked(false);
//  m_mouseSelectionAction->setChecked(false);
//  m_mouseAddSelectionAction->setChecked(false);
//  m_mouseToggleSelectionAction->setChecked(false);
//  m_mouseTooltipAction->setChecked(true);
//
//  m_mouseHandler->setMode(MouseHandler::TooltipMode);
//  m_mouseHandler->setCursor();
//}
//
//void MyDisplay::setMouseOperationToShowCoordSlot()
//{
//  m_mouseZoomInAction->setChecked(false);
//  m_mouseZoomOutAction->setChecked(false);
//  m_mousePanAction->setChecked(false);
//  m_mouseSelectionAction->setChecked(false);
//  m_mouseAddSelectionAction->setChecked(false);
//  m_mouseToggleSelectionAction->setChecked(false);
//  m_mouseTooltipAction->setChecked(false);
//  m_mouseShowCoordAction->setChecked(true);
//
//  m_mouseHandler->setMode(MouseHandler::ShowCoordMode);
//  m_mouseHandler->setCursor();
//}
//
//void MyDisplay::setMouseOperationToZoomInSlot()
//{
//  m_mouseHandler->setMode(MouseHandler::ZoomInMode);
//  m_mouseHandler->setCursor();
//}
//
//void MyDisplay::setMouseOperationToZoomOutSlot()
//{
//  m_mouseHandler->setMode(MouseHandler::ZoomOutMode);
//  m_mouseHandler->setCursor();
//}
//
//void MyDisplay::setMouseOperationToPanSlot()
//{
//  m_mouseHandler->setMode(MouseHandler::PanMode);
//  m_mouseHandler->setCursor();
//}
//
//
//void MyDisplay::selectionChangedSlot(te::map::DataGridOperation* op)
//{
//  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
//  draw();
//}
//
//void MyDisplay::removeDrawOnlyChanged(te::map::AbstractLayer* al)
//{
//  m_drawOnlyChanged.erase(al);
//}
//
//void MyDisplay::addDrawOnlyChanged(te::map::AbstractLayer* al)
//{
//  m_drawOnlyChanged.insert(al);
//}
//
//// se o layer e' do tipo temporal
//// existe apenas um canvas associado ao pai do layer.
//te::qt::widgets::Canvas* MyDisplay::getCanvas(te::map::AbstractLayer* layer)
//{
//  MyLayer* l = (MyLayer*)layer;
//  if(m_extent == 0)
//  {
//    if(layer->getExtent())
//      m_extent = new te::gm::Envelope(*(layer->getExtent()));
//    else
//      return 0;
//  }
//
//  if(l->isTemporal())
//  {
//    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)l->getParent();
//    return te::qt::widgets::MapDisplay::getCanvas(parent);
//  }
//  return te::qt::widgets::MapDisplay::getCanvas(l);
//}
//
//// se o layer e' do tipo temporal
//// existe apenas um canvas associado ao pai do layer.
//void MyDisplay::setCanvas(te::map::AbstractLayer* layer)
//{
//  MyLayer* l = (MyLayer*)layer;
//  if(l->isTemporal())
//  {
//    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)l->getParent();    
//    if(m_layerCanvasMap.find(parent) == m_layerCanvasMap.end())
//    {
//      te::qt::widgets::Canvas* c = new te::qt::widgets::Canvas(m_displayPixmap->width(), m_displayPixmap->height());
//      m_layerCanvasMap[parent] = c;
//    }  
//  }
//  else
//  {
//    if(m_layerCanvasMap.find(l) == m_layerCanvasMap.end())
//    {
//      te::qt::widgets::Canvas* c = new te::qt::widgets::Canvas(m_displayPixmap->width(), m_displayPixmap->height());
//      m_layerCanvasMap[l] = c;
//    }  
//  }
//}
//
//void MyDisplay::draw()
//{
//  try
//  {
//    if(m_temporalDisplay)
//    {
//      TimeSlider* ts = m_temporalDisplay->getTimeSlider();
//      if(ts)
//      {
//        std::vector<te::map::AbstractLayer*> layers =  ts->getLayers();
//        std::vector<te::map::AbstractLayer*>::iterator it;
//        for(it = layers.begin(); it != layers.end(); ++it)
//          clearTemporalCanvas(*it);
//      }
//    }
//
//    if(m_displayPixmap)
//    {
//      m_displayPixmap->fill(m_backgroundColor);
//
//      if(m_layerTree) // Use the tree if it exists, otherwise use the list of layers.
//      {
//        std::list<te::map::AbstractLayer*> layerList;
//        getLayerList(m_layerTree, layerList);
//        draw(layerList);
//      }
//      else if(m_layerList.empty() == false)
//        draw(m_layerList);
//    }
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::draw(std::list<te::map::AbstractLayer*>& layerList)
//{
//  try
//  {
//    QPainter painter(m_displayPixmap);
//    std::list<te::map::AbstractLayer*>::iterator it;
// 
//    if(m_srid <= 0 || m_extent == 0)
//    {
//      if(m_extent == 0)
//        m_extent = new te::gm::Envelope();
//      for(it = layerList.begin(); it != layerList.end(); ++it)
//      {
//        te::gm::Envelope env = getLayerExtent(*it);
//
//        int srid = (*it)->getSRID();
//        if(m_srid <= 0)
//          m_srid = srid;
//        if(srid != m_srid)
//          transform(env, srid, m_srid);
//
//        m_extent->Union(env);
//      }
//      if(m_extent->isValid() == false)
//      {
//        delete m_extent;
//        m_extent = 0;
//        return;
//      }
//      m_envelope = *m_extent;
//    }
//    if(m_srid <= 0)
//      return;
//
//    for(it = layerList.begin(); it != layerList.end(); ++it)
//    {
//      te::qt::widgets::Canvas *c = getCanvas(*it);
//      if(c == 0)
//      {
//        setCanvas(*it);
//        c = getCanvas(*it);
//        c->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
//        c->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
//      }
//      draw(*it);
//
//      if(m_displayPixmap->size() == c->getPixmap()->size())
//        painter.drawPixmap(0, 0, *(c->getPixmap()));
//    }
//    update();
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::draw(te::map::AbstractLayer* al)
//{
//  try
//  {
//    te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
//    te::qt::widgets::Canvas* canvas = getCanvas(al);
//    if(canvas == 0)
//      return;
//
//    MyLayer* layer =  (MyLayer*)al;
//    te::map::DataGridOperation* op = layer->getDataGridOperation();
//    if(op == 0 && layer->isTemporal() == false)
//    {
//      std::string name = layer->getId();
//      te::da::DataSource* ds = layer->getDataSource();
//      te::da::DataSourceTransactor* t = ds->getTransactor();
//      te::da::DataSet* dataSet = t->getDataSet(name);
//      te::da::DataSetType* dsType = ds->getCatalog()->getDataSetType(name);
//
//      assert(dataSet);
//
//      if(dsType->getPrimaryKey())
//      {
//        op = new te::map::DataGridOperation();
//        op->init(dsType, dataSet);
//        layer->setDataGridOperation(op);
//      }
//    }
//
//    bool b = false;
//    if(m_drawOnlyChanged.find(layer) != m_drawOnlyChanged.end())
//      b = true;
//    MyLayerRenderer* renderer = new MyLayerRenderer(b);
//    layer->setRenderer(renderer);
//
//    if(layer->isTemporal() == false)
//      al->draw(canvas, *m_extent, m_srid);
//
//    addDrawOnlyChanged(layer);
//  }
//  catch(std::exception& e)
//  {
//    QMessageBox::information(this, tr("Error Drawing..."), tr(e.what()));
//    return;
//  }
//}
//
//void MyDisplay::reorderDrawing(std::vector<te::map::AbstractLayer*> layers)
//{
//  try
//  {
//    te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
//
//    m_displayPixmap->fill();
//    QPainter painter(m_displayPixmap);
//
//    std::vector<te::map::AbstractLayer*>::reverse_iterator it;
//    for(it = layers.rbegin(); it != layers.rend(); ++it)
//    {
//      MyLayer* layer =  (MyLayer*)(*it);
//      te::qt::widgets::Canvas* c = getCanvas(layer);
//      if(layer->getVisibility() == te::map::VISIBLE)
//      {
//        if(c == 0)
//        {
//          setCanvas(layer);
//          c = getCanvas(layer);
//          draw(layer);
//        }
//        painter.drawPixmap(0, 0, *(c->getPixmap()));
//      }
//    }
//  }
//  catch(std::exception& e)
//  {
//    QMessageBox::information(this, tr("Error Drawing..."), tr(e.what()));
//    return;
//  }
//}
//
//te::gm::Envelope MyDisplay::getAllExtent()
//{
//  try
//  {
//    if(m_layerList.empty() == false)
//      return getAllExtent(m_layerList);
//
//    std::list<te::map::AbstractLayer*> layerList;
//    if(m_layerTree)
//      getLayerList(m_layerTree, layerList);
//    return getAllExtent(layerList);
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//te::gm::Envelope MyDisplay::getAllExtent(std::list<te::map::AbstractLayer*>& layerList)
//{
//  try
//  {
//    std::list<te::map::AbstractLayer*>::iterator it;
//
//    te::gm::Envelope extent;
//    for(it = layerList.begin(); it != layerList.end(); ++it)
//    {
//      te::gm::Envelope env = getLayerExtent(*it);
//
//      int srid = (*it)->getSRID();
//      if(srid > 0)
//      {
//        if(m_srid <= 0)
//          m_srid = srid;
//        if(srid != m_srid)
//          transform(env, srid, m_srid);
//      }
//
//      extent.Union(env);
//    }
//    return extent;
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::clearTemporalPixmaps(std::vector<te::map::AbstractLayer*> layers)
//{
//  std::set<te::map::AbstractLayer*> als;
//  te::map::AbstractLayer* al;
//
//  std::vector<te::map::AbstractLayer*>::iterator it;
//  for(it = layers.begin(); it != layers.end(); ++it)
//  {
//    al = (te::map::AbstractLayer*)(*it)->getParent();
//    if(als.find(al) == als.end())
//    {
//      als.insert(al);
//      clearTemporalCanvas(*it);
//    }
//  }
//
//  int w = m_displayPixmap->width();
//  int h = m_displayPixmap->height();
//
//  if(w != m_temporalVectorialDisplayPixmap->width() || h != m_temporalVectorialDisplayPixmap->height())
//  {
//    delete m_temporalVectorialDisplayPixmap;
//    m_temporalVectorialDisplayPixmap = new QPixmap(w, h);
//  }
//  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));
//
//  if(w != m_temporalImageDisplayPixmap->width() || h != m_temporalImageDisplayPixmap->height())
//  {
//    delete m_temporalImageDisplayPixmap;
//    m_temporalImageDisplayPixmap = new QPixmap(w, h);
//  }
//  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));
//}
//
//void MyDisplay::clearTemporalCanvas(te::map::AbstractLayer* al)
//{
//  te::qt::widgets::Canvas* canvas = getCanvas(al);
//  if(canvas)
//    canvas->clear();
//}
//
//void MyDisplay::paintEvent(QPaintEvent* e)
//{
//  if(m_displayPixmap == 0)
//  return;
//
//  double w, h, nw, nh, sw, sh;
//  QMatrix m;
//  QPointF p1, p2;
//  te::qt::widgets::Canvas *c = 0;
//
//  if(m_resize == false)
//  {
//    if(m_repaint)
//    {
//      QPainter painter(this);
//      QRect rec = e->rect();
//      painter.drawPixmap(rec, *m_displayPixmap, rec);
//    }
//  }
//  else // fazendo resize do display
//  {
//    if(m_layerCanvasMap.begin() != m_layerCanvasMap.end())
//      c = m_layerCanvasMap.begin()->second;
//
//    if(c == 0)
//      return;
//
//    m = c->getMatrix();
//
//    p1 = m.map(QPointF(m_envelope.getLowerLeftX(), m_envelope.getLowerLeftY()));
//    p2 = m.map(QPointF(m_envelope.getUpperRightX(), m_envelope.getUpperRightY()));
//    m_resizeWRec = QRect(QPoint(p1.x(), p2.y()), QPoint(p2.x(), p1.y()));
//    nw = m_resizeWRec.width();
//    nh = m_resizeWRec.height();
//
//    QPainter painter(this);
//    m_resizeRec = rect();
//    w = m_resizeRec.width();
//    h = m_resizeRec.height();
//
//    QPixmap pix(w, h);
//    pix.fill();
//    painter.drawPixmap(0, 0, pix);
//
//    sh = h/m_resizeWRec.height();
//    sw = w/m_resizeWRec.width();
//    if(sw < sh)
//    {
//      nw *= sw;
//      nh *= sw;
//    }
//    else if(sh < sw)
//    {
//      nw *= sh;
//      nh *= sh;
//    }
//
//    if(m_hAlign == te::map::Center)
//    {
//      m_resizeRec.setLeft((w - nw) / 2);
//      m_resizeRec.setRight(m_resizeRec.left() + nw);
//    }
//    else if(m_hAlign == te::map::Left)
//      m_resizeRec.setRight(nw);
//    else if(m_hAlign == te::map::Right)
//      m_resizeRec.setLeft(m_resizeRec.width() - nw);
//
//    if(m_vAlign == te::map::Center)
//    {
//      m_resizeRec.setTop((h - nh) / 2);
//      m_resizeRec.setBottom(m_resizeRec.top() + nh);
//    }
//    else if(m_vAlign == te::map::Top)
//      m_resizeRec.setBottom(nh);
//    else if(m_vAlign == te::map::Bottom)
//      m_resizeRec.setTop(m_resizeRec.height() - nh);
//
//    painter.drawPixmap(m_resizeRec, *m_displayPixmap, m_resizeWRec);
//  }
//
//  QRect rec, wrec;
//
//  if(m_resize == false)
//  {
//    if(m_repaint)
//    {
//      QPainter painter(this);
//      QRect rec = e->rect();
//      painter.drawPixmap(rec, *m_temporalVectorialDisplayPixmap, rec);
//      painter.drawPixmap(rec, *m_temporalImageDisplayPixmap, rec);
//    }
//  }
//  else // fazendo resize do display com os pixmaps temporais
//  {
//    if(m_layerCanvasMap.begin() != m_layerCanvasMap.end())
//      c = m_layerCanvasMap.begin()->second;
//
//    if(c == 0)
//      return;
//
//    QPainter painter(this);
//    painter.drawPixmap(m_resizeRec, *m_temporalVectorialDisplayPixmap, m_resizeWRec);
//    painter.drawPixmap(m_resizeRec, *m_temporalImageDisplayPixmap, m_resizeWRec);
//  }
//}
//
//void MyDisplay::resizeEvent(QResizeEvent* e)
//{ 
//  m_resize = true;
//  QWidget::resizeEvent(e);
//
//  if(m_timer)
//  {
//    m_timer->disconnect();
//    delete m_timer;
//  }
//  m_timer = new QTimer;
//  m_timer->setSingleShot(true);
//  m_timer->start(500);
//  connect(m_timer, SIGNAL(timeout()), this, SLOT(onResizeTimeout()));
//}
//
//void MyDisplay::onResizeTimeout()
//{
//  m_resize = false;
//  if(m_displayPixmap)
//    delete m_displayPixmap;
//  m_displayPixmap = new QPixmap(width(), height());
//  m_displayPixmap->fill(QColor(0, 0, 0, 0));
//  int w = m_displayPixmap->width();
//  int h = m_displayPixmap->height();
//
//  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
//
//  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
//  {
//    te::qt::widgets::Canvas *c = it->second;
//    c->resize(w, h);
//  }
//  setExtent();
//
//  draw();
//
//  emit sizeChanged(QSize(w, h));
//}
//
//
//void MyDisplay::setExtent()
//{
//  m_drawOnlyChanged.clear();
//  te::map::MapDisplay::setExtent(m_envelope);
//  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
//  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
//  {
//    te::qt::widgets::Canvas* canvas = it->second;
//    canvas->calcAspectRatio(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury, m_hAlign, m_vAlign);
//    canvas->setWindow(m_extent->m_llx, m_extent->m_lly, m_extent->m_urx, m_extent->m_ury);
//    canvas->clear();
//  }
//
//  if(m_temporalDisplay)
//  {
//    TimeSlider* ts = m_temporalDisplay->getTimeSlider();
//    if(ts)
//      clearTemporalPixmaps(ts->getLayers());
//  }
//}
//
//void MyDisplay::setRepaint(bool s)
//{
//  m_repaint = s;
//}
//
//
//void MyDisplay::setSRID(const int& srid)
//{
//  m_drawOnlyChanged.clear();
//  if(m_srid == srid)
//    return;
//
//  if(m_extent)
//  {
//    m_envelope = *m_extent;
//    if(transform(m_envelope, m_srid, srid) == false)
//    {
//      QMessageBox::information(this, tr("Projection Fail..."), tr("Fail to change projection!"));
//      return;
//    }
//    setExtent();
//  }
//  m_srid = srid;
//}
//
//void MyDisplay::changeObjectStatus(QRect rec, const std::string& mode)
//{
//  std::list<te::map::AbstractLayer*> layerList;
//  getLayerList(m_layerTree, layerList);
//
//  std::list<te::map::AbstractLayer*>::iterator it;
//  if(layerList.size() == 0)
//    return;
//
//  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
//  for(it = layerList.begin(); it != layerList.end(); ++it)
//  {
//    MyLayer* layer = (MyLayer*)*it;
//    te::map::DataGridOperation* op = layer->getDataGridOperation();
//    if(op == 0)
//      continue;
//
//    te::qt::widgets::Canvas* canvas = getCanvas(layer);
//
//    te::gm::Point worldPoint(0., 0., m_srid);
//    QPointF p(rec.bottomLeft().x(), rec.bottomLeft().y());
//    p = canvas->getMatrix().inverted().map(p);
//    worldPoint.setX(p.x());
//    worldPoint.setY(p.y());
//
//    te::gm::Polygon worldRec(0, te::gm::PolygonType, m_srid);
//    QRectF r(rec);
//    r = canvas->getMatrix().inverted().mapRect(r);
//    // tem que ser ponteiro porque o destrutor de polygon deleta os rings
//    te::gm::LinearRing* line = new te::gm::LinearRing(5, te::gm::LineStringType);
//    line->setPoint(0, r.bottomLeft().x(), r.topRight().y()); // lower left
//    line->setPoint(1, r.bottomRight().x(), r.topRight().y()); // lower rigth
//    line->setPoint(2, r.topRight().x(), r.bottomRight().y()); // upper rigth
//    line->setPoint(3, r.topLeft().x(), r.bottomLeft().y()); // upper left
//    line->setPoint(4, r.bottomLeft().x(), r.topRight().y()); // closing
//    worldRec.push_back(line);
//
//    te::gm::Polygon worldRecExpanded(0, te::gm::PolygonType, m_srid);
//    QRectF re(0., 0., 9., 9.);
//    re.moveCenter(rec.center());
//    re = canvas->getMatrix().inverted().mapRect(re);
//    // tem que ser ponteiro porque o destrutor de polygon deleta os rings
//    te::gm::LinearRing* linee = new te::gm::LinearRing(5, te::gm::LineStringType);
//    linee->setPoint(0, re.bottomLeft().x(), re.topRight().y()); // lower left
//    linee->setPoint(1, re.bottomRight().x(), re.topRight().y()); // lower rigth
//    linee->setPoint(2, re.topRight().x(), re.bottomRight().y()); // upper rigth
//    linee->setPoint(3, re.topLeft().x(), re.bottomLeft().y()); // upper left
//    linee->setPoint(4, re.bottomLeft().x(), re.topRight().y()); // closing
//    worldRecExpanded.push_back(linee);
// 
//    te::da::DataSet* dataSet = op->getDataSet();
//    te::da::DataSetType* dsType = op->getDataSetType();
//    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
//    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
//    std::string pkv;
//    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
//    int gtype = dsType->getDefaultGeomProperty()->getGeometryType();
//
//    std::vector<int> visRows;
//    dataSet->moveBeforeFirst();
//    while(dataSet->moveNext())
//    {
//      pkv.clear();
//      for (size_t i = 0; i < pkProps.size(); ++i)
//        pkv += dataSet->getAsString(pkProps[i]->getName());
//
//      te::gm::Geometry* g = dataSet->getGeometry(gPos);
//      if(g == 0)
//        continue;
//      g->setSRID(layer->getSRID());
//      g->transform(m_srid);
//
//      if(gtype == te::gm::PointType || gtype == te::gm::MultiPointType)
//      {
//        if(rec.width() == 1 && rec.height() == 1)
//        {
//          if(worldRecExpanded.contains(g))
//            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
//        }
//        else
//        {
//          if(worldRec.contains(g))
//            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
//        }
//      }
//      else if(gtype == te::gm::LineStringType || gtype == te::gm::MultiLineStringType)
//      { 
//        if(rec.width() == 1 && rec.height() == 1)
//        {
//          if(g->intersects(&worldRecExpanded))
//            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
//        }
//        else
//        {
//          if(g->intersects(&worldRec))
//            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
//        }
//      }
//      else if(gtype == te::gm::PolygonType || gtype == te::gm::MultiPolygonType)
//      {
//        if(rec.width() == 1 && rec.height() == 1)
//        {
//          if(g->contains(&worldPoint))
//            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
//        }
//        else
//        {
//          if(g->intersects(&worldRec))
//            visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
//        }
//      }
//
//      delete g;
//    }
//
//    if(visRows.empty())
//      continue;
//
//    if(mode == "POINTING")
//      op->setRowsAsPointed(visRows);
//    else if(mode == "ADDPOINTING")
//      op->addRowsToPointed(visRows);
//    else if(mode == "TOGGLEPOINTING")
//      op->toggleRowsPointingStatus(visRows);
//    Q_EMIT selectionChanged(op);
//  }
//}
//
//void MyDisplay::execZoomAreaSlot(const QRect& rec)
//{
//  try
//  {
//    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
//    if(canvas == NULL)
//      return;
//
//    QPointF pll(rec.left(), rec.bottom());
//    QPointF pur(rec.right(), rec.top());
//
//    pll = canvas->getMatrix().inverted().map(pll);
//    pur = canvas->getMatrix().inverted().map(pur);
//    m_envelope = te::gm::Envelope(pll.x(), pll.y(), pur.x(), pur.y());
//
//    setExtent();
//
//    draw();
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::execZoomInSlot(const QPoint& p)
//{
//  try
//  {
//    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
//    if(canvas == NULL)
//      return;
//
//    QRect r(0, 0, canvas->getWidth(), canvas->getHeight());
//    r.setWidth(r.width() / 2);
//    r.setHeight(r.height() / 2);
//    r.moveCenter(p);
//    QPointF pll(r.left(), r.bottom());
//    QPointF pur(r.right(), r.top());
//
//    pll = canvas->getMatrix().inverted().map(pll);
//    pur = canvas->getMatrix().inverted().map(pur);
//    m_envelope = te::gm::Envelope(pll.x(), pll.y(), pur.x(), pur.y());
//
//    setExtent();
//
//    draw();
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::execZoomOutSlot(const QPoint& p)
//{
//  try
//  {
//    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
//    if(canvas == NULL)
//      return;
//
//    QRect rec(0, 0, canvas->getWidth(), canvas->getHeight());
//    rec.setWidth(rec.width() * 2);
//    rec.moveCenter(p);
//    QPointF pll(rec.left(), rec.bottom());
//    QPointF pur(rec.right(), rec.top());
//
//    pll = canvas->getMatrix().inverted().map(pll);
//    pur = canvas->getMatrix().inverted().map(pur);
//    m_envelope = te::gm::Envelope(pll.x(), pll.y(), pur.x(), pur.y());
//    setExtent();
//
//    draw();
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::execPanSlot(const QPoint& from, const QPoint& to)
//{
//  try
//  {
//    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
//    if(canvas == NULL)
//      return;
//  
//    QPoint p = to - from;
//    QRect rec(0, 0, canvas->getWidth(), canvas->getHeight());
//    QPoint c = rec.center();
//    c -= p;
//    rec.moveCenter(c);
//
//    QPointF pll(rec.left(), rec.bottom());
//    QPointF pur(rec.right(), rec.top());
//
//    pll = canvas->getMatrix().inverted().map(pll);
//    pur = canvas->getMatrix().inverted().map(pur);
//    m_envelope = te::gm::Envelope(pll.x(), pll.y(), pur.x(), pur.y());
//
//    setExtent();
//    draw();
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::mouseSelectionSlot(QRect rec)
//{
//  changeObjectStatus(rec, "POINTING");
//}
//
//void MyDisplay::mouseAddSelectionSlot(QRect rec)
//{
//  changeObjectStatus(rec, "ADDPOINTING");
//}
//
//void MyDisplay::mouseToggleSelectionSlot(QRect rec)
//{
//  changeObjectStatus(rec, "TOGGLEPOINTING");
//}
//
//void MyDisplay::mouseTooltipSlot(QPoint p)
//{
//  std::list<te::map::AbstractLayer*> layerList;
//  getLayerList(m_layerTree, layerList);
//
//  std::list<te::map::AbstractLayer*>::iterator it;
//  if(layerList.size() == 0)
//    return;
//
//  for(it = layerList.begin(); it != layerList.end(); ++it)
//  {
//    MyLayer* layer = (MyLayer*)*it;
//    te::map::DataGridOperation* op = layer->getDataGridOperation();
//    if(op == 0)
//      continue;
//
//    std::vector<int> tooltipColumns = layer->getTooltipColumns();
//    if(tooltipColumns.empty())
//      continue;
//    
//    te::qt::widgets::Canvas* canvas = getCanvas(layer);
//    if(canvas == 0)
//      continue;
//
//    QRectF mouseRectF(0., 0., 5., 5.);
//    mouseRectF.moveCenter(p);     
//
//    //get coordenates in the display projection
//    QPointF pw = canvas->getMatrix().inverted().map(QPointF(p));
//    te::gm::Point mouseWPos(pw.x(), pw.y(), m_srid);
//
//    mouseRectF = canvas->getMatrix().inverted().mapRect(mouseRectF);
//    // bottom and top are inverted in device coordenates
//    te::gm::Envelope env(mouseRectF.bottomLeft().x(), mouseRectF.topRight().y(), mouseRectF.topRight().x(), mouseRectF.bottomLeft().y());
//    te::gm::Polygon* poly = new te::gm::Polygon(0, te::gm::PolygonType, m_srid);
//    te::gm::LinearRing* line = new te::gm::LinearRing(5, te::gm::LineStringType, m_srid);
//    line->setPoint(0, env.getLowerLeftX(), env.getLowerLeftY()); // lower left
//    line->setPoint(1, env.getUpperRightX(), env.getLowerLeftY()); // lower rigth
//    line->setPoint(2, env.getUpperRightX(), env.getUpperRightY()); // upper rigth
//    line->setPoint(3, env.getLowerLeftX(), env.getUpperRightY()); // upper left
//    line->setPoint(4, env.getLowerLeftX(), env.getLowerLeftY()); // closing
//
//    poly->push_back(line);
//
//    te::da::DataSet* dataSet = op->getDataSet();
//    te::da::DataSetType* dsType = op->getDataSetType();
//    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
//    te::gm::GeomType gtype = dsType->getDefaultGeomProperty()->getGeometryType();
//
//    QString val;
//    std::vector<QString> values;
//    std::vector<int>::iterator it;
//    dataSet->moveBeforeFirst();
//    while(dataSet->moveNext())
//    {
//      te::gm::Geometry* g = dataSet->getGeometry(gPos);
//      if(g == 0)
//        continue;
//      
//      g->setSRID(layer->getSRID());
//      g->transform(m_srid);
//
//      if(gtype == te::gm::PolygonType || gtype == te::gm::MultiPolygonType)
//      {
//        if(g->contains(&mouseWPos))
//        {
//          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
//          {
//            val = dsType->getProperty(*it)->getName().c_str();
//            val += ": ";
//            if(dataSet->isNull(*it) == false)
//              val += dataSet->getAsString(*it).c_str();
//            values.push_back(val);
//          }
//          delete g;
//          break;
//        }
//      }
//      else if(gtype == te::gm::PointType || gtype == te::gm::MultiPointType)
//      {
//        if(poly->contains(g))
//        {
//          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
//          {
//            val = dsType->getProperty(*it)->getName().c_str();
//            val += ": ";
//            if(dataSet->isNull(*it) == false)
//              val += dataSet->getAsString(*it).c_str();
//            values.push_back(val);
//          }
//          delete g;
//          break;
//        }
//      }
//      else if(gtype == te::gm::LineStringType || gtype == te::gm::MultiLineStringType)
//      {
//        if(g->crosses(poly))
//        {
//          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
//          {
//            val = dsType->getProperty(*it)->getName().c_str();
//            val += ": ";
//            if(dataSet->isNull(*it) == false)
//              val += dataSet->getAsString(*it).c_str();
//            values.push_back(val);
//          }
//          delete g;
//          break;
//        }
//      }
//      delete g;
//    }
//    delete poly;
//
//    if(values.empty() == false)
//    {
//      QString s;
//      std::vector<QString>::iterator it = values.begin();
//      s = *it;
//      it++;
//      while(it != values.end())
//      {
//        s += "\n" + *it;
//        it++;
//      }
//
//      if(s != QToolTip::text())
//        QToolTip::showText(mapToGlobal(p), s);
//    }
//    else
//      QToolTip::hideText();
//  }
//}
//
//void MyDisplay::mouseShowCoordSlot(QPoint p)
//{
//  QString s, pixx, pixy, dispx, dispy, layerx, layery, srid;
//
//  pixx.setNum(p.x());
//  pixy.setNum(p.y());
//
//  s += "display(pixel) x:" + pixx + " y:" + pixy;
//
//  std::list<te::map::AbstractLayer*> layerList;
//  getLayerList(m_layerTree, layerList);
//
//  std::list<te::map::AbstractLayer*>::iterator it;
//
//  for(it = layerList.begin(); it != layerList.end(); ++it)
//  {
//    MyLayer* layer = (MyLayer*)*it;
//
//    te::qt::widgets::Canvas* canvas = getCanvas(layer);
//    if(canvas)
//    {
//      //get coodenates in the display projection
//      QPointF pf = canvas->getMatrix().inverted().map(QPointF(p));
//      if(dispx.isEmpty())
//      {
//        dispx.setNum(pf.x());
//        dispy.setNum(pf.y());
//        srid.setNum(m_srid);
//        s += "\ndisplay(" + srid + ") x:" + dispx + " y:" + dispy;
//      }
//
//      te::gm::Point g(pf.x(), pf.y());
//      g.setSRID(m_srid);
//
//      //transform to layer projection
//      g.transform(layer->getSRID());
//      srid.setNum(layer->getSRID());
//      layerx.setNum(g.getX());
//      layery.setNum(g.getY());
//      s += "\n";
//      s +=  layer->getId().c_str();
//      s += "(" + srid + ") x:" + layerx + " y:" + layery;
//    }
//  }
//  if(dispx.isEmpty() == false)
//
//  QToolTip::showText(mapToGlobal(p), s);
//}
//
//void MyDisplay::drawAllPointedsSlot()
//{
//  te::qt::widgets::Canvas* canvas;
//  int status = -1;
//  std::list<te::map::AbstractLayer*> layerList;
//  getLayerList(m_layerTree, layerList);
//
//  std::list<te::map::AbstractLayer*>::iterator it;
//  if(layerList.size() == 0)
//    return;
//
//  te::gm::Envelope fitEnv;
//  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
//  for(it = layerList.begin(); it != layerList.end(); ++it)
//  {
//    MyLayer* layer = (MyLayer*)*it;
//    te::map::DataGridOperation* op = layer->getDataGridOperation();
//    if(op == 0)
//      continue;
//    canvas = getCanvas(layer);
//   
//    te::da::DataSet* dataSet = op->getDataSet();
//    te::da::DataSetType* dsType = op->getDataSetType();
//    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
//    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
//    std::string pkv;
//    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
//
//    dataSet->moveBeforeFirst();
//    while(dataSet->moveNext())
//    {
//      te::gm::Geometry* g = dataSet->getGeometry(gPos);
//      if(g == 0)
//        continue;
//
//      pkv.clear();
//      for (size_t i = 0; i < pkProps.size(); ++i)
//        pkv += dataSet->getAsString(pkProps[i]->getName());
//
//      status = op->getDataSetSelectionStatus(pkv);
//      if(status == te::map::DataGridOperation::POINTED || status == te::map::DataGridOperation::POINTED_AND_QUERIED)
//      {
//        te::gm::Envelope env(*g->getMBR());
//        if(transform(env, layer->getSRID(), m_srid))
//          fitEnv.Union(env);
//      }
//      delete g;
//    }
//  }
//
//  if(fitEnv.isValid())
//  {
//    m_envelope = fitEnv;
//    setExtent();
//    draw();
//  }
//  else
//  {
//    if(status != -1)
//    {
//      QMessageBox::information(this, tr("Projection Error"), tr("Convertion Error"));
//      return;
//    }
//  }
//}
//
//void MyDisplay::drawAllQueriedsSlot()
//{
//  te::qt::widgets::Canvas* canvas;
//  int status;
//  std::list<te::map::AbstractLayer*> layerList;
//  getLayerList(m_layerTree, layerList);
//
//  std::list<te::map::AbstractLayer*>::iterator it;
//  if(layerList.size() == 0)
//    return;
//
//  te::gm::Envelope fitEnv;
//  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
//  for(it = layerList.begin(); it != layerList.end(); ++it)
//  {
//    MyLayer* layer = (MyLayer*)*it;
//    te::map::DataGridOperation* op = layer->getDataGridOperation();
//    if(op == 0)
//      continue;
//    canvas = getCanvas(layer);
//   
//    te::da::DataSet* dataSet = op->getDataSet();
//    te::da::DataSetType* dsType = op->getDataSetType();
//    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
//    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
//    std::string pkv;
//    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
//
//    dataSet->moveBeforeFirst();
//    while(dataSet->moveNext())
//    {
//      te::gm::Geometry* g = dataSet->getGeometry(gPos);
//      if(g == 0)
//        continue;
//
//      pkv.clear();
//      for (size_t i = 0; i < pkProps.size(); ++i)
//        pkv += dataSet->getAsString(pkProps[i]->getName());
//
//      status = op->getDataSetSelectionStatus(pkv);
//      if(status == te::map::DataGridOperation::QUERIED || status == te::map::DataGridOperation::POINTED_AND_QUERIED)
//      {
//        te::gm::Envelope env(*g->getMBR());
//        if(transform(env, layer->getSRID(), m_srid))
//          fitEnv.Union(env);
//      }
//      delete g;
//    }
//  }
//
//  if(fitEnv.isValid())
//  {
//    m_envelope = fitEnv;
//    setExtent();
//    draw();
//  }
//  else
//  {
//    if(status != -1)
//    {
//      QMessageBox::information(this, tr("Projection Error"), tr("Convertion Error"));
//      return;
//    }
//  }
//}
//
//void MyDisplay::drawAllPointedsAndQueriedsSlot()
//{
//  te::qt::widgets::Canvas* canvas;
//  QRectF recF;
//  int status;
//  std::list<te::map::AbstractLayer*> layerList;
//  getLayerList(m_layerTree, layerList);
//
//  std::list<te::map::AbstractLayer*>::iterator it;
//  if(layerList.size() == 0)
//    return;
//
//  te::gm::Envelope fitEnv;
//  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
//  for(it = layerList.begin(); it != layerList.end(); ++it)
//  {
//    MyLayer* layer = (MyLayer*)*it;
//    te::map::DataGridOperation* op = layer->getDataGridOperation();
//    if(op == 0)
//      continue;
//    canvas = getCanvas(layer);
//   
//    te::da::DataSet* dataSet = op->getDataSet();
//    te::da::DataSetType* dsType = op->getDataSetType();
//    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
//    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
//    std::string pkv;
//    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
//
//    dataSet->moveBeforeFirst();
//    while(dataSet->moveNext())
//    {
//      te::gm::Geometry* g = dataSet->getGeometry(gPos);
//      if(g == 0)
//        continue;
//
//      pkv.clear();
//      for (size_t i = 0; i < pkProps.size(); ++i)
//        pkv += dataSet->getAsString(pkProps[i]->getName());
//
//      status = op->getDataSetSelectionStatus(pkv);
//      if(status == te::map::DataGridOperation::POINTED_AND_QUERIED)
//      {
//        te::gm::Envelope env(*g->getMBR());
//        if(transform(env, layer->getSRID(), m_srid))
//          fitEnv.Union(env);
//      }
//      delete g;
//    }
//  }
//
//  if(fitEnv.isValid())
//  {
//    m_envelope = fitEnv;
//    setExtent();
//    draw();
//  }
//  else
//  {
//    if(status != -1)
//    {
//      QMessageBox::information(this, tr("Projection Error"), tr("Convertion Error"));
//      return;
//    }
//  }
//}
//
//void MyDisplay::printSlot()
//{
//  QPrintDialog w(this);
//  
//  if(w.exec() ==  QDialog::Rejected)
//    return;
//
//  QPrinter* printer = w.printer();
//
//  int fx = printer->physicalDpiX();
//  int fy = printer->physicalDpiY();
//  printer->setResolution(fx);  // precisa fazer isto para que a impressora tenha a resolucao configurada
//                               // na interface. Caso contrario a resolucao sempre sera´ 96 dpi.
//  print(printer);
//}
//
//void MyDisplay::printFileSlot()
//{
//  PrintInFile w(this);
//  if(w.exec() ==  QDialog::Rejected)
//    return;
//
//  QPrinter printer(QPrinter::HighResolution);
//  printer.setOutputFileName(w.m_fileLineEdit->text());
//
//  if(w.m_fileFormatComboBox->currentText() == "PDF")
//    printer.setOutputFormat(QPrinter::PdfFormat);
//  else
//    printer.setOutputFormat(QPrinter::PostScriptFormat);
//
//  double width = w.m_paperWidthLineEdit->text().toDouble();
//  double height = w.m_paperHeightLineEdit->text().toDouble();
//  QSizeF paperSize(width, height);     
//  printer.setPaperSize(paperSize, QPrinter::Millimeter);
//  double left = w.m_leftMarginSpinBox->text().toDouble();
//  double top = w.m_rightMarginSpinBox->text().toDouble();
//  double right = w.m_topMarginSpinBox->text().toDouble();
//  double bottom = w.m_bottomMarginSpinBox->text().toDouble();
//  printer.setPageMargins(left, top, right, bottom, QPrinter::Millimeter);
//
//  int resolution = w.m_resolutionComboBox->currentText().toInt();
//  printer.setResolution(resolution);
//  int r = printer.resolution();
//
//  if(w.m_paperOrientationComboBox->currentText() == "Portrait")
//    printer.setOrientation(QPrinter::Portrait);
//  else
//    printer.setOrientation(QPrinter::Landscape);
//
//  QString file = w.m_fileLineEdit->text();
//  printer.setOutputFileName(file);
//
//  print(&printer);
//}
//
//void MyDisplay::print(QPrinter* printer)
//{
//  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);
//  QSizeF paperSizeMM = printer->paperSize(QPrinter::Millimeter);
//
//  int resolution = printer->resolution();
//  QPainter painter(printer); // so pode inicializar painter depois de configurar a resolucao da impressora
//
//  QPrinter::Orientation orientation = printer->orientation();
//  int width, height;
//
//  int pwidth = (double)printer->widthMM() * (double)resolution / 25.4;
//  int pheight = (double)printer->heightMM() * (double)resolution / 25.4;
//
//  double left, top, right, bottom;
//  printer->getPageMargins (&left, &top, &right, &bottom, QPrinter::Millimeter);
//  left = (double)left * (double)resolution / 25.4;
//  top = (double)top * (double)resolution / 25.4;
//  right = (double)right * (double)resolution / 25.4;
//  bottom = (double)bottom * (double)resolution / 25.4;
//
//  pwidth -= (left + right);
//  pheight -= (top + bottom);
//
//  int mwidth = m_displayPixmap->width();
//  int mheight = m_displayPixmap->height();
//
//  double a = (double)mwidth / (double)mheight;
//  double b = (double)pwidth / (double)pheight;
//  
//  if(a < b)
//  {
//    width = (int)((double)pheight * a + .5);
//    height = pheight;
//  }
//  else
//  {
//    height = (int)((double)pwidth / a + .5);
//    width = pwidth;
//  }
//
//  left = (pwidth - width) / 2;
//  top = (pheight - height) / 2;
//
//  //unsigned long memAvailable = te::common::GetFreePhysicalMemory();
//
//  int n = 1;
//  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(width, height/n);
//  while(canvas->getWidth() == 0 || canvas->getHeight() == 0)
//  {
//    delete canvas;
//    ++n;
//    canvas = new te::qt::widgets::Canvas(width, height/n);
//  }
//
//  if(n == 1)
//  {
//    te::gm::Envelope e(*m_extent);
//    canvas->calcAspectRatio(e.m_llx, e.m_lly, e.m_urx, e.m_ury, m_hAlign, m_vAlign);
//    canvas->setWindow(e.m_llx, e.m_lly, e.m_urx, e.m_ury);
//
//    std::list<te::map::AbstractLayer*> layerList;
//    std::list<te::map::AbstractLayer*>::iterator it;
//    getLayerList(m_layerTree, layerList);
//
//    for(it = layerList.begin(); it != layerList.end(); ++it)
//    {
//      MyLayer* layer =  (MyLayer*)(*it);
//      MyLayerRenderer* renderer = new MyLayerRenderer(false);
//      layer->setRenderer(renderer);
//      layer->draw(canvas, e, m_srid);
//
//      painter.drawPixmap(left, top, *(canvas->getPixmap()));
//    }
//  }
//  else
//  {
//    // 4096 x 4096 pixels
//    int w = 4096;
//    int h = 4096;
//    int wn = (int)((double)width / (double)w + 1.);
//    n =  (int)((double)height / (double)h + 1.);
//
//    double y1, y2;
//    double x1 = m_extent->getLowerLeftX();
//    double x2 = m_extent->getUpperRightX();
//
//    delete canvas;
//    canvas = new te::qt::widgets::Canvas(w, h);
//    QPixmap pix(w, h);
//    QPainter painterAux; 
//    te::gm::Envelope env(*m_extent);
//    double eh = m_extent->getHeight() / (double)n;
//    double ew = m_extent->getWidth() / (double)wn;
//
//    y2 = m_extent->getUpperRightY();
//    y1 = y2 - eh;
//    x1 = m_extent->getLowerLeftX();
//    x2 = x1 + ew;
//    canvas->setWindow(x1, y1, x2, y2);
//    canvas->calcAspectRatio(x1, y1, x2, y2, m_hAlign, m_vAlign);
//    double x1ini = x1;
//    eh = y2 - y1;
//    ew = x2 - x1;
//    double wmin = ew / (double)w;
//    double hmin = eh / (double)h;
//
//    double d = (m_extent->getUpperRightY() - y1) / (y2 - y1);
//    double ssy = h - (int)(d * (double)h + .5);
//    d = (m_extent->getLowerLeftX() - x1) / (x2 - x1);
//    double ssx = (int)(d * (double)w + .5);
//
//    int yini = top;
//    while((y1 + eh) > m_extent->getLowerLeftY())
//    {
//      int xini = left;
//      x1 = x1ini;
//      while(x1 < m_extent->getUpperRightX())
//      {
//        canvas->setWindow(x1, y1, x1+ew, y1+eh);
//        env.init(x1, y1, x1+ew, y1+eh);
//        pix.fill(QColor(0, 0, 0, 0));
//        painterAux.begin(&pix);
//
//        std::list<te::map::AbstractLayer*> layerList;
//        std::list<te::map::AbstractLayer*>::iterator it;
//        getLayerList(m_layerTree, layerList);
//
//        for(it = layerList.begin(); it != layerList.end(); ++it)
//        {
//          MyLayer* layer =  (MyLayer*)(*it);
//          MyLayerRenderer* renderer = new MyLayerRenderer(false); // clear canvas
//          layer->setRenderer(renderer);
//          layer->draw(canvas, env, m_srid);
//
//          int sx = 0;
//          int sy = 0;
//          int dx = w;
//          int dy = h;
//          if(yini == (int)top)
//          {
//            sy = ssy;
//            dy = 0;
//          }
//          if(xini == (int)left)
//          {
//            sx = ssx;
//            dx = 0;
//          }
//          if((x1 + ew) > m_extent->getUpperRightX())
//          {
//            QPointF pf(m_extent->getUpperRightX(), m_extent->getUpperRightY());
//            QPoint p(canvas->getMatrix().map(pf).toPoint());
//            dx = p.x();
//          }
//          if(y1 < m_extent->getLowerLeftY())
//          {
//            QPointF pf(m_extent->getLowerLeftX(), m_extent->getLowerLeftY());
//            QPoint p(canvas->getMatrix().map(pf).toPoint());
//            dy = p.y();
//          }
//
//          painterAux.drawPixmap(sx, sy, *canvas->getPixmap(), sx, sy, dx, dy);
//        }
//
//        painterAux.end();
//        painter.drawPixmap(xini, yini, pix);
//
//        xini += w;
//        x1 += (ew + wmin);
//      }
//
//      yini += h;
//      y1 -= (eh + hmin);
//    }
//  }
//  painter.end();
//  delete canvas;
//}
//
//void MyDisplay::getLayerList(te::map::AbstractLayer* al, std::list<te::map::AbstractLayer*>& layerList)
//{
//  try
//  {
//    if(al->getType() == "LAYER")
//    {
//      if(al->getVisibility() == te::map::VISIBLE)
//        layerList.push_front(al);
//    }
//
//    te::map::AbstractLayer::iterator it;
//    for(it = al->begin(); it != al->end(); ++it)
//    {
//      te::map::AbstractLayer* t = dynamic_cast<te::map::AbstractLayer*>(*it);
//      getLayerList(t, layerList);
//    }
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::fitAllLayersSlot()
//{
//  try
//  {
//    if(m_layerTree)
//    {
//      std::list<te::map::AbstractLayer*> layerList;
//      getLayerList(m_layerTree, layerList);
//      fit(layerList);
//    }
//    else if(m_layerList.empty() == false)
//      fit(m_layerList);
//  }
//  catch(te::common::Exception& e)
//  {
//    QMessageBox::information(this, tr("Error drawing..."), tr(e.what()));
//    return;
//  }
//}
//
//void MyDisplay::fit(std::list<te::map::AbstractLayer*>& layerList)
//{
//  try
//  {
//    m_envelope = te::gm::Envelope();
//    std::list<te::map::AbstractLayer*>::iterator it;
//    for(it = layerList.begin(); it != layerList.end(); ++it)
//    {
//      te::gm::Envelope env = getLayerExtent(*it);
//
//      int srid = (*it)->getSRID();
//      if(m_srid <= 0)
//        m_srid = srid;
//      if(srid != m_srid)
//      {
//        transform(env, srid, m_srid);
//        //env.transform(srid, m_srid);
//      }
//      m_envelope.Union(env);
//    }
//    setExtent();
//    draw();
//  }
//  catch(te::common::Exception& e)
//  {
//    throw(te::common::Exception(e.what()));
//  }
//}
//
//void MyDisplay::setSRIDSlot()
//{
//  int srid = getSRID();
//  std::map<int, QString> sridMap;
//  std::map<int, QString>::iterator it;
//
//  sridMap[TE_SRS_SAD69] = "TE_SRS_SAD69";
//  sridMap[TE_SRS_CORREGO_ALEGRE] = "TE_SRS_CORREGO_ALEGRE";
//  sridMap[TE_SRS_WGS84] = "TE_SRS_WGS84";
//  sridMap[TE_SRS_SIRGAS2000] = "TE_SRS_SIRGAS2000";
//  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S";
//  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S";
//  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S";
//  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S";
//  sridMap[TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S] = "TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S";
//  sridMap[TE_SRS_SAD69_POLYCONIC] = "TE_SRS_SAD69_POLYCONIC";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_18N] = "TE_SRS_SAD69_UTM_ZONE_18N";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_19N] = "TE_SRS_SAD69_UTM_ZONE_19N";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_20N] = "TE_SRS_SAD69_UTM_ZONE_20N";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_21N] = "TE_SRS_SAD69_UTM_ZONE_21N";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_22N] = "TE_SRS_SAD69_UTM_ZONE_22N";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_17S] = "TE_SRS_SAD69_UTM_ZONE_17S";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_18S] = "TE_SRS_SAD69_UTM_ZONE_18S";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_19S] = "TE_SRS_SAD69_UTM_ZONE_19S";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_20S] = "TE_SRS_SAD69_UTM_ZONE_20S";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_21S] = "TE_SRS_SAD69_UTM_ZONE_21S";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_22S] = "TE_SRS_SAD69_UTM_ZONE_22S";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_23S] = "TE_SRS_SAD69_UTM_ZONE_23S";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_24S] = "TE_SRS_SAD69_UTM_ZONE_24S";
//  sridMap[TE_SRS_SAD69_UTM_ZONE_25S] = "TE_SRS_SAD69_UTM_ZONE_25S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_17N] = "TE_SRS_SIRGAS2000_UTM_ZONE_17N";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_18N] = "TE_SRS_SIRGAS2000_UTM_ZONE_18N";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_19N] = "TE_SRS_SIRGAS2000_UTM_ZONE_19N";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_20N] = "TE_SRS_SIRGAS2000_UTM_ZONE_20N";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_21N] = "TE_SRS_SIRGAS2000_UTM_ZONE_21N";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_22N] = "TE_SRS_SIRGAS2000_UTM_ZONE_22N";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_17S] = "TE_SRS_SIRGAS2000_UTM_ZONE_17S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_18S] = "TE_SRS_SIRGAS2000_UTM_ZONE_18S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_19S] = "TE_SRS_SIRGAS2000_UTM_ZONE_19S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_20S] = "TE_SRS_SIRGAS2000_UTM_ZONE_20S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_21S] = "TE_SRS_SIRGAS2000_UTM_ZONE_21S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_22S] = "TE_SRS_SIRGAS2000_UTM_ZONE_22S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_23S] = "TE_SRS_SIRGAS2000_UTM_ZONE_23S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_24S] = "TE_SRS_SIRGAS2000_UTM_ZONE_24S";
//  sridMap[TE_SRS_SIRGAS2000_UTM_ZONE_25S] = "TE_SRS_SIRGAS2000_UTM_ZONE_25S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_18N] = "TE_SRS_WGS84_UTM_ZONE_18N";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_19N] = "TE_SRS_WGS84_UTM_ZONE_19N";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_20N] = "TE_SRS_WGS84_UTM_ZONE_20N";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_21N] = "TE_SRS_WGS84_UTM_ZONE_21N";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_22N] = "TE_SRS_WGS84_UTM_ZONE_22N";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_17S] = "TE_SRS_WGS84_UTM_ZONE_17S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_18S] = "TE_SRS_WGS84_UTM_ZONE_18S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_19S] = "TE_SRS_WGS84_UTM_ZONE_19S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_20S] = "TE_SRS_WGS84_UTM_ZONE_20S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_21S] = "TE_SRS_WGS84_UTM_ZONE_21S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_22S] = "TE_SRS_WGS84_UTM_ZONE_22S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_23S] = "TE_SRS_WGS84_UTM_ZONE_23S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_24S] = "TE_SRS_WGS84_UTM_ZONE_24S";
//  sridMap[TE_SRS_WGS84_UTM_ZONE_25S] = "TE_SRS_WGS84_UTM_ZONE_25S";
//  sridMap[TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC] = "TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC";
//  sridMap[4291] = "4291";
//  sridMap[29177] = "29177";
//  sridMap[29178] = "29178";
//  sridMap[29179] = "29179";
//  sridMap[29180] = "29180";
//  sridMap[29181] = "29181";
//  sridMap[29182] = "29182";
//  sridMap[29183] = "29183";
//  sridMap[29184] = "29184";
//  sridMap[29185] = "29185";
//
//  QStringList items;
//  for(it = sridMap.begin(); it != sridMap.end(); ++it)
//  {
//    if(it->first == srid)
//    {
//      items.append(it->second);
//      break;
//    }
//  }
//
//  for(it = sridMap.begin(); it != sridMap.end(); ++it)
//  {
//    if(it->first != srid)
//      items.append(it->second);
//  }
//
//  bool ok;
//  QString item = QInputDialog::getItem(this, "Config SRID", "SRID:", items, 0, false, &ok);
//
//  if(ok && !items.isEmpty())
//  {
//    for(it = sridMap.begin(); it != sridMap.end(); ++it)
//    {
//      if(item == it->second)
//      {
//        srid = it->first;
//        setSRID(srid);
//        draw();
//        break;
//      }
//    }
//  }
//}
//
//bool MyDisplay::transform(te::gm::Envelope& e, int oldsrid, int newsrid)
//{
//  if(oldsrid == newsrid)
//    return true;
//
//  try
//  {
//    double x1, y1, x2, y2, cx, cy, dx, dy;
//  
//    std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
//
//    converter->setSourceSRID(oldsrid);
//    converter->setTargetSRID(newsrid);
//
//    cx = e.getLowerLeftX() + e.getWidth() / 2.;
//    cy = e.getLowerLeftY() + e.getHeight() / 2.;
//    converter->convert(cx, cy);
//
//    x1 = e.getLowerLeftX();
//    y1 = e.getLowerLeftY();
//    converter->convert(x1, y1);
//
//    x2 = e.getUpperRightX();
//    y2 = e.getUpperRightY();
//    converter->convert(x2, y2);
//    if(std::numeric_limits<double>::has_infinity)
//    {
//      if(x1 == std::numeric_limits<double>::infinity() || y1 == std::numeric_limits<double>::infinity() ||
//         x2 == std::numeric_limits<double>::infinity() || y2 == std::numeric_limits<double>::infinity())
//      {
//        if(cx == std::numeric_limits<double>::infinity() || cy == std::numeric_limits<double>::infinity())
//          return false;
//        else
//        {
//          x1 = y1 = -20000000;
//          x2 = y2 = 20000000;
//        }
//      }
//    }
//
//    dx = fabs(x2 - x1) / 2.;
//    dy = fabs(y2 - y1) / 2.;
//
//    x1 = cx - dx;
//    x2 = cx + dx;
//    y1 = cy - dy;
//    y2 = cy + dy;
//
//    e.init(x1, y1, x2, y2);
//    if(e.getWidth() == 0. || e.getHeight() == 0.)
//      return false;
//    return true;
//  }
//  catch(std::exception& e)
//  {
//    QMessageBox::information(this, tr("Projection convertion error..."), tr(e.what()));
//    return false;
//  }
//}
//
//void MyDisplay::setTemporalDisplay(MyTemporalDisplay* t)
//{
//  m_temporalDisplay = t;
//}
//
//MyTemporalDisplay* MyDisplay::getTemporalDisplay()
//{
//  return m_temporalDisplay;
//}
//
//QPixmap* MyDisplay::getTemporalVectorialDisplayPixmap()
//{
//  return m_temporalVectorialDisplayPixmap;
//}
//
//QPixmap* MyDisplay::getTemporalImageDisplayPixmap()
//{
//  return m_temporalImageDisplayPixmap;
//}
//
//MouseHandler* MyDisplay::getMouseHandler()
//{
//  return m_mouseHandler;
//}
//





