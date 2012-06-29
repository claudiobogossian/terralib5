#ifndef __TEMPORAL_DRAWING_CONFIG_H_
#define __TEMPORAL_DRAWING_CONFIG_H_

//QT
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class TemporalDrawingConfig : public QDialog
{
  Q_OBJECT

public:
  TemporalDrawingConfig(QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~TemporalDrawingConfig();
  void populeWidgets();

public slots:
  void okSlot();
  void cancelSlot();

public:
  QComboBox* m_intervalDateComboBox;
  QComboBox* m_intervalDrawingComboBox;
  QCheckBox* m_drawLinesCheckBox;
  QCheckBox* m_loopCheckBox;
  QPushButton* m_okPushButton;
  QPushButton* m_cancelPushButton;
};

#endif
