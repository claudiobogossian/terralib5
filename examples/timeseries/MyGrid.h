#ifndef __MYGRID_H_
#define __MYGRID_H_

#include "MyDisplay.h"
#include <terralib/qt/widgets/datagrid/DataGridView.h>
#include <terralib/qt/qwt/HistogramDisplay.h>
#include <terralib/qt/qwt/ScatterDisplay.h>
#include <terralib/qt/qwt/TimeSeriesDisplay.h>

class MyLayer;

class MyGrid : public te::qt::widgets::DataGridView
{
  Q_OBJECT

  public:
  MyGrid(MyLayer*, QWidget* parent = 0);

  // o destrutor faz o disconnect com os MapDisplays e deleta todos os graficos relacionados a este grid
  ~MyGrid();
  void closeEvent(QCloseEvent*);

  public slots:
    void plotHistogram();
    void plotScatter();
    void plotTimeSeries();
    void tooltip();
    void selectionChangedSlot(te::map::DataGridOperation*);
    MyLayer* getLayer();

  Q_SIGNALS:
    void selectionChanged(te::map::DataGridOperation*);
    void closed(MyGrid*);
    void plotHistogram(MyGrid*);
    void plotScatter(MyGrid*);
    void plotTimeSeries(MyGrid*);
    void tooltip(MyGrid*);

  protected slots:
    virtual void rowClicked(int clickedVisualRow);
    virtual void removeAllSelections();

  private:
    MyLayer* m_layer;
    bool m_localSelection;
};

#endif
