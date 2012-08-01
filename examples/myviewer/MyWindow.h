#ifndef __MY_WINDOW_H
#define __MY_WINDOW_H

// QT
#include <QWidget>
#include <QModelIndex>
#include <QSplitter>
#include <QAction>
#include <QMenu>
#include <QGroupBox>

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/postgis.h>
#include <terralib/qt/widgets.h>
#include <terralib/st.h>

#include "MyGrid.h"
#include "MyLayer.h"
#include "ConfigStyle.h"
#include "TemporalDrawingConfig.h"
#include "TimeSlider.h"

class MyWindow : public QWidget
{
  Q_OBJECT

  public:
  MyWindow(QWidget* parent = 0);
  ~MyWindow();

  void getLayers(te::map::AbstractLayer*, std::vector<te::map::AbstractLayer*>& layers);
  void closeEvent(QCloseEvent*);
  void deleteGridOperation(te::map::AbstractLayer*);
  bool isUsed(te::map::AbstractLayer*);
  void updateDisplays(MyLayer*);

  Q_SIGNALS:
    void selectionChanged(te::map::DataGridOperation*);

protected slots:
  void layerVisibilityChanged(const QModelIndex&);
  void layerItemMoved(const QModelIndex&, const QModelIndex&);
  void contextMenuPressed(const QModelIndex& mi, const QPoint& pos);
  void setStyleSlot();
  void openNewMapDisplaySlot();
  void openGridSlot();
  void renameSlot();
  void removeSlot();
  void addFolderSlot();
  void addLayerSlot();
  void plotTemporalDistanceSlot();
  void timeSliderContextMenu(const QPoint&);
  void autoDrawingSlot();
  void manualDrawingSlot();
  void configDrawingSlot();
  void timeSliderValueChangedSlot(int);
  void removeDisplaySlot(MyDisplay*);
  void removeGridSlot(MyGrid*);
  void plotHistogramSlot(MyGrid*);
  void plotScatterSlot(MyGrid*);
  void plotTimeSeriesSlot(MyGrid*);
  void addTooltipSlot(MyGrid*);
  void clearTooltipSlot(MyGrid*);
  void removePlotSlot(QwtPlot*);
  void keepOnMemorySlot();
  void selectionChangedSlot(te::map::DataGridOperation*);
  //void changeDefaultColorSlot();
  void changePointedColorSlot();
  void changeQueriedColorSlot();
  void changePointedAndQueriedColorSlot();
  void changePointStyleSlot();
  void changeLineStyleSlot();
  void changePolygonStyleSlot();

private:
  void generatePNGs(std::vector<MyLayer*>&);

private:
  te::map::FolderLayer* m_rootFolderLayer;
  te::qt::widgets::LayerExplorerModel* m_layerExplorerModel;
  te::qt::widgets::LayerExplorer* m_layerExplorer;
  std::vector<te::map::MapDisplay*> m_mapDisplayVec;
//  std::map<MyLayer*, MyGrid*> m_gridMap; // para que cada layer tenha apenas um unico grid correspondente
                                         // se o grid ja foi aberto nao cria outro DataSet - apenas da' show
  te::map::AbstractLayer* m_selectedLayer;
  QModelIndex m_parentModelIndex;
  QGroupBox* m_displayBox;
  QSplitter* m_splitter;
  TimeSlider* m_timeSlider;
  QAction* m_styleAction;
  QAction* m_openNewMapDisplayAction;
  QAction* m_openGridAction;
  QAction* m_renameAction;
  QAction* m_removeAction;
  QAction* m_addFolderAction;
  QAction* m_addLayerAction;
  QAction* m_plotTemporalDistanceAction;
  QAction* m_keepOnMemoryAction;
  //QAction* m_changeDefaultColorAction;
  QAction* m_changePointedColorAction;
  QAction* m_changeQueriedColorAction;
  QAction* m_changePointedAndQueriedColorAction;
  QAction* m_changePointStyleAction;
  QAction* m_changeLineStyleAction;
  QAction* m_changePolygonStyleAction;

  QMenu* m_treeMenu;
  QMenu* m_changeStatusColorMenu;
  QMenu* m_changeDefaultStyleMenu;
  QMenu* m_timeSliderMenu;
  ConfigStyle* m_configStyle;
  TemporalDrawingConfig* m_temporalDrawingConfig;
  std::vector<te::st::MovingObject*> m_movObjOutput;

  int m_dateInterval;
  int m_minuteInterval;
  int m_temporalDrawingInterval;
  bool m_temporalDrawLines;
  bool m_temporalLoop;


  //HistogramDisplay *m_histogramDisplay;
};
#endif