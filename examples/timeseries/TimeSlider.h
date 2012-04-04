#ifndef __TIME_SLIDER_H_
#define __TIME_SLIDER_H_

//TerraLib
#include <terralib/datatype.h>
#include <terralib/maptools.h>
#include <terralib/st/movingobject/MovingObject.h>

//QT
#include <QSlider>

class TimeSlider : public QSlider
{
  Q_OBJECT

public:
  TimeSlider(QWidget* parent = 0);
  TimeSlider(Qt::Orientation orientation, QWidget * parent = 0);
  ~TimeSlider();

  //carrega dados vetoriais e inicializa os tempos inicial e final
  // addTime: if false: inicializa os tempos inicial e final
  // addTime: if true: adiciona aos tempos ja existentes (como faz o retangulo envolvente do box).
  void loadMovingObjects(bool addTime = false);

  //calcula os tempos inicial e final
  // addTime: if false: inicializa os tempos inicial e final
  // addTime: if true: adiciona aos tempos ja existentes (como faz o retangulo envolvente do box).
  // este metodo pega todos os layers existentes debaixo da pasta TemporalImages e calcula o tempo inicial e final.
  // talves seja melhor varrer m_layers e recriar todas as imagens que estao visiveis e depois
  // setar os tempos inicial e final (mas, isto pode levar tempo demais e prejudicar a animacao)
  void calculateTemporalImageTimes(bool addTime = false);

  void setMapDisplay(te::map::MapDisplay*);
  void addLayer(te::map::AbstractLayer*);
  void removeLayer(te::map::AbstractLayer*);
  void removeAllLayers();
  std::vector<te::map::AbstractLayer*> getLayers();
  virtual void timerEvent(QTimerEvent*);
  void draw(te::dt::DateTime* initial, te::dt::DateTime* final);
  void setInitialDateTime(te::dt::DateTime*);
  void setFinalDateTime(te::dt::DateTime*);
  void setMinuteInterval(int);
  te::map::AbstractLayer* findImage(te::dt::DateTime* initial, te::dt::DateTime* final);
  void setLoop(bool);
  void setLines(bool);
  void clearLastPointMap();
  int getTimerId();
  void killTimer();
  void backToInit();

  public slots:
  void valueChangedSlot(int);

private:
  te::map::MapDisplay* m_mapDisplay;
  std::vector<te::map::AbstractLayer*> m_layers;
  te::dt::DateTime* m_initialDateTime;
  te::dt::DateTime* m_finalDateTime;
  te::dt::DatePeriod* m_datePeriod;
  int m_minuteInterval;
  int m_timerId;
  bool m_loop;
  bool m_lines;
  std::vector<te::st::MovingObject*> m_mObs;
  std::map<std::string, te::gm::Point*> m_lastPointMap;
};

#endif
