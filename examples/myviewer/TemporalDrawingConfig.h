#ifndef __TEMPORAL_DRAWING_CONFIG_H_
#define __TEMPORAL_DRAWING_CONFIG_H_

#include <terralib/datatype.h>

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
  void setDefaultTimes(te::dt::DateTime* initial, te::dt::DateTime* final);

public slots:
  void defaultTimeSlot();
  void okSlot();
  void cancelSlot();

public:
  QComboBox* m_intervalDateComboBox;
  QComboBox* m_intervalDrawingComboBox;
  QCheckBox* m_drawLinesCheckBox;
  QCheckBox* m_loopCheckBox;
  QLineEdit* m_initialTimeLineEdit;
  QLineEdit* m_finalTimeLineEdit;
  te::dt::TimeInstant m_initialDefaultTime;
  te::dt::TimeInstant m_finalDefaultTime;

  QPushButton* m_defaultTimePushButton;
  QPushButton* m_okPushButton;
  QPushButton* m_cancelPushButton;
};

#endif
