#ifndef __POINT_STYLE_H_
#define __POINT_STYLE_H_

#include "RenderArea.h"

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
#include <QSpinBox>

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
  void drawRenderArea();

protected slots:
  void onWidthComboBoxActivated(int);
  //void onRotationSpinBoxValueChanged(int);
  void onIconPushButtonClicked(bool);
  void onNoIconPushButtonClicked(bool);
  void onColorPushButtonClicked(bool);
  void onOkPushButtonClicked(bool);
  void onCancelPushButtonClicked(bool);

public:
  QComboBox* m_widthComboBox;
  //QSpinBox* m_rotationSpinBox;
  QPushButton* m_iconPushButton;
  QPushButton* m_noIconPushButton;
  QPushButton* m_colorPushButton;
  te::color::RGBAColor m_color;

  QString m_patternIconFileName;
  char* m_pattern;
  unsigned int m_patternSize;
  te::map::ImageType m_imageType;

  QPushButton* m_okPushButton;
  QPushButton* m_cancelPushButton;

  RenderArea* m_pointRenderArea;

  te::map::DataGridOperation* m_op;
};
#endif
