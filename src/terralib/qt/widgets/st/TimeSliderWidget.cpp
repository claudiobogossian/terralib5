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
  \file terralib/qt/widgets/st/TimeSliderWidget.cpp

  \brief A widget used to alow the user to controle the visualization of temporal data
*/

//Terralib
#include "../../../st/maptools/TrajectoryDataSetLayer.h"
#include "../canvas/MapDisplay.h"
#include "Animation.h"
#include "AnimationScene.h"
#include "TrajectoryItem.h"
#include "PixmapItem.h"
#include "TimeSliderWidget.h"
#include "ui_TimeSliderWidgetForm.h"
#include "SliderPropertiesDialog.h"

#include <terralib/dataaccess.h>
#include <terralib/stmemory.h>
#include <terralib/st.h>
#include <terralib/qt/widgets/utils/ScopedCursor.h>
#include <terralib/qt/widgets.h>
#include <terralib/maptools.h>
#include <terralib/dataaccess.h>

//QT
#include <QGraphicsEffect>
#include <QSlider>
#include <QGraphicsView>
#include <QMimeData>
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
#include <QUrl>

//ST
#include <cmath>

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
  setAcceptDrops(true);

  te::gm::Envelope env = m_display->getExtent();
  m_initialDisplayRect = QRectF(env.m_llx, env.m_lly, env.getWidth(), env.getHeight());

  // slider normalized between 0 and m_maxSliderValue
  m_ui->m_TemporalHorizontalSlider->setMinimum(0);
  m_ui->m_TemporalHorizontalSlider->setMaximum(m_maxSliderValue);

  m_ui->m_durationSpinBox->setMinimum(100);
  m_ui->m_durationSpinBox->setMaximum(100000000);
  m_ui->m_durationSpinBox->setValue(10000);
  m_ui->m_durationSpinBox->setSingleStep(10000);

  //Adjusting the icons and the graphic effects
  m_ui->m_settingsToolButton->setIcon(QIcon::fromTheme("preferences-system"));
  m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-start"));
  m_ui->m_stopToolButton->setIcon(QIcon::fromTheme("media-playback-stop"));

  // connect signal and slots

  connect(m_ui->m_dateTimeEdit, SIGNAL(dateTimeChanged(const QDateTime&) ), this, SLOT(onDateTimeEditChanged(const QDateTime&)));
  connect(m_ui->m_durationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDurationValueChanged(int)));
  connect(m_ui->m_settingsToolButton, SIGNAL(clicked()), this, SLOT(onSettingsToolButtonnClicked()));
  connect(m_ui->m_playToolButton, SIGNAL(clicked()), this, SLOT(onPlayToolButtonnClicked()));
  connect(m_ui->m_stopToolButton, SIGNAL(clicked()), this, SLOT(onStopToolButtonnClicked()));
  connect(m_ui->m_TemporalHorizontalSlider, SIGNAL(sliderMoved (int)), this, SLOT(onSliderMoved(int)));

/*
========================================================================================================
                                     To be updated/removed
========================================================================================================
*/

  //connect(m_ui->m_addEtaPushButton, SIGNAL(clicked(bool) ), this, SLOT(onAddEtaPushButtonClicked(bool)));
  //connect(m_ui->m_addHidroPushButton, SIGNAL(clicked(bool) ), this, SLOT(onAddHidroPushButtonClicked(bool)));
  //connect(m_ui->addPushButton, SIGNAL(clicked(bool)), this, SLOT(onAddPushButtonClicked(bool)));
  //connect(m_ui->removePushButton, SIGNAL(clicked(bool)), this, SLOT(onRemovePushButtonClicked(bool)));
  //connect(m_ui->frontPushButton, SIGNAL(clicked(bool)), this, SLOT(onFrontPushButtonClicked(bool)));
  //connect(m_ui->backPushButton, SIGNAL(clicked(bool)), this, SLOT(onBackPushButtonClicked(bool)));

  setAttribute(Qt::WA_DeleteOnClose, true);

  m_loop = true; //By default, the animation will be looping.
  m_duration = 10000; //By default, the animation journey time will be 10000 miliseconds.
  m_ui->m_durationSpinBox->setValue(m_duration);
  m_direction = QAbstractAnimation::Forward; //By default, the animation direction will be forward.
  setGoAndBack(true); //By default, the animation is forward and then backward.

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

  m_ui->m_dateTimeEdit->installEventFilter(this);
  m_ui->m_dateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");

  if(m_animationScene->items().isEmpty())
  {
    m_ui->m_durationSpinBox->setEnabled(false);
    m_ui->m_settingsToolButton->setEnabled(false);
    m_ui->m_playToolButton->setEnabled(false);
    m_ui->m_stopToolButton->setEnabled(false);
    m_ui->m_dateTimeEdit->setEnabled(false);
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

void te::qt::widgets::TimeSliderWidget::layerSelected(te::map::AbstractLayerPtr layer)
{
  //setCurrentLayer(layer);
}

void te::qt::widgets::TimeSliderWidget::layerAdded(te::map::AbstractLayerPtr layer)
{
  if(layer->getType() == "TRAJECTORYDATASETLAYER")
  {
    te::st::TrajectoryDataSet* trajLayer = dynamic_cast<te::st::TrajectoryDataSetLayer*>(layer.get())->getTrajectoryDataset().release();
    addTrajectory(QString::fromStdString(trajLayer->getId()), "", trajLayer);
    if(isHidden())
      show();
  }
}

void te::qt::widgets::TimeSliderWidget::layerRemoved(te::map::AbstractLayerPtr layer)
{
  if(layer->getType() == "TRAJECTORYDATASETLAYER")
  {
    //te::st::TrajectoryDataSet* trajLayer = dynamic_cast<te::st::TrajectoryDataSetLayer*>(layer.get())->getTrajectoryDataset().release();
    //addTrajectory(QString::fromStdString(trajLayer->getId()), "", trajLayer);
  }
}

bool te::qt::widgets::TimeSliderWidget::alreadyExists(QPair<QString, QString>& item)
{
  QList<QPair<QString, QString> >::iterator it;
  for(it = m_itemList.begin(); it != m_itemList.end(); ++it)
  {
    if(*it == item)
      return true;
  }
  m_itemList.append(item);
  return false;
}

void te::qt::widgets::TimeSliderWidget::openTrajectory(const QString file, const QString& leao)
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

  if(leao == "40: locations")
  {
    te::st::TrajectoryDataSetInfo tjinfo40(dsinfo, "40: locations", phTimeIdx, geomIdx, -1, "40");
    te::st::TrajectoryDataSet* tjDS40 = te::st::STDataLoader::getDataSet(tjinfo40).release();
    addTrajectory(leao, "c:/lixo/helicopteroT2.png", tjDS40);
    delete tjDS40;
  }
  else if(leao == "41: locations")
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

  ti->m_opacity = 120;
  ti->m_forwardColor = QColor(rand() % 255, rand() % 255, rand() % 255, TE_OPAQUE);
  ti->m_forwardColor.setAlpha(ti->m_opacity);
  ti->m_backwardColor = QColor(rand() % 255, rand() % 255, rand() % 255, TE_OPAQUE);;
  ti->m_backwardColor.setAlpha(ti->m_opacity);

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
  calculateSpatialExtent();
  calculateTemporalExtent();
  createAnimations();
  setDuration(m_duration);
  setDirection(m_direction);

  int state = m_parallelAnimation->state();
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
}

void te::qt::widgets::TimeSliderWidget::addTemporalImages(const QString& filePath)
{
  if(m_animationScene->items().isEmpty())
    m_ui->m_durationSpinBox->setValue(m_duration);

  int state = m_parallelAnimation->state();
  m_currentTime = m_parallelAnimation->currentTime();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();

  te::qt::widgets::PixmapItem* pi = getMetadata(filePath);
  if(pi == 0)
  {
    QMessageBox::information(this, "Error", "Load error");
    m_itemList.pop_back();
    return;
  }

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
//  initProperty();

  //QDir dir(filePath);
  //m_ui->m_opacityComboBox->addItem(dir.dirName());
  //int count = m_ui->m_opacityComboBox->count();
  //m_ui->m_opacityComboBox->setCurrentIndex(count-1);
  //onOpacityComboBoxActivated(count-1);
}

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getGoesMetadata(const QString& path)
{
  PixmapItem* pi = getTemporalImageCtlParameters(path);
  if(pi == 0)
    return 0;

  m_animationScene->addItem(pi);
  Animation* animation = new Animation(pi, "pos");
  animation->m_spatialExtent = te::gm::Envelope(pi->m_imaRect.x(), pi->m_imaRect.y(), pi->m_imaRect.right(), pi->m_imaRect.bottom());
  animation->setEasingCurve(QEasingCurve::Linear);
  m_parallelAnimation->addAnimation(animation);

  QString suffix = "." + pi->m_suffix;
  QStringList nameFilter;
  nameFilter.append("*" + suffix);
  QDir dir(path);
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  size_t count = files.count();

  // get time extent
  te::dt::TimeInstant ti = getGoesTime(path + "/" + files.first());
  te::dt::TimeInstant tf = getGoesTime(path +  "/" + files.last());
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

  return pi;
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
  //if(obj == m_ui->m_forwardColorPushButton)
  //{
  //  if(e->type() == QEvent::Paint)
  //  {
  //    QPainter painter(m_ui->m_forwardColorPushButton);
  //    QBrush brush(m_ui->m_forwardColorPushButton->palette().color(QPalette::Window));
  //    painter.fillRect(m_ui->m_forwardColorPushButton->rect(), brush);
  //    return true;
  //  }
  //  else if(e->type() == QEvent::MouseButtonPress)
  //  {
  //    QColor cor = QColorDialog::getColor(m_ui->m_forwardColorPushButton->palette().color(QPalette::Window), this, "Forward Color", QColorDialog::ShowAlphaChannel);
  //    if(cor.isValid() == false)
  //      return true;
  //    else if(cor == m_ui->m_forwardColorPushButton->palette().color(QPalette::Window))
  //      return true;

  //    m_ui->m_forwardColorPushButton->setPalette(QPalette(cor));

  //    QString titlecb = m_ui->m_trajectoryColorComboBox->currentText();

  //    QList<QGraphicsItem*> list = m_animationScene->items();
  //    QList<QGraphicsItem*>::iterator it;
  //    te::qt::widgets::TrajectoryItem* ti = 0;

  //    for(it = list.begin(); it != list.end(); ++it)
  //    {
  //      ti = (te::qt::widgets::TrajectoryItem*)(*it);
  //      QString title = ti->m_title;
  //      if(title == titlecb)
  //        break;
  //    }
  //    if(ti != 0)
  //      ti->m_forwardColor = cor;

  //    return true;
  //  }
  //}
  //else if(obj == m_ui->m_backwardColorPushButton)
  //{
  //  if(e->type() == QEvent::Paint)
  //  {
  //    QPainter painter(m_ui->m_backwardColorPushButton);
  //    QBrush brush(m_ui->m_backwardColorPushButton->palette().color(QPalette::Window));
  //    painter.fillRect(m_ui->m_backwardColorPushButton->rect(), brush);
  //    return true;
  //  }
    //else if(e->type() == QEvent::MouseButtonPress)
    //{
    //  QColor cor = QColorDialog::getColor(m_ui->m_backwardColorPushButton->palette().color(QPalette::Window), this, "Forward Color", QColorDialog::ShowAlphaChannel);
    //  if(cor.isValid() == false)
    //    return true;
    //  else if(cor == m_ui->m_backwardColorPushButton->palette().color(QPalette::Window))
    //    return true;

    //  m_ui->m_backwardColorPushButton->setPalette(QPalette(cor));

    //  QString titlecb = m_ui->m_trajectoryColorComboBox->currentText();

    //  QList<QGraphicsItem*> list = m_animationScene->items();
    //  QList<QGraphicsItem*>::iterator it;
    //  te::qt::widgets::TrajectoryItem* ti = 0;

    //  for(it = list.begin(); it != list.end(); ++it)
    //  {
    //    ti = (te::qt::widgets::TrajectoryItem*)(*it);
    //    QString title = ti->m_title;
    //    if(title == titlecb)
    //      break;
    //  }
    //  if(ti != 0)
    //    ti->m_backwardColor = cor;

    //  return true;
    //}
//  }

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
    //else if(obj == m_ui->m_initialAnimationDateTimeEdit)
    //{
    //  QDateTime d = m_ui->m_initialAnimationDateTimeEdit->dateTime();
    //  if(d == m_oldIQDateTime)
    //  {
    //    d = fixDateTimeEdit(m_ui->m_initialAnimationDateTimeEdit, d);
    //    m_ui->m_initialAnimationDateTimeEdit->setDateTime(d);
    //  }
    //  m_oldIQDateTime = d;
    //  return false;
    //}
    //else if(obj == m_ui->m_finalAnimationDateTimeEdit)
    //{
    //  QDateTime d = m_ui->m_finalAnimationDateTimeEdit->dateTime();
    //  if(d == m_oldFQDateTime)
    //  {
    //    d = fixDateTimeEdit(m_ui->m_finalAnimationDateTimeEdit, d);
    //    m_ui->m_finalAnimationDateTimeEdit->setDateTime(d);
    //  }
    //  m_oldFQDateTime = d;
    //  return false;
    //}
  }

  return QWidget::eventFilter(obj, e);
}

void te::qt::widgets::TimeSliderWidget::onSettingsToolButtonnClicked()
{
  std::auto_ptr<te::qt::widgets::SliderPropertiesDialog> dlg (new te::qt::widgets::SliderPropertiesDialog(this));
  dlg->populateUi(m_temporalExtent, m_animationScene->items(), (m_direction == QAbstractAnimation::Forward), m_loop, m_goAndBack);

  if (dlg->exec() == QDialog::Accepted)
  {
    if(m_parallelAnimation->state() == QAbstractAnimation::Running)
      onStopToolButtonnClicked();

    //Adjusting basic options
    dlg->isForwardChecked() ? m_direction = QAbstractAnimation::Forward : m_direction = QAbstractAnimation::Backward;
    m_loop = dlg->isLoopChecked();
    m_goAndBack = dlg->isGoBackChecked();

    //Adjusting temporal options
    m_temporalAnimationExtent = te::dt::TimePeriod(dlg->getInitialTime(), dlg->getFinalTime());
    m_ui->m_dateTimeEdit->setMinimumDateTime(dlg->getInitialQDate());
    m_ui->m_dateTimeEdit->setMaximumDateTime(dlg->getFinalQDate());
    onDateTimeEditChanged(m_ui->m_dateTimeEdit->dateTime());

    //Updating
    createNewPixmap();
    calculateSpatialExtent();
    createAnimations();
    setDuration(m_duration);
    setDirection(m_direction);
    m_display->update();
  }
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

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getHidroCtlParameters(const QString& path)
{
  char buf[2000];
  QString file(path + "/racc.ctl");
  FILE* fp = fopen(file.toStdString().c_str(), "r");
  if(fp == 0)
    return 0;

  fread(buf, 2000, sizeof(char), fp);
  fclose(fp);
  QString ss(QString(buf).simplified());

  // validation
  if(!(ss.contains("undef ", Qt::CaseInsensitive) && ss.contains("title", Qt::CaseInsensitive) && 
    ss.contains("xdef", Qt::CaseInsensitive) && ss.contains("ydef", Qt::CaseInsensitive) && 
    ss.contains("linear", Qt::CaseInsensitive) && ss.contains("zdef", Qt::CaseInsensitive)))
    return 0;

  QString s;
  QDir dir(path);
  te::qt::widgets::PixmapItem* pi = new PixmapItem(dir.dirName(), path, m_display);

  // CHUTE SRID 4326 WGS84
  pi->m_SRID = 4326;

  // get UNDEF value
  size_t pos = ss.indexOf("UNDEF ", Qt::CaseInsensitive) + strlen("UNDEF "); 
  ss.remove(0, pos);
  pos = ss.indexOf("TITLE", Qt::CaseInsensitive);
  s = ss.left(pos);
  pi->m_undef = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  // get XDEF ulx and resX values
  pos = ss.indexOf("XDEF ", Qt::CaseInsensitive) + strlen("XDEF ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  pi->m_ncols = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  pos = ss.indexOf("LINEAR ", Qt::CaseInsensitive) + strlen("LINEAR ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  double llx = atof(s.toStdString().c_str()) - 360.;
  ss.remove(0, pos);
  pos = ss.indexOf("YDEF ", Qt::CaseInsensitive);
  s = ss.left(pos);
  double resX = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get YDEF uly and resY values
  pos = ss.indexOf("YDEF ", Qt::CaseInsensitive) + strlen("YDEF ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  pi->m_nlines = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  pos = ss.indexOf("LINEAR ", Qt::CaseInsensitive) + strlen("LINEAR ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  double lly = atof(s.toStdString().c_str());
  ss.remove(0, pos);
  pos = ss.indexOf("ZDEF ", Qt::CaseInsensitive);
  s = ss.left(pos);
  double resY = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  double w = (double)pi->m_ncols * resX;
  double h = (double)pi->m_nlines * resY;
  pi->m_imaRect = QRectF(llx, lly, w, h);

  return pi;
}

void te::qt::widgets::TimeSliderWidget::setHidroLUT(te::qt::widgets::PixmapItem* pi)
{
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
}

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getEtaCtlParameters(const QString& path)
{
  char buf[2000];
  QString file(path + "/Prec5km.ctl");
  FILE* fp = fopen(file.toStdString().c_str(), "r");
  if(fp == 0)
    return 0;

  fread(buf, 2000, sizeof(char), fp);
  fclose(fp);
  QString ss(QString(buf).simplified());

  // validation
  if(!(ss.contains("undef ", Qt::CaseInsensitive) && ss.contains("xdef", Qt::CaseInsensitive) 
    && ss.contains("ydef", Qt::CaseInsensitive) && ss.contains("linear", Qt::CaseInsensitive)
    && ss.contains("zdef", Qt::CaseInsensitive)))
    return 0;

  QString s;
  QDir dir(path);
  te::qt::widgets::PixmapItem* pi = new PixmapItem(dir.dirName(), path, m_display);

  // CHUTE SRID 4326 WGS84
  pi->m_SRID = 4326;

  // get UNDEF value
  size_t pos = ss.indexOf("undef ", Qt::CaseInsensitive) + strlen("undef "); 
  ss.remove(0, pos);
  pos = ss.indexOf("xdef", Qt::CaseInsensitive);
  s = ss.left(pos);
  pi->m_undef = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get XDEF ulx and resX values
  pos = ss.indexOf("xdef ", Qt::CaseInsensitive) + strlen("xdef ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  pi->m_ncols = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  pos = ss.indexOf("linear ", Qt::CaseInsensitive) + strlen("linear ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  double llx = atof(s.toStdString().c_str());
  ss.remove(0, pos);
  pos = ss.indexOf("ydef ", Qt::CaseInsensitive);
  s = ss.left(pos);
  double resX = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get YDEF uly and resY values
  pos = ss.indexOf("ydef ", Qt::CaseInsensitive) + strlen("ydef ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  pi->m_nlines = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  pos = ss.indexOf("linear ", Qt::CaseInsensitive) + strlen("linear ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ");
  s = ss.left(pos);
  double lly = atof(s.toStdString().c_str());
  ss.remove(0, pos);
  pos = ss.indexOf("zdef ", Qt::CaseInsensitive);
  s = ss.left(pos);
  double resY = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  double w = (double)pi->m_ncols * resX;
  double h = (double)pi->m_nlines * resY;
  pi->m_imaRect = QRectF(llx, lly, w, h);

  return pi;
}

void te::qt::widgets::TimeSliderWidget::setEtaLUT(te::qt::widgets::PixmapItem* pi)
{
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
    size_t nchars = pi->m_ncols * 2;
    uchar* buf = new uchar[nchars];
    FILE* fp = fopen(file.toStdString().c_str(), "rb");
    ima = new QImage(pi->m_ncols, pi->m_nlines, QImage::Format_ARGB32);
    ima->fill(Qt::transparent);

    uchar uc[3];
    uc[2] = 0;
    for(size_t j = 0; j < pi->m_nlines; ++j)
    {
      uchar* u = ima->scanLine(j);
      fread(buf, nchars, sizeof(char), fp);
      for(size_t i = 0; i < pi->m_ncols; i++)
      {
        uc[0] = *(buf + (i<<1));
        uc[1] = *(buf + (1+(i<<1)));
        ushort b = *(ushort*)uc;
        if(b != pi->m_undef)
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
    QString auxFile(file);
    size_t pos = auxFile.indexOf(baseName);
    auxFile.remove(0, pos);
    pos = auxFile.indexOf("_");
    size_t pp = auxFile.indexOf(".bin");
    int offset = atoi(auxFile.mid(pos+1, pp-pos+1).toStdString().c_str());
    size_t fileSize = pi->m_nlines * pi->m_ncols * 4 + 8; // dado é float e desprepreza 4 bytes iniciais e 4 bytes finais
    offset *= fileSize;
    auxFile.remove(pos, auxFile.length()-pos);
    auxFile = path + auxFile + pi->m_suffix;

    size_t nchars = pi->m_ncols * 4;
    uchar* buf = new uchar[nchars];
    FILE* fp = fopen(auxFile.toStdString().c_str(), "rb");
    fseek(fp, offset, SEEK_SET);
    fseek(fp, 4, SEEK_CUR); // despreza 4 bytes da primeira linha
    ima = new QImage(pi->m_ncols, pi->m_nlines, QImage::Format_ARGB32);
    ima->fill(Qt::transparent);

    uchar uc[5];
    uc[4] = 0;
    for(size_t j = 0; j < pi->m_nlines; ++j)
    {
      uchar* u = ima->scanLine(pi->m_nlines-1-j); // origem bottom left
      fread(buf, nchars, sizeof(char), fp);

      for(size_t i = 0; i < pi->m_ncols; i++)
      {
        uc[0] = *(buf + (i<<2));
        uc[1] = *(buf + (1+(i<<2)));
        uc[2] = *(buf + (2+(i<<2)));
        uc[3] = *(buf + (3+(i<<2)));
        float b = *(float*)uc;
        if(b != pi->m_undef)
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

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getMetadata(const QString& path)
{
  te::qt::widgets::PixmapItem* pi;
  QDir dir(path);
  QString d = dir.dirName();
  QStringList nameFilter;
  nameFilter.append("*.ctl");
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  QString ctlFile = files.first();

  if(ctlFile == "racc.ctl")
    pi = getHidroMetadata(path);
  else if(ctlFile == "Prec5km.ctl")
    pi = getEtaMetadata(path);
  else
  {
    nameFilter.clear();
    nameFilter.append("S11*.jpg");
    QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
    if(files.empty() == false && files.first().length() == 26)
      pi = getGoesMetadata(path);
    else
      pi = getTemporalImageMetadata(path);
  }

  return pi;
}

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getHidroMetadata(const QString& path)
{
  PixmapItem* pi = getHidroCtlParameters(path);
  if(pi == 0)
    return 0;

  m_animationScene->addItem(pi);
  Animation* animation = new Animation(pi, "pos");
  animation->m_spatialExtent = te::gm::Envelope(pi->m_imaRect.x(), pi->m_imaRect.y(), pi->m_imaRect.right(), pi->m_imaRect.bottom());
  animation->setEasingCurve(QEasingCurve::Linear);
  m_parallelAnimation->addAnimation(animation);

  QStringList nameFilter;
  nameFilter.append("*.bin");
  QDir dir(path);
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  size_t count = files.count();

  // get time extent
  te::dt::TimeInstant ti = getHidroTime(path + "/" + files.first());
  te::dt::TimeInstant tf = getHidroTime(path + "/" + files.last());
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

  setHidroLUT(pi);
  return pi;
}

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getEtaMetadata(const QString& path)
{
  PixmapItem* pi = getEtaCtlParameters(path);
  if(pi == 0)
    return 0;

  m_animationScene->addItem(pi);
  Animation* animation = new Animation(pi, "pos");
  animation->m_spatialExtent = te::gm::Envelope(pi->m_imaRect.x(), pi->m_imaRect.y(), pi->m_imaRect.right(), pi->m_imaRect.bottom());
  animation->setEasingCurve(QEasingCurve::Linear);
  m_parallelAnimation->addAnimation(animation);

  QStringList nameFilter;
  nameFilter.append("*.bin");
  QDir dir(path);
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  size_t count = files.count();

  // get time extent
  te::dt::TimeInstant ti = getEtaTime(path + "/" + files.first());
  te::dt::TimeInstant tf = getEtaTime(path + "/" + files.last());
  // the file contains 3 days of data
  tf = te::dt::TimeInstant(tf.getTimeInstant() + boost::posix_time::seconds(60*60*24*3 - 3600));
  animation->m_temporalExtent = te::dt::TimePeriod(ti, tf);

  // set data
  te::dt::TimeInstant tnext, t;
  QString sn, fname;
  for(size_t i = 0; i < count; ++i)
  {
    QString f = files[i];
    QFileInfo fi(path + "/" + f);
    QString baseName = fi.baseName();
    t = getEtaTime(f);
 
    if(i != count-1)
      tnext = getEtaTime(files[i+1]);
    else
      tnext = te::dt::TimeInstant(t.getTimeInstant() + boost::posix_time::seconds(60*60*24*3));

    size_t n = 1;
    while((t < tnext || t == tnext) && n <= 72)
    {
      pi->m_time.push_back(t);
      t = te::dt::TimeInstant(t.getTimeInstant() + boost::posix_time::seconds(60*60));
      sn.setNum(n++);
      fname = path + "/" + baseName + "_" + sn + "." + fi.suffix();
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

  setEtaLUT(pi);
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

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getTemporalImageMetadata(const QString& path)
{
  PixmapItem* pi = getTemporalImageCtlParameters(path);
  if(pi == 0)
    return 0;

  m_animationScene->addItem(pi);
  Animation* animation = new Animation(pi, "pos");
  animation->m_spatialExtent = te::gm::Envelope(pi->m_imaRect.x(), pi->m_imaRect.y(), pi->m_imaRect.right(), pi->m_imaRect.bottom());
  animation->setEasingCurve(QEasingCurve::Linear);
  m_parallelAnimation->addAnimation(animation);

  QString suffix = "." + pi->m_suffix;
  QStringList nameFilter;
  nameFilter.append("*" + suffix);
  QDir dir(path);
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  size_t count = files.count();

  // get time extent
  te::dt::TimeInstant ti = getTemporalImageTime(path + "/" + files.first());
  te::dt::TimeInstant tf = getTemporalImageTime(path + "/" + files.last());
  animation->m_temporalExtent = te::dt::TimePeriod(ti, tf);

  // set data
  te::dt::TimeInstant t;
  for(size_t i = 0; i < count; ++i)
  {
    QString f = files[i];
    pi->m_files.push_back(f);
    t = getTemporalImageTime(f);
    pi->m_time.push_back(t);
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
  return pi;
}

te::qt::widgets::PixmapItem* te::qt::widgets::TimeSliderWidget::getTemporalImageCtlParameters(const QString& path)
{
  char buf[2000];
  QString file(path + "/image.ctl");
  FILE* fp = fopen(file.toStdString().c_str(), "r");
  if(fp == 0)
    return 0;

  fread(buf, 2000, sizeof(char), fp);
  fclose(fp);
  QString ss(QString(buf).simplified());

  // validation
  if(!(ss.contains("suffix ", Qt::CaseInsensitive) && ss.contains("undef", Qt::CaseInsensitive) && 
    ss.contains("srid", Qt::CaseInsensitive) && ss.contains("llx", Qt::CaseInsensitive) && 
    ss.contains("lly", Qt::CaseInsensitive) && ss.contains("urx", Qt::CaseInsensitive) && 
    ss.contains("ury", Qt::CaseInsensitive)))
    return 0;

  QString s;
  QDir dir(path);
  te::qt::widgets::PixmapItem* pi = new PixmapItem(dir.dirName(), path, m_display);

  // get suffix 
  size_t pos = ss.indexOf("suffix ", Qt::CaseInsensitive) + strlen("suffix "); 
  ss.remove(0, pos);
  pos = ss.indexOf(" undef", Qt::CaseInsensitive);
  s = ss.left(pos);
  pi->m_suffix = s;
  ss.remove(0, pos);

  // get undef
  pos = ss.indexOf("undef ", Qt::CaseInsensitive) + strlen("undef ");
  ss.remove(0, pos);
  pos = ss.indexOf(" srid", Qt::CaseInsensitive);
  s = ss.left(pos);
  pi->m_undef = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get srid
  pos = ss.indexOf("srid ", Qt::CaseInsensitive) + strlen("srid ");
  ss.remove(0, pos);
  pos = ss.indexOf(" llx", Qt::CaseInsensitive);
  s = ss.left(pos);
  pi->m_SRID = atoi(s.toStdString().c_str());
  ss.remove(0, pos);

  // get llx
  pos = ss.indexOf("llx ", Qt::CaseInsensitive) + strlen("llx ");
  ss.remove(0, pos);
  pos = ss.indexOf(" lly", Qt::CaseInsensitive);
  s = ss.left(pos);
  double llx = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get lly
  pos = ss.indexOf("lly ", Qt::CaseInsensitive) + strlen("lly ");
  ss.remove(0, pos);
  pos = ss.indexOf(" urx", Qt::CaseInsensitive);
  s = ss.left(pos);
  double lly = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get urx
  pos = ss.indexOf("urx ", Qt::CaseInsensitive) + strlen("urx ");
  ss.remove(0, pos);
  pos = ss.indexOf(" ury", Qt::CaseInsensitive);
  s = ss.left(pos);
  double urx = atof(s.toStdString().c_str());
  ss.remove(0, pos);

  // get ury
  pos = ss.indexOf("ury ", Qt::CaseInsensitive) + strlen("ury ");
  ss.remove(0, pos);
  double ury = atof(ss.toStdString().c_str());

  double w = urx - llx;
  double h = ury - lly;
  pi->m_imaRect = QRectF(llx, lly, w, h);

  return pi;
}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getTemporalImageTime(const QString& fileName)
{
  QString file(fileName);
  int ind = file.indexOf("_") + strlen("_");
  file.remove(0, ind);
  QString ano = file.mid(0, 4);
  QString mes = file.mid(5, 2);
  QString dia = file.mid(8, 2);
  QString hour = file.mid(11, 2);
  QString min = file.mid(14, 2);
  QString sec = file.mid(17, 2);

  te::dt::Date date(ano.toInt(), mes.toInt(), dia.toInt());
  te::dt::TimeDuration dur(hour.toInt(), min.toInt(), sec.toInt());
  return te::dt::TimeInstant(date, dur);
}

/*
========================================================================================================
                                     To be updated/removed
========================================================================================================
*/

/*

void te::qt::widgets::TimeSliderWidget::onAddEtaPushButtonClicked(bool)
{
  if(m_animationScene->items().isEmpty())
    m_ui->m_durationSpinBox->setValue(m_duration);

  int state = m_parallelAnimation->state();
  m_currentTime = m_parallelAnimation->currentTime();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();

  te::qt::widgets::PixmapItem* pi = getEtaMetadata("C:/lixo/FORECAST/eta5km");
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

  te::qt::widgets::PixmapItem* pi = getHidroMetadata("C:/lixo/FORECAST/hidro");
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

  m_ui->m_opacityComboBox->addItem("hidro");
  int count = m_ui->m_opacityComboBox->count();
  m_ui->m_opacityComboBox->setCurrentIndex(count-1);
  onOpacityComboBoxActivated(count-1);

}

void te::qt::widgets::TimeSliderWidget::onAddPushButtonClicked(bool b)
{
  QString title = QInputDialog::getText(this, "Add Item", "title");

  if(title.isEmpty())
    return;

  te::qt::widgets::ScopedCursor scopedCursor(Qt::WaitCursor);

  QString path;
  if(title == "eta5km"|| title == "hidro")
    path = "C:/lixo/FORECAST/" + title;
  else
    path = "C:/lixo/TemporalImages/" + title;

  addTemporalImages(path);
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
*/
