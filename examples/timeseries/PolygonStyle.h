#ifndef __POLYGON_STYLE_H_
#define __POLYGON_STYLE_H_
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

protected slots:
  void contourColorClickedSlot(bool); 
  void contourIconClickedSlot(bool); 
  void contourMarkClickedSlot();

  void fillPatternIconClickedSlot(bool); 
  void fillMarkClickedSlot();
  void fillColorClickedSlot(bool); 
  void polygonFillMarkColorSlot(bool);

  void applyClickedSlot(bool);
  void cancelClickedSlot(bool);

public:
  QPushButton* m_polygonContourColorPushButton;
  te::color::RGBAColor m_polygonContourColor;
  QComboBox* m_polygonContourWidthComboBox;
  QComboBox* m_polygonContourMarkComboBox;
  QLineEdit* m_polygonContourIconLineEdit;
  QPushButton* m_polygonContourIconPushButton;

  QPushButton* m_polygonFillColorPushButton;
  te::color::RGBAColor m_polygonFillColor;
  QComboBox* m_polygonFillPatternWidthComboBox;
  QLineEdit* m_polygonFillPatternIconLineEdit;
  QPushButton* m_polygonFillPatternIconPushButton;
  te::color::RGBAColor m_polygonFillMarkColor;
  QComboBox* m_polygonFillMarkComboBox;
  QPushButton* m_polygonFillMarkColorPushButton;
  QPushButton* m_applyPushButton;
  QPushButton* m_cancelPushButton;
};
#endif
