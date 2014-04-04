/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */
 
 /*!
  \file terralib/qt/widgets/slider/TimeSliderWidget.cpp

  \brief A widget used to alow the user to controle the visualization of temporal data
*/

//Terralib
//#include "TimePropertiesDialog.h"
#include "../canvas/MapDisplay.h"
#include "Animation.h"
#include "AnimationScene.h"
#include "TrajectoryItem.h"
#include "PixmapItem.h"
#include "TimeSliderWidget.h"
#include "ui_TimeSliderWidgetForm.h"
#include <terralib/dataaccess.h>
#include <terralib/stmemory.h>
#include <terralib/st.h>
#include <terralib/qt/widgets/utils/ScopedCursor.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

//QT
#include <qgraphicseffect.h>
#include <QSlider>
#include <QGraphicsView>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTimer>
#include <QThread>
#include <QTextStream>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTimeEdit>
#include <QColorDialog>


te::qt::widgets::TimeSliderWidget::TimeSliderWidget(te::qt::widgets::MapDisplay* md, QWidget* parent,  Qt::WindowFlags f)
  : QWidget(parent, f),
    m_display(md),
    m_erasePerfectly(false),
    m_maxSliderValue(1000000000),
    m_ui(new Ui::TimeSliderWidgetForm),
    m_finished(false)
{
  te::qt::widgets::ScopedCursor scopedCursor(Qt::WaitCursor);
  m_ui->setupUi(this);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);

  showPropertySection(false);

  te::gm::Envelope env = m_display->getExtent();
  m_initialDisplayRect = QRectF(env.m_llx, env.m_lly, env.getWidth(), env.getHeight());

  // slider normalized between 0 and m_maxSliderValue
  m_ui->m_TemporalHorizontalSlider->setMinimum(0);
  m_ui->m_TemporalHorizontalSlider->setMaximum(m_maxSliderValue);

  m_ui->m_durationSpinBox->setMinimum(100);
  m_ui->m_durationSpinBox->setMaximum(100000000);
  m_ui->m_durationSpinBox->setValue(10000);
  m_ui->m_durationSpinBox->setSingleStep(10000);

  m_ui->m_opacitySpinBox->setMinimum(0);
  m_ui->m_opacitySpinBox->setMaximum(255);
  m_ui->m_opacitySpinBox->setValue(255);
  m_ui->m_opacitySpinBox->setSingleStep(5);

  //setAutoFillBackground(true);
  //QPalette palette;
  //palette.setColor(QPalette::Background, Qt::white);
  //setPalette(palette);

  //The opacity used to fade the widget if the mouse is not over it
  //QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect;
  //opacityEffect->setOpacity(0.85);
  //this->setGraphicsEffect(opacityEffect);

  //Adjusting the icons and the graphic effects
  m_ui->m_settingsToolButton->setIcon(QIcon::fromTheme("preferences-system"));
  m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-start"));
  m_ui->m_stopToolButton->setIcon(QIcon::fromTheme("media-playback-stop"));

  // connect signal and slots
  connect(m_ui->m_addEtaPushButton, SIGNAL(clicked(bool) ), this, SLOT(onAddEtaPushButtonClicked(bool)));
  connect(m_ui->m_addHidroPushButton, SIGNAL(clicked(bool) ), this, SLOT(onAddHidroPushButtonClicked(bool)));
  connect(m_ui->m_autoPanCheckBox, SIGNAL(clicked(bool) ), this, SLOT(onAutoPanCheckBoxClicked(bool)));
  connect(m_ui->m_forwardRadioButton, SIGNAL(clicked(bool) ), this, SLOT(onForwardRadioButtonClicked(bool)));
  connect(m_ui->m_backwardRadioButton, SIGNAL(clicked(bool) ), this, SLOT(onBackwardRadioButtonClicked(bool)));
  connect(m_ui->m_loopCheckBox, SIGNAL(clicked(bool) ), this, SLOT(onLoopCheckBoxClicked(bool)));
  connect(m_ui->m_goAndBackCheckBox, SIGNAL(clicked(bool) ), this, SLOT(onGoAndBackCheckBoxClicked(bool)));
  connect(m_ui->m_applyTimeIntervalPushButton, SIGNAL(clicked(bool) ), this, SLOT(onApplyTimeIntervalPushButtonClicked(bool)));
  connect(m_ui->m_trajectoryColorComboBox, SIGNAL(activated(int) ), this, SLOT(onTrajectoryColorComboBoxActivated(int)));
  connect(m_ui->m_forwardColorPushButton, SIGNAL(clicked(bool) ), this, SLOT(onForwardColorPushButton(bool)));
  connect(m_ui->m_backwardColorPushButton, SIGNAL(clicked(bool) ), this, SLOT(onBackwardColorPushButton(bool)));
  connect(m_ui->m_opacityComboBox, SIGNAL(activated(int) ), this, SLOT(onOpacityComboBoxActivated(int)));
  connect(m_ui->m_dateTimeEdit, SIGNAL(dateTimeChanged(const QDateTime&) ), this, SLOT(onDateTimeEditChanged(const QDateTime&)));
  connect(m_ui->m_durationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDurationValueChanged(int)));
  connect(m_ui->m_opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOpacityValueChanged(int)));
  connect(m_ui->m_settingsToolButton, SIGNAL(clicked()), this, SLOT(onSettingsToolButtonnClicked()));
  connect(m_ui->m_playToolButton, SIGNAL(clicked()), this, SLOT(onPlayToolButtonnClicked()));
  connect(m_ui->m_stopToolButton, SIGNAL(clicked()), this, SLOT(onStopToolButtonnClicked()));
  connect(m_ui->m_TemporalHorizontalSlider, SIGNAL(sliderMoved (int)), this, SLOT(onSliderMoved(int)));
  connect(m_ui->addPushButton, SIGNAL(clicked(bool)), this, SLOT(onAddPushButtonClicked(bool)));
  connect(m_ui->removePushButton, SIGNAL(clicked(bool)), this, SLOT(onRemovePushButtonClicked(bool)));
  connect(m_ui->frontPushButton, SIGNAL(clicked(bool)), this, SLOT(onFrontPushButtonClicked(bool)));
  connect(m_ui->backPushButton, SIGNAL(clicked(bool)), this, SLOT(onBackPushButtonClicked(bool)));

  connect(m_ui->m_resetInitialTimePushButton, SIGNAL(clicked()), this, SLOT(onResetInitialTimePushButtonClicked()));
  connect(m_ui->m_resetFinalTimePushButton, SIGNAL(clicked()), this, SLOT(onResetFinalTimePushButtonClicked()));

  setAttribute(Qt::WA_DeleteOnClose, true);

  m_loop = true; //By default, the animation not will be looping.
  m_duration = 10000; //By default, the animation journey time will be 10000 miliseconds.
  m_ui->m_durationSpinBox->setValue(m_duration);
  m_direction = QAbstractAnimation::Forward; //By default, the animation direction will be forward.
  setGoAndBack(true); //By default, the animation is forward and then backward.

  m_ui->m_forwardRadioButton->setCheckable(true);

  m_animationView = new QGraphicsView();
  m_parallelAnimation = new QParallelAnimationGroup(m_animationView);
  m_animationView->installEventFilter(this);
  m_animationScene = new AnimationScene(m_display);
  m_animationView->setScene(m_animationScene);
  m_animationView->showMaximized();
  m_animationView->move(5000, 5000);

  connect(m_display, SIGNAL(displaySridChanged()), this, SLOT(onSridChanged()));
  connect(m_display, SIGNAL(extentChanged()), this, SLOT(onExtentChanged()));
  connect(m_display, SIGNAL(displayPaintEvent(QPainter*)), this, SLOT(onDisplayPaintEvent(QPainter*)));
  connect(m_parallelAnimation, SIGNAL(finished()), this, SLOT(onFinishAnimation()));

  installEventFilter(this);
  m_ui->m_forwardColorPushButton->installEventFilter(this);
  m_ui->m_backwardColorPushButton->installEventFilter(this);
  m_ui->m_dateTimeEdit->installEventFilter(this);
  m_ui->m_initialAnimationDateTimeEdit->installEventFilter(this);
  m_ui->m_finalAnimationDateTimeEdit->installEventFilter(this);

  m_ui->m_dateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");
  m_ui->m_initialAnimationDateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");
  m_ui->m_finalAnimationDateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");

  //onAddHidroPushButtonClicked(true);

  //calculateSpatialExtent();
  //calculateTemporalExtent();
  //createAnimations();
  //setDuration(m_duration);
  //setDirection(m_direction);
  //updateSliderPos();
  //onStopToolButtonnClicked();

  if(m_animationScene->items().isEmpty())
  {
    m_ui->m_durationSpinBox->setEnabled(false);
    m_ui->m_settingsToolButton->setEnabled(false);
    m_ui->m_playToolButton->setEnabled(false);
    m_ui->m_stopToolButton->setEnabled(false);
    m_ui->m_dateTimeEdit->setEnabled(false);
    showPropertySection(false);
  }
  else
  {
    m_ui->m_durationSpinBox->setEnabled(true);
    m_ui->m_settingsToolButton->setEnabled(true);
    m_ui->m_playToolButton->setEnabled(true);
    m_ui->m_stopToolButton->setEnabled(true);
    if(m_parallelAnimation->state() == QAbstractAnimation::Paused)
      m_ui->m_dateTimeEdit->setEnabled(true);
    else
      m_ui->m_dateTimeEdit->setEnabled(false);
  }

  if(m_display->getExtent().isValid() == false)
  {
    QRectF r = m_animationScene->sceneRect();
    te::gm::Envelope e(r.left(), r.top(), r.right(), r.bottom());
    m_display->setExtent(e);
  }
  if(m_display->getSRID() <= 0)
  {
    QList<QGraphicsItem*> list = m_animationScene->items();
    QList<QGraphicsItem*>::iterator it = list.begin(); 
    if(it != list.end())
    {
      AnimationItem* ai = (AnimationItem*)(*it);
      int srid = ai->m_SRID;
      m_display->setSRID(srid);
    }
  }
}

te::qt::widgets::TimeSliderWidget::~TimeSliderWidget()
{ 
  m_parallelAnimation->stop(); // should be before clear
  m_parallelAnimation->clear();
  delete m_parallelAnimation;
  delete m_animationScene;

  bool b = m_animationView->testAttribute(Qt::WA_DeleteOnClose);
  m_animationView->close();
  if(b == false)
    delete m_animationView;
}

void te::qt::widgets::TimeSliderWidget::openKml(const QString& file)
{
  te::da::DataSourceInfo dsinfo;
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = file.toStdString(); 
  dsinfo.setConnInfo(connInfo);
  dsinfo.setType("OGR");
  dsinfo.setId(file.toStdString());

  if(te::da::DataSourceManager::getInstance().find(file.toStdString()) == 0)
  {
    //Create the data source and put it into the manager
    te::da::DataSourceManager::getInstance().open(dsinfo.getId(), dsinfo.getType(), dsinfo.getConnInfo());
  }
    
  //Indicates how the trajectories are stored in the data source -> This structure is fixed for OGR driver
  int phTimeIdx = 3;  /* property name: timestamp */
  int geomIdx = 12;    /* property name: geom */

  //It initializes the st data loader support
  te::st::STDataLoader::initialize();

  //Use the STDataLoader to create a TrajectoryDataSet with all observations

  te::st::TrajectoryDataSetInfo tjinfo40(dsinfo, "40: locations", phTimeIdx, geomIdx, -1, "40");
  te::st::TrajectoryDataSet* tjDS40 = te::st::STDataLoader::getDataSet(tjinfo40).release();
  addTrajectory("helicoptero", "c:/lixo/helicopteroT2.png", tjDS40);
  delete tjDS40;

  te::st::TrajectoryDataSetInfo tjinfo41(dsinfo, "41: locations", phTimeIdx, geomIdx, -1, "41");
  te::st::TrajectoryDataSet* tjDS41 = te::st::STDataLoader::getDataSet(tjinfo41).release();
  addTrajectory("cachorro", "c:/lixo/cachorro_correndo_31.gif", tjDS41);
  delete tjDS41;

  te::st::STDataLoader::finalize();
}

void te::qt::widgets::TimeSliderWidget::openTrajectory(const QString& leao)
{
  if(leao != "lm40" && leao != "lm41")
    return;

  QString file = "C:/lixo/kml/t_40_41.kml";
  te::da::DataSourceInfo dsinfo;
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = file.toStdString(); 
  dsinfo.setConnInfo(connInfo);
  dsinfo.setType("OGR");
  dsinfo.setId(file.toStdString());

  if(te::da::DataSourceManager::getInstance().find(file.toStdString()) == 0)
  {
    //Create the data source and put it into the manager
    te::da::DataSourceManager::getInstance().open(dsinfo.getId(), dsinfo.getType(), dsinfo.getConnInfo());
  }
    
  //Indicates how the trajectories are stored in the data source -> This structure is fixed for OGR driver
  int phTimeIdx = 3;  /* property name: timestamp */
  int geomIdx = 12;    /* property name: geom */

  //It initializes the st data loader support
  te::st::STDataLoader::initialize();

  //Use the STDataLoader to create a TrajectoryDataSet with all observations

  if(leao == "lm40")
  {
    te::st::TrajectoryDataSetInfo tjinfo40(dsinfo, "40: locations", phTimeIdx, geomIdx, -1, "40");
    te::st::TrajectoryDataSet* tjDS40 = te::st::STDataLoader::getDataSet(tjinfo40).release();
    addTrajectory(leao, "c:/lixo/helicopteroT2.png", tjDS40);
    delete tjDS40;
  }
  else if(leao == "lm41")
  {
    te::st::TrajectoryDataSetInfo tjinfo41(dsinfo, "41: locations", phTimeIdx, geomIdx, -1, "41");
    te::st::TrajectoryDataSet* tjDS41 = te::st::STDataLoader::getDataSet(tjinfo41).release();
    addTrajectory(leao, "c:/lixo/cachorro_correndo_31.gif", tjDS41);
    delete tjDS41;
  }
  te::st::STDataLoader::finalize();
}

void te::qt::widgets::TimeSliderWidget::addTrajectory(const QString& title, const QString& pixmapFile, te::st::TrajectoryDataSet* dset)
{
  TrajectoryItem* ti = new TrajectoryItem(title, pixmapFile, m_display);
  m_animationScene->addItem(ti);
  Animation* animation = new Animation(ti, "pos");
  animation->setEasingCurve(QEasingCurve::Linear);
    
  //Indicates how the trajectories are stored in the data source -> This structure is fixed for OGR driver
  int phTimeIdx = 3;  /* property name: timestamp */
  int geomIdx = 12;    /* property name: geom */

  if(title == "lm40") // change default colors
  {
    ti->m_opacity = 120;
    ti->m_forwardColor = Qt::green;
    ti->m_forwardColor.setAlpha(ti->m_opacity);
    ti->m_backwardColor = Qt::cyan;
    ti->m_backwardColor.setAlpha(ti->m_opacity);
  }

  animation->m_spatialExtent = dset->getSpatialExtent();
  animation->m_temporalExtent = *static_cast<te::dt::TimePeriod*>(dset->getTemporalExtent());

  size_t size = dset->size();
  //********************************************************************************************************
  ti->m_SRID = 4326; // ver como obter SRID...
  //********************************************************************************************************
  QPointF pf;
  if(size > 0)
  {
    te::dt::TimeInstant tiraRepetido;
    ti->m_route = new te::gm::LineString(size, te::gm::LineStringType, ti->m_SRID);
    size_t i = 0;
    while(dset->moveNext())
    {
      std::auto_ptr<te::dt::DateTime> time = dset->getTime();
      te::dt::DateTimeType type = time->getDateTimeType();
      te::dt::TimeInstant* tinstant = static_cast<te::dt::TimeInstant*>(time.get());
      if(tiraRepetido == *tinstant)
        continue;
      tiraRepetido = *tinstant;

      ti->m_time.push_back(te::dt::TimeInstant(tinstant->getTimeInstant()));

      std::auto_ptr<te::gm::Geometry> geom = dset->getGeometry();
      const te::gm::Point* p = static_cast<const te::gm::Point*>(geom.get());
      te::gm::Point pp(*p);
      ti->m_route->setPointN(i++, pp);
    }
  }

  m_parallelAnimation->addAnimation(animation);

  m_ui->m_trajectoryColorComboBox->addItem(title);
  int count = m_ui->m_trajectoryColorComboBox->count();
  m_ui->m_trajectoryColorComboBox->setCurrentIndex(count-1);
  onTrajectoryColorComboBoxActivated(count-1);
}
//
//void te::qt::widgets::TimeSliderWidget::addTemporalImages(const QString& filePath, const bool& center)
//{
//  // definam onde as imagens serão posicionadas
//  te::gm::Envelope env = m_display->getExtent();
//  QPointF p(env.m_llx + env.getWidth()/2, env.m_lly + env.getHeight()/2);
//  
//  QFileInfo fi(filePath);
//  QDir dir = fi.absoluteDir();
//  QString imageFile = fi.completeBaseName();
//  QString suffix = "." + fi.suffix();
//
//  if(center == false)
//  {
//    //if(imageFile == "golfinho")
//    //  p = QPointF(-16, -56);
//    //else if(imageFile == "caogordo")
//    //  p = QPointF(-64, -37);
//    //else if(imageFile == "cao")
//    //  p = QPointF(-100, 38);
//    //else if(imageFile == "gatinho")
//    //  p = QPointF(22, 6);
//    //else if(imageFile == "dalmata")
//    //  p = QPointF(-60, -24);
//    //else if(imageFile == "tom")
//    //  p = QPointF(-64, -55);
//    //else if(imageFile == "bruxa")
//    //  p = QPointF(50, 10);
//    //else if(imageFile == "Terra")
//    //  p = QPointF(80, 40);
//    //else if(imageFile == "planetas")
//    //  p = QPointF(78, -40);
//    //else if(imageFile == "gotas")
//    //  p = QPointF(-100, -24);
//    //else if(imageFile == "malvado")
//    //  p = QPointF(-100, -55);
//    //else if(imageFile == "fastfood")
//    //  p = QPointF(-64, -24);
//    if(imageFile == "segredo ")
//      p = QPointF(20, -24);
//    else if(imageFile == "NoDoubt ")
//      p = QPointF(20, 40);
//  }
//
//  PixmapItem* pi = new PixmapItem(imageFile, filePath, m_display);
//  if(imageFile == "segredo ")
//    pi->m_opacity = m_ui->m_opacitySpinBox->value();
//
//  m_animationScene->addItem(pi);
//  Animation* animation = new Animation(pi, "pos");
//  animation->setEasingCurve(QEasingCurve::Linear);
//
//  // somente para testar: copie o spatial extent e o temporal extent do m_animationItemAux
//  animation->m_spatialExtent = m_display->getExtent();
//
//  te::dt::TimeInstant iTime(boost::posix_time::time_from_string("2008-01-01 00:00:00"));
//  te::dt::TimeInstant fTime(boost::posix_time::time_from_string("2010-12-31 23:59:59"));
//  animation->m_temporalExtent = te::dt::TimePeriod(iTime, fTime);
//
//  char c[20];
//
//  QString file = imageFile;
//  if(imageFile != "fastfood" && imageFile.contains("fastfood"))
//    file = "fastfood";
//
//  QStringList nameFilter;
//  nameFilter.append(file + "*");
//  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
//  int max = files.count();
//
//  //int conta = 1;
//  //while(true)
//  //{
//  //  sprintf(c, "%06d%s", conta++, suffix.toStdString().c_str());
//  //  QString f = pi->m_dir.path() + "/" + file;
//  //  f += c;
//  //  if(QFile::exists(f) == false)
//  //  {
//  //    max = conta - 1;
//  //    break;
//  //  }
//  //}
//
//  // para o teste vou colocar 1 dado por dia ou 86400 segundos
//  size_t tdia = 86400;
//
//  if(imageFile == "segredo " || imageFile == "NoDoubt ")
//    tdia /= 24;
//
//  boost::posix_time::time_duration td = boost::posix_time::seconds(tdia);
//
//  boost::posix_time::time_duration td1 = boost::posix_time::seconds(tdia*20);
//  boost::posix_time::ptime time1 = iTime.getTimeInstant() + td1;
//  te::dt::TimeInstant Time1 = te::dt::TimeInstant(time1);
//  boost::posix_time::time_duration td2 = boost::posix_time::seconds(tdia*100);
//  boost::posix_time::ptime time2 = iTime.getTimeInstant() + td2;
//  te::dt::TimeInstant Time2 = te::dt::TimeInstant(time2);
//  boost::posix_time::time_duration td3 = boost::posix_time::seconds(tdia*158);
//  boost::posix_time::ptime time3 = iTime.getTimeInstant() + td3;
//  te::dt::TimeInstant Time3 = te::dt::TimeInstant(time3);
//
//  int r = 1;
//  int rr = 1;
//  if(imageFile == "fastfoodfail4")
//    rr = 10;
//  else if(imageFile == "fastfoodfail5")
//    rr = 20;
//  td = boost::posix_time::seconds(tdia * rr);
//
//  while(iTime < fTime)
//  {
//    if(imageFile == "fastfoodfail1" && iTime < Time1)
//    {
//      iTime = te::dt::TimeInstant(iTime.getTimeInstant() + td);
//      r += rr;
//      continue;
//    }
//    else if(imageFile == "fastfoodfail2" && iTime > Time2)
//    {
//      iTime = te::dt::TimeInstant(iTime.getTimeInstant() + td);
//      r += rr;
//      continue;
//    }
//    else if(imageFile == "fastfoodfail3" && (iTime > Time1 && iTime < Time2))
//    {
//      iTime = te::dt::TimeInstant(iTime.getTimeInstant() + td);
//      r += rr;
//      continue;
//    }
//
//    pi->m_time.push_back(iTime);
//    if(r == max)
//      r = 1;
//      
//    sprintf(c, "%06d%s", r, suffix.toStdString().c_str());
//
//    QString f = file + c;
//    pi->m_files.push_back(f);
//
//    iTime = te::dt::TimeInstant(iTime.getTimeInstant() + td);
//    r += rr;
//    if(file == "fastfood" && iTime > Time3)
//      break;
//  }
//
//  pi->m_SRID = 4326;
//  size_t size =  pi->m_files.size();
//  pi->m_route = new te::gm::LineString(size, te::gm::LineStringType, pi->m_SRID);
//
//  // crie valores não repetitivos e nem muito grandes ou pequenos
//  QPointF pos(env.m_llx, env.m_lly);
//  int dd = 1000;
//  double w = env.getWidth()/(double)dd;
//  double h = env.getHeight()/(double)dd;
//  for(size_t i = 0; i < size; ++i)
//  {
//    if((i % dd) != 0)
//      pos += QPointF(w, h);
//    else
//      pos = QPointF(env.m_llx, env.m_lly);
//
//    pi->m_route->setPoint(i, pos.x(), pos.y());
//  }
//
//  pi->setImagePosition(p, m_initialDisplayRect);
//  m_parallelAnimation->addAnimation(animation);
//}

void te::qt::widgets::TimeSliderWidget::addTemporalImages(const QString& filePath, const bool& center)
{
  // definir onde as imagens serão posicionadas
  te::gm::Envelope env = m_display->getExtent();
  QPointF p(env.m_llx + env.getWidth()/2, env.m_lly + env.getHeight()/2);
  
  QFileInfo fi(filePath);
  QDir dir = fi.absoluteDir();
  QString imageFile = fi.completeBaseName();
  QString suffix = "." + fi.suffix();

  if(center == false)
  {
    if(imageFile == "fastfood")
      p = QPointF(-64, -24);
    if(imageFile == "segredo ")
      p = QPointF(20, -24);
    else if(imageFile == "NoDoubt ")
      p = QPointF(20, 40);
  }

  PixmapItem* pi = new PixmapItem(imageFile, filePath, m_display);
  if(imageFile == "segredo ")
    pi->m_opacity = m_ui->m_opacitySpinBox->value();

  m_animationScene->addItem(pi);
  Animation* animation = new Animation(pi, "pos");
  animation->setEasingCurve(QEasingCurve::Linear);

  // somente para testar: copie o spatial extent e o temporal extent do m_animationItemAux
  animation->m_spatialExtent = m_display->getExtent();

  te::dt::TimeInstant iTime;
  te::dt::TimeInstant fTime(boost::posix_time::time_from_string("2010-12-31 23:59:59"));
  if(imageFile == "segredo ")
    iTime = te::dt::TimeInstant(boost::posix_time::time_from_string("2008-01-01 00:00:00"));
  else if(imageFile == "NoDoubt ")
  {
    iTime = te::dt::TimeInstant(boost::posix_time::time_from_string("2008-04-01 00:00:00"));
    fTime = te::dt::TimeInstant(boost::posix_time::time_from_string("2009-01-03 09:00:00"));
  }

  animation->m_temporalExtent = te::dt::TimePeriod(iTime, fTime);

  char c[20];

  QString file = imageFile;
  if(imageFile != "fastfood" && imageFile.contains("fastfood"))
    file = "fastfood";

  QStringList nameFilter;
  nameFilter.append(file + "*");
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  int max = files.count();

  // para o teste vou colocar 1 dado por dia ou 86400 segundos
  size_t tdia = 86400;

  if(imageFile == "segredo " || imageFile == "NoDoubt ")
    tdia /= 24;

  boost::posix_time::time_duration td = boost::posix_time::seconds(tdia);

  boost::posix_time::time_duration td1 = boost::posix_time::seconds(tdia*20);
  boost::posix_time::ptime time1 = iTime.getTimeInstant() + td1;
  te::dt::TimeInstant Time1 = te::dt::TimeInstant(time1);
  boost::posix_time::time_duration td2 = boost::posix_time::seconds(tdia*100);
  boost::posix_time::ptime time2 = iTime.getTimeInstant() + td2;
  te::dt::TimeInstant Time2 = te::dt::TimeInstant(time2);
  boost::posix_time::time_duration td3 = boost::posix_time::seconds(tdia*158);
  boost::posix_time::ptime time3 = iTime.getTimeInstant() + td3;
  te::dt::TimeInstant Time3 = te::dt::TimeInstant(time3);

  int r = 1;
  int rr = 1;
  if(imageFile == "fastfoodfail4")
    rr = 10;
  else if(imageFile == "fastfoodfail5")
    rr = 20;
  td = boost::posix_time::seconds(tdia * rr);

  while(iTime < fTime)
  {
    if(imageFile == "fastfoodfail1" && iTime < Time1)
    {
      iTime = te::dt::TimeInstant(iTime.getTimeInstant() + td);
      r += rr;
      continue;
    }
    else if(imageFile == "fastfoodfail2" && iTime > Time2)
    {
      iTime = te::dt::TimeInstant(iTime.getTimeInstant() + td);
      r += rr;
      continue;
    }
    else if(imageFile == "fastfoodfail3" && (iTime > Time1 && iTime < Time2))
    {
      iTime = te::dt::TimeInstant(iTime.getTimeInstant() + td);
      r += rr;
      continue;
    }

    if(iTime > fTime)
      break;
    if(r > max)
      break;

    pi->m_time.push_back(iTime);
      
    sprintf(c, "%06d%s", r, suffix.toStdString().c_str());
    pi->m_files.push_back(file + c);

    iTime = te::dt::TimeInstant(iTime.getTimeInstant() + td);
    r += rr;
    if(file == "fastfood" && iTime > Time3)
      break;
  }

  pi->m_SRID = 4326;
  size_t size =  pi->m_files.size();
  pi->m_route = new te::gm::LineString(size, te::gm::LineStringType, pi->m_SRID);

  // crie valores não repetitivos e nem muito grandes ou pequenos
  QPointF pos(animation->m_spatialExtent.m_llx, animation->m_spatialExtent.m_lly);
  double w = animation->m_spatialExtent.getWidth();
  double h = animation->m_spatialExtent.getHeight();
  for(size_t i = 0; i < size; ++i)
  {
    if(i & 1)
      pos -= QPointF(w, h);
    else
      pos += QPointF(w, h);

    pi->m_route->setPoint(i, pos.x(), pos.y());
  }

  pi->setImagePosition(p, m_initialDisplayRect);
  m_parallelAnimation->addAnimation(animation);
}

void te::qt::widgets::TimeSliderWidget::getGoesData(const QString& path)
{
  QDir dir(path);
  QString dirName = dir.dirName();
  QString suffix(".jpg");


  PixmapItem* pi = new PixmapItem(dirName, path, m_display);
  // so funciona na projeção WGS84
  pi->m_SRID = 4326;
  m_animationScene->addItem(pi);
  Animation* animation = new Animation(pi, "pos");
  animation->setEasingCurve(QEasingCurve::Linear);

  m_parallelAnimation->addAnimation(animation);

  // get spatial extent CHUTE!!!!
  if(dirName.contains("_high"))
  {
    if(dirName.contains("Realce_"))
      animation->m_spatialExtent = te::gm::Envelope(-99.8, -56.92, -25, 14.1);
    else if(dirName.contains("Realce2_"))
      animation->m_spatialExtent = te::gm::Envelope(-99.8, -60.59, -25, 14.17);
    else if(dirName.contains("GMVapor_"))
      animation->m_spatialExtent = te::gm::Envelope(-142.5, -75, 75.8, 79.8);
    else
      animation->m_spatialExtent = te::gm::Envelope(-99.8, -56, -25, 14);
  }
  else
  {
    if(dirName.contains("Realce_"))
      animation->m_spatialExtent = te::gm::Envelope(-99.8, -58.6, -25, 15.12);
    else if(dirName.contains("Realce2_"))
      animation->m_spatialExtent = te::gm::Envelope(-99.8, -60.70, -25, 14.16);
    else
      animation->m_spatialExtent = te::gm::Envelope(-99.8, -56, -25, 15);
  }

  pi->m_imaRect = QRectF(animation->m_spatialExtent.m_llx, animation->m_spatialExtent.m_lly, animation->m_spatialExtent.getWidth(), animation->m_spatialExtent.getHeight());

  QStringList files = dir.entryList(QDir::Files, QDir::Name);
  size_t count = files.count();

  // get time extent
  te::dt::TimeInstant ti = getGoesTime(path + files.first());
  te::dt::TimeInstant tf = getGoesTime(path + files.last());
  animation->m_temporalExtent = te::dt::TimePeriod(ti, tf);

  // set data
  te::dt::TimeInstant t;
  for(size_t i = 0; i < count; ++i)
  {
    QString f = files[i];
    t = getGoesTime(f);
    pi->m_time.push_back(t);
    pi->m_files.push_back(f);
  }

  pi->m_route = new te::gm::LineString(count, te::gm::LineStringType, pi->m_SRID);

  // crie valores não repetitivos e nem muito grandes ou pequenos
  QPointF pos(animation->m_spatialExtent.m_llx, animation->m_spatialExtent.m_lly);
  double w = animation->m_spatialExtent.getWidth();
  double h = animation->m_spatialExtent.getHeight();
  for(size_t i = 0; i < count; ++i)
  {
    if(i & 1)
      pos -= QPointF(w, h);
    else
      pos += QPointF(w, h);

    pi->m_route->setPoint(i, pos.x(), pos.y());
  }
}

void te::qt::widgets::TimeSliderWidget::calculateSpatialExtent()
{
  int srid = -1;
  m_spatialExtent = te::gm::Envelope();

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for(it = list.begin(); it != list.end(); ++it)
  {
    AnimationItem* ai = (AnimationItem*)(*it);
    srid = ai->m_SRID;
    if(m_display->getSRID() != TE_UNKNOWN_SRS && m_display->getSRID() != ai->m_SRID)
    {
      te::gm::Envelope e = ai->m_animation->m_spatialExtent;
      e.transform(ai->m_SRID, m_display->getSRID());
      m_spatialExtent.Union(e);
    }
    else
      m_spatialExtent.Union(ai->m_animation->m_spatialExtent);
  }
  QRect rect(m_spatialExtent.m_llx, m_spatialExtent.m_lly, m_spatialExtent.getWidth(), m_spatialExtent.getHeight());
  m_animationScene->setSceneRect(rect);
  m_animationView->setSceneRect(rect);
}

void te::qt::widgets::TimeSliderWidget::calculateTemporalExtent()
{
  te::dt::TimeInstant initial, final, t_initial, t_final;

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it = list.begin();
  if(it != list.end())
  {
    AnimationItem* ai = (AnimationItem*)(*it);
    t_initial = ai->m_animation->m_temporalExtent.getInitialTimeInstant();
    t_final = ai->m_animation->m_temporalExtent.getFinalTimeInstant();
    ++it;
  }

  while(it != list.end())
  {
    AnimationItem* ai = (AnimationItem*)(*it);
    initial = ai->m_animation->m_temporalExtent.getInitialTimeInstant();
    if(initial < t_initial)
      t_initial = initial;

    final = ai->m_animation->m_temporalExtent.getFinalTimeInstant();
    if(final > t_final)
      t_final = final;
    ++it;
  }

  m_temporalExtent = te::dt::TimePeriod(t_initial, t_final);
  te::dt::TimePeriod tp(t_initial, t_final);
  m_temporalAnimationExtent = tp;

  for(it = list.begin(); it != list.end(); ++it)
  {
    AnimationItem* ai = (AnimationItem*)(*it);
    ai->m_animation->m_temporalAnimationExtent = m_temporalExtent;
  }

  QDateTime minimum(QDate(t_initial.getDate().getYear(), t_initial.getDate().getMonth(), t_initial.getDate().getDay()),
    QTime(t_initial.getTime().getHours(), t_initial.getTime().getMinutes(), t_initial.getTime().getSeconds()));
  QDateTime maximum(QDate(t_final.getDate().getYear(), t_final.getDate().getMonth(), t_final.getDate().getDay()),
    QTime(t_final.getTime().getHours(), t_final.getTime().getMinutes(), t_final.getTime().getSeconds()));

  m_ui->m_dateTimeEdit->setMinimumDateTime(minimum);
  m_ui->m_dateTimeEdit->setMaximumDateTime(maximum);
}

void te::qt::widgets::TimeSliderWidget::createAnimations()
{
  size_t i;
  QVector<te::dt::TimePeriod> times;
  QVector<te::gm::Envelope> envelopes;
  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;
  for(it = list.begin(); it != list.end(); ++it)
  {
    AnimationItem* ai = (AnimationItem*)(*it);
    times.push_back(ai->m_animation->m_temporalExtent);
    envelopes.push_back(ai->m_animation->m_spatialExtent);
  }

  if(m_parallelAnimation->state() == QAbstractAnimation::Stopped)
    m_parallelAnimation->clear();
  else
  {
    int cur = m_parallelAnimation->currentTime();
    m_parallelAnimation->stop();
    m_parallelAnimation->clear();
    m_parallelAnimation->setCurrentTime(cur);
  }

  for(it = list.begin(), i = 0 ; it != list.end(); ++it, ++i)
  {
    AnimationItem* ai = (AnimationItem*)(*it);
    te::qt::widgets::Animation* a = new te::qt::widgets::Animation(ai, "pos");
    a->m_spatialExtent = envelopes[i];
    a->m_temporalExtent = times[i];
    a->m_temporalAnimationExtent = m_temporalAnimationExtent;
    a->createAnimationDataInDisplayProjection(m_temporalAnimationExtent);
    m_parallelAnimation->addAnimation(a);
  }
}

int te::qt::widgets::TimeSliderWidget::getAnimationTime(const te::dt::TimeInstant& timeInstant)
{
  boost::posix_time::ptime pTime = timeInstant.getTimeInstant();

  // total time duration 
  boost::posix_time::ptime iTime = m_temporalAnimationExtent.getInitialTimeInstant().getTimeInstant();
  boost::posix_time::ptime fTime = m_temporalAnimationExtent.getFinalTimeInstant().getTimeInstant();
  boost::posix_time::time_duration diff = fTime - iTime;
  double totalSeconds = diff.total_seconds();

  diff = pTime - iTime;
  double seconds = diff.total_seconds();
  // normalizing the time
  double t = seconds / totalSeconds;
  t *= 1000;
  int ret = (int)t;
  return ret;
}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getTimeInstant()
{
  double t = m_parallelAnimation->currentTime();
  double trel = t / (double)m_duration;

  // total time duration 
  boost::posix_time::ptime iTime = m_temporalAnimationExtent.getInitialTimeInstant().getTimeInstant();
  boost::posix_time::ptime fTime = m_temporalAnimationExtent.getFinalTimeInstant().getTimeInstant();
  boost::posix_time::time_duration diff = fTime - iTime;
  double totalSeconds = diff.total_seconds();

  int secs = qRound(totalSeconds * trel);
  boost::posix_time::time_duration td = boost::posix_time::seconds(secs);
  boost::posix_time::ptime time = iTime + td;
  te::dt::TimeInstant timei(time);
  return timei;
}

void te::qt::widgets::TimeSliderWidget::onDisplayPaintEvent(QPainter* painter)
{
  te::gm::Envelope env = m_display->getExtent();
  QRectF drect(env.m_llx, env.m_lly, env.getWidth(), env.getHeight());

  te::dt::TimeInstant t;
  if(m_ui->m_dateTimeEdit->isEnabled() == false)
    t = getTimeInstant();
  else
  {
    QDateTime d = m_ui->m_dateTimeEdit->dateTime();
    t = te::dt::TimeInstant(te::dt::Date(d.date().year(), d.date().month(), d.date().day()),
                            te::dt::TimeDuration(d.time().hour(), d.time().minute(), d.time().second(), 0));
  }

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  if(m_animationScene->m_numberOfPixmaps) // has images
  {
    // draw pixmap items
    for(it = list.begin(); it != list.end(); ++it)
    {
      AnimationItem* ai = (AnimationItem*)(*it);
      if(ai->pixmap().isNull())
      {
        // draw only the items that are within the time frame 
        te::dt::TimeInstant tini = ai->m_animationTime.first();
        te::dt::TimeInstant tfim = ai->m_animationTime.last();
        if(t < tini || t > tfim)
          continue;

        // draw pixmap itens
        PixmapItem* pi = (PixmapItem*)ai;
        drawPixmapItem(pi, drect, painter);
      }
    }
  }

  if(m_animationScene->m_numberOfTrajectories) // has trajectories
  {
    // draw trail trajectories
    QPixmap* pixt = ((AnimationScene*)(m_animationScene))->m_trajectoryPixmap;
    painter->drawPixmap(0, 0, *pixt);

    // draw icon trajectories
    for(it = list.begin(); it != list.end(); ++it)
    {
      AnimationItem* ai = (AnimationItem*)(*it);
      if(ai->pixmap().isNull() == false)
      {
        // draw only the items that are within the time frame 
        te::dt::TimeInstant tini = ai->m_animationTime.first();
        te::dt::TimeInstant tfim = ai->m_animationTime.last();
        if(t < tini || t > tfim)
          continue;

        // draw trajectory icons
        QPoint p = ai->getPosInDeviceCoordinate();
        QRect r = ai->pixmap().rect();
        r.moveCenter(p);
        QRect dr = m_display->rect();
        if(dr.intersects(r))
          drawTrajectoryIcon((te::qt::widgets::TrajectoryItem*)ai, r.topLeft(), painter);
      }
    }
  }

  updateSliderPos();
}

void te::qt::widgets::TimeSliderWidget::setGoAndBack(bool v)
{
  m_goAndBack = v;
  m_comingBack = !v;
}

void te::qt::widgets::TimeSliderWidget::setDirection(const QAbstractAnimation::Direction& direction)
{
  m_direction = direction;
  m_parallelAnimation->setDirection(m_direction);

  unsigned int count = m_parallelAnimation->animationCount();
  for(unsigned int i = 0; i < count; ++i)
  {
    QPropertyAnimation* a = (QPropertyAnimation*)m_parallelAnimation->animationAt(i);
    AnimationItem* ai = (AnimationItem*)a->targetObject();
    ai->setDirection(m_direction);
    ai->setDuration(m_duration);
  }
}

void te::qt::widgets::TimeSliderWidget::setDuration(const unsigned int& duration)
{
    m_animationScene->setDuration(duration);
}

void te::qt::widgets::TimeSliderWidget::createNewPixmap()
{
  m_animationScene->createNewPixmap();
}

void te::qt::widgets::TimeSliderWidget::draw()
{
  m_animationScene->createNewPixmap();
  m_animationScene->setMatrix();
  m_animationScene->draw(m_currentTime);
}

void te::qt::widgets::TimeSliderWidget::setAutomaticPan(const QString& title)
{
  m_animationScene->setAutomaticPan(title);
}

bool te::qt::widgets::TimeSliderWidget::eventFilter(QObject* obj, QEvent* e)
{
  if(obj == m_ui->m_forwardColorPushButton)
  {
    if(e->type() == QEvent::Paint)
    {
      QPainter painter(m_ui->m_forwardColorPushButton);
      QBrush brush(m_ui->m_forwardColorPushButton->palette().color(QPalette::Window));
      painter.fillRect(m_ui->m_forwardColorPushButton->rect(), brush);
      return true;
    }
    else if(e->type() == QEvent::MouseButtonPress)
    {
      QColor cor = QColorDialog::getColor(m_ui->m_forwardColorPushButton->palette().color(QPalette::Window), this, "Forward Color", QColorDialog::ShowAlphaChannel);
      if(cor.isValid() == false)
        return true;
      else if(cor == m_ui->m_forwardColorPushButton->palette().color(QPalette::Window))
        return true;

      m_ui->m_forwardColorPushButton->setPalette(QPalette(cor));

      QString titlecb = m_ui->m_trajectoryColorComboBox->currentText();

      QList<QGraphicsItem*> list = m_animationScene->items();
      QList<QGraphicsItem*>::iterator it;
      te::qt::widgets::TrajectoryItem* ti = 0;

      for(it = list.begin(); it != list.end(); ++it)
      {
        ti = (te::qt::widgets::TrajectoryItem*)(*it);
        QString title = ti->m_title;
        if(title == titlecb)
          break;
      }
      if(ti != 0)
        ti->m_forwardColor = cor;

      return true;
    }
  }
  else if(obj == m_ui->m_backwardColorPushButton)
  {
    if(e->type() == QEvent::Paint)
    {
      QPainter painter(m_ui->m_backwardColorPushButton);
      QBrush brush(m_ui->m_backwardColorPushButton->palette().color(QPalette::Window));
      painter.fillRect(m_ui->m_backwardColorPushButton->rect(), brush);
      return true;
    }
    else if(e->type() == QEvent::MouseButtonPress)
    {
      QColor cor = QColorDialog::getColor(m_ui->m_backwardColorPushButton->palette().color(QPalette::Window), this, "Forward Color", QColorDialog::ShowAlphaChannel);
      if(cor.isValid() == false)
        return true;
      else if(cor == m_ui->m_backwardColorPushButton->palette().color(QPalette::Window))
        return true;

      m_ui->m_backwardColorPushButton->setPalette(QPalette(cor));

      QString titlecb = m_ui->m_trajectoryColorComboBox->currentText();

      QList<QGraphicsItem*> list = m_animationScene->items();
      QList<QGraphicsItem*>::iterator it;
      te::qt::widgets::TrajectoryItem* ti = 0;

      for(it = list.begin(); it != list.end(); ++it)
      {
        ti = (te::qt::widgets::TrajectoryItem*)(*it);
        QString title = ti->m_title;
        if(title == titlecb)
          break;
      }
      if(ti != 0)
        ti->m_backwardColor = cor;

      return true;
    }
  }

  if(e->type() == QEvent::Close)
  {
    if(obj == this)
    {
      e->ignore();
      onStopToolButtonnClicked();
      m_display->update();

      hide();
      deleteMe();
      return true;
    }
    else if(obj == m_animationView)
    {
      e->ignore();
      return true;
    }
  }
  else if(e->type() == QEvent::Show)
  {
    if(obj == this)
      return true;
  }
  else if(e->type() == QEvent::Enter)
  {
    if(obj == this)
    {
      if(graphicsEffect())
        graphicsEffect()->setEnabled(false);
      return true;
    }
  }
  else if(e->type() == QEvent::Leave)
  {
    if(obj == this)
    {
      if(graphicsEffect())
        graphicsEffect()->setEnabled(true);
      return true;
    }
  }
  else if(e->type() == QEvent::MouseButtonRelease)
  {
    if(obj == m_ui->m_dateTimeEdit)
    {
      QDateTime d = m_ui->m_dateTimeEdit->dateTime();
      if(d == m_oldQDateTime)
      {
        d = fixDateTimeEdit(m_ui->m_dateTimeEdit, d);
        m_ui->m_dateTimeEdit->setDateTime(d);
      }
      m_oldQDateTime = d;
      return false;
    }
    else if(obj == m_ui->m_initialAnimationDateTimeEdit)
    {
      QDateTime d = m_ui->m_initialAnimationDateTimeEdit->dateTime();
      if(d == m_oldIQDateTime)
      {
        d = fixDateTimeEdit(m_ui->m_initialAnimationDateTimeEdit, d);
        m_ui->m_initialAnimationDateTimeEdit->setDateTime(d);
      }
      m_oldIQDateTime = d;
      return false;
    }
    else if(obj == m_ui->m_finalAnimationDateTimeEdit)
    {
      QDateTime d = m_ui->m_finalAnimationDateTimeEdit->dateTime();
      if(d == m_oldFQDateTime)
      {
        d = fixDateTimeEdit(m_ui->m_finalAnimationDateTimeEdit, d);
        m_ui->m_finalAnimationDateTimeEdit->setDateTime(d);
      }
      m_oldFQDateTime = d;
      return false;
    }
  }

  return QWidget::eventFilter(obj, e);
}

void te::qt::widgets::TimeSliderWidget::onSettingsToolButtonnClicked()
{
  if(m_ui->m_propertyWidget->isHidden())
  {
    showPropertySection(true);
    initProperty();
  }
  else
    showPropertySection(false);
}

void te::qt::widgets::TimeSliderWidget::onPlayToolButtonnClicked()
{
  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
  {
    m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-start"));
    m_parallelAnimation->pause();
    m_ui->m_dateTimeEdit->setEnabled(true);
  }
  else
  {
    m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-pause"));
    if(m_parallelAnimation->direction() == QAbstractAnimation::Forward)
    {
      if(m_ui->m_TemporalHorizontalSlider->value() == m_maxSliderValue)
      {
        m_parallelAnimation->setCurrentTime(0);
        m_ui->m_TemporalHorizontalSlider->setValue(0);
      }
    }
    else
    {
      if(m_ui->m_TemporalHorizontalSlider->value() == 0)
      {
        m_parallelAnimation->setCurrentTime(m_duration);
        m_ui->m_TemporalHorizontalSlider->setValue(m_maxSliderValue);
      }
    }
    m_ui->m_dateTimeEdit->setEnabled(false);
    play();
  }
}

void te::qt::widgets::TimeSliderWidget::onStopToolButtonnClicked()
{
  int state = m_parallelAnimation->state();
  m_parallelAnimation->stop();
  m_ui->m_dateTimeEdit->setEnabled(false);
  m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-start"));

  if(m_goAndBack)
  {
    if(m_comingBack)
    {
      changeDirection();
      m_comingBack = false;
    }
  }

  if(m_finished == false)
  {
    if(m_parallelAnimation->direction() == QAbstractAnimation::Forward)
    {
      m_ui->m_TemporalHorizontalSlider->setValue(0);
      m_parallelAnimation->setCurrentTime(0);
    }
    else
    {
      m_ui->m_TemporalHorizontalSlider->setValue(m_maxSliderValue);
      m_parallelAnimation->setCurrentTime(m_parallelAnimation->duration());
    }
    updateSliderPos();
    m_display->update();
  }
  m_finished = false;
}

void te::qt::widgets::TimeSliderWidget::onExtentChanged()
{
  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
  {
    m_parallelAnimation->pause();
    createNewPixmap();
    draw();
    m_display->update();
    m_parallelAnimation->resume();
  }
  else
  {
    createNewPixmap();
    draw();
    m_display->update();
  }
}

void te::qt::widgets::TimeSliderWidget::onSridChanged()
{
  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
  {
    createNewPixmap();
    calculateSpatialExtent();
    createAnimations();
    draw();
    m_display->update();
    m_parallelAnimation->start();
  }
  else
  {
    createNewPixmap();
    calculateSpatialExtent();
    createAnimations();
    draw();
    m_display->update();
  }
}

void te::qt::widgets::TimeSliderWidget::play()
{
  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Stopped)
  {
    createNewPixmap();
    m_parallelAnimation->start();
  }
  else if(state == QAbstractAnimation::Paused)
  {
    m_parallelAnimation->resume();
  }
}

void te::qt::widgets::TimeSliderWidget::onSliderMoved(int value)
{
  int oldCurTime = m_parallelAnimation->currentTime();

  if(m_parallelAnimation->state() == QAbstractAnimation::Stopped)
  {
    onStopToolButtonnClicked();
    return;
  }

  double v = (double)value / (double)m_maxSliderValue;

  int curTime = qRound(v * (double)m_parallelAnimation->duration());

  if(curTime == 0)
    curTime = 1;
  else if(curTime == m_parallelAnimation->duration())
    curTime = m_parallelAnimation->duration() - 1;

  bool erase = false;
  if(m_goAndBack == true)
  {
    if(m_comingBack == false && m_direction == QAbstractAnimation::Forward && curTime < oldCurTime)
      erase = true;
    else if(m_comingBack == false && m_direction == QAbstractAnimation::Backward && curTime > oldCurTime)
      erase = true;
    else if(m_comingBack == true && m_direction == QAbstractAnimation::Forward && curTime < oldCurTime)
      erase = true;
    else if(m_comingBack == true && m_direction == QAbstractAnimation::Backward && curTime > oldCurTime)
      erase = true;
  }
  else
  {
    if(m_direction == QAbstractAnimation::Forward && curTime < oldCurTime)
      erase = true;
    else if(m_direction == QAbstractAnimation::Backward && curTime > oldCurTime)
      erase = true;
  }

  m_parallelAnimation->setCurrentTime(curTime);
  if(erase)
    this->erase(curTime);

  m_currentTime = curTime;
  if(m_parallelAnimation->state() == QAbstractAnimation::Paused)
    m_display->update();

  te::dt::TimeInstant tinstant = getTimeInstant();
  QDate qdate(tinstant.getDate().getYear(), tinstant.getDate().getMonth(), tinstant.getDate().getDay());
  QTime qtime(tinstant.getTime().getHours(), tinstant.getTime().getMinutes(), tinstant.getTime().getSeconds());
  QDateTime qdatetime(qdate, qtime);
  m_ui->m_dateTimeEdit->setDateTime(qdatetime);
}

void te::qt::widgets::TimeSliderWidget::changeDirection()
{
  QAbstractAnimation::Direction direction = m_parallelAnimation->direction();
  if(direction == QAbstractAnimation::Forward)
    direction = QAbstractAnimation::Backward;
  else
    direction = QAbstractAnimation::Forward;
  setDirection(direction);
}

void te::qt::widgets::TimeSliderWidget::onFinishAnimation()
{
  m_finished = true;
  if(m_goAndBack)
  {
    changeDirection();
    m_comingBack = !m_comingBack;
    if(m_comingBack)
    {
      play();
      updateSliderPos();
      return;
    }

    if(m_loop)
    {
      play();
    }
    else
    {
      if(m_comingBack == false)
        onStopToolButtonnClicked();
    }
  }
  else
  {
    if(m_loop)
    {
      play();
    }
    else
      onStopToolButtonnClicked();
  }
}

void te::qt::widgets::TimeSliderWidget::updateSliderPos()
{
  m_currentTime = m_parallelAnimation->currentTime();
  int v = qRound((double)m_maxSliderValue * (double)m_currentTime/(double)m_duration);

  m_ui->m_TemporalHorizontalSlider->setValue(v);

  updateTimeTextEdit();
}

void te::qt::widgets::TimeSliderWidget::updateTimeTextEdit()
{
  if(m_ui->m_dateTimeEdit->isEnabled() == false && m_animationScene->items().isEmpty() == false)
  {
    te::dt::TimeInstant tinstant = getTimeInstant();
    QDate qdate(tinstant.getDate().getYear(), tinstant.getDate().getMonth(), tinstant.getDate().getDay());
    QTime qtime(tinstant.getTime().getHours(), tinstant.getTime().getMinutes(), tinstant.getTime().getSeconds());
    QDateTime qdatetime(qdate, qtime);
    m_ui->m_dateTimeEdit->setDateTime(qdatetime);
  }
}

void te::qt::widgets::TimeSliderWidget::deleteMe()
{
  emit deleteTimeSliderWidget();
}

void te::qt::widgets::TimeSliderWidget::onAddPushButtonClicked(bool b)
{
  QString title = QInputDialog::getText(this, "Add Item", "title");

  if(title.isEmpty())
    return;

  te::qt::widgets::ScopedCursor scopedCursor(Qt::WaitCursor);

  if(title == "lm40" || title == "lm41")
  {
    if(m_animationScene->items().isEmpty())
      m_ui->m_durationSpinBox->setValue(m_duration);

    int state = m_parallelAnimation->state();
    m_currentTime = m_parallelAnimation->currentTime();
    if(state == QAbstractAnimation::Running)
      onPlayToolButtonnClicked();
    openTrajectory(title);
    calculateSpatialExtent();
    calculateTemporalExtent();
    createAnimations();
    setDuration(m_duration);
    setDirection(m_direction);
    if(state == QAbstractAnimation::Running)
    {
      onPlayToolButtonnClicked();
      m_parallelAnimation->setCurrentTime(m_currentTime);
    }
    m_ui->m_playToolButton->setEnabled(true);
  }
  else
  {
    QString path = "C:/lixo/TemporalImages/";
    QString suffix = ".png";
    bool meteorological = false;

    if(title == "NoDoubt")
    {
      title = "NoDoubt ";
      path += "NoDoubt/";
      suffix = ".jpg";
    }
    else if(title == "segredo")
    {
      title = "segredo ";
      path += "segredo/";
      suffix = ".jpg";
    }
    else if(title == "IR_high")
    {
      //title = "S11232950_";
      path += "IR_high/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "IR_low")
    {
      //title = "S11232949_";
      path += "IR_low/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "WV_high")
    {
      //title = "S11232954_";
      path += "WV_high/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "WV_low")
    {
      //title = "S11232953_";
      path += "WV_low/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "GOESColorfull_high")
    {
      //title = "S11232956_";
      path += "GOESColorfull_high/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "GOESColorfull_low")
    {
      //title = "S11232955_";
      path += "GOESColorfull_low/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "GOESRealce_high")
    {
      //title = "S11232958_";
      path += "GOESRealce_high/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "GOESRealce_low")
    {
      //title = "S11232957_";
      path += "GOESRealce_low/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "GOESRealce2_high")
    {
      //title = "S11235909_";
      path += "GOESRealce2_high/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "GOESRealce2_low")
    {
      //title = "S11235908_";
      path += "GOESRealce2_low/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "GMVapor_high")
    {
      //title = "S11235908_";
      path += "GMVapor_high/";
      //suffix = ".jpg";
      meteorological = true;
    }
    else if(title == "GMVapor_low")
    {
      //title = "S11235908_";
      path += "GMVapor_low/";
      //suffix = ".jpg";
      meteorological = true;
    }

    if(meteorological)
    {
      if(m_animationScene->items().isEmpty())
        m_ui->m_durationSpinBox->setValue(m_duration);

      int state = m_parallelAnimation->state();
      m_currentTime = m_parallelAnimation->currentTime();
      if(state == QAbstractAnimation::Running)
        onPlayToolButtonnClicked();

      getGoesData(path);
      calculateSpatialExtent();
      calculateTemporalExtent();
      createAnimations();
      setDuration(m_duration);
      setDirection(m_direction);
      if(state == QAbstractAnimation::Running)
      {
        onPlayToolButtonnClicked();
        m_parallelAnimation->setCurrentTime(m_currentTime);
      }
      if(m_animationScene->items().isEmpty() == false)
      {
        m_ui->m_durationSpinBox->setEnabled(true);
        m_ui->m_settingsToolButton->setEnabled(true);
        m_ui->m_playToolButton->setEnabled(true);
        m_ui->m_stopToolButton->setEnabled(true);
        if(m_parallelAnimation->state() == QAbstractAnimation::Paused)
          m_ui->m_dateTimeEdit->setEnabled(true);
        else
          m_ui->m_dateTimeEdit->setEnabled(false);
      }
      initProperty();

      m_ui->m_opacityComboBox->addItem(title);
      int count = m_ui->m_opacityComboBox->count();
      m_ui->m_opacityComboBox->setCurrentIndex(count-1);
      onOpacityComboBoxActivated(count-1);
      return;
    }


    QString file = path + title + "000001" + suffix;
    if(title.contains("fastfood"))
      file = path + "fastfood000001.png";

    QFile qfile(file);
    if(qfile.exists())
    {
      if(m_animationScene->items().isEmpty())
        m_ui->m_durationSpinBox->setValue(m_duration);

      int state = m_parallelAnimation->state();
      m_currentTime = m_parallelAnimation->currentTime();
      if(state == QAbstractAnimation::Running)
        onPlayToolButtonnClicked();

      addTemporalImages(path + title + suffix, true);
      calculateSpatialExtent();
      calculateTemporalExtent();
      createAnimations();
      setDuration(m_duration);
      setDirection(m_direction);
      if(state == QAbstractAnimation::Running)
      {
        onPlayToolButtonnClicked();
        m_parallelAnimation->setCurrentTime(m_currentTime);
      }
    }
    else
    {
      QMessageBox::information(this, "Not Added", "Invalid Animation item"); 
      return;
    }
  }

  if(m_animationScene->items().isEmpty() == false)
  {
    m_ui->m_durationSpinBox->setEnabled(true);
    m_ui->m_settingsToolButton->setEnabled(true);
    m_ui->m_playToolButton->setEnabled(true);
    m_ui->m_stopToolButton->setEnabled(true);
    if(m_parallelAnimation->state() == QAbstractAnimation::Paused)
      m_ui->m_dateTimeEdit->setEnabled(true);
    else
      m_ui->m_dateTimeEdit->setEnabled(false);
  }
  initProperty();

  m_ui->m_opacityComboBox->addItem(title);
  int count = m_ui->m_opacityComboBox->count();
  m_ui->m_opacityComboBox->setCurrentIndex(count-1);
  onOpacityComboBoxActivated(count-1);
}

void te::qt::widgets::TimeSliderWidget::onRemovePushButtonClicked(bool b)
{
  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for(it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = (AnimationItem*)(*it);
    QString title = ai->m_title;
    int r = QMessageBox::question(this, "Remove Item", "Remove " + title, QMessageBox::Ok, QMessageBox::No);
    if(r == QMessageBox::Ok)
    {
      removeComboItem(ai);
      if(list.count() == 1)
      {
        onStopToolButtonnClicked();
        m_animationScene->removeItem(ai);
        m_parallelAnimation->removeAnimation(ai->m_animation);
        delete ai->m_animation;
        delete ai;
        m_ui->m_TemporalHorizontalSlider->setValue(0);
        m_parallelAnimation->setCurrentTime(0);
        m_ui->m_settingsToolButton->setEnabled(false);
        m_ui->m_playToolButton->setEnabled(false);
        m_ui->m_stopToolButton->setEnabled(false);
        m_ui->m_durationSpinBox->setEnabled(false);
        m_ui->m_dateTimeEdit->setEnabled(false);
        showPropertySection(false);
        m_display->update();
        initProperty();
        return;
      }

      bool running = false;
      int state = m_parallelAnimation->state();
      if(state == QAbstractAnimation::Running)
      {
        running = true;;
        onPlayToolButtonnClicked();
      }

      m_currentTime = m_parallelAnimation->currentTime();
      m_animationScene->removeItem(ai);
      m_parallelAnimation->removeAnimation(ai->m_animation);
      if(ai->pixmap().isNull() == false)
      {
        if(m_animationScene->m_numberOfTrajectories)
        {
          m_animationScene->m_trajectoryPixmap->fill(Qt::transparent);
          draw();
        }
      }
      delete ai->m_animation;
      delete ai;

      calculateSpatialExtent();
      calculateTemporalExtent();
      createAnimations();
      setDuration(m_duration);
      setDirection(m_direction);

      if(running)
      {
        onPlayToolButtonnClicked();
        m_parallelAnimation->setCurrentTime(m_currentTime);
      }

      break;
    }
  }
  initProperty();
}

void te::qt::widgets::TimeSliderWidget::putToFront(te::qt::widgets::AnimationItem* item)
{
  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();
  
  QList<QGraphicsItem*> list = m_animationScene->items();
  int i = list.indexOf(item);
  list.takeAt(i);
  list.prepend(item); // put to last item

  QList<QGraphicsItem*>::iterator it = list.begin();
  while(it != list.end())
  {
    te::qt::widgets::AnimationItem* ai = (te::qt::widgets::AnimationItem*)*it++;
    m_animationScene->removeItem(ai);
  }

  while(list.isEmpty() == false)
  {
    te::qt::widgets::AnimationItem* ai = (te::qt::widgets::AnimationItem*)list.takeFirst();
    m_animationScene->addItem(ai);
  }

  onPlayToolButtonnClicked();
}

void te::qt::widgets::TimeSliderWidget::putToBack(te::qt::widgets::AnimationItem* item)
{
  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();
  
  QList<QGraphicsItem*> list = m_animationScene->items();
  int i = list.indexOf(item);
  list.takeAt(i);
  list.append(item); // put to first item

  QList<QGraphicsItem*>::iterator it = list.begin();
  while(it != list.end())
  {
    te::qt::widgets::AnimationItem* ai = (te::qt::widgets::AnimationItem*)*it++;
    m_animationScene->removeItem(ai);
  }

  while(list.isEmpty() == false)
  {
    te::qt::widgets::AnimationItem* ai = (te::qt::widgets::AnimationItem*)list.takeFirst();
    m_animationScene->addItem(ai);
  }

  onPlayToolButtonnClicked();
}

void te::qt::widgets::TimeSliderWidget::onFrontPushButtonClicked(bool b)
{
  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for(it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = (AnimationItem*)(*it);
    QString title = ai->m_title;
    int r = QMessageBox::question(this, "Put To Front", "Animation Item: " + title, QMessageBox::Ok, QMessageBox::No);
    if(r == QMessageBox::Ok)
    {
      putToFront(ai);
      break;
    }
  }
}

void te::qt::widgets::TimeSliderWidget::onBackPushButtonClicked(bool b)
{
  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for(it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = (AnimationItem*)(*it);
    QString title = ai->m_title;
    int r = QMessageBox::question(this, "Put To Back", "Animation Item: " + title, QMessageBox::Ok, QMessageBox::No);
    if(r == QMessageBox::Ok)
    {
      putToBack(ai);
      break;
    }
  }
}

void te::qt::widgets::TimeSliderWidget::erase(const unsigned int& curTime)
{
  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;
  for(it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = (AnimationItem*)(*it);
    if(ai->pixmap().isNull() == false)
      ((te::qt::widgets::TrajectoryItem*)ai)->m_erasePerfectly = m_erasePerfectly;
  }

  if(m_erasePerfectly)
  {
    m_parallelAnimation->setCurrentTime(curTime);
    draw();
  }
  else
    m_parallelAnimation->setCurrentTime(curTime);

  m_currentTime = curTime;
}

void te::qt::widgets::TimeSliderWidget::onDurationValueChanged(int val)
{
  if(val == m_duration)
    return;

  m_currentTime = m_parallelAnimation->currentTime();
  double f = double(m_currentTime) / (double)m_duration;

  int step = m_ui->m_durationSpinBox->singleStep();
  int v = val;
  if(v == 100)
    v = 0;

  if(v == 0 || v == m_duration + step || v == m_duration - step)
  {
    if(step == 100)
    {
      if(v < step/10)
        v = 100;
      else if(v > step*10)
      {
        step *= 10;
        v = m_duration + step;
      }
    }
    else if(step == 1000)
    {
      if(v < step/10)
      {
        step /= 10;
        v = m_duration - step;
      }
      else if(v > step*10)
      {
        step *= 10;
        v = m_duration + step;
      }
    }
    else if(step == 10000)
    {
      if(v < step/10)
      {
        step /= 10;
        v = m_duration - step;
      }
      else if(v > step*10)
      {
        step *= 10;
        v = m_duration + step;
      }
    }
    else if(step == 100000)
    {
      if(v < step/10)
      {
        step /= 10;
        v = m_duration - step;
      }
      else if(v > step*10)
      {
        step *= 10;
        v = m_duration + step;
      }
    }
    else if(step == 1000000)
    {
      if(v < step/10)
      {
        step /= 10;
        v = m_duration - step;
      }
      else if(v > step*10)
      {
        step *= 10;
        v = m_duration + step;
      }
    }

    m_duration = (v / step) * step;
    m_ui->m_durationSpinBox->setValue(m_duration);
    m_ui->m_durationSpinBox->setSingleStep(step);
  }
  else
    m_duration = val;

  bool running = false;
  if(m_parallelAnimation->state() == QAbstractAnimation::Running)
  {
    running = true;;
    onPlayToolButtonnClicked();
  }

  m_currentTime = qRound(f * (double)m_duration);
  m_animationScene->m_trajectoryPixmap->fill(Qt::transparent);
  setDuration(m_duration);
  m_parallelAnimation->setCurrentTime(m_currentTime);
  draw();
  if(running)
    onPlayToolButtonnClicked();
}

void te::qt::widgets::TimeSliderWidget::onDateTimeEditChanged(const QDateTime& qdt)
{
  if(m_ui->m_dateTimeEdit->isEnabled() == false)
    return;

  QDateTime t(qdt);
  int oldCurTime = m_parallelAnimation->currentTime();

  te::dt::TimeInstant tini = m_temporalAnimationExtent.getInitialTimeInstant();
  unsigned long totalSecs =  m_temporalAnimationExtent.getTimePeriod().length().total_seconds();

  te::dt::Date date(t.date().year(), t.date().month(), t.date().day());
  te::dt::TimeDuration duration(t.time().hour(), t.time().minute(), t.time().second());
  te::dt::TimeInstant time(date, duration);

  te::dt::TimePeriod tp(tini, time);
  unsigned long secs =  tp.getTimePeriod().length().total_seconds();

  double v = (double)secs / (double)totalSecs;
  int curTime = qRound(v * (double)m_parallelAnimation->duration());

  if(curTime == 0)
    curTime = 1;
  else if(curTime == m_parallelAnimation->duration())
    curTime = m_parallelAnimation->duration() - 1;

  bool erase = false;
  if(m_goAndBack == true)
  {
    if(m_comingBack == false && m_direction == QAbstractAnimation::Forward && curTime < oldCurTime)
      erase = true;
    else if(m_comingBack == false && m_direction == QAbstractAnimation::Backward && curTime > oldCurTime)
      erase = true;
    else if(m_comingBack == true && m_direction == QAbstractAnimation::Forward && curTime < oldCurTime)
      erase = true;
    else if(m_comingBack == true && m_direction == QAbstractAnimation::Backward && curTime > oldCurTime)
      erase = true;
  }
  else
  {
    if(m_direction == QAbstractAnimation::Forward && curTime < oldCurTime)
      erase = true;
    else if(m_direction == QAbstractAnimation::Backward && curTime > oldCurTime)
      erase = true;
  }

  m_currentTime = curTime;
  m_parallelAnimation->setCurrentTime(curTime);
  if(erase)
    this->erase(curTime);

  if(m_parallelAnimation->state() == QAbstractAnimation::Paused)
    m_display->update();
}

QDateTime te::qt::widgets::TimeSliderWidget::fixDateTimeEdit(QDateTimeEdit* dte, const QDateTime& qdt)
{
  QDateTime t(qdt);
  QDateTimeEdit::Section section = dte->currentSection();
  if(section == QDateTimeEdit::SecondSection)
  {
    int sec = t.time().second();
    if(sec == 0)
      t = t.addSecs(-1);
    else
      t = t.addSecs(1);
  }
  else if(section == QDateTimeEdit::MinuteSection)
  {
    int min = t.time().minute();
    if(min == 0)
      t = t.addSecs(-60);
    else
      t = t.addSecs(60);
  }
  else if(section == QDateTimeEdit::HourSection)
  {
    int hour = t.time().hour();
    if(hour == 0)
      t = t.addSecs(-3600);
    else
      t = t.addSecs(3600);
  }
  else if(section == QDateTimeEdit::DaySection)
  {
    int day = t.date().day();
    if(day == 1)
      t = t.addDays(-1);
    else
      t = t.addDays(1);
  }
  else if(section == QDateTimeEdit::MonthSection)
  {
    int month = t.date().month();
    if(month == 1)
      t = t.addMonths(-1);
    else
      t = t.addMonths(1);
  }

  return t;
}

void te::qt::widgets::TimeSliderWidget::drawPixmapItem(te::qt::widgets::PixmapItem* pi, const QRectF& dwrect, QPainter* painter)
{
  if(pi->m_currentImageFile.isEmpty())
    return;

  QRectF rec = pi->m_imaRect;
  if(dwrect.intersects(rec))
  {
    QRect r = pi->m_matrix.mapRect(rec).toRect();
    QImage* ima = getImage(pi);

    if(pi->m_opacity == 255)
      painter->drawImage(r, *ima);
    else
    {
      QSize size = ima->size();
      int width = size.width();
      int height = size.height();

      if(ima->format() == QImage::Format_ARGB32)
      {
        for(int i = 0; i < height; ++i)
        {
          unsigned char* u = ima->scanLine(i);
          for(int j = 0; j < width; ++j)
          {
            QRgb* v = (QRgb*)(u + (j << 2));
            if(qAlpha(*v) > 50)
              *v =  qRgba(qRed(*v), qGreen(*v), qBlue(*v) , pi->m_opacity);
          }
        }
        painter->drawImage(r, *ima);
      }
      else
      {
        QImage img(size, QImage::Format_ARGB32);
        for(int i = 0; i < height; ++i)
        {
          unsigned char* u = ima->scanLine(i);
          unsigned char* uu = img.scanLine(i);

          for(int j = 0; j < width; ++j)
          {
            QRgb* v = (QRgb*)(u + (j << 2));
            QRgb* uv = (QRgb*)(uu + (j << 2));
            if(qAlpha(*v) > 50)
              *uv =  qRgba(qRed(*v), qGreen(*v), qBlue(*v) , pi->m_opacity);
          }
        }
        painter->drawImage(r, img);
      }
    }
    delete ima;
  }
}

void te::qt::widgets::TimeSliderWidget::drawTrajectoryIcon(const te::qt::widgets::TrajectoryItem* t, const QPoint& pos, QPainter* painter)
{
  if(t->m_opacity == 255)
    painter->drawPixmap(pos, t->pixmap());
  else
  {
    QSize size = t->pixmap().size();
    int width = size.width();
    int height = size.height();
    QImage ima = t->pixmap().toImage();

    if(ima.format() == QImage::Format_ARGB32)
    {
      for(int i = 0; i < height; ++i)
      {
        unsigned char* u = ima.scanLine(i);
        for(int j = 0; j < width; ++j)
        {
          QRgb* v = (QRgb*)(u + (j << 2));
          if(qAlpha(*v) < 50)
            *v =  qRgba(255, 255, 255 , 0);
          else
            *v =  qRgba(qRed(*v), qGreen(*v), qBlue(*v) , t->m_opacity);
        }
      }
      painter->drawImage(pos, ima);
    }
    else
    {
      QImage img(size, QImage::Format_ARGB32);
      for(int i = 0; i < height; ++i)
      {
        unsigned char* u = ima.scanLine(i);
        unsigned char* uu = img.scanLine(i);

        for(int j = 0; j < width; ++j)
        {
          QRgb* v = (QRgb*)(u + (j << 2));
          QRgb* uv = (QRgb*)(uu + (j << 2));
          if(qAlpha(*v) < 50)
            *uv =  qRgba(255, 255, 255 , 0);
          else
            *uv =  qRgba(qRed(*v), qGreen(*v), qBlue(*v) , t->m_opacity);
        }
      }
      painter->drawImage(pos, img);
    }
  }
}

void te::qt::widgets::TimeSliderWidget::onOpacityValueChanged(int v)
{
  QString titlecb = m_ui->m_opacityComboBox->currentText();

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for(it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = (AnimationItem*)(*it);
    QString title = ai->m_title;
    if(title.contains(titlecb))
    {
      ai->m_opacity = v;
      m_display->update();
      break;
    }
  }
}

void te::qt::widgets::TimeSliderWidget::initProperty()
{
  te::dt::TimeInstant ti = m_temporalAnimationExtent.getInitialTimeInstant();
  QDate qdatei(ti.getDate().getYear(), ti.getDate().getMonth(), ti.getDate().getDay());
  QTime qtimei(ti.getTime().getHours(), ti.getTime().getMinutes(), ti.getTime().getSeconds());
  QDateTime qdatetimei(qdatei, qtimei);
  m_ui->m_initialAnimationDateTimeEdit->setDateTime(qdatetimei);

  te::dt::TimeInstant tf = m_temporalAnimationExtent.getFinalTimeInstant();
  QDate qdatef(tf.getDate().getYear(), tf.getDate().getMonth(), tf.getDate().getDay());
  QTime qtimef(tf.getTime().getHours(), tf.getTime().getMinutes(), tf.getTime().getSeconds());
  QDateTime qdatetimef(qdatef, qtimef);
  m_ui->m_finalAnimationDateTimeEdit->setDateTime(qdatetimef);

  // set mimimum datetime
  te::dt::TimeInstant tmin = m_temporalExtent.getInitialTimeInstant();
  QDateTime minimum(QDate(tmin.getDate().getYear(), tmin.getDate().getMonth(), tmin.getDate().getDay()),
                    QTime(tmin.getTime().getHours(), tmin.getTime().getMinutes(), tmin.getTime().getSeconds()));
  m_ui->m_initialAnimationDateTimeEdit->setMinimumDateTime(minimum);

  // set maximum datetime
  te::dt::TimeInstant tmax = m_temporalExtent.getFinalTimeInstant();
  QDateTime maximum(QDate(tmax.getDate().getYear(), tmax.getDate().getMonth(), tmax.getDate().getDay()),
                    QTime(tmax.getTime().getHours(), tmax.getTime().getMinutes(), tmax.getTime().getSeconds()));
  m_ui->m_finalAnimationDateTimeEdit->setMaximumDateTime(maximum);

  if(m_direction == QAbstractAnimation::Forward)
  {
    if(m_ui->m_forwardRadioButton->isChecked() == false)
      m_ui->m_forwardRadioButton->toggle();
  }

  if(m_loop)
    m_ui->m_loopCheckBox->setChecked(true);
  if(m_goAndBack)
    m_ui->m_goAndBackCheckBox->setChecked(true);
}

//void te::qt::widgets::TimeSliderWidget::onOkPushButtonClicked()
//{
//  te::dt::TimeInstant ti = getInitialTime();
//  te::dt::TimeInstant tf = getFinalTime();
//  if(ti > tf || ti == tf)
//  {
//    QMessageBox::warning(this, "Time Extent Error", "Initial time can not be larger than the end time!", QMessageBox::Ok);
//    return;
//  }
//
//  QDateTime dt = m_ui->m_dateTimeEdit->dateTime();
//  te::dt::TimePeriod tp = m_temporalAnimationExtent;
//
//  if(isSettingChanged())
//  {
//    m_currentTime = m_parallelAnimation->currentTime();
//    int state = m_parallelAnimation->state();
//    if(state == QAbstractAnimation::Running)
//      onPlayToolButtonnClicked();
//
//    QAbstractAnimation::Direction direction = m_direction;
//
//    if(m_ui->m_forwardRadioButton->isChecked())
//      m_direction = QAbstractAnimation::Forward;
//    else
//      m_direction = QAbstractAnimation::Backward;
//
//    m_loop = m_ui->m_loopCheckBox->isChecked();
//    m_goAndBack = m_ui->m_forwardAndThenBackwardCheckBox->isChecked();
//
//    m_temporalAnimationExtent = te::dt::TimePeriod(ti, tf);
//
//    QDateTime minimum(QDate(ti.getDate().getYear(), ti.getDate().getMonth(), ti.getDate().getDay()),
//      QTime(ti.getTime().getHours(), ti.getTime().getMinutes(), ti.getTime().getSeconds()));
//    QDateTime maximum(QDate(tf.getDate().getYear(), tf.getDate().getMonth(), tf.getDate().getDay()),
//      QTime(tf.getTime().getHours(), tf.getTime().getMinutes(), tf.getTime().getSeconds()));
//
//    m_ui->m_dateTimeEdit->setMinimumDateTime(minimum);
//    m_ui->m_dateTimeEdit->setMaximumDateTime(maximum);
//
//    createNewPixmap();
//    calculateSpatialExtent();
//    createAnimations();
//    setDuration(m_duration);
//    setDirection(m_direction);
//
//    if(state == QAbstractAnimation::Running)
//    {
//      onPlayToolButtonnClicked();
//      m_parallelAnimation->setCurrentTime(m_currentTime);
//      m_parallelAnimation->start();
//    }
//    else
//    {
//      m_parallelAnimation->setCurrentTime(m_currentTime);
//      m_parallelAnimation->start();
//      if(state == QAbstractAnimation::Paused)
//        m_parallelAnimation->pause();   
//    }
//
//    if(tp == m_temporalAnimationExtent)
//      onDateTimeEditChanged(dt);
//  }
//  showPropertySection(false);
//}

//void te::qt::widgets::TimeSliderWidget::onCancelPushButtonClicked()
//{
//  showPropertySection(false);
//}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getInitialTime()
{
  QDateTime t = m_ui->m_initialAnimationDateTimeEdit->dateTime();
  te::dt::Date date(t.date().year(), t.date().month(), t.date().day());
  te::dt::TimeDuration duration(t.time().hour(), t.time().minute(), t.time().second());
  te::dt::TimeInstant time(date, duration);
  return time;
  //QString time = m_ui->m_initialTimeLineEdit->text();
  //QString s = simpleTimeString2IsoString(time);
  //if(s.isEmpty())
  //  return te::dt::TimeInstant();

  //te::dt::TimeInstant t(s.toStdString());
  //return t;
}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getFinalTime()
{
  QDateTime t = m_ui->m_finalAnimationDateTimeEdit->dateTime();
  te::dt::Date date(t.date().year(), t.date().month(), t.date().day());
  te::dt::TimeDuration duration(t.time().hour(), t.time().minute(), t.time().second());
  te::dt::TimeInstant time(date, duration);
  return time;
}

//QString te::qt::widgets::TimeSliderWidget::simpleTimeString2IsoString(QString timeString)
//{
//  QString timeIso;
//
//  QString tt = timeString.toUpper();
//  tt.remove(" ");
//  std::string time = tt.toStdString();
//  if(time.size() != 19)
//    return timeIso;
//  std::string tIso = time.substr(0, 4);
//
//  std::string s = time.substr(5, 3);
//  std::string mes;
//  if(s == "JAN")
//    mes = "01";
//  else if(s == "FEB")
//    mes = "02";
//  else if(s == "MAR")
//    mes = "03";
//  else if(s == "APR")
//    mes = "04";
//  else if(s == "MAY")
//    mes = "05";
//  else if(s == "JUN")
//    mes = "06";
//  else if(s == "JUL")
//    mes = "07";
//  else if(s == "AUG")
//    mes = "08";
//  else if(s == "SEP")
//    mes = "09";
//  else if(s == "OCT")
//    mes = "10";
//  else if(s == "NOV")
//    mes = "11";
//  else if(s == "DEC")
//    mes = "12";
//  if(mes.empty())
//    return timeIso;
//
//  tIso += mes;
//
//  tIso += time.substr(9, 2);
//  tIso += "T";
//  tIso += time.substr(11, 2);
//  tIso += time.substr(14, 2);
//  tIso += time.substr(17, 2);
//
//  timeIso = tIso.c_str();
//  return timeIso;
//}

void te::qt::widgets::TimeSliderWidget::onResetInitialTimePushButtonClicked()
{
  te::dt::TimeInstant t = m_temporalExtent.getInitialTimeInstant();
  QDate qdate(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtime(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetime(qdate, qtime);
  m_ui->m_initialAnimationDateTimeEdit->setDateTime(qdatetime);
}

void te::qt::widgets::TimeSliderWidget::onResetFinalTimePushButtonClicked()
{
  te::dt::TimeInstant t = m_temporalExtent.getFinalTimeInstant();
  QDate qdate(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtime(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetime(qdate, qtime);
  m_ui->m_finalAnimationDateTimeEdit->setDateTime(qdatetime);
}

void te::qt::widgets::TimeSliderWidget::showPropertySection(bool b)
{
  if(b==false && m_ui->m_propertyWidget->isHidden())
    return;
  if(b==true && m_ui->m_propertyWidget->isHidden()==false)
    return;

  int h = m_ui->m_propertyWidget->height();

  if(b == false)
  {
    m_ui->m_propertyWidget->hide();
    resize(width(), height()-h);
  }
  else
  {
    m_ui->m_propertyWidget->show();
    resize(width(), height()+h);
  }
}

bool te::qt::widgets::TimeSliderWidget::isSettingChanged()
{
  te::dt::TimeInstant ti = m_temporalAnimationExtent.getInitialTimeInstant();
  QDate qdatei(ti.getDate().getYear(), ti.getDate().getMonth(), ti.getDate().getDay());
  QTime qtimei(ti.getTime().getHours(), ti.getTime().getMinutes(), ti.getTime().getSeconds());
  QDateTime qdatetimei(qdatei, qtimei);
  QDateTime qdi = m_ui->m_initialAnimationDateTimeEdit->dateTime();
  if(qdatetimei != qdi)
    return true;

  te::dt::TimeInstant tf = m_temporalAnimationExtent.getFinalTimeInstant();
  QDate qdatef(tf.getDate().getYear(), tf.getDate().getMonth(), tf.getDate().getDay());
  QTime qtimef(tf.getTime().getHours(), tf.getTime().getMinutes(), tf.getTime().getSeconds());
  QDateTime qdatetimef(qdatef, qtimef);
  QDateTime qdf = m_ui->m_finalAnimationDateTimeEdit->dateTime();
  if(qdatetimef != qdf)
    return true;

  if(m_ui->m_forwardRadioButton->isChecked() && m_direction != QAbstractAnimation::Forward)
    return true;
  if(m_ui->m_forwardRadioButton->isChecked() == false && m_direction != QAbstractAnimation::Backward)
    return true;
  if(m_ui->m_loopCheckBox->isChecked() != m_loop)
    return true;
  if(m_ui->m_goAndBackCheckBox->isChecked() != m_goAndBack)
    return true;

  return false;
}

void te::qt::widgets::TimeSliderWidget::onForwardRadioButtonClicked(bool b)
{
  if(m_direction == QAbstractAnimation::Forward)
  {
    if(m_comingBack)
      m_comingBack = false;
  }
  else
    changeDirection();
}

void te::qt::widgets::TimeSliderWidget::onBackwardRadioButtonClicked(bool b)
{
  if(m_direction == QAbstractAnimation::Backward)
  {
    if(m_comingBack)
      m_comingBack = false;
  }
  else
    changeDirection();
}

void te::qt::widgets::TimeSliderWidget::onLoopCheckBoxClicked(bool b)
{
  m_loop = b;
}

void te::qt::widgets::TimeSliderWidget::onGoAndBackCheckBoxClicked(bool b)
{
  if(m_goAndBack != b)
  {
    if(m_comingBack)
    {
      changeDirection();
      m_comingBack = false;
    }
  }

  m_goAndBack = b;
}

void te::qt::widgets::TimeSliderWidget::onApplyTimeIntervalPushButtonClicked(bool b)
{
  te::dt::TimeInstant ti = getInitialTime();
  te::dt::TimeInstant tf = getFinalTime();
  if(ti > tf || ti == tf)
  {
    QMessageBox::warning(this, "Time Extent Error", "Initial time can not be larger than the end time!", QMessageBox::Ok);
    return;
  }

  te::dt::TimeInstant t = m_temporalAnimationExtent.getInitialTimeInstant();
  QDate qdatei(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtimei(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetimei(qdatei, qtimei);
  QDateTime qdi = m_ui->m_initialAnimationDateTimeEdit->dateTime();

  t = m_temporalAnimationExtent.getFinalTimeInstant();
  QDate qdatef(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtimef(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetimef(qdatef, qtimef);
  QDateTime qdf = m_ui->m_finalAnimationDateTimeEdit->dateTime();
  if(qdatetimei == qdi && qdatetimef == qdf)
    return;

  m_currentTime = m_parallelAnimation->currentTime();
  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();

  QDateTime dt = m_ui->m_dateTimeEdit->dateTime();
  te::dt::TimePeriod tp = m_temporalAnimationExtent;

  m_temporalAnimationExtent = te::dt::TimePeriod(ti, tf);

  QDateTime minimum(QDate(ti.getDate().getYear(), ti.getDate().getMonth(), ti.getDate().getDay()),
    QTime(ti.getTime().getHours(), ti.getTime().getMinutes(), ti.getTime().getSeconds()));
  QDateTime maximum(QDate(tf.getDate().getYear(), tf.getDate().getMonth(), tf.getDate().getDay()),
    QTime(tf.getTime().getHours(), tf.getTime().getMinutes(), tf.getTime().getSeconds()));

  m_ui->m_dateTimeEdit->setMinimumDateTime(minimum);
  m_ui->m_dateTimeEdit->setMaximumDateTime(maximum);

  createNewPixmap();
  calculateSpatialExtent();
  createAnimations();
  setDuration(m_duration);
  setDirection(m_direction);

  if(state == QAbstractAnimation::Running)
  {
    onPlayToolButtonnClicked();
    m_parallelAnimation->setCurrentTime(m_currentTime);
    m_parallelAnimation->start();
  }
  else
  {
    m_parallelAnimation->setCurrentTime(m_currentTime);
    m_parallelAnimation->start();
    if(state == QAbstractAnimation::Paused)
      m_parallelAnimation->pause();   
  }

  if(tp == m_temporalAnimationExtent)
    onDateTimeEditChanged(dt);
}

void te::qt::widgets::TimeSliderWidget::onTrajectoryColorComboBoxActivated(int i)
{
  QString titlecb = m_ui->m_trajectoryColorComboBox->currentText();

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;
  te::qt::widgets::TrajectoryItem* ti = 0;

  for(it = list.begin(); it != list.end(); ++it)
  {
    ti = (te::qt::widgets::TrajectoryItem*)(*it);
    QString title = ti->m_title;
    if(title == titlecb)
      break;
  }
  if(ti == 0)
    return;

  m_ui->m_forwardColorPushButton->setPalette(QPalette(ti->m_forwardColor));
  m_ui->m_forwardColorPushButton->update();
  m_ui->m_backwardColorPushButton->setPalette(QPalette(ti->m_backwardColor));
  m_ui->m_backwardColorPushButton->update();
  m_ui->m_autoPanCheckBox->setChecked(ti->m_automaticPan);
}

void te::qt::widgets::TimeSliderWidget::onOpacityComboBoxActivated(int i)
{
  QString titlecb = m_ui->m_opacityComboBox->currentText();

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for(it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = (AnimationItem*)(*it);
    QString title = ai->m_title;
    if(title.contains(titlecb))
    {
      int op = ai->m_opacity;
      m_ui->m_opacitySpinBox->setValue(op);
      break;
    }
  }
}

void te::qt::widgets::TimeSliderWidget::removeComboItem(te::qt::widgets::AnimationItem* ai)
{
  QString title = ai->m_title;
  //if(title[title.length()-1] == ' ')
  //  title = title.remove(title.length()-1, 1);
  int index = m_ui->m_opacityComboBox->findText(title);
  m_ui->m_opacityComboBox->removeItem(index);
  onOpacityComboBoxActivated(m_ui->m_opacityComboBox->currentIndex());

  index = m_ui->m_trajectoryColorComboBox->findText(title);
  m_ui->m_trajectoryColorComboBox->removeItem(index);
  onTrajectoryColorComboBoxActivated(m_ui->m_trajectoryColorComboBox->currentIndex());
}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getGoesTime(const QString& fileName)
{
  QString file(fileName);
  int ind = file.indexOf("_");
  if(file.indexOf("_", ind+1) != -1)
    ind = file.indexOf("_", ind+1);
  file.remove(0, ind+1);
  QString ano(file), mes(file), dia(file), hour(file), min(file);
  ano.remove(4, ano.length()-4);
  mes.remove(0, 4);
  mes.remove(2, mes.length()-2);
  dia.remove(0, 6);
  dia.remove(2, dia.length()-2);
  hour.remove(0, 8);
  hour.remove(2, hour.length()-2);
  min.remove(0, 10);
  min.remove(2, min.length()-2);

  te::dt::Date date(ano.toInt(), mes.toInt(), dia.toInt());
  te::dt::TimeDuration dur(hour.toInt(), min.toInt(), 0);
  return te::dt::TimeInstant(date, dur);
}

void te::qt::widgets::TimeSliderWidget::onAddEtaPushButtonClicked(bool)
{
  if(m_animationScene->items().isEmpty())
    m_ui->m_durationSpinBox->setValue(m_duration);

  int state = m_parallelAnimation->state();
  m_currentTime = m_parallelAnimation->currentTime();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();

  te::qt::widgets::PixmapItem* pi = getEtaData("C:/lixo/FORECAST/eta5km/");
  std::vector<std::pair<int, QColor> > tab;
  int i = 0;
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(2, 1, 201, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(2, 18, 213, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 24, 220, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 36, 227, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 54, 240, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 71, 254, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 90, 252, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(3, 106, 251, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(3, 130, 253, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(3, 148, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(3, 160, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(4, 174, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(4, 186, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(5, 200, 251, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(5, 214, 246, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(3, 227, 241, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(3, 240, 237, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(2, 254, 233, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(2, 254, 212, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 254, 190, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 254, 170, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 254, 160, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 254, 151, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 254, 105, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 254, 72, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(1, 254, 37, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(2, 254, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(25, 254, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(56, 254, 3, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(71, 254, 3, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(99, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(125, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(150, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(175, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(200, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(209, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(218, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(229, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(237, 255, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(240, 248, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(244, 241, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(250, 237, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(251, 232, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(251, 226, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(252, 215, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 208, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 199, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 194, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 190, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 184, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(252, 179, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(252, 172, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 164, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 155, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 147, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 136, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 125, 3, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 112, 3, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 99, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 75, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 50, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(253, 25, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 1, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 1, 12, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 1, 25, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 1, 37, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 0, 49, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 2, 37, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 3, 25, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(254, 4, 12, 255)));
  tab.push_back(std::pair<int, QColor>(i+=1, QColor(255, 5, 1, 255)));

  pi->setLUT(tab);
  calculateSpatialExtent();
  calculateTemporalExtent();
  createAnimations();
  setDuration(m_duration);
  setDirection(m_direction);
  if(state == QAbstractAnimation::Running)
  {
    onPlayToolButtonnClicked();
    m_parallelAnimation->setCurrentTime(m_currentTime);
  }
  if(m_animationScene->items().isEmpty() == false)
  {
    m_ui->m_durationSpinBox->setEnabled(true);
    m_ui->m_settingsToolButton->setEnabled(true);
    m_ui->m_playToolButton->setEnabled(true);
    m_ui->m_stopToolButton->setEnabled(true);
    if(m_parallelAnimation->state() == QAbstractAnimation::Paused)
      m_ui->m_dateTimeEdit->setEnabled(true);
    else
      m_ui->m_dateTimeEdit->setEnabled(false);
  }
  initProperty();

  m_ui->m_opacityComboBox->addItem("eta5km");
  int count = m_ui->m_opacityComboBox->count();
  m_ui->m_opacityComboBox->setCurrentIndex(count-1);
  onOpacityComboBoxActivated(count-1);
}

void te::qt::widgets::TimeSliderWidget::onAddHidroPushButtonClicked(bool)
{
  if(m_animationScene->items().isEmpty())
    m_ui->m_durationSpinBox->setValue(m_duration);

  int state = m_parallelAnimation->state();
  m_currentTime = m_parallelAnimation->currentTime();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();

  te::qt::widgets::PixmapItem* pi = getHidroData("C:/lixo/FORECAST/hidro/");
  std::vector<std::pair<int, QColor> > tab;
  int i = 0;
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(2, 1, 201, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(1, 71, 254, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(3, 148, 250, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(5, 200, 251, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(2, 254, 233, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(1, 254, 151, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(2, 254, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(99, 254, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(200, 255, 1, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(237, 255, 0, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(251, 232, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(254, 199, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(252, 179, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(254, 147, 4, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(253, 99, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(254, 1, 2, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(254, 0, 49, 255)));
  tab.push_back(std::pair<int, QColor>(i+=3, QColor(255, 5, 1, 255)));

  pi->setLUT(tab);
  calculateSpatialExtent();
  calculateTemporalExtent();
  createAnimations();
  setDuration(m_duration);
  setDirection(m_direction);
  if(state == QAbstractAnimation::Running)
  {
    onPlayToolButtonnClicked();
    m_parallelAnimation->setCurrentTime(m_currentTime);
  }
  if(m_animationScene->items().isEmpty() == false)
  {
    m_ui->m_durationSpinBox->setEnabled(true);
    m_ui->m_settingsToolButton->setEnabled(true);
    m_ui->m_playToolButton->setEnabled(true);
    m_ui->m_stopToolButton->setEnabled(true);
    if(m_parallelAnimation->state() == QAbstractAnimation::Paused)
      m_ui->m_dateTimeEdit->setEnabled(true);
    else
      m_ui->m_dateTimeEdit->setEnabled(false);
  }
  initProperty();

  m_ui->m_opacityComboBox->addItem("hidro");
  int count = m_ui->m_opacityComboBox->count();
  m_ui->m_opacityComboBox->setCurrentIndex(count-1);
  onOpacityComboBoxActivated(count-1);

}

void te::qt::widgets::TimeSliderWidget::onAutoPanCheckBoxClicked(bool)
{
  QString title = m_ui->m_trajectoryColorComboBox->currentText();
  setAutomaticPan(title);
}

void te::qt::widgets::TimeSliderWidget::getHidroParameters(const QString& path, size_t& nlines, size_t& ncols, size_t& undef, QRectF& rect)
{
  QString s;
  char buf[2000];
  QString file(path + "racc.ctl");
  FILE* fp = fopen(file.toStdString().c_str(), "r");
  fread(buf, 2000, sizeof(char), fp);

  // get UNDEF value
  QString ss(QString(buf).simplified());
  size_t pos = ss.indexOf("UNDEF ") + strlen("UNDEF "); 
  ss.remove(0, pos);
  pos = ss.indexOf("TITLE");
  s = ss.left(pos);
  undef = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  // get XDEF ulx and resX values
  pos = ss.indexOf("XDEF ") + strlen("XDEF ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  ncols = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  pos = ss.indexOf("LINEAR ") + strlen("LINEAR ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  double llx = atof(s.toStdString().c_str()) - 360.;
  ss.remove(0, pos);
  pos = ss.indexOf("YDEF ");
  s = ss.left(pos);
  double resX = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get YDEF uly and resY values
  pos = ss.indexOf("YDEF ") + strlen("YDEF ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  nlines = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  pos = ss.indexOf("LINEAR ") + strlen("LINEAR ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  double lly = atof(s.toStdString().c_str());
  ss.remove(0, pos);
  pos = ss.indexOf("ZDEF ");
  s = ss.left(pos);
  double resY = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  double w = (double)ncols * resX;
  double h = (double)nlines * resY;
  rect = QRectF(llx, lly, w, h);

  fclose(fp);
}

void te::qt::widgets::TimeSliderWidget::getEtaParameters(const QString& path, size_t& nlines, size_t& ncols, float& undef, QRectF& rect)
{
  QString s;
  char buf[2000];
  QString file(path + "Prec5km.ctl");
  FILE* fp = fopen(file.toStdString().c_str(), "r");
  fread(buf, 2000, sizeof(char), fp);

  // get UNDEF value
  QString ss(QString(buf).simplified());
  size_t pos = ss.indexOf("undef ") + strlen("undef "); 
  ss.remove(0, pos);
  pos = ss.indexOf("xdef");
  s = ss.left(pos);
  undef = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get XDEF ulx and resX values
  pos = ss.indexOf("xdef ") + strlen("xdef ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  ncols = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  pos = ss.indexOf("linear ") + strlen("linear ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  double llx = atof(s.toStdString().c_str());
  ss.remove(0, pos);
  pos = ss.indexOf("ydef ");
  s = ss.left(pos);
  double resX = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get YDEF uly and resY values
  pos = ss.indexOf("ydef ") + strlen("ydef ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  nlines = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  pos = ss.indexOf("linear ") + strlen("linear ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  double lly = atof(s.toStdString().c_str());
  ss.remove(0, pos);
  pos = ss.indexOf("zdef ");
  s = ss.left(pos);
  double resY = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  double w = (double)ncols * resX;
  double h = (double)nlines * resY;
  rect = QRectF(llx, lly, w, h);

  fclose(fp);
}

QImage* te::qt::widgets::TimeSliderWidget::getImage(te::qt::widgets::PixmapItem* pi)
{
  QString path = pi->m_dir.absolutePath() + "/";
  QString file = pi->m_currentImageFile;
  QFileInfo fi(file);
  QString baseName = fi.baseName();
  QImage* ima;

  if(pi->m_suffix == ".bin" && baseName == "racc")
  {
    QRectF rect;
    size_t nlines, ncols, undef;
    getHidroParameters(path, nlines, ncols, undef, rect);

    size_t nchars = ncols * 2;
    uchar* buf = new uchar[nchars];
    FILE* fp = fopen(file.toStdString().c_str(), "rb");
    ima = new QImage(ncols, nlines, QImage::Format_ARGB32);
    ima->fill(Qt::transparent);

    uchar uc[3];
    uc[2] = 0;
    for(size_t j = 0; j < nlines; ++j)
    {
      uchar* u = ima->scanLine(j);
      fread(buf, nchars, sizeof(char), fp);
      for(size_t i = 0; i < ncols; i++)
      {
        uc[0] = *(buf + (i<<1));
        uc[1] = *(buf + (1+(i<<1)));
        ushort b = *(ushort*)uc;
        if(b != undef)
        {
          b = (b+5) / 10;
          QRgb* v = (QRgb*)(u + (i << 2));
          *v = qRgba(pi->m_lut[b].red(), pi->m_lut[b].green(), pi->m_lut[b].blue(), 255);
        }
      }
    }
    fclose(fp);
    delete []buf;
  }
  else if(pi->m_suffix == ".bin" && baseName.contains("Prec5km"))
  {
    QRectF rect;
    size_t nlines, ncols;
    float undef;
    getEtaParameters(path, nlines, ncols, undef, rect);

    QString auxFile(file);
    size_t pos = auxFile.indexOf(baseName);
    auxFile.remove(0, pos);
    pos = auxFile.indexOf("_");
    size_t pp = auxFile.indexOf(".bin");
    int offset = atoi(auxFile.mid(pos+1, pp-pos+1).toStdString().c_str());
    size_t fileSize = nlines * ncols * 4 + 8; // dado é float e desprepreza 4 bytes iniciais e 4 bytes finais
    offset *= fileSize;
    auxFile.remove(pos, auxFile.length()-pos);
    auxFile = path + auxFile + pi->m_suffix;

    size_t nchars = ncols * 4;
    uchar* buf = new uchar[nchars];
    FILE* fp = fopen(auxFile.toStdString().c_str(), "rb");
    fseek(fp, offset, SEEK_SET);
    fseek(fp, 4, SEEK_CUR); // despreza 4 bytes da primeira linha
    ima = new QImage(ncols, nlines, QImage::Format_ARGB32);
    ima->fill(Qt::transparent);

    uchar uc[5];
    uc[4] = 0;
    for(size_t j = 0; j < nlines; ++j)
    {
      uchar* u = ima->scanLine(nlines-1-j); // origem bottom left
      fread(buf, nchars, sizeof(char), fp);

      for(size_t i = 0; i < ncols; i++)
      {
        uc[0] = *(buf + (i<<2));
        uc[1] = *(buf + (1+(i<<2)));
        uc[2] = *(buf + (2+(i<<2)));
        uc[3] = *(buf + (3+(i<<2)));
        float b = *(float*)uc;
        if(b != undef)
        {
          uchar a = (uchar) (b * 10000. + .5);
          QRgb* v = (QRgb*)(u + (i << 2));
          *v = qRgba(pi->m_lut[a].red(), pi->m_lut[a].green(), pi->m_lut[a].blue(), 255);
        }
      }
    }
    fclose(fp);
    delete []buf;
  }
  else
    ima = new QImage(file);

  return ima;
}

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getHidroData(QString path)
{
  QDir dir(path);
  size_t nlines, ncols, undef;
  QRectF rect;

  QString dirName = dir.dirName();
  QString suffix(".bin");

  PixmapItem* pi = new PixmapItem(dirName, path, m_display);
  getHidroParameters(path, nlines, ncols, undef, pi->m_imaRect);

  // CHUTE WGS84
  pi->m_SRID = 4326;
  m_animationScene->addItem(pi);
  Animation* animation = new Animation(pi, "pos");
  animation->m_spatialExtent = te::gm::Envelope(pi->m_imaRect.x(), pi->m_imaRect.y(), pi->m_imaRect.right(), pi->m_imaRect.bottom());
  animation->setEasingCurve(QEasingCurve::Linear);
  m_parallelAnimation->addAnimation(animation);

  QStringList nameFilter;
  nameFilter.append("*.bin");
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  size_t count = files.count();

  // get time extent
  te::dt::TimeInstant ti = getHidroTime(path + files.first());
  te::dt::TimeInstant tf = getHidroTime(path + files.last());
  animation->m_temporalExtent = te::dt::TimePeriod(ti, tf);

  // set data
  te::dt::TimeInstant t;
  for(size_t i = 0; i < count; ++i)
  {
    QString f = files[i];
    t = getHidroTime(f);
    pi->m_time.push_back(t);
    pi->m_files.push_back(f);
  }

  pi->m_route = new te::gm::LineString(count, te::gm::LineStringType, pi->m_SRID);

  // crie valores não repetitivos e nem muito grandes ou pequenos
  QPointF pos(animation->m_spatialExtent.m_llx, animation->m_spatialExtent.m_lly);
  double w = animation->m_spatialExtent.getWidth();
  double h = animation->m_spatialExtent.getHeight();
  double dw = w / 64;
  double dh = h / 64;
  for(size_t i = 0; i < count; ++i)
  {
    if(i & 64)
      pos -= QPointF(dw, dh);
    else
      pos += QPointF(dw, dh);

    pi->m_route->setPoint(i, pos.x(), pos.y());
  }
  return pi;
}

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getEtaData(QString path)
{
  QDir dir(path);

  QString suffix = ".bin";
  QStringList nameFilter;
  nameFilter.append("*" + suffix);
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  if(files.isEmpty())
    return 0;

  // CHUTE WGS84
  PixmapItem* pi = new PixmapItem(dir.dirName(), path, m_display);
  pi->m_SRID = 4326;
  m_animationScene->addItem(pi);
  Animation* animation = new Animation(pi, "pos");

  size_t nlines, ncols;
  float undef;
  QRectF rect;
  getEtaParameters(path, nlines, ncols, undef, pi->m_imaRect);
  animation->m_spatialExtent = te::gm::Envelope(pi->m_imaRect.x(), pi->m_imaRect.y(), pi->m_imaRect.right(), pi->m_imaRect.bottom());
  animation->setEasingCurve(QEasingCurve::Linear);
  m_parallelAnimation->addAnimation(animation);

  size_t count = files.count();

  // get time extent
  te::dt::TimeInstant ti = getEtaTime(path + files.first());
  te::dt::TimeInstant tf = getEtaTime(path + files.last());
  tf = te::dt::TimeInstant(tf.getTimeInstant() + boost::posix_time::seconds(60*60*24*3));
  animation->m_temporalExtent = te::dt::TimePeriod(ti, tf);

  // set data
  te::dt::TimeInstant tnext, t;
  QString sn, fname;
  for(size_t i = 0; i < count; ++i)
  {
    QString f = files[i];
    QFileInfo fi(path + f);
    QString baseName = fi.baseName();
    t = getEtaTime(f);
 
    if(i != count-1)
      tnext = getEtaTime(files[i+1]);
    else
      tnext = te::dt::TimeInstant(t.getTimeInstant() + boost::posix_time::seconds(60*60*24*3));

    size_t n = 1;
    while((t < tnext || t == tnext) && n < 72)
    {
      pi->m_time.push_back(t);
      t = te::dt::TimeInstant(t.getTimeInstant() + boost::posix_time::seconds(60*60));
      sn.setNum(n++);
      fname = path + baseName + "_" + sn + suffix;
      pi->m_files.push_back(fname);
    }
  }

  size_t tsize = pi->m_files.count();
  pi->m_route = new te::gm::LineString(tsize, te::gm::LineStringType, pi->m_SRID);

  // crie valores não repetitivos e nem muito grandes ou pequenos
  QPointF pos(animation->m_spatialExtent.m_llx, animation->m_spatialExtent.m_lly);
  double w = animation->m_spatialExtent.getWidth();
  double h = animation->m_spatialExtent.getHeight();
  double dw = w / 64;
  double dh = h / 64;
  for(size_t i = 0; i < tsize; ++i)
  {
    if(i & 64)
      pos -= QPointF(dw, dh);
    else
      pos += QPointF(dw, dh);

    pi->m_route->setPoint(i, pos.x(), pos.y());
  }
  return pi;
}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getHidroTime(const QString& fileName)
{
  QString file(fileName);
  int ind = file.indexOf("racc.") + strlen("racc.");
  file.remove(0, ind);
  QString ano = "20" + file.mid(0, 2);
  QString mes = file.mid(2, 2);
  QString dia = file.mid(4, 2);
  QString hour = file.mid(7, 2);
  QString min = file.mid(9, 2);

  te::dt::Date date(ano.toInt(), mes.toInt(), dia.toInt());
  te::dt::TimeDuration dur(hour.toInt(), min.toInt(), 0);
  return te::dt::TimeInstant(date, dur);
}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getEtaTime(const QString& fileName)
{
  QString file(fileName);
  int ind = file.indexOf("Prec5km") + strlen("Prec5km");
  file.remove(0, ind);
  QString ano = file.mid(0, 4);
  QString mes = file.mid(4, 2);
  QString dia = file.mid(6, 2);
  QString hour = file.mid(8, 2);

  te::dt::Date date(ano.toInt(), mes.toInt(), dia.toInt());
  te::dt::TimeDuration dur(hour.toInt() + 1, 0, 0); // tem deslocamento de 1 hora
  return te::dt::TimeInstant(date, dur);
}
