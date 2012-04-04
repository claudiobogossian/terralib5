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
  void insertGridOperation(te::map::DataGridOperation*, QWidget*);
  void removeGridOperation(te::map::DataGridOperation*, QWidget*);
  void createGridOperation(MyLayer*);
  void insertPlot(std::string, QwtPlot*); //gridName or layerId
  void removePlot(std::string, QwtPlot*); //gridName or layerId
  QwtPlot* getPlot(std::string, std::string); //gridName or layerId, plot window title
  std::vector<QwtPlot*> getPlots(std::string); //gridName or layerId
  bool isDrawnOnTheDisplay(te::map::Layer*);

protected slots:
  void layerVisibilityChangedSlot(const QModelIndex&);
  void layerItemMovedSlot(const QModelIndex&, const QModelIndex&);
  void contextMenuPressedSlot(const QModelIndex& mi, const QPoint& pos);
  void timeSliderContextMenuSlot(const QPoint&);
  void setStyleSlot();
  void openNewMapDisplaySlot();
  void openGridSlot();
  void renameSlot();
  void removeSlot();
  void addFolderSlot();
  void addLayerSlot();
  void plotTemporalDistanceSlot();
  void autoDrawingSlot();
  void manualDrawingSlot();
  void configDrawingSlot();
  void timeSliderValueChangedSlot(int);
  void removeDisplaySlot(MyDisplay*);
  void removeGridSlot(MyGrid*);
  void removePlotSlot(QwtPlot*);

private:
  void generatePNGs(std::vector<MyLayer*>&);

private:
  te::da::DataSource* m_ds;
  te::map::FolderLayer* m_rootFolderLayer;
  te::qt::widgets::LayerExplorerModel* m_layerExplorerModel;
  te::qt::widgets::LayerExplorer* m_layerExplorer;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
  // O data grid operation custa caro porque precisa do DataSet e Transactor.
  // Entao, vamos usar somente quando estiver precisando. Ou seja, quando ele estiver
  // sendo usado pelo display (que tem forte ligacao com a arvore) ou pelo grid ou por algum plot.
  // Quando ele deixar de ser usado, vamos deletar o data grip operation (e tambem os DataSet e Transactor).
  std::map<te::map::DataGridOperation*, std::set<QWidget*> > m_operationMap;

  // Todos os plos, displays e grids tem que se conectarem para fazer a correta visualizacao.
  // Entao, os seguintes mapas e vetores serao criados:
  std::vector<te::map::MapDisplay*> m_mapDisplayVec;

  // Cada layer pode ter apenas um grid.
  // Se tentar abrir um novo grid e o layerr ja tem um grid, nao e' criado um novo,
  // apenas e' dado um show no grid ja existente.
  std::map<std::string, MyGrid*> m_gridMap; // primeiro parametro e' o gridName or layerId

  // Os plots sao abertos atraves do grid.
  // O grid e' deletado quando ele e' fechado, mas, os plots nao sao.
  // O mapa abaixo serve para que um novo grid aberto faca conexao com os plots ja abertos deste layer.
  std::map<std::string, std::vector<QwtPlot*> > m_plotMap; // primeiro parametro e' o gridName or layerId 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

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
  QMenu* m_treeMenu;
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
