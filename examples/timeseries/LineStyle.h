#ifndef __LINE_STYLE_H_
#define __LINE_STYLE_H_
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

class LineStyle : public QDialog
{
  Q_OBJECT

public:
  LineStyle(te::map::DataGridOperation* op, QWidget* parent = 0);
  ~LineStyle();

  void closeEvent(QCloseEvent*);

protected:
  void paintEvent(QPaintEvent *event);

protected slots:
  void lineColorClickedSlot(bool);
  void iconClickedSlot(bool);
  void applyClickedSlot(bool);
  void cancelClickedSlot(bool);

public:
  QComboBox* m_lineWidthComboBox;
  QPushButton* m_lineColorPushButton;
  te::color::RGBAColor m_lineColor;
  QLineEdit* m_lineIconLineEdit;
  QPushButton* m_applyPushButton;
  QPushButton* m_cancelPushButton;
};
#endif
