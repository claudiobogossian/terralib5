#ifndef __TIME_SLIDER_H_
#define __TIME_SLIDER_H_

#include "TemporalDrawingConfig.h"

//TerraLib
#include <terralib/maptools.h>
#include <terralib/datatype.h>
#include <terralib/st/movingobject/MovingObject.h>

//QT
#include <QSlider>
#include <QMenu>

class MyDisplay;

class TimeSlider : public QSlider
{
  Q_OBJECT

public:
  TimeSlider(MyDisplay* md, QWidget* parent = 0);
  ~TimeSlider();

  //carrega dados vetoriais e inicializa os tempos inicial e final
  // addTime: if false: inicializa os tempos inicial e final
  // addTime: if true: adiciona aos tempos ja existentes (como faz o retangulo envolvente do box).
  bool loadMovingObjects(bool addTime = false);

  //calcula os tempos inicial e final
  // addTime: if false: inicializa os tempos inicial e final
  // addTime: if true: adiciona aos tempos ja existentes (como faz o retangulo envolvente do box).
  // este metodo pega todos os layers existentes debaixo da pasta TemporalImages e calcula o tempo inicial e final.
  // talves seja melhor varrer m_layers e recriar todas as imagens que estao visiveis e depois
  // setar os tempos inicial e final (mas, isto pode levar tempo demais e prejudicar a animacao)
  void calculateTemporalImageTimes(bool addTime = false);

  void addLayer(te::map::AbstractLayer*);
  void removeLayer(te::map::AbstractLayer*);
  void removeAllLayers();
  std::vector<te::map::AbstractLayer*> getLayers();
  virtual void timerEvent(QTimerEvent*);
  void draw(te::dt::TimeInstant* initial, te::dt::TimeInstant* final);
  void setInitialTime(te::dt::TimeInstant*);
  void setFinalTime(te::dt::TimeInstant*);
  void setInitialTime(QString t);
  void setFinalTime(QString t);
  QString simpleTimeString2IsoString(QString t);
  void setMinuteInterval(int);
  te::map::AbstractLayer* findImage(te::dt::TimeInstant* initial, te::dt::TimeInstant* final);
  void setLoop(bool);
  void setLines(bool);
  void clearLastPointMap();
  int getTimerId();
  void killTimer();
  void startTimer(int);
  void backToInit();
  void play();
  void pause();
  void stop();
  void clearDrawing();
  void configDrawing();
  te::dt::TimeInstant* getCurrentTime();

public slots:
  void valueChangedSlot(int);
  void playPauseSlot();
  void stopSlot();

private:
  MyDisplay* m_display;
  std::vector<te::map::AbstractLayer*> m_layers;
  te::dt::TimeInstant* m_initialTime;
  te::dt::TimeInstant* m_finalTime;
  te::dt::TimeInstant* m_currentTime;
  te::dt::DatePeriod* m_datePeriod;
  int m_minuteInterval;
  int m_timerId;
  bool m_loop;
  bool m_lines;
  std::vector<te::st::MovingObject*> m_mObs;
  std::map<std::string, te::gm::Point*> m_lastPointMap;
  TemporalDrawingConfig* m_temporalDrawingConfig;
  int m_dateInterval;
  int m_drawingInterval;
  bool m_play;
  bool m_stop;
  int m_value;
  QPixmap* m_playPixmap;
  QPixmap* m_pausePixmap;
};

#endif
