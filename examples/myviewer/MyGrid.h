#ifndef __MYGRID_H_
#define __MYGRID_H_

#include "MyDisplay.h"
#include <terralib/qt/widgets/datagrid/DataGridView.h>
#include <terralib/qt/qwt/HistogramDisplay.h>
#include <terralib/qt/qwt/ScatterDisplay.h>
#include <terralib/qt/qwt/TimeSeriesDisplay.h>

class MyGrid : public te::qt::widgets::DataGridView
{
  Q_OBJECT

  public:
  MyGrid(std::vector<te::map::MapDisplay*>*, QWidget* parent = 0);

  // o destrutor faz o disconnect com os MapDisplays e deleta todos os graficos relacionados a este grid
  ~MyGrid();

  public slots:
    void plotHistogram();
    void plotScatter();
    void plotTimeSeries();
    void selectionChangedSlot(te::map::DataGridOperation*);

  Q_SIGNALS:
    void selectionChanged(te::map::DataGridOperation*);

  protected slots:
    virtual void rowClicked(int clickedVisualRow);
    virtual void removeAllSelections();

  private:
    std::multimap<std::string, QwtPlot*> m_plotMMap;
    std::vector<te::map::MapDisplay*>* m_mapDisplayVec;
};

#endif
