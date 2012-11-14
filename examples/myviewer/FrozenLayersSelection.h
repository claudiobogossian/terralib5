#ifndef __FROZEN_LAYERS_SELECTION_H_
#define __FROZEN_LAYERS_SELECTION_H_

#include <QDialog>
#include <QTableWidget>

//terralib
#include<terralib/maptools.h>

class MyLayer;

class FrozenLayersSelection : public QDialog
{
  Q_OBJECT

public:
  FrozenLayersSelection(std::list<te::map::AbstractLayer*> list, std::set<MyLayer*> lset, QWidget* parent, Qt::WindowFlags f);

  ~FrozenLayersSelection();

public slots:
  void checkAllSlot();
  void uncheckAllSlot();
  void toggleSlot();
  void okSlot();
  void cancelSlot();

public:
  QTableWidget* m_table;
  std::set<MyLayer*> m_layerSet;
  std::list<te::map::AbstractLayer*> m_list;
};

#endif
