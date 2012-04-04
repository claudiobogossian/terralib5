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
#include "ConfigStyle.h"

class MyWindow : public QWidget
{
  Q_OBJECT

  public:
  MyWindow(QWidget* parent = 0);
  ~MyWindow();

  void getLayers(te::map::AbstractLayer*, std::vector<te::map::AbstractLayer*>& layers);


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


private:
  te::da::DataSource* m_ds;
  te::map::FolderLayer* m_rootFolderLayer;
  te::qt::widgets::LayerExplorerModel* m_layerExplorerModel;
  te::qt::widgets::LayerExplorer* m_layerExplorer;
  std::vector<te::map::MapDisplay*> m_mapDisplayVec;
  std::map<std::string, MyGrid*> m_gridMap; // para que cada layer tenha apenas um unico grid correspondente
                                            // se o grid ja foi aberto nao cria outro DataSet - apenas da' show
  te::map::AbstractLayer* m_selectedLayer;
  QModelIndex m_parentModelIndex;
  QGroupBox* m_displayBox;
  QSplitter* m_splitter;
  QAction* m_styleAction;
  QAction* m_openNewMapDisplayAction;
  QAction* m_openGridAction;
  QAction* m_renameAction;
  QAction* m_removeAction;
  QAction* m_addFolderAction;
  QAction* m_addLayerAction;
  QMenu* m_treeMenu;
  ConfigStyle* m_configStyle;



  //HistogramDisplay *m_histogramDisplay;
};
#endif
