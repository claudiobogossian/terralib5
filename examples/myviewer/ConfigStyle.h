#ifndef __CONFIG_STYLE_H_
#define __CONFIG_STYLE_H_
// QT
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSplitter>
#include <QTextEdit>
#include <QTextDocument>

// TerraLib
#include <terralib/color.h>
#include <terralib/maptools.h>

class ConfigStyle : public QDialog
{
  Q_OBJECT

  public:
  ConfigStyle(QWidget* parent = 0);
  ~ConfigStyle();

  //void setPolygonColor(const te::color::RGBAColor& cor) {m_polygonColor = cor;}
  //void setPolygonContourColor(const te::color::RGBAColor& cor) {m_polygonContourColor = cor;}
  //void setPolygonContourWidth(const int& w) {m_polygonContourWidth = w;}

  //void setLineColor(const te::color::RGBAColor& cor) {m_lineColor = cor;}
  //void setLineWidth(const int& w) {m_lineWidth = w;}

  //void setPointColor(const te::color::RGBAColor& cor) {m_pointColor = cor;}
  //void setPointWidth(const int& w) {m_pointWidth = w;}
  //void setPointType(const te::map::Canvas::ptMarkerType& t) {m_pointType = t;}

  //void setFilterText(const std::string& f);
  void setLayer(te::map::AbstractLayer* layer);

protected:
  void paintEvent(QPaintEvent *event);

protected slots:

private:
  te::color::RGBAColor m_polygonColor;
  te::color::RGBAColor m_polygonContourColor;
  int m_polygonContourWidth;

  te::color::RGBAColor m_lineColor;
  int m_lineWidth;

  te::color::RGBAColor m_pointColor;
  int m_pointWidth;
  te::map::PtMarkerType m_pointType;

  QLabel* m_polygonWidget;
  QLabel* m_lineWidget;
  QLabel* m_pointWidget;

  QHBoxLayout* m_hBoxLayout;
  QVBoxLayout* m_vBoxLayout;
  QGroupBox* m_groupBox;

  QGroupBox* m_polygonGroupBox;
  QGroupBox* m_lineGroupBox;
  QGroupBox* m_pointGroupBox;
  QGroupBox* m_filterGroupBox;

  QHBoxLayout* m_polygonHBoxLayout;
  QHBoxLayout* m_lineHBoxLayout;
  QHBoxLayout* m_pointHBoxLayout;
  QHBoxLayout* m_filterHBoxLayout;

  QPushButton* m_polygonColorButton;
  QPushButton* m_polygonStyleButton;
  QPushButton* m_polygonContourColorButton;
  QPushButton* m_polygonContourStyleButton;
  QPushButton* m_polygonContourWidthButton;

  QPushButton* m_lineColorButton;
  QPushButton* m_lineStyleButton;
  QPushButton* m_lineWidthButton;

  QPushButton* m_pointColorButton;
  QPushButton* m_pointStyleButton;
  QPushButton* m_pointWidthButton;

  QPushButton* m_filterButton;
  QTextEdit* m_filterTextEdit;


  QSplitter* m_splitter;
};
#endif
