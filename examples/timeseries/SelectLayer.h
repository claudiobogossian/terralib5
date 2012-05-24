#ifndef __SELECT_LAYER_H_
#define __SELECT_LAYER_H_

//TerraLib
#include <terralib/dataaccess.h>

//QT
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

class SelectLayer : public QDialog
{
  Q_OBJECT

public:
  SelectLayer(te::da::DataSource* = 0, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~SelectLayer();
  void populeWidgets(te::da::DataSource*);

public slots:
  void dataSourceChangedSlot(int);
  void connectionStringEditedSlot();
  void okSlot();
  void cancelSlot();

public:
  QComboBox* m_dataSourceTypeComboBox;
  QLineEdit* m_connectionStringLineEdit;
  QComboBox* m_layerNameComboBox;
  QLineEdit* m_titleNameLineEdit;
  QPushButton* m_okPushButton;
  QPushButton* m_cancelPushButton;
};

#endif
