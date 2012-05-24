#include "PointStyle.h"

// TerraLib
#include <terralib/maptools.h>

PointStyle::PointStyle(te::map::DataGridOperation* op, QWidget* parent) : QDialog(parent)
{
  setWindowTitle("Config Point Style");
  QVBoxLayout* vbox = new QVBoxLayout(this);
  QGroupBox* gb1 = new QGroupBox("Point Width:", this); 
  m_pointWidthComboBox = new QComboBox(gb1);
  for(int i = 0; i < 128; ++i)
  {
    QString s;
    s.setNum(i+1);
    m_pointWidthComboBox->addItem(s);
  }
  int width = op->getPointWidth();
  m_pointWidthComboBox->setCurrentIndex(width - 1);

  QHBoxLayout* hbox1 = new QHBoxLayout(gb1);
  hbox1->addWidget(m_pointWidthComboBox);

  QGroupBox* gb2 = new QGroupBox("Point Icon:", this); 
  m_pointIconLineEdit = new QLineEdit(gb2);
  QPushButton* iconPushButton = new QPushButton("Icon...", gb2);
  QHBoxLayout* hbox2 = new QHBoxLayout(gb2);
  hbox2->addWidget(m_pointIconLineEdit);
  hbox2->addWidget(iconPushButton);

  QGroupBox* gb2a = new QGroupBox("Point Marker:", this); 
  m_pointMarkComboBox = new QComboBox(gb2a);
  m_pointMarkComboBox->addItem("MarkerNone");
  m_pointMarkComboBox->addItem("MarkerSquare");
  m_pointMarkComboBox->addItem("MarkerDiamond");
  m_pointMarkComboBox->addItem("MarkerCircle");
  m_pointMarkComboBox->addItem("MarkerCrossedCircle");
  m_pointMarkComboBox->addItem("MarkerSemiEllipseLeft");
  m_pointMarkComboBox->addItem("MarkerSemiEllipseRight");
  m_pointMarkComboBox->addItem("MarkerSemiEllipseUp");
  m_pointMarkComboBox->addItem("MarkerSemiEllipseDown");
  m_pointMarkComboBox->addItem("MarkerTriangleLeft");
  m_pointMarkComboBox->addItem("MarkerTriangleRight");
  m_pointMarkComboBox->addItem("MarkerTriangleUp");
  m_pointMarkComboBox->addItem("MarkerTriangleDown");
  m_pointMarkComboBox->addItem("MarkerFourRays");
  m_pointMarkComboBox->addItem("MarkerCross");
  m_pointMarkComboBox->addItem("MarkerX");
  m_pointMarkComboBox->addItem("MarkerDash");
  m_pointMarkComboBox->addItem("MarkerDot");
  m_pointMarkComboBox->addItem("MarkerPixel");
  m_pointMarkComboBox->addItem("MarkerPattern");

  int t = op->getPointMarkerType();
  m_pointMarkComboBox->setCurrentIndex(t);

  QHBoxLayout* hbox2a = new QHBoxLayout(gb2a);
  hbox2a->addWidget(m_pointMarkComboBox);

  QGroupBox* gb3 = new QGroupBox(this); 
  m_applyPushButton = new QPushButton("Apply", gb3);
  m_cancelPushButton = new QPushButton("Cancel", gb3);
  QHBoxLayout* hbox3 = new QHBoxLayout(gb3);
  hbox3->addSpacing(100);
  hbox3->addWidget(m_applyPushButton);
  hbox3->addSpacing(50);
  hbox3->addWidget(m_cancelPushButton);

  vbox->addWidget(gb1);
  vbox->addWidget(gb2);
  vbox->addWidget(gb2a);
  vbox->addWidget(gb3);

  QObject::connect(m_pointMarkComboBox, SIGNAL(clicked(bool)), this, SLOT(markClickedSlot(bool))); 
  QObject::connect(iconPushButton, SIGNAL(clicked(bool)), this, SLOT(iconClickedSlot(bool))); 
  QObject::connect(m_applyPushButton, SIGNAL(clicked(bool)), this, SLOT(applyClickedSlot(bool))); 
  QObject::connect(m_cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(cancelClickedSlot(bool))); 

  setLayout(vbox);
}

PointStyle::~PointStyle()
{
}

void PointStyle::paintEvent(QPaintEvent*)
{
}

void PointStyle::iconClickedSlot(bool)
{
  QString fileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "C:/lixo", tr("Image Files (*.png *.jpg *.bmp)"));

  if(fileName.isEmpty() == false)
  {
    m_pointIconLineEdit->setText(fileName);
    m_pointMarkComboBox->setCurrentIndex(19);
  }
}

void PointStyle::markClickedSlot(bool)
{
  if(m_pointMarkComboBox->currentIndex() != 19)
    m_pointIconLineEdit->setText("");
}

void PointStyle::applyClickedSlot(bool)
{
  accept();
  hide();
}

void PointStyle::cancelClickedSlot(bool)
{
  reject();
  hide();
}

void PointStyle::closeEvent(QCloseEvent *event)
{
  event->accept();
  cancelClickedSlot(true);
}
