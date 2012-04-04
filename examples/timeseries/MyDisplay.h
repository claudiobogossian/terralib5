#ifndef __MY_DISPLAY_H_
#define __MY_DISPLAY_H_

#include "TimeSlider.h"
#include<terralib/qt/widgets.h>
#include<terralib/maptools/DataGridOperation.h>

#include<map>

class MyDisplay : public te::qt::widgets::MapDisplay
{
  Q_OBJECT

public:
  MyDisplay(int w, int h, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~MyDisplay();

  void paintEvent(QPaintEvent* e);
  void closeEvent(QCloseEvent *event);
  virtual void draw();
  virtual void draw(te::map::AbstractLayer* layerTree);
  virtual void setExtent(const te::gm::Envelope& e);
  virtual void setSRID(const int& srid);
  void drawTemporalData(te::map::AbstractLayer* layer, std::vector<te::gm::Geometry*>& geoms, bool drawLines = false);
  void clearTemporalPixmaps(std::vector<te::map::AbstractLayer*>);
  void clearTemporalCanvas(te::map::AbstractLayer*);
  void setTimeSlider(TimeSlider*);
  void deleteCanvas(te::map::AbstractLayer*);

public Q_SLOTS:
  void selectionChangedSlot(te::map::DataGridOperation*);
  void drawAllPointedsSlot();
  void drawAllQueriedsSlot();
  void drawAllPointedsAndQueriedsSlot();
  void mouseSelectionSlot(QRect);
  void mouseAddSelectionSlot(QRect);

Q_SIGNALS:
  void selectionChanged(te::map::DataGridOperation*);
  void closed(MyDisplay*);

protected:
  virtual te::qt::widgets::Canvas* getCanvas(te::map::AbstractLayer* layer);
  virtual void setCanvas(te::map::AbstractLayer* layer);

private:
  bool m_useChanged;
  QRect m_rec;
  QPixmap* m_temporalVectorialDisplayPixmap;     //!< This pixmap will be the result of all temporal drawing, i. e., the result of drawing all visible vectorial layers.
  QPixmap* m_temporalImageDisplayPixmap;         //!< This pixmap will be the result of all temporal drawing, i. e., the result of drawing all visible image layers.
  TimeSlider* m_timeSlider;
};

#endif
