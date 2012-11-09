#ifndef __MY_WINDOW_H
#define __MY_WINDOW_H

// QT
#include <QWidget>
#include <QModelIndex>
#include <QSplitter>
#include <QAction>
#include <QMenu>
#include <QGroupBox>
#include <QVBoxLayout>

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/postgis.h>
#include <terralib/qt/widgets.h>
#include <terralib/st.h>

class MyGrid;
class MyLayer;
class MyDisplay;
class ConfigStyle;
class QwtPlot;

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
  int getFreeMemory(); // retorna quanto tem de memoria

  Q_SIGNALS:
    void selectionChanged(te::map::DataGridOperation*);

protected slots:
  void layerVisibilityChanged(const QModelIndex&);
  void contextMenuActivated(const QModelIndex& mi, const QPoint& pos);
  void setStyleSlot();
  void openNewMapDisplaySlot();
  void openGridSlot();
  void renameSlot();
  void removeLayerSlot();
  void takeLayerSlot(te::qt::widgets::AbstractTreeItem* dragItem, te::qt::widgets::AbstractTreeItem* oldParent);
  void addFolderSlot();
  void addLayerSlot();
  void plotTemporalDistanceSlot();
  void removeDisplaySlot(MyDisplay*);
  void removeGridSlot(MyGrid*);
  void plotHistogramSlot(MyGrid*);
  void plotScatterSlot(MyGrid*);
  void plotTimeSeriesSlot(MyGrid*);
  void addTooltipSlot(MyGrid*);
  void clearTooltipSlot(MyGrid*);
  void removePlotSlot(QwtPlot*);
  void keepOnMemorySlot();
  void getAvailableMemorySlot();
  void selectionChangedSlot(te::map::DataGridOperation*);
  //void changeDefaultColorSlot();
  void changePointedColorSlot();
  void changeQueriedColorSlot();
  void changePointedAndQueriedColorSlot();
  void changePointStyleSlot();
  void changeLineStyleSlot();
  void changePolygonStyleSlot();
  void editLegendSlot();
  void removeLegendSlot();

protected:
  void AdjustmentsBeforeRemoveLayer(te::map::AbstractLayer*);
  void AdjustmentsAfterTakeLayer(te::map::AbstractLayer*, te::map::AbstractLayer*);
  void AdjustmentsAfterInsertLayer(te::map::AbstractLayer*);
  void reoderDrawing(te::map::AbstractLayer*);

private:
  void generatePNGs(std::vector<MyLayer*>&);

private:
  std::set<te::da::DataSource*> m_dataSourceSet;
  te::map::FolderLayer* m_rootFolderLayer;
  te::qt::widgets::LayerExplorerModel* m_layerExplorerModel;
  te::qt::widgets::LayerExplorer* m_layerExplorer;
  std::vector<MyDisplay*> m_mapDisplayVec;
  MyDisplay* m_display;
//  std::map<MyLayer*, MyGrid*> m_gridMap; // para que cada layer tenha apenas um unico grid correspondente
                                         // se o grid ja foi aberto nao cria outro DataSet - apenas da' show
  te::map::AbstractLayer* m_selectedLayer;
  QModelIndex m_parentModelIndex;
  QGroupBox* m_displayBox;
  QVBoxLayout* m_displayLayout;
  QAction* m_styleAction;
  QAction* m_openNewMapDisplayAction;
  QAction* m_openGridAction;
  QAction* m_renameAction;
  QAction* m_removeAction;
  QAction* m_addFolderAction;
  QAction* m_addLayerAction;
  QAction* m_plotTemporalDistanceAction;
  QAction* m_keepOnMemoryAction;
  QAction* m_getFreeMemoryAction;
  //QAction* m_changeDefaultColorAction;
  QAction* m_changePointedColorAction;
  QAction* m_changeQueriedColorAction;
  QAction* m_changePointedAndQueriedColorAction;
  QAction* m_changePointStyleAction;
  QAction* m_changeLineStyleAction;
  QAction* m_changePolygonStyleAction;
  QAction* m_editLegendAction;
  QAction* m_removeLegendAction;
  QAction* m_getAvailableMemoryAction;

  QMenu* m_treeMenu;
  QMenu* m_changeStatusColorMenu;
  QMenu* m_changeDefaultStyleMenu;
  ConfigStyle* m_configStyle;
  std::vector<te::st::MovingObject*> m_movObjOutput;
};
#endif
