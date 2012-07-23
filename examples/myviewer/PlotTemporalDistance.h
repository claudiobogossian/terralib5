#ifndef __PLOT_TEMPORAL_DISTANCE_H_
#define __PLOT_TEMPORAL_DISTANCE_H_

//QT
#include <QDialog>
#include <QComboBox>
#include <QPushButton>

//TerraLib
#include <terralib/maptools.h>

class PlotTemporalDistance : public QDialog
{
  Q_OBJECT

public:
  PlotTemporalDistance(std::vector<te::map::AbstractLayer*>& layers, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~PlotTemporalDistance();

public slots:
  void okSlot();
  void cancelSlot();

public:
  QComboBox* m_layer1ComboBox;
  QComboBox* m_layer2ComboBox;
  QPushButton* m_okPushButton;
  QPushButton* m_cancelPushButton;
};

#endif
