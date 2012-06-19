#include "PointStyle.h"

// TerraLib
#include <terralib/maptools.h>
#include <terralib/qt/widgets/Utils.h>
#include <terralib/dataaccess.h>

//QT
#include <QColorDialog>
#include <QPainter>
#include <QImage>

PointStyle::PointStyle(te::map::DataGridOperation* op, QWidget* parent) : QDialog(parent)
{
  QString title = op->getDataSetType()->getTitle().c_str();
  title += ": Default Style";
  setWindowTitle(title);
  m_op = op;

  m_pattern = m_op->getPointIcon();
  m_imageType = m_op->getPointIconImageType();
  m_patternSize = m_op->getPointIconSize();
  m_color = m_op->getPointColor();

  QVBoxLayout* vbox = new QVBoxLayout(this);
  QGroupBox* gb1 = new QGroupBox("", this); 
  m_widthComboBox = new QComboBox(gb1);
  for(int i = 0; i < 128; ++i)
  {
    QString s;
    s.setNum(i+1);
    m_widthComboBox->addItem(s);
  }
  int width = op->getPointWidth();
  m_widthComboBox->setCurrentIndex(width - 1);

  QHBoxLayout* hbox1 = new QHBoxLayout(gb1);
  hbox1->addWidget(m_widthComboBox);
  hbox1->addStretch();

  m_iconPushButton = new QPushButton("Icon...", gb1);
  hbox1->addWidget(m_iconPushButton);
  hbox1->addStretch();

  m_noIconPushButton = new QPushButton("No Icon...", gb1);
  hbox1->addWidget(m_noIconPushButton);
  hbox1->addStretch();

  QLabel* label = new QLabel("Rotation:", gb1);
  m_rotationSpinBox = new QSpinBox(gb1);
  m_rotationSpinBox->setRange(-359, 359);
  m_rotationSpinBox->setValue(0);
  m_rotationSpinBox->setEnabled(false);
  hbox1->addWidget(label);
  hbox1->addWidget(m_rotationSpinBox);
  hbox1->addStretch();

  m_colorPushButton = new QPushButton("Color...", gb1);
  hbox1->addWidget(m_colorPushButton);
  hbox1->addStretch();

  //render area
  QGroupBox* renderAreaGroupBox = new QGroupBox("", this);
  m_pointRenderArea = new RenderArea(renderAreaGroupBox);
  QHBoxLayout* hbox2 = new QHBoxLayout(renderAreaGroupBox);
  hbox2->addStretch();
  hbox2->addWidget(m_pointRenderArea);
  hbox2->addStretch();

  QGroupBox* gb3 = new QGroupBox(this); 
  m_okPushButton = new QPushButton("Ok", gb3);
  m_cancelPushButton = new QPushButton("Cancel", gb3);
  QHBoxLayout* hbox3 = new QHBoxLayout(gb3);
  hbox3->addSpacing(100);
  hbox3->addWidget(m_okPushButton);
  hbox3->addSpacing(50);
  hbox3->addWidget(m_cancelPushButton);

  vbox->addWidget(gb1);
  vbox->addWidget(renderAreaGroupBox);
  vbox->addWidget(gb3);

  QObject::connect(m_widthComboBox, SIGNAL(activated(int)), this, SLOT(onWidthComboBoxActivated(int))); 
  QObject::connect(m_iconPushButton, SIGNAL(clicked(bool)), this, SLOT(onIconPushButtonClicked(bool))); 
  QObject::connect(m_noIconPushButton, SIGNAL(clicked(bool)), this, SLOT(onNoIconPushButtonClicked(bool))); 
  QObject::connect(m_colorPushButton, SIGNAL(clicked(bool)), this, SLOT(onColorPushButtonClicked(bool))); 
  QObject::connect(m_rotationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onRotationSpinBoxValueChanged(int))); 
  QObject::connect(m_okPushButton, SIGNAL(clicked(bool)), this, SLOT(onOkPushButtonClicked(bool))); 
  QObject::connect(m_cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(onCancelPushButtonClicked(bool))); 

  setLayout(vbox);
}

PointStyle::~PointStyle()
{
}

void PointStyle::onIconPushButtonClicked(bool)
{
  m_patternIconFileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "C:/lixo", tr("Image Files (*.png *.jpg *.bmp)"));

  update();
}

void PointStyle::onNoIconPushButtonClicked(bool)
{
  m_patternIconFileName = "";
  m_pattern = 0;
  m_patternSize = 0;
  update();
}

void PointStyle::onColorPushButtonClicked(bool)
{
  QColor color, oldColor(m_color.getRed(), m_color.getGreen(), m_color.getBlue(), m_color.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Polygon Contour Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
  {
    m_color.setColor(color.red(), color.green(), color.blue(), color.alpha());
    update();
  }
}

void PointStyle::onWidthComboBoxActivated(int)
{
  update();
}

void PointStyle::onRotationSpinBoxValueChanged(int)
{
  update();
}

void PointStyle::drawRenderArea()
{
  QPixmap* pix = m_pointRenderArea->m_pixmap;
  pix->fill(Qt::white);
  QPainter painter(pix);

  QRect rec = pix->rect();
  QPoint pc = rec.center();

  int w = m_widthComboBox->currentIndex() + 1;

  double alfa = m_rotationSpinBox->value();

  if(m_patternIconFileName.isEmpty() == false)
  {
    QFileInfo file(m_patternIconFileName);
    QString fileName = file.filePath();

    FILE* fp = fopen(fileName.toStdString().c_str(), "rb");
    fseek(fp , 0 , SEEK_END);
    m_patternSize = (int)ftell(fp);
    rewind(fp);
    //if(m_contourPattern)
    //  delete []m_contourPattern;
    m_pattern = new char[m_patternSize];
    fread(m_pattern, sizeof(char), m_patternSize, fp);
    fclose(fp);

    if(fileName.contains("PNG", Qt::CaseInsensitive))
      m_imageType = te::map::PNG;
    else if(fileName.contains("BMP", Qt::CaseInsensitive))
      m_imageType = te::map::BMP;
    else if(fileName.contains("JPG", Qt::CaseInsensitive))
      m_imageType = te::map::JPEG;
    else if(fileName.contains("JPEG", Qt::CaseInsensitive))
      m_imageType = te::map::JPEG;
    else if(fileName.contains("GIF", Qt::CaseInsensitive))
      m_imageType = te::map::GIF;
    //else if(fileName.contains("PBM", Qt::CaseInsensitive))
    //  m_imageType = te::map::PBM;
    //else if(fileName.contains("PGM", Qt::CaseInsensitive))
    //  m_imageType = te::map::PGM;
    //else if(fileName.contains("PPM", Qt::CaseInsensitive))
    //  m_imageType = te::map::PPM;
    else if(fileName.contains("XBM", Qt::CaseInsensitive))
      m_imageType = te::map::XBM;
    else if(fileName.contains("XPM", Qt::CaseInsensitive))
      m_imageType = te::map::XPM;
  }

  if(m_pattern)
  {
    QImage ima;
    bool result = ima.loadFromData((const uchar*)m_pattern, m_patternSize, te::qt::widgets::GetFormat(m_imageType));
    if(result)
    {
      int h = (int)((double)w * (double)ima.height() / (double)ima.width());
      QRect tamRect(0, 0, w, h);
      tamRect.moveCenter(pc);

      QMatrix m;
      if(alfa != 0)
      {
        m.rotate(-alfa);
        ima = ima.transformed(m);
      }
      
      if(m_color.getAlpha() == 0)
        painter.drawImage(tamRect, ima);
      else
      {
        double a = (double)m_color.getAlpha() / (double)255;
        double b = (double)(255 - m_color.getAlpha()) / (double)255;
        QImage imac(ima);
        for(int i = 0; i < ima.height(); ++i)
        {
          unsigned char* u = ima.scanLine(i);
          unsigned char* uu =imac.scanLine(i);
          for(int j = 0; j < ima.width(); ++j)
          {
            QRgb* v = (QRgb*)(u + (j << 2));
            QRgb* vv = (QRgb*)(uu + (j << 2));
            if(qAlpha(*v) == 0)
            {
              *vv = qRgba(0, 0, 0, 0);
            }
            else
            {
              int red = (int)((double)m_color.getRed() * a + (double)qRed(*v) * b);
              int green = (int)((double)m_color.getGreen() * a + (double)qGreen(*v) * b);
              int blue = (int)((double)m_color.getBlue() * a + (double)qBlue(*v) * b);
              *vv = qRgba(red, green, blue, qAlpha(*v));
            }
          }
        }
        painter.drawImage(tamRect, imac);
      }
    }
  }
  else
  {
    if(m_color.getAlpha() != 0)
    {
      QColor color(m_color.getRed(), m_color.getGreen(), m_color.getBlue(), m_color.getAlpha());
      QPen p(color);
      p.setWidthF(w);
      painter.setPen(p);
      painter.drawPoint(pc);
    }
  }
}

void PointStyle::paintEvent(QPaintEvent* e)
{
  QWidget::paintEvent(e);
  drawRenderArea();
}

void PointStyle::onOkPushButtonClicked(bool)
{
  accept();
  hide();
}

void PointStyle::onCancelPushButtonClicked(bool)
{
  reject();
  hide();
}

void PointStyle::closeEvent(QCloseEvent *event)
{
  event->accept();
  onCancelPushButtonClicked(true);
}
