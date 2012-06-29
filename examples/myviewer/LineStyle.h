#ifndef __LINE_STYLE_H_
#define __LINE_STYLE_H_

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

class LineStyle : public QDialog
{
  Q_OBJECT

public:
  LineStyle(te::map::DataGridOperation* op, QWidget* parent = 0);
  ~LineStyle();

  void closeEvent(QCloseEvent*);

protected:
  void paintEvent(QPaintEvent *event);
  void drawRenderArea();

protected slots:
  void onWidthComboBoxActivated(int);
  void onColorPushButtonClicked(bool);
  void onIconPushButtonClicked(bool);
  void onNoIconPushButtonClicked(bool);
  void onOkPushButtonClicked(bool);
  void onCancelPushButtonClicked(bool);

public:
  QComboBox* m_widthComboBox;
  QPushButton* m_colorPushButton;
  QPushButton* m_iconPushButton;
  QPushButton* m_noIconPushButton;
  te::color::RGBAColor m_color;

  QString m_patternIconFileName;
  char* m_pattern;
  unsigned int m_patternSize;
  te::map::ImageType m_imageType;

  QPushButton* m_okPushButton;
  QPushButton* m_cancelPushButton;

  RenderArea* m_lineRenderArea;

  te::map::DataGridOperation* m_op;
};
#endif
