#include "PolygonStyle.h"

//QT
#include <QColorDialog>

// TerraLib
#include <terralib/maptools.h>

PolygonStyle::PolygonStyle(te::map::DataGridOperation* op, QWidget* parent) : QDialog(parent)
{
  QVBoxLayout* vbox = new QVBoxLayout(this);

  // polygon contour
  QGroupBox* contourGroupBox = new QGroupBox("Polygon Contour:", this);
  QVBoxLayout* contourLayout = new QVBoxLayout(contourGroupBox);

  m_polygonContourColor = op->getPolygonContourColor();
  QGroupBox* gb = new QGroupBox("Contour Color:", contourGroupBox); 
  m_polygonContourColorPushButton = new QPushButton("Color...", gb);
  QHBoxLayout* hbox = new QHBoxLayout(gb);
  hbox->addSpacing(150);
  hbox->addWidget(m_polygonContourColorPushButton);
  hbox->addSpacing(150);

  QGroupBox* gb1 = new QGroupBox("Contour Width:", contourGroupBox); 
  m_polygonContourWidthComboBox = new QComboBox(gb1);
  for(int i = 0; i < 32; ++i)
  {
    QString s;
    s.setNum(i+1);
    m_polygonContourWidthComboBox->addItem(s);
  }
  int width = op->getPolygonContourWidth();
  m_polygonContourWidthComboBox->setCurrentIndex(width - 1);
  QHBoxLayout* hbox1 = new QHBoxLayout(gb1);
  hbox1->addWidget(m_polygonContourWidthComboBox);

  QGroupBox* gb2 = new QGroupBox("Contour Icon:", contourGroupBox); 
  m_polygonContourIconLineEdit = new QLineEdit(gb2);
  m_polygonContourIconPushButton = new QPushButton("Icon...", gb2);
  QHBoxLayout* hbox2 = new QHBoxLayout(gb2);
  hbox2->addWidget(m_polygonContourIconLineEdit);
  hbox2->addWidget(m_polygonContourIconPushButton);

  QGroupBox* gb21 = new QGroupBox("Contour Mark:", contourGroupBox); 
  m_polygonContourMarkComboBox = new QComboBox(gb21);
  m_polygonContourMarkComboBox->addItem("MarkerNone");
  m_polygonContourMarkComboBox->addItem("MarkerSquare");
  m_polygonContourMarkComboBox->addItem("MarkerDiamond");
  m_polygonContourMarkComboBox->addItem("MarkerCircle");
  m_polygonContourMarkComboBox->addItem("MarkerCrossedCircle");
  m_polygonContourMarkComboBox->addItem("MarkerSemiEllipseLeft");
  m_polygonContourMarkComboBox->addItem("MarkerSemiEllipseRight");
  m_polygonContourMarkComboBox->addItem("MarkerSemiEllipseUp");
  m_polygonContourMarkComboBox->addItem("MarkerSemiEllipseDown");
  m_polygonContourMarkComboBox->addItem("MarkerTriangleLeft");
  m_polygonContourMarkComboBox->addItem("MarkerTriangleRight");
  m_polygonContourMarkComboBox->addItem("MarkerTriangleUp");
  m_polygonContourMarkComboBox->addItem("MarkerTriangleDown");
  m_polygonContourMarkComboBox->addItem("MarkerFourRays");
  m_polygonContourMarkComboBox->addItem("MarkerCross");
  m_polygonContourMarkComboBox->addItem("MarkerX");
  m_polygonContourMarkComboBox->addItem("MarkerDash");
  m_polygonContourMarkComboBox->addItem("MarkerDot");
  m_polygonContourMarkComboBox->addItem("MarkerPixel");
  m_polygonContourMarkComboBox->addItem("MarkerPattern");

  int t = op->getPolygonContourPatternIconImageType();
  m_polygonContourMarkComboBox->setCurrentIndex(t);
  QHBoxLayout* hbox21 = new QHBoxLayout(gb21);
  hbox21->addWidget(m_polygonContourMarkComboBox);

  contourLayout->addWidget(gb);
  contourLayout->addWidget(gb1);
  contourLayout->addWidget(gb2);
  contourLayout->addWidget(gb21);

  //polygon fill
  QGroupBox* polygonGroupBox = new QGroupBox("Polygon Fill:", this);
  QVBoxLayout* polygonLayout = new QVBoxLayout(polygonGroupBox);

  m_polygonFillColor = op->getPolygonFillColor();
  QGroupBox* gb30 = new QGroupBox("Fill Color:", contourGroupBox); 
  m_polygonFillColorPushButton = new QPushButton("Color...", gb30);
  QHBoxLayout* hbox30 = new QHBoxLayout(gb30);
  hbox30->addSpacing(150);
  hbox30->addWidget(m_polygonFillColorPushButton);
  hbox30->addSpacing(150);

  QGroupBox* gb3 = new QGroupBox("Fill Pattern Width:", polygonGroupBox); 
  m_polygonFillPatternWidthComboBox = new QComboBox(gb3);
  for(int i = 0; i < 64; ++i)
  {
    QString s;
    s.setNum(i+1);
    m_polygonFillPatternWidthComboBox->addItem(s);
  }
  int pwidth = op->getPolygonPatternWidth();
  m_polygonFillPatternWidthComboBox->setCurrentIndex(pwidth - 1);
  QHBoxLayout* hbox3 = new QHBoxLayout(gb3);
  hbox3->addWidget(m_polygonFillPatternWidthComboBox);

  QGroupBox* gb4 = new QGroupBox("Fill Icon:", polygonGroupBox); 
  m_polygonFillPatternIconLineEdit = new QLineEdit(gb4);
  m_polygonFillPatternIconPushButton = new QPushButton("Icon...", gb4);
  QHBoxLayout* hbox4 = new QHBoxLayout(gb4);
  hbox4->addWidget(m_polygonFillPatternIconLineEdit);
  hbox4->addWidget(m_polygonFillPatternIconPushButton);

  QGroupBox* gb41 = new QGroupBox("Fill Mark:", polygonGroupBox); 
  m_polygonFillMarkComboBox = new QComboBox(gb41);
  m_polygonFillMarkComboBox->addItem("MarkerNone");
  m_polygonFillMarkComboBox->addItem("MarkerSquare");
  m_polygonFillMarkComboBox->addItem("MarkerDiamond");
  m_polygonFillMarkComboBox->addItem("MarkerCircle");
  m_polygonFillMarkComboBox->addItem("MarkerCrossedCircle");
  m_polygonFillMarkComboBox->addItem("MarkerSemiEllipseLeft");
  m_polygonFillMarkComboBox->addItem("MarkerSemiEllipseRight");
  m_polygonFillMarkComboBox->addItem("MarkerSemiEllipseUp");
  m_polygonFillMarkComboBox->addItem("MarkerSemiEllipseDown");
  m_polygonFillMarkComboBox->addItem("MarkerTriangleLeft");
  m_polygonFillMarkComboBox->addItem("MarkerTriangleRight");
  m_polygonFillMarkComboBox->addItem("MarkerTriangleUp");
  m_polygonFillMarkComboBox->addItem("MarkerTriangleDown");
  m_polygonFillMarkComboBox->addItem("MarkerFourRays");
  m_polygonFillMarkComboBox->addItem("MarkerCross");
  m_polygonFillMarkComboBox->addItem("MarkerX");
  m_polygonFillMarkComboBox->addItem("MarkerDash");
  m_polygonFillMarkComboBox->addItem("MarkerDot");
  m_polygonFillMarkComboBox->addItem("MarkerPixel");
  m_polygonFillMarkComboBox->addItem("MarkerPattern");

  t = op->getPolygonMarkerType();
  m_polygonFillMarkComboBox->setCurrentIndex(t);

  m_polygonFillMarkColor = op->getPolygonFillMarkerColor();
  m_polygonFillMarkColorPushButton = new QPushButton("Marker Color...", gb41);
  m_polygonFillMarkColorPushButton->setMaximumWidth(100);

  QHBoxLayout* hbox41 = new QHBoxLayout(gb41);
  hbox41->addWidget(m_polygonFillMarkComboBox);
  hbox41->addWidget(m_polygonFillMarkColorPushButton);

  polygonLayout->addWidget(gb30);
  polygonLayout->addWidget(gb3);
  polygonLayout->addWidget(gb4);
  polygonLayout->addWidget(gb41);

  QGroupBox* gb5 = new QGroupBox(this); 
  m_applyPushButton = new QPushButton("Apply", gb5);
  m_cancelPushButton = new QPushButton("Cancel", gb5);
  QHBoxLayout* hbox5 = new QHBoxLayout(gb5);
  hbox5->addSpacing(200);
  hbox5->addWidget(m_applyPushButton);
  hbox5->addSpacing(50);
  hbox5->addWidget(m_cancelPushButton);

  vbox->addWidget(contourGroupBox);
  vbox->addSpacing(40);
  vbox->addWidget(polygonGroupBox);
  vbox->addSpacing(40);
  vbox->addWidget(gb5);

  QObject::connect(m_polygonContourMarkComboBox, SIGNAL(clicked(bool)), this, SLOT(contourMarkClickedSlot(bool))); 
  QObject::connect(m_polygonFillMarkComboBox, SIGNAL(clicked(bool)), this, SLOT(fillMarkClickedSlot(bool))); 
  QObject::connect(m_polygonContourColorPushButton, SIGNAL(clicked(bool)), this, SLOT(contourColorClickedSlot(bool))); 
  QObject::connect(m_polygonContourIconPushButton, SIGNAL(clicked(bool)), this, SLOT(contourIconClickedSlot(bool))); 

  QObject::connect(m_polygonFillColorPushButton, SIGNAL(clicked(bool)), this, SLOT(fillColorClickedSlot(bool))); 
  QObject::connect(m_polygonFillPatternIconPushButton, SIGNAL(clicked(bool)), this, SLOT(fillPatternIconClickedSlot(bool))); 
  QObject::connect(m_polygonFillMarkColorPushButton, SIGNAL(clicked(bool)), this, SLOT(polygonFillMarkColorSlot(bool))); 

  QObject::connect(m_applyPushButton, SIGNAL(clicked(bool)), this, SLOT(applyClickedSlot(bool))); 
  QObject::connect(m_cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(cancelClickedSlot(bool))); 

  setLayout(vbox);
}

PolygonStyle::~PolygonStyle()
{
}

void PolygonStyle::paintEvent(QPaintEvent*)
{
}

void PolygonStyle::contourColorClickedSlot(bool)
{
  QColor color, oldColor(m_polygonContourColor.getRed(), m_polygonContourColor.getGreen(), m_polygonContourColor.getBlue(), m_polygonContourColor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Polygon Contour Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
    m_polygonContourColor.setColor(color.red(), color.green(), color.blue(), color.alpha());
}

void PolygonStyle::contourIconClickedSlot(bool)
{
  QString fileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "C:/lixo", tr("Image Files (*.png *.jpg *.bmp)"));

  if(fileName.isEmpty() == false)
  {
    m_polygonContourIconLineEdit->setText(fileName);
    m_polygonContourMarkComboBox->setCurrentIndex(19);
  }
}

void PolygonStyle::fillPatternIconClickedSlot(bool)
{
  QString fileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "C:/lixo", tr("Image Files (*.png *.jpg *.bmp)"));

  if(fileName.isEmpty() == false)
  {
    m_polygonFillPatternIconLineEdit->setText(fileName);
    m_polygonFillMarkComboBox->setCurrentIndex(19);
  }
}

void PolygonStyle::applyClickedSlot(bool)
{
  accept();
  hide();
}

void PolygonStyle::cancelClickedSlot(bool)
{
  reject();
  hide();
}

void PolygonStyle::closeEvent(QCloseEvent *event)
{
  event->accept();
  cancelClickedSlot(true);
}

void PolygonStyle::contourMarkClickedSlot()
{
  if(m_polygonContourMarkComboBox->currentIndex() != 19)
    m_polygonContourIconLineEdit->setText("");
}

void PolygonStyle::fillMarkClickedSlot()
{
  if(m_polygonFillMarkComboBox->currentIndex() != 19)
    m_polygonFillPatternIconLineEdit->setText("");
}

void PolygonStyle::fillColorClickedSlot(bool)
{
  QColor color, oldColor(m_polygonFillColor.getRed(), m_polygonFillColor.getGreen(), m_polygonFillColor.getBlue(), m_polygonFillColor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Polygon Fill Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
    m_polygonFillColor.setColor(color.red(), color.green(), color.blue(), color.alpha());
}

void PolygonStyle::polygonFillMarkColorSlot(bool)
{
  QColor color, oldColor(m_polygonFillMarkColor.getRed(), m_polygonFillMarkColor.getGreen(), m_polygonFillMarkColor.getBlue(), m_polygonFillMarkColor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Polygon Fill Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
    m_polygonFillMarkColor.setColor(color.red(), color.green(), color.blue(), color.alpha());
}