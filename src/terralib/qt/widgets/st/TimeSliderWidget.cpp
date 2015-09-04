/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
#include "../../../common/TreeItem.h"
#include "../../../st/maptools/TrajectoryDataSetLayer.h"
#include "../../../st/core/trajectory/TrajectoryDataSet.h"
#include "../../../qt/widgets/layer/explorer/LayerItem.h"
#include "../canvas/MapDisplay.h"
#include "../utils/ScopedCursor.h"
#include "AnimationView.h"
#include "Animation.h"
#include "AnimationScene.h"
#include "TrajectoryItem.h"
#include "Eta5kmItem.h"
#include "HidroItem.h"
#include "GoesItem.h"
#include "QtKnowItem.h"
// include here your new temporal image type.
#include "TimeSliderWidget.h"
#include "SliderPropertiesDialog.h"
#include "ui_TimeSliderWidgetForm.h"
#include "ui_SliderPropertiesDialogForm.h"

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
#include <QListWidget>
#include <QMap>
#include <QUrl>
#include <QFileDialog>

te::qt::widgets::TimeSliderWidget::TimeSliderWidget(te::qt::widgets::MapDisplay* md, QWidget* parent,  Qt::WindowFlags f)
  : QWidget(parent, f),
  m_duration(10000),
    m_display(md),
    m_erasePerfectly(false),
    m_ui(new Ui::TimeSliderWidgetForm),
    m_maxSliderValue(1000000000),
    m_finished(false),
    m_panFactor(.01)
{
  te::qt::widgets::ScopedCursor scopedCursor(Qt::WaitCursor);
  m_ui->setupUi(this);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  setAcceptDrops(true);

  //QColor wc(50, 50, 200, 80);
  //QBrush wbrush(wc);
  //QPalette wpalette(wbrush, wbrush, wbrush, wbrush, wbrush, wbrush, wbrush, wbrush, wbrush);
  //setPalette(wpalette);
  //setAutoFillBackground(true);
  //setBackgroundRole(QPalette::Window);

  m_spd = new SliderPropertiesDialog(this);

  // slider normalized between 0 and m_maxSliderValue
  m_ui->m_TemporalHorizontalSlider->setMinimum(0);
  m_ui->m_TemporalHorizontalSlider->setMaximum(m_maxSliderValue);

  m_ui->m_durationSpinBox->setMinimum(100);
  m_ui->m_durationSpinBox->setMaximum(100000000);
  m_ui->m_durationSpinBox->setValue(m_duration);
  m_ui->m_durationSpinBox->setSingleStep(10000);

  m_spd->m_ui->m_initialTimeLineEdit->setEnabled(false);
  m_spd->m_ui->m_finalTimeLineEdit->setEnabled(false);

  m_spd->m_ui->m_opacitySpinBox->setMinimum(0);
  m_spd->m_ui->m_opacitySpinBox->setMaximum(255);
  m_spd->m_ui->m_opacitySpinBox->setValue(255);
  m_spd->m_ui->m_opacitySpinBox->setSingleStep(5);

  m_spd->m_ui->m_panFactorDoubleSpinBox->setValue(m_panFactor);
  m_spd->m_ui->m_panFactorDoubleSpinBox->setSingleStep(.01);
  m_spd->m_ui->m_panFactorDoubleSpinBox->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);


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
  connect(m_ui->m_dateTimeEdit, SIGNAL(dateTimeChanged(const QDateTime&) ), this, SLOT(onDateTimeEditChanged(const QDateTime&)));
  connect(m_ui->m_durationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDurationValueChanged(int)));
  connect(m_ui->m_settingsToolButton, SIGNAL(clicked()), this, SLOT(onSettingsToolButtonnClicked()));
  connect(m_ui->m_playToolButton, SIGNAL(clicked()), this, SLOT(onPlayToolButtonnClicked()));
  connect(m_ui->m_stopToolButton, SIGNAL(clicked()), this, SLOT(onStopToolButtonnClicked()));
  connect(m_ui->m_TemporalHorizontalSlider, SIGNAL(sliderMoved (int)), this, SLOT(onSliderMoved(int)));

  setAttribute(Qt::WA_DeleteOnClose, true);

  m_loop = true; //By default, the animation not will be looping.
  m_duration = 10000; //By default, the animation journey time will be 10000 miliseconds.
  m_ui->m_durationSpinBox->setValue(m_duration);
  m_direction = QAbstractAnimation::Forward; //By default, the animation direction will be forward.
  setGoAndBack(true); //By default, the animation is forward and then backward.

  m_spd->m_ui->m_forwardRadioButton->setCheckable(true);

  m_animationView = new AnimationView(m_display);
  m_animationView->setWindowFlags(Qt::Widget);
  QBrush brush(Qt::transparent);
  QPalette palette(brush, brush, brush, brush, brush, brush, brush, brush, brush);
  m_animationView->setPalette(palette);
  m_parallelAnimation = new QParallelAnimationGroup();
  m_animationScene = new AnimationScene(m_display);
  m_animationView->setScene(m_animationScene);
  m_animationView->show();
  m_animationView->resize(4000, 2200);
  m_animationView->move(0, 0);

  raise();

  connect(m_display, SIGNAL(displaySridChanged()), this, SLOT(onSridChanged()));
  connect(m_display, SIGNAL(extentChanged()), this, SLOT(onExtentChanged()));
  connect(m_display, SIGNAL(displayPaintEvent(QPainter*)), this, SLOT(onDisplayPaintEvent(QPainter*)));
  connect(m_animationView, SIGNAL(animationDragEnterEvent(QDragEnterEvent*)), this, SLOT(onAnimationDragEnterEvent(QDragEnterEvent*)));
  connect(m_animationView, SIGNAL(animationDropEvent(QDropEvent*)), this, SLOT(onAnimationDropEvent(QDropEvent*)));

  connect(m_parallelAnimation, SIGNAL(finished()), this, SLOT(onFinishAnimation()));

  m_spd->m_ui->m_forwardColorPushButton->installEventFilter(this);
  m_spd->m_ui->m_backwardColorPushButton->installEventFilter(this);
  m_spd->m_ui->m_iconPushButton->installEventFilter(this);
  m_ui->m_dateTimeEdit->installEventFilter(this);

  m_ui->m_dateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");

  if(m_animationScene->items().isEmpty())
  {
    m_ui->m_durationSpinBox->setEnabled(false);
    m_ui->m_settingsToolButton->setEnabled(false);
    m_ui->m_playToolButton->setEnabled(false);
    m_ui->m_stopToolButton->setEnabled(false);
    m_ui->m_dateTimeEdit->setEnabled(false);
	  m_ui->m_TemporalHorizontalSlider->setEnabled(false);
	  m_ui->label->setEnabled(false);
	  m_ui->label_2->setEnabled(false);
    m_spd->hide();
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

	  m_ui->m_TemporalHorizontalSlider->setEnabled(true);
	  m_ui->label->setEnabled(true);
	  m_ui->label_2->setEnabled(true);
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
    te::st::TrajectoryDataSetLayer* tl = dynamic_cast<te::st::TrajectoryDataSetLayer*>(layer.get());
    QString title(tl->getTitle().c_str());
    QString layerId(tl->getId().c_str());
    QPair<QString, QString> p(title, layerId);
    if(trajectoryAlreadyExists(p))
      QMessageBox::information(this, title + " already exists", title + " is already being animated!");
    else
      addTrajectory(tl, "");
    if(isHidden())
      show();
  }
}

void te::qt::widgets::TimeSliderWidget::layerRemoved(te::map::AbstractLayerPtr layer)
{
  if(layer->getType() == "TRAJECTORYDATASETLAYER")
  {
    QString id(layer->getId().c_str());

    int ind = 0;
    QList<QString>::Iterator it;
    for (it = m_animationIdList.begin(); it != m_animationIdList.end(); ++it)
    {
      if (*it == id)
        break;
      ++ind;
    }
    if (it  != m_animationIdList.end())
      removeAnimation(ind);
  }
}

void te::qt::widgets::TimeSliderWidget::onDrawTrailCheckBoxClicked(bool b)
{
  te::qt::widgets::TrajectoryItem* ti = 0;
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false)
    {
      ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
      {
        ti->m_drawTrail = b;
        break;
      }
    }
  }
}

void te::qt::widgets::TimeSliderWidget::onRemovePushButtonClicked(bool)
{
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  if(ind != -1)
    removeAnimation(ind);
}

void te::qt::widgets::TimeSliderWidget::onRemoveAllPushButtonClicked(bool)
{
  while(true)
  {
    int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
    if(ind == -1)
      break;
    m_animationIdList.removeAt(ind);
    removeAnimation(ind);
  }
}

void te::qt::widgets::TimeSliderWidget::removeAnimation(const int& ind)
{
  QString aid = m_animationIdList.takeAt(ind);
  QString title = m_spd->m_ui->m_animationComboBox->itemText(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>((*it));
    te::qt::widgets::ImageItem* ii = 0;
    te::qt::widgets::TrajectoryItem* ti = 0;
    QString id;

    if (ai->pixmap().isNull()) // image
    {
      ii = dynamic_cast<te::qt::widgets::ImageItem*>(ai);
      id = ii->m_dir.absolutePath();
    }
    else  // trajectory
    {
      ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      id = ti->m_layerId;
    }

    if (id == aid)
    {
      if (list.count() == 1)
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
        m_ui->m_TemporalHorizontalSlider->setEnabled(false);
        m_ui->label->setEnabled(false);
        m_ui->label_2->setEnabled(false);
        m_display->update();
        m_spd->m_ui->m_animationComboBox->removeItem(ind);
        size_t size = m_spd->m_ui->m_animationComboBox->count();
        if (size)
          m_spd->m_ui->m_animationComboBox->setCurrentIndex(0);
        initProperty();
        return;
      }

      bool running = false;
      int state = m_parallelAnimation->state();
      if (state == QAbstractAnimation::Running)
      {
        running = true;;
        onPlayToolButtonnClicked();
      }

      m_currentTime = m_parallelAnimation->currentTime();
      m_animationScene->removeItem(ai);
      m_parallelAnimation->removeAnimation(ai->m_animation);
      if (ti)
      {
        if (m_animationScene->m_numberOfTrajectories)
        {
          m_animationScene->m_mutex.lock();
          m_animationScene->m_trajectoryPixmap->fill(Qt::transparent);
          m_animationScene->m_mutex.unlock();
          draw();
        }
      }
      delete ai->m_animation;
      delete ai;

      calculateAllSpatialExtent();
      calculateAllTemporalExtent();
      createAnimations();
      setDuration(m_duration);
      setDirection(m_direction);

      if (running)
      {
        onPlayToolButtonnClicked();
        m_parallelAnimation->setCurrentTime(m_currentTime);
      }

      break;
    }
  }
  m_spd->m_ui->m_animationComboBox->removeItem(ind);
  size_t size = m_spd->m_ui->m_animationComboBox->count();
  if (size)
    m_spd->m_ui->m_animationComboBox->setCurrentIndex(0);
  initProperty();
}

void te::qt::widgets::TimeSliderWidget::onAnimationDragEnterEvent(QDragEnterEvent* e)
{
  bool accept = false;
  const QMimeData* mdata = e->mimeData();
  QList<QUrl> urls = mdata->urls();
  if(urls.empty())
  {
    QByteArray ba = mdata->data("application/x-terralib;value=\"DraggedItems\"");
    if(ba.count() != 0)
    {
      QString s(ba);
      unsigned long v = s.toULongLong();
      std::vector<te::qt::widgets::TreeItem*>* draggedItems = reinterpret_cast<std::vector<te::qt::widgets::TreeItem*>*>(v);

      for (std::vector<te::qt::widgets::TreeItem*>::iterator it = draggedItems->begin(); it != draggedItems->end(); ++it)
      {
        if ((*it)->getType() == "TRAJECTORYDATASETLAYER")
          accept = true;
      }
    }
  }
  else
  {
    e->setDropAction(Qt::LinkAction);

    QString path = urls.begin()->path();
    size_t pos = path.indexOf("/");
    if(pos == 0)
      path.remove(0, 1);
    QDir dir(path);
    QStringList nameFilter;
    nameFilter.append("*.ctl");
    QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
    if(files.empty() == false)
    {
      QString file(path + "/" + files.first());
      FILE* fp = fopen(file.toStdString().c_str(), "r");
      char buf[2000];
      size_t c = fread(buf, sizeof(char), 2000, fp);
      fclose(fp);
      buf[c] = 0;
      QString s(buf);
      if(s.contains("undef", Qt::CaseInsensitive))
        accept = true;
    }
    else
    {
      nameFilter.clear();
      nameFilter.append("S1123*.jpg");
      QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
      if(files.empty() == false && files.first().length() == 26)
        accept = true;
      else
      {
        nameFilter.clear();
        nameFilter.append("S1118*.jpg");
        QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
        if(files.empty() == false && files.first().length() == 26)
          accept = true;
      }
    }
  }
  te::qt::widgets::AnimationView* a = (te::qt::widgets::AnimationView*)m_animationView;
  a->m_dragDropAccepted = accept;
  e->setAccepted(a->m_dragDropAccepted);
}

void te::qt::widgets::TimeSliderWidget::onAnimationDropEvent(QDropEvent* e)
{
  m_dropModifiers = e->keyboardModifiers();
  const QMimeData* mdata = e->mimeData();
  m_dropUrls = mdata->urls();
  m_dropBA = mdata->data("application/x-terralib;value=\"DraggedItems\"");
  QTimer::singleShot(10, this, SLOT(dropAction()));
}

void te::qt::widgets::TimeSliderWidget::dropAction()
{
  te::qt::widgets::ScopedCursor scopedCursor(Qt::WaitCursor);
  int state = m_parallelAnimation->state();
  int tempo = m_currentTime;
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked(); // put to paused state

  if(m_dropModifiers == Qt::NoModifier)
  {
    onStopToolButtonnClicked();
    m_spd->m_ui->m_animationComboBox->clear();
    m_animationIdList.clear();
    m_ui->m_TemporalHorizontalSlider->setValue(0);
    m_parallelAnimation->setCurrentTime(0);
    m_ui->m_settingsToolButton->setEnabled(false);
    m_ui->m_playToolButton->setEnabled(false);
    m_ui->m_stopToolButton->setEnabled(false);
    m_ui->m_durationSpinBox->setEnabled(false);
    m_ui->m_dateTimeEdit->setEnabled(false);
	  m_ui->m_TemporalHorizontalSlider->setEnabled(false);
	  m_ui->label->setEnabled(false);
	  m_ui->label_2->setEnabled(false);

    QList<QGraphicsItem*> list = m_animationScene->items();
    QList<QGraphicsItem*>::iterator it;
    for(it = list.begin(); it != list.end(); ++it)
    {
      te::qt::widgets::AnimationItem* ai = (AnimationItem*)(*it);
      m_animationScene->removeItem(ai);
      m_parallelAnimation->removeAnimation(ai->m_animation);
      delete ai->m_animation;
      delete ai;
    }
    
    m_spd->m_ui->m_animationComboBox->clear();
    m_display->update();
  }

  if(m_dropUrls.empty())
  {
    if(m_dropBA.count() != 0)
    {
      QString s(m_dropBA);
      unsigned long v = s.toULongLong();
      std::vector<te::qt::widgets::TreeItem*>* draggedItems = reinterpret_cast<std::vector<te::qt::widgets::TreeItem*>*>(v);

      for (std::vector<te::qt::widgets::TreeItem*>::iterator it = draggedItems->begin(); it != draggedItems->end(); ++it)
      {
        te::qt::widgets::LayerItem* lItem = (te::qt::widgets::LayerItem*)(*it);
        te::st::TrajectoryDataSetLayer* tl = dynamic_cast<te::st::TrajectoryDataSetLayer*>(lItem->getLayer().get());
        std::string ltype = tl->getType();
        
        if (ltype == "TRAJECTORYDATASETLAYER")
        {
          QString layerId(lItem->getLayer()->getId().c_str());
          QString title(lItem->getLayer()->getTitle().c_str());
          QPair<QString, QString> p(title, layerId);
          if (trajectoryAlreadyExists(p))
            QMessageBox::information(this, "animation already exists", title + " is already being animated!");
          else
            addTrajectory(tl, "");
        }
      }
    }
  }
  else
  {
    QList<QUrl>::iterator it;
    for(it = m_dropUrls.begin(); it != m_dropUrls.end(); ++it)
    {
      QString path = it->path();
      QDir dir(path);
      QString title = dir.dirName();
      path.remove(0, 1);

      QPair<QString, QString> p(title, path);

      if(coverageAlreadyExists(p))
        QMessageBox::information(this, "animation already exists", title + " is already being animated!");
      else
        addTemporalImages(path);
    }
  }

  if(state == QAbstractAnimation::Running || m_dropModifiers == Qt::NoModifier)
  {
    m_currentTime = tempo;
    onPlayToolButtonnClicked();
    m_parallelAnimation->setCurrentTime(m_currentTime);
  }
}

bool te::qt::widgets::TimeSliderWidget::trajectoryAlreadyExists(QPair<QString, QString>& item)
{
  AnimationItem* ai;
  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;
  for (it = list.begin(); it != list.end(); ++it)
  {
    ai = dynamic_cast<AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false && ai->m_title == item.first)
    {
      te::qt::widgets::TrajectoryItem* ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == item.second)
        return true;
    }
  }
  return false;
}

bool te::qt::widgets::TimeSliderWidget::coverageAlreadyExists(QPair<QString, QString>& item)
{
  AnimationItem* ai;
  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;
  for (it = list.begin(); it != list.end(); ++it)
  {
    ai = dynamic_cast<AnimationItem*>(*it);
    if (ai->pixmap().isNull() && ai->m_title == item.first)
    {
      te::qt::widgets::ImageItem* ii = dynamic_cast<te::qt::widgets::ImageItem*>(ai);
      if (ii->m_dir.absolutePath() == item.second)
        return true;
    }
  }
  return false;
}

void te::qt::widgets::TimeSliderWidget::addTrajectory(te::st::TrajectoryDataSetLayer* tl, const QString& pixmapFile)
{
  int state = m_parallelAnimation->state();

  QString title(tl->getTitle().c_str());
  QString layerId(tl->getId().c_str());
  TrajectoryItem* ti = new TrajectoryItem(title, layerId, m_display, QString(), QSize());
  m_animationScene->addItem(ti);
  Animation* animation = new Animation(ti, "pos");
  animation->setEasingCurve(QEasingCurve::Linear);
    
  te::st::TrajectoryDataSet* dset = tl->getTrajectoryDataset().release();
  animation->m_temporalExtent = *static_cast<te::dt::TimePeriod*>(dset->getTemporalExtent()->clone());

  ti->m_SRID = tl->getSRID();
    
  QVector<QPointF> points;
  te::dt::TimeInstant tiraRepetido;
  size_t i = 0;
  while(dset->moveNext())
  {
    std::auto_ptr<te::dt::DateTime> time = dset->getTime();
    te::dt::TimeInstant* tinstant = static_cast<te::dt::TimeInstant*>(time.get());
    if(tiraRepetido == *tinstant)
      continue;
    tiraRepetido = *tinstant;
    ti->m_time.push_back(te::dt::TimeInstant(tinstant->getTimeInstant()));

    std::auto_ptr<te::gm::Geometry> geom = dset->getGeometry();
    const te::gm::Point* p = static_cast<const te::gm::Point*>(geom.get());
    points.push_back(QPointF(p->getX(), p->getY()));
    ++i;
  }
  delete dset;

  if(i == 0)
    return;

  ti->m_route = new te::gm::LineString(i, te::gm::LineStringType, ti->m_SRID);

  QVector<QPointF>::iterator it;
  i = 0;
  for (it = points.begin(); it != points.end(); ++it)
  {
    te::gm::Point p((*it).x(), (*it).y());
    ti->m_route->setPointN(i++, p);
  }

  ti->m_route->computeMBR(true);
  te::gm::Envelope e(*ti->m_route->getMBR());
  animation->m_spatialExtent = e;

  m_parallelAnimation->addAnimation(animation);
  calculateAllSpatialExtent();
  calculateAllTemporalExtent();
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

	  m_ui->m_TemporalHorizontalSlider->setEnabled(true);
	  m_ui->label->setEnabled(true);
	  m_ui->label_2->setEnabled(true);
  }

  initProperty();

  m_spd->m_ui->m_animationComboBox->addItem(title);
  m_animationIdList.append(ti->m_layerId);
  int count = m_spd->m_ui->m_animationComboBox->count();
  m_spd->m_ui->m_animationComboBox->setCurrentIndex(count-1);
  onAnimationComboBoxActivated(count-1);
}

void te::qt::widgets::TimeSliderWidget::addTemporalImages(const QString& filePath)
{
  if(m_animationScene->items().isEmpty())
    m_ui->m_durationSpinBox->setValue(m_duration);

  int state = m_parallelAnimation->state();
  m_currentTime = m_parallelAnimation->currentTime();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();

  te::qt::widgets::ImageItem* pi = loadImageData(filePath);
  if(pi == 0)
  {
    QMessageBox::information(this, "Error", "Load error: " + filePath);
    return;
  }
  m_animationScene->addItem(pi);
  m_parallelAnimation->addAnimation(pi->m_animation);

  calculateAllSpatialExtent();
  calculateAllTemporalExtent();
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
	  m_ui->m_TemporalHorizontalSlider->setEnabled(true);
	  m_ui->label->setEnabled(true);
	  m_ui->label_2->setEnabled(true);
  }

  QDir dir(filePath);
  initProperty();

  m_spd->m_ui->m_animationComboBox->addItem(dir.dirName());
  m_animationIdList.append(pi->m_dir.absolutePath());
  int count = m_spd->m_ui->m_animationComboBox->count();
  m_spd->m_ui->m_animationComboBox->setCurrentIndex(count-1);
  onAnimationComboBoxActivated(count-1);
}

void te::qt::widgets::TimeSliderWidget::calculateAllSpatialExtent()
{
  QList<QGraphicsItem*> list = m_animationScene->items();
  if (list.empty())
    return;

  if (m_display->getExtent().isValid() == false)
    return;

  // each animation has its SRID and they may differ from each other
  // Then, using the map display projection to update scene rect
  m_spatialExtent = te::gm::Envelope();
  QList<QGraphicsItem*>::iterator it;
  AnimationItem* ai;
  for(it = list.begin(); it != list.end(); ++it)
  {
    ai = dynamic_cast<AnimationItem*>(*it);
    if(m_display->getSRID() != TE_UNKNOWN_SRS && m_display->getSRID() != ai->m_SRID)
    {
      te::gm::Envelope e = ai->m_animation->m_spatialExtent;
      e.transform(ai->m_SRID, m_display->getSRID());
      m_spatialExtent.Union(e);
    }
    else
      m_spatialExtent.Union(ai->m_animation->m_spatialExtent);
  }
  QRectF rect(m_spatialExtent.m_llx, m_spatialExtent.m_lly, m_spatialExtent.getWidth(), m_spatialExtent.getHeight());
  m_animationScene->setSceneRect(rect);
  m_animationView->setSceneRect(rect);
}

void te::qt::widgets::TimeSliderWidget::calculateAllTemporalExtent()
{
  te::dt::TimeInstant initial, final, t_initial, t_final;

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it = list.begin();
  if(it != list.end())
  {
    AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
    t_initial = ai->m_animation->m_temporalExtent.getInitialTimeInstant();
    t_final = ai->m_animation->m_temporalExtent.getFinalTimeInstant();
    ++it;
  }

  while(it != list.end())
  {
    AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
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
  if(list.count() == 1)
  {
    m_temporalAnimationExtent = tp;

    for(it = list.begin(); it != list.end(); ++it)
    {
      AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
      ai->m_animation->m_temporalAnimationExtent = m_temporalExtent;
    }
  }
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
    AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
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
    AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
    te::qt::widgets::Animation* a = new te::qt::widgets::Animation(ai, "pos");
    a->m_spatialExtent = envelopes[(int)i];
    a->m_temporalExtent = times[(int)i];
    a->m_temporalAnimationExtent = m_temporalAnimationExtent;
    a->adjustDataToAnimationTemporalExtent(m_temporalAnimationExtent);
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
  int ret = qRound(t * (double)m_duration);
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
  if(m_animationScene->items().isEmpty())
    return;
  if(m_parallelAnimation->state() == QAbstractAnimation::Stopped)
    return;

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

  if(m_animationScene->m_numberOfCoverages) // has images
  {
    // draw pixmap items
    for(it = list.begin(); it != list.end(); ++it)
    {
      AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
      if(ai->pixmap().isNull())
      {
        // draw only the items that are within the time frame 
        te::dt::TimeInstant tini = ai->m_animationTime.first();
        te::dt::TimeInstant tfim = ai->m_animationTime.last();
        if(t < tini || t > tfim)
          continue;

        // draw pixmap itens
        ImageItem* pi = dynamic_cast<ImageItem*>(ai);
        QRect r(pi->getRect());
        QRect dr(m_display->rect());
        if (dr.intersects(r))
        {
          pi->loadCurrentImage();
          pi->drawCurrentImage(painter);
        }
      }
    }
  }

  if(m_animationScene->m_numberOfTrajectories) // has trajectories
  {
    // is there any trajectory in the current time?
    for (it = list.begin(); it != list.end(); ++it)
    {
      AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
      if (ai->pixmap().isNull() == false)
      {
        // draw only the items that are within the time frame 
        TrajectoryItem* ti = dynamic_cast<TrajectoryItem*>(ai);
        te::dt::TimeInstant tini = ti->m_animationTime.first();
        te::dt::TimeInstant tfim = ti->m_animationTime.last();
        if (t > tini || t < tfim)
          break; // yes
      }
    }
    if (it != list.end())
    {
      // draw trail trajectories
      m_animationScene->m_mutex.lock();
      QPixmap* pixt = (dynamic_cast<AnimationScene*>(m_animationScene))->m_trajectoryPixmap;
      painter->drawPixmap(0, 0, *pixt);
      m_animationScene->m_mutex.unlock();

      // draw icon trajectories
      for(it = list.begin(); it != list.end(); ++it)
      {
        AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
        if(ai->pixmap().isNull() == false)
        {
          // draw only the items that are within the time frame 
          TrajectoryItem* ti = dynamic_cast<TrajectoryItem*>(ai);
          te::dt::TimeInstant tini = ti->m_animationTime.first();
          te::dt::TimeInstant tfim = ti->m_animationTime.last();
          if(t < tini || t > tfim)
            continue;

          ti->drawIcon(painter);
        }
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
    QPropertyAnimation* a = dynamic_cast<QPropertyAnimation*>(m_parallelAnimation->animationAt(i));
    AnimationItem* ai = dynamic_cast<AnimationItem*>(a->targetObject());
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
  m_animationScene->draw(m_currentTime);
}
//
//void te::qt::widgets::TimeSliderWidget::setAutomaticPan(const QString& title)
//{
//  m_animationScene->setAutomaticPan(title);
//}

void te::qt::widgets::TimeSliderWidget::onPanFactorValueChanged(double v)
{
  double ss = m_spd->m_ui->m_panFactorDoubleSpinBox->singleStep();
  if (ss == .1)
    v = (double)((int)(v * 10. + .5)) / 10.;
  else if (ss == .01)
    v = (double)((int)(v * 100. + .5)) / 100.;
  else
    v = (double)((int)(v * 1000. + .5)) / 1000.;

  if (v <= 0)
  {
    if (m_panFactor >= .1 && m_panFactor <= .5)
    {
      m_panFactor = .09;
      m_spd->m_ui->m_panFactorDoubleSpinBox->setSingleStep(.01);
    }
    else if (m_panFactor >= .01 && m_panFactor < .1)
    {
      m_panFactor = .009;
      m_spd->m_ui->m_panFactorDoubleSpinBox->setSingleStep(.001);
    }
    m_spd->m_ui->m_panFactorDoubleSpinBox->setValue(m_panFactor);
  }
  else
  {
    if (m_panFactor >= .1 && v <= .5)
      m_spd->m_ui->m_panFactorDoubleSpinBox->setSingleStep(.1);
    else if (v >= .01 && v < .1)
      m_spd->m_ui->m_panFactorDoubleSpinBox->setSingleStep(.01);
    else
      m_spd->m_ui->m_panFactorDoubleSpinBox->setSingleStep(.001);

    if (v < m_panFactor)
      m_panFactor -= ss;
    else if (v > m_panFactor)
      m_panFactor += ss;
    else
      return;

    if (ss == .1)
      m_panFactor = (double)((int)(m_panFactor * 10. + .5)) / 10.;
    else if (ss == .01)
      m_panFactor = (double)((int)(m_panFactor * 100. + .5)) / 100.;
    else
      m_panFactor = (double)((int)(m_panFactor * 1000. + .5)) / 1000.;

    m_spd->m_ui->m_panFactorDoubleSpinBox->setValue(m_panFactor);
  }

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;
  for(it = list.begin(); it != list.end(); ++it)
  {
    AnimationItem* ai = dynamic_cast<AnimationItem*>(*it);
    ai->m_panFactor = m_panFactor;
  }
}

bool te::qt::widgets::TimeSliderWidget::eventFilter(QObject* obj, QEvent* e)
{
  if(m_animationScene->items().isEmpty())
	return QObject::eventFilter(obj, e);

  te::qt::widgets::TrajectoryItem* ti = 0;
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false)
    {
      ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
        break;
    }
  }

  te::qt::widgets::ImageItem* ii = 0;
  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull())
    {
      ii = dynamic_cast<te::qt::widgets::ImageItem*>(ai);
      if (ii->m_dir.absolutePath() == id)
        break;
    }
  }

  if(obj == m_spd->m_ui->m_forwardColorPushButton)
  {
    if(e->type() == QEvent::Paint)
    {
      QPainter painter(m_spd->m_ui->m_forwardColorPushButton);
      QBrush brush(m_spd->m_ui->m_forwardColorPushButton->palette().color(QPalette::Window));
      painter.fillRect(m_spd->m_ui->m_forwardColorPushButton->rect(), brush);
      return true;
    }
    else if(e->type() == QEvent::MouseButtonPress)
    {
      QColor cor = QColorDialog::getColor(m_spd->m_ui->m_forwardColorPushButton->palette().color(QPalette::Window), this, "Forward Color", QColorDialog::ShowAlphaChannel);
      if(cor.isValid() == false)
        return true;
      else if(cor == m_spd->m_ui->m_forwardColorPushButton->palette().color(QPalette::Window))
        return true;

      m_spd->m_ui->m_forwardColorPushButton->setPalette(QPalette(cor));

      if(ti != 0)
        ti->m_forwardColor = cor;
      return true;
    }
  }
  else if(obj == m_spd->m_ui->m_backwardColorPushButton)
  {
    if(e->type() == QEvent::Paint)
    {
      QPainter painter(m_spd->m_ui->m_backwardColorPushButton);
      QBrush brush(m_spd->m_ui->m_backwardColorPushButton->palette().color(QPalette::Window));
      painter.fillRect(m_spd->m_ui->m_backwardColorPushButton->rect(), brush);
      return true;
    }
    else if(e->type() == QEvent::MouseButtonPress)
    {
      QColor cor = QColorDialog::getColor(m_spd->m_ui->m_backwardColorPushButton->palette().color(QPalette::Window), this, "Forward Color", QColorDialog::ShowAlphaChannel);
      if(cor.isValid() == false)
        return true;
      else if(cor == m_spd->m_ui->m_backwardColorPushButton->palette().color(QPalette::Window))
        return true;

      m_spd->m_ui->m_backwardColorPushButton->setPalette(QPalette(cor));

      if(ti != 0)
        ti->m_backwardColor = cor;
      return true;
    }
  }
  else if(obj == m_spd->m_ui->m_iconPushButton)
  {
    if(e->type() == QEvent::Paint)
    {
      QPainter painter(m_spd->m_ui->m_iconPushButton);
      painter.fillRect(m_spd->m_ui->m_iconPushButton->rect(), QBrush(Qt::white));
      QPixmap pix = ti->pixmap();
      QRect r = pix.rect();
      r.moveCenter(m_spd->m_ui->m_iconPushButton->rect().center());
      painter.drawPixmap(r, pix, pix.rect());
      painter.end();
      return true;
    }

    else if(ti != 0 && e->type() == QEvent::MouseButtonPress)
    {
      QString fileName = QFileDialog::getOpenFileName(this, tr("Select File"), "C:/", tr("Images (*.png *.xpm *.jpg *.gif)"));
      if(fileName.isNull())
        return true;

      QPixmap pix(fileName);
      if(pix.isNull())
      {
        QMessageBox::information(this, "Pixmap Error", "Invalid File");
        return true;
      }
      QPixmap pp(ti->m_iconSize);
      pp.fill(Qt::transparent);
      QPainter painter(&pp);
      painter.drawPixmap(pp.rect(), pix, pix.rect());
      painter.end();
      ti->m_iconFile = fileName;
      ti->setPixmap(pp);
      ti->m_iconSize = pp.size();
      m_spd->m_ui->m_iconPushButton->update();
      return true;
    }
  }
  else if(obj == m_ui->m_dateTimeEdit)
  {
    if(e->type() == QEvent::MouseButtonRelease)
    {
      QDateTime d = m_ui->m_dateTimeEdit->dateTime();
      te::dt::TimeInstant tmin = m_temporalAnimationExtent.getInitialTimeInstant();
      QDateTime minimum(QDate(tmin.getDate().getYear(), tmin.getDate().getMonth(), tmin.getDate().getDay()),
                        QTime(tmin.getTime().getHours(), tmin.getTime().getMinutes(), tmin.getTime().getSeconds()));

      te::dt::TimeInstant tmax = m_temporalAnimationExtent.getFinalTimeInstant();
      QDateTime maximun(QDate(tmax.getDate().getYear(), tmax.getDate().getMonth(), tmax.getDate().getDay()),
                        QTime(tmax.getTime().getHours(), tmax.getTime().getMinutes(), tmax.getTime().getSeconds()));
      if(d <= minimum)
        d = minimum;
      else if(d >= maximun)
        d = maximun;
      else if(d == m_oldQDateTime)
        d = fixDateTimeEdit(m_ui->m_dateTimeEdit, d);

      m_ui->m_dateTimeEdit->setDateTime(d);
      m_oldQDateTime = d;

      if(d == minimum)
        onFinishAnimation();
      return false;
    }
  }
  else if(obj == m_spd->m_ui->m_initialAnimationDateTimeEdit)
  {
    if(e->type() == QEvent::MouseButtonRelease)
    {
      QDateTime d = m_spd->m_ui->m_initialAnimationDateTimeEdit->dateTime();
      te::dt::TimeInstant tmin = m_temporalExtent.getInitialTimeInstant();
      QDateTime minimum(QDate(tmin.getDate().getYear(), tmin.getDate().getMonth(), tmin.getDate().getDay()),
                        QTime(tmin.getTime().getHours(), tmin.getTime().getMinutes(), tmin.getTime().getSeconds()));

      te::dt::TimeInstant tmax = m_temporalExtent.getFinalTimeInstant();
      QDateTime maximun(QDate(tmax.getDate().getYear(), tmax.getDate().getMonth(), tmax.getDate().getDay()),
                        QTime(tmax.getTime().getHours(), tmax.getTime().getMinutes(), tmax.getTime().getSeconds()));
      if(d <= minimum)
        d = minimum;
      else if(d >= maximun)
        d = maximun;
      else if(d == m_oldQDateTime)
        d = fixDateTimeEdit(m_spd->m_ui->m_initialAnimationDateTimeEdit, d);

      m_spd->m_ui->m_initialAnimationDateTimeEdit->setDateTime(d);
      m_oldQDateTime = d;
      return false;
    }
  }
  else if(obj == m_spd->m_ui->m_finalAnimationDateTimeEdit)
  {
    if(e->type() == QEvent::MouseButtonRelease)
    {
      QDateTime d = m_spd->m_ui->m_finalAnimationDateTimeEdit->dateTime();
      te::dt::TimeInstant tmin = m_temporalExtent.getInitialTimeInstant();
      QDateTime minimum(QDate(tmin.getDate().getYear(), tmin.getDate().getMonth(), tmin.getDate().getDay()),
                        QTime(tmin.getTime().getHours(), tmin.getTime().getMinutes(), tmin.getTime().getSeconds()));

      te::dt::TimeInstant tmax = m_temporalExtent.getFinalTimeInstant();
      QDateTime maximun(QDate(tmax.getDate().getYear(), tmax.getDate().getMonth(), tmax.getDate().getDay()),
                        QTime(tmax.getTime().getHours(), tmax.getTime().getMinutes(), tmax.getTime().getSeconds()));
      if(d <= minimum)
        d = minimum;
      else if(d >= maximun)
        d = maximun;
      else if(d == m_oldQDateTime)
        d = fixDateTimeEdit(m_spd->m_ui->m_finalAnimationDateTimeEdit, d);

      m_spd->m_ui->m_finalAnimationDateTimeEdit->setDateTime(d);
      m_oldQDateTime = d;
      return false;
    }
  }
  return QObject::eventFilter(obj, e);
}

void te::qt::widgets::TimeSliderWidget::onSettingsToolButtonnClicked()
{
  if(m_spd->isHidden())
    m_spd->show();
  else
    m_spd->hide();
}

void te::qt::widgets::TimeSliderWidget::onPlayToolButtonnClicked()
{
  if(m_animationScene->items().isEmpty())
    return;

  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
  {
    m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-start"));
    m_parallelAnimation->pause();
    m_ui->m_dateTimeEdit->setEnabled(true);
    m_paused = true;
  }
  else
  {
    m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-pause"));
    if(m_comingBack == false)
    {
      if(m_spd->m_ui->m_forwardRadioButton->isChecked())
        onForwardRadioButtonClicked(true);
      if(m_spd->m_ui->m_backwardRadioButton->isChecked())
        onBackwardRadioButtonClicked(true);
    }
    if(m_spd->m_ui->m_loopCheckBox->isChecked())
      onLoopCheckBoxClicked(true);
    if(m_spd->m_ui->m_goAndBackCheckBox->isChecked())
      onGoAndBackCheckBoxClicked(true);

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
    m_paused = false;
    play();
  }
}

void te::qt::widgets::TimeSliderWidget::onStopToolButtonnClicked()
{
  if(m_animationScene->items().isEmpty())
    return;

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
    draw();
    m_parallelAnimation->resume();
  }
  else
  {
    draw();
  }
}

void te::qt::widgets::TimeSliderWidget::onSridChanged()
{
  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
  {
    calculateAllSpatialExtent();
    createAnimations();
    draw();
    m_display->update();
    m_parallelAnimation->start();
  }
  else
  {
    calculateAllSpatialExtent();
    createAnimations();
    draw();
    m_display->update();
  }
}

void te::qt::widgets::TimeSliderWidget::play()
{
  if(m_animationScene->items().isEmpty())
    return;

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

  if(curTime >= m_parallelAnimation->duration())
    curTime = m_parallelAnimation->duration();

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

  if(m_currentTime == 0 || m_currentTime == m_parallelAnimation->duration())
    onFinishAnimation();

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
  if(m_paused)
  {
    if(m_currentTime >= m_duration && m_parallelAnimation->direction() == QAbstractAnimation::Forward)
    {
      m_currentTime = m_duration;
      changeDirection();
      m_comingBack = true;
      m_parallelAnimation->setCurrentTime(m_currentTime);
    }
    else if(m_currentTime <= 0 && m_parallelAnimation->direction() == QAbstractAnimation::Backward)
    {
      m_currentTime = 0;
      changeDirection();
      m_comingBack = false;
      m_parallelAnimation->setCurrentTime(m_currentTime);
    }
    play();
    m_parallelAnimation->pause();
    updateSliderPos();
    return;
  }

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
      m_currentTime = 0;
      m_parallelAnimation->setCurrentTime(m_currentTime);
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
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it++);
    m_animationScene->removeItem(ai);
  }

  while(list.isEmpty() == false)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(list.takeFirst());
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
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it++);
    m_animationScene->removeItem(ai);
  }

  while(list.isEmpty() == false)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(list.takeFirst());
    m_animationScene->addItem(ai);
  }

  onPlayToolButtonnClicked();
}

void te::qt::widgets::TimeSliderWidget::onFrontPushButtonClicked(bool b)
{
  te::qt::widgets::TrajectoryItem* ti = 0;
  te::qt::widgets::ImageItem* ii = 0;
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false)
    {
      ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
      {
        putToFront(ai);
        break;
      }
    }
    else
    {
      ii = dynamic_cast<te::qt::widgets::ImageItem*>(ai);
      if (ii->m_dir.absolutePath() == id)
      {
        putToFront(ai);
        break;
      }
    }
  }
}

void te::qt::widgets::TimeSliderWidget::onBackPushButtonClicked(bool b)
{
  te::qt::widgets::TrajectoryItem* ti = 0;
  te::qt::widgets::ImageItem* ii = 0;
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false)
    {
      ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
      {
        putToBack(ai);
        break;
      }
    }
    else
    {
      ii = dynamic_cast<te::qt::widgets::ImageItem*>(ai);
      if (ii->m_dir.absolutePath() == id)
      {
        putToBack(ai);
        break;
      }
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
      (dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai))->m_erasePerfectly = m_erasePerfectly;
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
  if (val == m_duration)
    return;

  m_currentTime = m_parallelAnimation->currentTime();
  double f = double(m_currentTime) / (double)m_duration;

  int ss = m_ui->m_durationSpinBox->singleStep();

  bool isRound = true;
  double d = (double)m_duration / (double)ss;
  double dd = (int)d;
  if (d != dd)
    isRound = false;

  // set new single step
  int ns = 10;
  while ((m_duration / ns) > 10)
    ns *= 10;

  int nns = 10;
  while ((val / nns) > 10)
    nns *= 10;
  if (nns > ns)
    ns = nns;

  if (ns < m_ui->m_durationSpinBox->minimum())
    ns = m_ui->m_durationSpinBox->minimum();
  m_ui->m_durationSpinBox->setSingleStep(ns);

  // calcute new duration
  if (val == 100 && ns != ss)
    m_duration -= ns;
  else if (val == (m_duration + ss) || val == (m_duration - ss)) // increment or decrement by mouse
  {
    if (isRound == false)
    {
      int nd = ((int)((double)m_duration / (double)ns)) * ns;
      if (val < m_duration)
        m_duration = nd;
      else
        m_duration = nd + ns;
    }
    else
    {
      if (val < m_duration)
        m_duration -= ns;
      else
        m_duration += ns;
    }
  }
  else // manual edition
  {
    m_duration = val;
    m_ui->m_durationSpinBox->setSingleStep(nns);
  }

  if (m_duration <= m_ui->m_durationSpinBox->minimum())
  {
    m_duration = m_ui->m_durationSpinBox->minimum();
    m_ui->m_durationSpinBox->setSingleStep(m_duration);
  }
  else if (m_duration >= m_ui->m_durationSpinBox->maximum())
  {
    m_duration = m_ui->m_durationSpinBox->maximum();
    m_ui->m_durationSpinBox->setSingleStep(m_duration/10);
  }
  m_ui->m_durationSpinBox->setValue(m_duration);

  bool running = false;
  if (m_parallelAnimation->state() == QAbstractAnimation::Running)
  {
    running = true;;
    onPlayToolButtonnClicked();
  }

  m_currentTime = qRound(f * (double)m_duration);
  m_animationScene->m_mutex.lock();
  m_animationScene->m_trajectoryPixmap->fill(Qt::transparent);
  m_animationScene->m_mutex.unlock();
  setDuration(m_duration);
  m_parallelAnimation->setCurrentTime(m_currentTime);
  draw();
  if (running)
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

  if(curTime > m_parallelAnimation->duration())
    curTime = m_parallelAnimation->duration();

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

void te::qt::widgets::TimeSliderWidget::onOpacityValueChanged(int v)
{
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  te::qt::widgets::AnimationItem* ai = 0;
  for (it = list.begin(); it != list.end(); ++it)
  {
    ai = dynamic_cast<AnimationItem*>(*it);
    if (ai->pixmap().isNull())
    {
      te::qt::widgets::ImageItem* ii = dynamic_cast<te::qt::widgets::ImageItem*>(ai);
      if (ii->m_dir.absolutePath() == id)
        break;
    }
    else
    {
      te::qt::widgets::TrajectoryItem* ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
        break;
    }
  }
  if (it != list.end())
  {
    ai->m_opacity = v;
    m_display->update();
  }
}

void te::qt::widgets::TimeSliderWidget::onTrajectoryPixmapSizeChanged(int)
{
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false)
    {
      te::qt::widgets::TrajectoryItem* ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
      {
        QPixmap pix = ai->pixmap();
        QPixmap pn(m_spd->m_ui->m_widthSpinBox->value(), m_spd->m_ui->m_heightSpinBox->value());
        pn.fill(Qt::transparent);
        if (pix.isNull() == false)
        {
          te::qt::widgets::TrajectoryItem* ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
          QString file = ti->m_iconFile;
          if (file.isEmpty() == false)
          {
            QFile f(file);
            if (f.exists())
            {
              QPixmap pf(file);
              QPainter painter(&pn);
              painter.drawPixmap(pn.rect(), pf, pf.rect());
              painter.end();
              ti->setPixmap(pn);
              ti->m_iconSize = pn.size();
              m_spd->m_ui->m_iconPushButton->update();
              m_display->update();
              break;
            }
          }

          QPainter painter(&pn);
          QBrush b(Qt::red);
          painter.setBrush(b);
          QPen p(Qt::red);
          painter.setPen(p);
          painter.drawEllipse(pn.rect());
          painter.end();
          m_spd->m_ui->m_iconPushButton->update();
          ti->setPixmap(pn);
          ti->m_iconSize = pn.size();
        }
        m_display->update();
        break;
      }
    }
  }
}

void te::qt::widgets::TimeSliderWidget::initProperty()
{
  te::dt::TimeInstant ti = m_temporalAnimationExtent.getInitialTimeInstant();
  QDate qdatei(ti.getDate().getYear(), ti.getDate().getMonth(), ti.getDate().getDay());
  QTime qtimei(ti.getTime().getHours(), ti.getTime().getMinutes(), ti.getTime().getSeconds());
  QDateTime qdatetimei(qdatei, qtimei);
  m_spd->m_ui->m_initialAnimationDateTimeEdit->setDateTime(qdatetimei);

  te::dt::TimeInstant tf = m_temporalAnimationExtent.getFinalTimeInstant();
  QDate qdatef(tf.getDate().getYear(), tf.getDate().getMonth(), tf.getDate().getDay());
  QTime qtimef(tf.getTime().getHours(), tf.getTime().getMinutes(), tf.getTime().getSeconds());
  QDateTime qdatetimef(qdatef, qtimef);
  m_spd->m_ui->m_finalAnimationDateTimeEdit->setDateTime(qdatetimef);

  if(m_direction == QAbstractAnimation::Forward)
  {
    if(m_spd->m_ui->m_forwardRadioButton->isChecked() == false)
      m_spd->m_ui->m_forwardRadioButton->toggle();
  }

  if(m_loop)
    m_spd->m_ui->m_loopCheckBox->setChecked(true);
  if(m_goAndBack)
    m_spd->m_ui->m_goAndBackCheckBox->setChecked(true);

  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  onAnimationComboBoxActivated(ind);
}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getInitialTime()
{
  QDateTime t = m_spd->m_ui->m_initialAnimationDateTimeEdit->dateTime();
  te::dt::Date date(t.date().year(), t.date().month(), t.date().day());
  te::dt::TimeDuration duration(t.time().hour(), t.time().minute(), t.time().second());
  te::dt::TimeInstant time(date, duration);
  return time;
}

te::dt::TimeInstant te::qt::widgets::TimeSliderWidget::getFinalTime()
{
  QDateTime t = m_spd->m_ui->m_finalAnimationDateTimeEdit->dateTime();
  te::dt::Date date(t.date().year(), t.date().month(), t.date().day());
  te::dt::TimeDuration duration(t.time().hour(), t.time().minute(), t.time().second());
  te::dt::TimeInstant time(date, duration);
  return time;
}

void te::qt::widgets::TimeSliderWidget::onResetInitialTimePushButtonClicked()
{
  te::dt::TimeInstant t = m_temporalExtent.getInitialTimeInstant();
  QDate qdate(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtime(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetime(qdate, qtime);
  m_spd->m_ui->m_initialAnimationDateTimeEdit->setDateTime(qdatetime);
}

void te::qt::widgets::TimeSliderWidget::onResetFinalTimePushButtonClicked()
{
  te::dt::TimeInstant t = m_temporalExtent.getFinalTimeInstant();
  QDate qdate(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtime(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetime(qdate, qtime);
  m_spd->m_ui->m_finalAnimationDateTimeEdit->setDateTime(qdatetime);
}

void te::qt::widgets::TimeSliderWidget::showPropertySection(bool b)
{
  if(b==false && m_spd->m_ui->m_propertyWidget->isHidden())
    return;
  if(b==true && m_spd->m_ui->m_propertyWidget->isHidden()==false)
    return;

  int h = m_spd->m_ui->m_propertyWidget->height();

  if(b == false)
  {
    m_spd->m_ui->m_propertyWidget->hide();
    resize(width(), height()-h);
  }
  else
  {
    m_spd->m_ui->m_propertyWidget->show();
    resize(width(), height()+h);
  }
}

bool te::qt::widgets::TimeSliderWidget::isSettingChanged()
{
  te::dt::TimeInstant ti = m_temporalAnimationExtent.getInitialTimeInstant();
  QDate qdatei(ti.getDate().getYear(), ti.getDate().getMonth(), ti.getDate().getDay());
  QTime qtimei(ti.getTime().getHours(), ti.getTime().getMinutes(), ti.getTime().getSeconds());
  QDateTime qdatetimei(qdatei, qtimei);
  QDateTime qdi = m_spd->m_ui->m_initialAnimationDateTimeEdit->dateTime();
  if(qdatetimei != qdi)
    return true;

  te::dt::TimeInstant tf = m_temporalAnimationExtent.getFinalTimeInstant();
  QDate qdatef(tf.getDate().getYear(), tf.getDate().getMonth(), tf.getDate().getDay());
  QTime qtimef(tf.getTime().getHours(), tf.getTime().getMinutes(), tf.getTime().getSeconds());
  QDateTime qdatetimef(qdatef, qtimef);
  QDateTime qdf = m_spd->m_ui->m_finalAnimationDateTimeEdit->dateTime();
  if(qdatetimef != qdf)
    return true;

  if(m_spd->m_ui->m_forwardRadioButton->isChecked() && m_direction != QAbstractAnimation::Forward)
    return true;
  if(m_spd->m_ui->m_forwardRadioButton->isChecked() == false && m_direction != QAbstractAnimation::Backward)
    return true;
  if(m_spd->m_ui->m_loopCheckBox->isChecked() != m_loop)
    return true;
  if(m_spd->m_ui->m_goAndBackCheckBox->isChecked() != m_goAndBack)
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

  te::dt::TimeInstant tinst = getTimeInstant();
  boost::posix_time::ptime btinst = tinst.getTimeInstant();

  te::dt::TimeInstant t = m_temporalAnimationExtent.getInitialTimeInstant();
  QDate qdatei(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtimei(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetimei(qdatei, qtimei);
  QDateTime qdi = m_spd->m_ui->m_initialAnimationDateTimeEdit->dateTime();
  boost::posix_time::ptime iTime = t.getTimeInstant();

  t = m_temporalAnimationExtent.getFinalTimeInstant();
  QDate qdatef(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtimef(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetimef(qdatef, qtimef);
  QDateTime qdf = m_spd->m_ui->m_finalAnimationDateTimeEdit->dateTime();
  boost::posix_time::ptime fTime = t.getTimeInstant();
  if(qdatetimei == qdi && qdatetimef == qdf)
    return;

  boost::posix_time::ptime niTime = ti.getTimeInstant();
  boost::posix_time::ptime nfTime = tf.getTimeInstant();
  boost::posix_time::time_duration ndiff = nfTime - niTime;
  double ntotalSeconds = ndiff.total_seconds();

  boost::posix_time::time_duration diff = fTime - iTime;
  double totalSeconds = diff.total_seconds();
  double rel = m_duration / totalSeconds;

  int nduration = qRound(rel * ntotalSeconds);

  m_currentTime = 0;
  if(btinst >= niTime && btinst <= nfTime)
  {
    boost::posix_time::time_duration d = btinst - niTime;
    double tot = d.total_seconds();
    m_currentTime = qRound(nduration * tot / ntotalSeconds);
  }
  else
    m_currentTime = nduration;

  int state = m_parallelAnimation->state();
  if(state == QAbstractAnimation::Running)
    onPlayToolButtonnClicked();

  QDateTime dt = m_ui->m_dateTimeEdit->dateTime();
  te::dt::TimePeriod tp = m_temporalAnimationExtent;

  m_temporalAnimationExtent = te::dt::TimePeriod(ti, tf);

  createNewPixmap();
  calculateAllSpatialExtent();
  createAnimations();
  m_duration = nduration;
  m_ui->m_durationSpinBox->setValue(m_duration);
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

  if (tp == m_temporalAnimationExtent)
    onDateTimeEditChanged(dt);
  else
  {
    // calculate single step on duration spin box widget.
    int i = 1;
    while ((m_duration / i) > 10)
      i *= 10;
    m_ui->m_durationSpinBox->setSingleStep(i);
  }
}

void te::qt::widgets::TimeSliderWidget::onAnimationComboBoxActivated(int ind)
{
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false)
    {
      te::qt::widgets::TrajectoryItem* ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
      {
        int op = ai->m_opacity;
        m_spd->m_ui->m_opacitySpinBox->setValue(op);
        te::dt::TimeInstant tini = ai->m_time.first();
        QString sini = getDateString(tini);
        te::dt::TimeInstant tfim = ai->m_time.last();
        QString sfim = getDateString(tfim);
        m_spd->m_ui->m_initialTimeLineEdit->setText(sini);
        m_spd->m_ui->m_finalTimeLineEdit->setText(sfim);

        adjustPropertyDialog(ai);
        break;
      }
    }
  }
}

QString te::qt::widgets::TimeSliderWidget::getDateString(const te::dt::TimeInstant& t)
{
  QDate qdate(t.getDate().getYear(), t.getDate().getMonth(), t.getDate().getDay());
  QTime qtime(t.getTime().getHours(), t.getTime().getMinutes(), t.getTime().getSeconds());
  QDateTime qdatetime(qdate, qtime);
  return qdatetime.toString("dd/MMM/yyyy hh:mm:ss");
}

void te::qt::widgets::TimeSliderWidget::adjustPropertyDialog(te::qt::widgets::AnimationItem* ai)
{
  QPixmap pix = ai->pixmap();
  if(pix.isNull()) // is temporal image
  {
    m_spd->m_ui->m_trajectoryGroupBox->setEnabled(false);
    m_spd->m_ui->m_forwardColorPushButton->setPalette(QPalette(Qt::gray));
    m_spd->m_ui->m_forwardColorPushButton->update();
    m_spd->m_ui->m_backwardColorPushButton->setPalette(QPalette(Qt::gray));
    m_spd->m_ui->m_backwardColorPushButton->update();
    m_spd->m_ui->m_autoPanCheckBox->setChecked(false);
    m_spd->m_ui->m_drawTrailCheckBox->setChecked(false);
  }
  else // is trajectory
  {
    m_spd->m_ui->m_trajectoryGroupBox->setEnabled(true);
    te::qt::widgets::TrajectoryItem* ti = (te::qt::widgets::TrajectoryItem*)(ai);
    m_spd->m_ui->m_forwardColorPushButton->setPalette(QPalette(ti->m_forwardColor));
    m_spd->m_ui->m_forwardColorPushButton->update();
    m_spd->m_ui->m_backwardColorPushButton->setPalette(QPalette(ti->m_backwardColor));
    m_spd->m_ui->m_backwardColorPushButton->update();
    m_spd->m_ui->m_autoPanCheckBox->setChecked(ti->m_automaticPan);
    ti->m_panFactor = m_panFactor;
    m_spd->m_ui->m_drawTrailCheckBox->setChecked(ti->m_drawTrail);

    m_spd->m_ui->m_iconRotateCheckBox->setChecked(ti->m_doIconRotate);
    QPixmap pix = ti->pixmap();
    m_spd->m_ui->m_widthSpinBox->setValue(pix.width());
    m_spd->m_ui->m_heightSpinBox->setValue(pix.height());
    m_spd->m_ui->m_iconPushButton->update();
  }
}

void te::qt::widgets::TimeSliderWidget::onIconRotateCheckBoxClicked(bool b)
{
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false)
    {
      te::qt::widgets::TrajectoryItem* ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
      {
        ti->m_doIconRotate = b;
        break;
      }
    }
  }
}

void te::qt::widgets::TimeSliderWidget::onAutoPanCheckBoxClicked(bool b)
{
  int ind = m_spd->m_ui->m_animationComboBox->currentIndex();
  QString id = m_animationIdList.value(ind);

  QList<QGraphicsItem*> list = m_animationScene->items();
  QList<QGraphicsItem*>::iterator it;

  for (it = list.begin(); it != list.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = dynamic_cast<te::qt::widgets::AnimationItem*>(*it);
    if (ai->pixmap().isNull() == false)
    {
      te::qt::widgets::TrajectoryItem* ti = dynamic_cast<te::qt::widgets::TrajectoryItem*>(ai);
      if (ti->m_layerId == id)
        ai->m_automaticPan = b;
      else
        ai->m_automaticPan = !b;
    }
  }
}

te::qt::widgets::ImageItem* te::qt::widgets::TimeSliderWidget::loadImageData(const QString& path)
{
  QDir dir(path);
  QString d = dir.dirName();

  te::qt::widgets::ImageItem* pi = 0;
  QString type(getTemporalImageType(path));

  if(type == "HIDRO_TYPE")
    pi = new te::qt::widgets::HidroItem(d, path, m_display);
  else if (type == "ETA5KM_TYPE")
    pi = new te::qt::widgets::Eta5kmItem(d, path, m_display);
  else if (type == "GOES_TYPE")
    pi = new te::qt::widgets::GoesItem(d, path, m_display);
  else if (type == "QTKNOW_TYPE")
    pi = new te::qt::widgets::QtKnowItem(d, path, m_display);
  // edit here to add a new temporal image type

  if (pi)
  {
    if (pi->loadData() == false)
    {
      delete pi;
      return 0;
    }
  }
  return pi;
}

QString te::qt::widgets::TimeSliderWidget::getTemporalImageType(const QString& path)
{
  QDir dir(path);
  QString d = dir.dirName();
  QStringList nameFilter;
  nameFilter.append("*.ctl");
  QStringList files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  QString ctlFile = files.first();

  if (ctlFile == "racc.ctl")
    return "HIDRO_TYPE";
  else if (ctlFile == "Prec5km.ctl")
    return "ETA5KM_TYPE";

  files.clear();
  nameFilter.clear();
  nameFilter.append("S11*.jpg");
  files = dir.entryList(nameFilter, QDir::Files, QDir::Name);
  if (files.empty() == false && files.first().length() == 26)
    return "GOES_TYPE";
  else
    return "QTKNOW_TYPE";

  // edit here to add a new temporal image.
  // you must find a way to discover its kind and return your type.

  return "";
}

void te::qt::widgets::TimeSliderWidget::onHelpPushButtonClicked()
{
}