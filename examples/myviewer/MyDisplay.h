#ifndef __MY_DISPLAY_H_
#define __MY_DISPLAY_H_

#include<terralib/qt/widgets.h>
#include<terralib/maptools/DataGridOperation.h>

#include<map>

class MyDisplay : public te::qt::widgets::MapDisplay
{
  Q_OBJECT

public:
  MyDisplay(int w, int h, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~MyDisplay();

  virtual void draw();
  virtual void draw(te::map::AbstractLayer* layerTree);
  virtual void setExtent(const te::gm::Envelope& e);
  virtual void setSRID(const int& srid);

public Q_SLOTS:
  void selectionChangedSlot(te::map::DataGridOperation*);
  void drawAllPointedsSlot();
  void drawAllQueriedsSlot();
  void drawAllPointedsAndQueriedsSlot();
  void mouseSelectionSlot(QRect);
  void mouseAddSelectionSlot(QRect);

Q_SIGNALS:
  void selectionChanged(te::map::DataGridOperation*);

private:
  bool m_useChanged;
  QRect m_rec;
};

#endif
