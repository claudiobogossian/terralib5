#include "LineStyle.h"

// TerraLib
#include <terralib/maptools.h>

//QT
#include <QColorDialog>

LineStyle::LineStyle(te::map::DataGridOperation* op, QWidget* parent) : QDialog(parent)
{
  QVBoxLayout* vbox = new QVBoxLayout(this);

  m_lineColor = op->getLineColor();
  QGroupBox* gb = new QGroupBox("Line Color:", this); 
  m_lineColorPushButton = new QPushButton("Color...", gb);
  QHBoxLayout* hbox = new QHBoxLayout(gb);
  hbox->addSpacing(150);
  hbox->addWidget(m_lineColorPushButton);
  hbox->addSpacing(150);

  QGroupBox* gb1 = new QGroupBox("Line Width:", this); 
  m_lineWidthComboBox = new QComboBox(gb1);
  for(int i = 0; i < 32; ++i)
  {
    QString s;
    s.setNum(i+1);
    m_lineWidthComboBox->addItem(s);
  }
  int width = op->getLineWidth();
  m_lineWidthComboBox->setCurrentIndex(width - 1);

  QHBoxLayout* hbox1 = new QHBoxLayout(gb1);
  hbox1->addWidget(m_lineWidthComboBox);

  QGroupBox* gb2 = new QGroupBox("Line Icon:", this); 
  m_lineIconLineEdit = new QLineEdit(gb2);
  QPushButton* iconPushButton = new QPushButton("Icon...", gb2);
  QHBoxLayout* hbox2 = new QHBoxLayout(gb2);
  hbox2->addWidget(m_lineIconLineEdit);
  hbox2->addWidget(iconPushButton);

  QGroupBox* gb3 = new QGroupBox(this); 
  m_applyPushButton = new QPushButton("Apply", gb3);
  m_cancelPushButton = new QPushButton("Cancel", gb3);
  QHBoxLayout* hbox3 = new QHBoxLayout(gb3);
  hbox3->addSpacing(100);
  hbox3->addWidget(m_applyPushButton);
  hbox3->addSpacing(50);
  hbox3->addWidget(m_cancelPushButton);

  vbox->addWidget(gb);
  vbox->addWidget(gb1);
  vbox->addWidget(gb2);
  vbox->addWidget(gb3);

  QObject::connect(m_lineColorPushButton, SIGNAL(clicked(bool)), this, SLOT(lineColorClickedSlot(bool))); 
  QObject::connect(iconPushButton, SIGNAL(clicked(bool)), this, SLOT(iconClickedSlot(bool))); 
  QObject::connect(m_applyPushButton, SIGNAL(clicked(bool)), this, SLOT(applyClickedSlot(bool))); 
  QObject::connect(m_cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(cancelClickedSlot(bool))); 

  setLayout(vbox);
}

LineStyle::~LineStyle()
{
}


void LineStyle::lineColorClickedSlot(bool)
{
  QColor color, oldColor(m_lineColor.getRed(), m_lineColor.getGreen(), m_lineColor.getBlue(), m_lineColor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Polygon Contour Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
    m_lineColor.setColor(color.red(), color.green(), color.blue(), color.alpha());
}

void LineStyle::paintEvent(QPaintEvent*)
{
}

void LineStyle::iconClickedSlot(bool)
{
  QString fileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "C:/lixo", tr("Image Files (*.png *.jpg *.bmp)"));

  if(fileName.isEmpty() == false)
    m_lineIconLineEdit->setText(fileName);
}

void LineStyle::applyClickedSlot(bool)
{
  accept();
  hide();
}

void LineStyle::cancelClickedSlot(bool)
{
  reject();
  hide();
}

void LineStyle::closeEvent(QCloseEvent *event)
{
  event->accept();
  cancelClickedSlot(true);
}
