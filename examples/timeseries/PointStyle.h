#ifndef __POINT_STYLE_H_
#define __POINT_STYLE_H_
// QT
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QCloseEvent>

// TerraLib
#include <terralib/maptools.h>

class PointStyle : public QDialog
{
  Q_OBJECT

public:
  PointStyle(te::map::DataGridOperation* op, QWidget* parent = 0);
  ~PointStyle();

  void closeEvent(QCloseEvent*);

protected:
  void paintEvent(QPaintEvent *event);

protected slots:
  void iconClickedSlot(bool);
  void markClickedSlot(bool);
  void applyClickedSlot(bool);
  void cancelClickedSlot(bool);

public:
  QComboBox* m_pointWidthComboBox;
  QComboBox* m_pointMarkComboBox;
  QLineEdit* m_pointIconLineEdit;
  QPushButton* m_applyPushButton;
  QPushButton* m_cancelPushButton;
};
#endif
