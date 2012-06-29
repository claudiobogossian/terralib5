#ifndef __POLYGON_STYLE_H_
#define __POLYGON_STYLE_H_

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

// TerraLib
#include <terralib/maptools.h>

class PolygonStyle : public QDialog
{
  Q_OBJECT

public:
  PolygonStyle(te::map::DataGridOperation* op, QWidget* parent = 0);
  ~PolygonStyle();

  void closeEvent(QCloseEvent*);

protected:
  void paintEvent(QPaintEvent *event);
  void drawRenderArea();

protected slots:
  void onPolygonContourColorPushButtonClicked(bool); 
  void onPolygonContourIconPushButtonClicked(bool); 
  void onPolygonContourNoIconPushButtonClicked(bool); 
  void onPolygonContourWidthComboBoxActivated(int);

  void onPolygonFillColorPushButtonClicked(bool); 
  void onPolygonFillPatternIconPushButtonClicked(bool);
  void onPolygonFillPatternNoIconPushButtonClicked(bool);
  void onPolygonFillPatternWidthComboBoxActivated(int);

  void onOkPushButtonClicked(bool);
  void onCancelPushButtonClicked(bool);

public:
  QPushButton* m_polygonContourColorPushButton;
  te::color::RGBAColor m_polygonContourColor;
  QComboBox* m_polygonContourWidthComboBox;
  QString m_polygonContourIconFileName;
  QPushButton* m_polygonContourIconPushButton;
  QPushButton* m_polygonContourNoIconPushButton;
  char* m_contourPattern;
  unsigned int m_contourSize;
  te::map::ImageType m_contourImageType;

  QPushButton* m_polygonFillColorPushButton;
  te::color::RGBAColor m_polygonFillColor;
  QComboBox* m_polygonFillPatternWidthComboBox;
  QString m_polygonFillPatternIconFileName;
  QPushButton* m_polygonFillPatternIconPushButton;
  QPushButton* m_polygonFillPatternNoIconPushButton;
  char* m_fillPattern;
  unsigned int m_fillSize;
  te::map::ImageType m_fillImageType;

  QPushButton* m_okPushButton;
  QPushButton* m_cancelPushButton;

  RenderArea* m_polygonFillRenderArea;

  te::map::DataGridOperation* m_op;
};
#endif
