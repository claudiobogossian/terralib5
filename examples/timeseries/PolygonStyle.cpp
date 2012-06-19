#include "PolygonStyle.h"

//QT
#include <QColorDialog>
#include <QImage>
#include <QPainter>

// TerraLib
#include <terralib/maptools.h>
#include <terralib/qt/widgets/Utils.h>
#include <terralib/dataaccess.h>

PolygonStyle::PolygonStyle(te::map::DataGridOperation* op, QWidget* parent) : QDialog(parent)
{
  QString title = op->getDataSetType()->getTitle().c_str();
  title += ": Default Style";
  setWindowTitle(title);
  m_op = op;

  m_contourPattern = m_op->getPolygonContourPatternIcon();
  m_contourImageType = m_op->getPolygonContourPatternIconImageType();
  m_contourSize = m_op->getPolygonContourPatternIconSize();

  // polygon contour
  QGroupBox* contourGroupBox = new QGroupBox("Polygon Contour:", this);

  m_polygonContourColor = op->getPolygonContourColor();
  m_polygonContourColorPushButton = new QPushButton("Color...", contourGroupBox);
  QHBoxLayout* hbox = new QHBoxLayout(contourGroupBox);
  hbox->addWidget(m_polygonContourColorPushButton);
  hbox->addStretch();

  m_polygonContourWidthComboBox = new QComboBox(contourGroupBox);
  for(int i = 0; i < 32; ++i)
  {
    QString s;
    s.setNum(i+1);
    m_polygonContourWidthComboBox->addItem(s);
  }
  int width = op->getPolygonContourWidth();
  m_polygonContourWidthComboBox->setCurrentIndex(width - 1);
  hbox->addWidget(m_polygonContourWidthComboBox);
  hbox->addStretch();

  m_polygonContourIconPushButton = new QPushButton("Icon...", contourGroupBox);
  hbox->addWidget(m_polygonContourIconPushButton);
  hbox->addStretch();

  m_polygonContourNoIconPushButton = new QPushButton("No Icon", contourGroupBox);
  hbox->addWidget(m_polygonContourNoIconPushButton);

  //polygon fill
  m_fillPattern = m_op->getPolygonPatternIcon();
  m_fillImageType = m_op->getPolygonPatternIconImageType();
  m_fillSize = m_op->getPolygonPatternIconSize();

  QGroupBox* polygonGroupBox = new QGroupBox("Polygon Fill:", this);

  m_polygonFillColor = op->getPolygonFillColor();
  m_polygonFillColorPushButton = new QPushButton("Color...", polygonGroupBox);
  QHBoxLayout* hbox2 = new QHBoxLayout(polygonGroupBox);
  hbox2->addWidget(m_polygonFillColorPushButton);
  hbox2->addStretch();

  m_polygonFillPatternWidthComboBox = new QComboBox(polygonGroupBox);
  for(int i = 0; i < 64; ++i)
  {
    QString s;
    s.setNum(i+1);
    m_polygonFillPatternWidthComboBox->addItem(s);
  }
  int pwidth = op->getPolygonPatternWidth();
  m_polygonFillPatternWidthComboBox->setCurrentIndex(pwidth - 1);
  hbox2->addWidget(m_polygonFillPatternWidthComboBox);
  hbox2->addStretch();

  m_polygonFillPatternIconPushButton = new QPushButton("Icon...", polygonGroupBox);
  hbox2->addWidget(m_polygonFillPatternIconPushButton);
  hbox2->addStretch();

  m_polygonFillPatternNoIconPushButton = new QPushButton("No Icon", polygonGroupBox);
  hbox2->addWidget(m_polygonFillPatternNoIconPushButton);

  //render area
  QGroupBox* renderAreaGroupBox = new QGroupBox("", this);
  m_polygonFillRenderArea = new RenderArea(renderAreaGroupBox);
  QHBoxLayout* hbox3 = new QHBoxLayout(renderAreaGroupBox);
  hbox3->addStretch();
  hbox3->addWidget(m_polygonFillRenderArea);
  hbox3->addStretch();

  QGroupBox* gb = new QGroupBox(this); 
  m_okPushButton = new QPushButton("Ok", gb);
  m_cancelPushButton = new QPushButton("Cancel", gb);
  QHBoxLayout* hbox4 = new QHBoxLayout(gb);
  hbox4->addStretch();
  hbox4->addWidget(m_okPushButton);
  hbox4->addStretch();
  hbox4->addWidget(m_cancelPushButton);
  hbox4->addStretch();

  QVBoxLayout* polygonLayout = new QVBoxLayout(this);
  polygonLayout->addWidget(contourGroupBox);
  polygonLayout->addWidget(polygonGroupBox);
  polygonLayout->addWidget(renderAreaGroupBox);
  polygonLayout->addWidget(gb);

  QObject::connect(m_polygonContourColorPushButton, SIGNAL(clicked(bool)), this, SLOT(onPolygonContourColorPushButtonClicked(bool))); 
  QObject::connect(m_polygonContourIconPushButton, SIGNAL(clicked(bool)), this, SLOT(onPolygonContourIconPushButtonClicked(bool))); 
  QObject::connect(m_polygonContourNoIconPushButton, SIGNAL(clicked(bool)), this, SLOT(onPolygonContourNoIconPushButtonClicked(bool))); 
  QObject::connect(m_polygonContourWidthComboBox, SIGNAL(activated(int)), this, SLOT(onPolygonContourWidthComboBoxActivated(int))); 

  QObject::connect(m_polygonFillColorPushButton, SIGNAL(clicked(bool)), this, SLOT(onPolygonFillColorPushButtonClicked(bool))); 
  QObject::connect(m_polygonFillPatternIconPushButton, SIGNAL(clicked(bool)), this, SLOT(onPolygonFillPatternIconPushButtonClicked(bool))); 
  QObject::connect(m_polygonFillPatternNoIconPushButton, SIGNAL(clicked(bool)), this, SLOT(onPolygonFillPatternNoIconPushButtonClicked(bool))); 
  QObject::connect(m_polygonFillPatternWidthComboBox, SIGNAL(activated(int)), this, SLOT(onPolygonFillPatternWidthComboBoxActivated(int))); 

  QObject::connect(m_okPushButton, SIGNAL(clicked(bool)), this, SLOT(onOkPushButtonClicked(bool))); 
  QObject::connect(m_cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(onCancelPushButtonClicked(bool))); 

  setLayout(polygonLayout);
}

PolygonStyle::~PolygonStyle()
{
}

void PolygonStyle::paintEvent(QPaintEvent* e)
{
  QWidget::paintEvent(e);
  drawRenderArea();
}

void PolygonStyle::onPolygonContourColorPushButtonClicked(bool)
{
  QColor color, oldColor(m_polygonContourColor.getRed(), m_polygonContourColor.getGreen(), m_polygonContourColor.getBlue(), m_polygonContourColor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Polygon Contour Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
  {
    m_polygonContourColor.setColor(color.red(), color.green(), color.blue(), color.alpha());
    update();
  }
}

void PolygonStyle::onPolygonContourIconPushButtonClicked(bool)
{
  m_polygonContourIconFileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "C:/lixo", tr("Image Files (*.png *.jpg *.bmp)"));
  update();
}

void PolygonStyle::onPolygonContourNoIconPushButtonClicked(bool)
{
  m_polygonContourIconFileName = "";
  m_contourPattern = 0;
  m_contourSize = 0;
  update();
}

void PolygonStyle::onPolygonContourWidthComboBoxActivated(int)
{
  update();
}

void PolygonStyle::onPolygonFillColorPushButtonClicked(bool)
{
  QColor color, oldColor(m_polygonFillColor.getRed(), m_polygonFillColor.getGreen(), m_polygonFillColor.getBlue(), m_polygonFillColor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Polygon Fill Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
  {
    m_polygonFillColor.setColor(color.red(), color.green(), color.blue(), color.alpha());
    update();
  }
}

void PolygonStyle::onPolygonFillPatternIconPushButtonClicked(bool)
{
  m_polygonFillPatternIconFileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "C:/lixo", tr("Image Files (*.png *.jpg *.bmp)"));

  update();
}

void PolygonStyle::onPolygonFillPatternNoIconPushButtonClicked(bool)
{
  m_polygonFillPatternIconFileName = "";
  m_fillPattern = 0;
  m_fillSize = 0;
  update();
}

void PolygonStyle::onPolygonFillPatternWidthComboBoxActivated(int)
{
  update();
}

void PolygonStyle::drawRenderArea()
{
  QPixmap* pix = m_polygonFillRenderArea->m_pixmap;
  pix->fill(Qt::white);
  QPainter painter(pix);

  QRect rec = pix->rect();
  QPoint pc = rec.center();
  int w = rec.width() * 4 / 5;
  int h = rec.height() * 4 / 5;
  rec.setWidth(w);
  rec.setHeight(h);
  rec.moveCenter(pc);

  // fill Polygon
  painter.setPen(Qt::NoPen);
  if(m_polygonFillPatternIconFileName.isEmpty() == false)
  {
    QFileInfo file(m_polygonFillPatternIconFileName);
    QString fileName = file.filePath();

    FILE* fp = fopen(fileName.toStdString().c_str(), "rb");
    fseek(fp , 0 , SEEK_END);
    m_fillSize = (int)ftell(fp);
    rewind(fp);
    //if(m_fillPattern)
    //  delete []m_fillPattern;
    m_fillPattern = new char[m_fillSize];
    fread(m_fillPattern, sizeof(char), m_fillSize, fp);
    fclose(fp);

    if(fileName.contains("PNG", Qt::CaseInsensitive))
      m_fillImageType = te::map::PNG;
    else if(fileName.contains("BMP", Qt::CaseInsensitive))
      m_fillImageType = te::map::BMP;
    else if(fileName.contains("JPG", Qt::CaseInsensitive))
      m_fillImageType = te::map::JPEG;
    else if(fileName.contains("JPEG", Qt::CaseInsensitive))
      m_fillImageType = te::map::JPEG;
    else if(fileName.contains("GIF", Qt::CaseInsensitive))
      m_fillImageType = te::map::GIF;
    //else if(fileName.contains("PBM", Qt::CaseInsensitive))
    //  m_fillImageType = te::map::PBM;
    //else if(fileName.contains("PGM", Qt::CaseInsensitive))
    //  m_fillImageType = te::map::PGM;
    //else if(fileName.contains("PPM", Qt::CaseInsensitive))
    //  m_fillImageType = te::map::PPM;
    else if(fileName.contains("XBM", Qt::CaseInsensitive))
      m_fillImageType = te::map::XBM;
    else if(fileName.contains("XPM", Qt::CaseInsensitive))
      m_fillImageType = te::map::XPM;
  }

  if(m_fillPattern)
  {
    QImage ima;
    bool result = ima.loadFromData((const uchar*)m_fillPattern, m_fillSize, te::qt::widgets::GetFormat(m_fillImageType));
    if(result)
    {
      QBrush b;
      b.setTextureImage(ima);

      int h = m_polygonFillPatternWidthComboBox->currentIndex() + 1;
      QMatrix m;
      double s = (double)h / (double)ima.height();
      m.scale(s, s);
      b.setMatrix(m);
      painter.setBrush(b);
      painter.drawRect(rec);
    }
  }

  if(m_polygonFillColor.getAlpha() != 0)
  {
    QColor color(m_polygonFillColor.getRed(), m_polygonFillColor.getGreen(), m_polygonFillColor.getBlue(), m_polygonFillColor.getAlpha());
    QBrush b(color);
    painter.setBrush(b);
    painter.drawRect(rec);
  }    

  // draw contour
  painter.setBrush(Qt::NoBrush);
  QPen p;
  if(m_polygonContourIconFileName.isEmpty() == false)
  {
    QFileInfo file(m_polygonContourIconFileName);
    QString fileName = file.filePath();

    FILE* fp = fopen(fileName.toStdString().c_str(), "rb");
    fseek(fp , 0 , SEEK_END);
    m_contourSize = (int)ftell(fp);
    rewind(fp);
    //if(m_contourPattern)
    //  delete []m_contourPattern;
    m_contourPattern = new char[m_contourSize];
    fread(m_contourPattern, sizeof(char), m_contourSize, fp);
    fclose(fp);

    if(fileName.contains("PNG", Qt::CaseInsensitive))
      m_contourImageType = te::map::PNG;
    else if(fileName.contains("BMP", Qt::CaseInsensitive))
      m_contourImageType = te::map::BMP;
    else if(fileName.contains("JPG", Qt::CaseInsensitive))
      m_contourImageType = te::map::JPEG;
    else if(fileName.contains("JPEG", Qt::CaseInsensitive))
      m_contourImageType = te::map::JPEG;
    else if(fileName.contains("GIF", Qt::CaseInsensitive))
      m_contourImageType = te::map::GIF;
    //else if(fileName.contains("PBM", Qt::CaseInsensitive))
    //  m_contourImageType = te::map::PBM;
    //else if(fileName.contains("PGM", Qt::CaseInsensitive))
    //  m_contourImageType = te::map::PGM;
    //else if(fileName.contains("PPM", Qt::CaseInsensitive))
    //  m_contourImageType = te::map::PPM;
    else if(fileName.contains("XBM", Qt::CaseInsensitive))
      m_contourImageType = te::map::XBM;
    else if(fileName.contains("XPM", Qt::CaseInsensitive))
      m_contourImageType = te::map::XPM;
  }

  if(m_contourPattern)
  {
    QImage ima;
    bool result = ima.loadFromData((const uchar*)m_contourPattern, m_contourSize, te::qt::widgets::GetFormat(m_contourImageType));
    if(result)
    {
      QBrush b;
      b.setTextureImage(ima);

      int h = m_polygonContourWidthComboBox->currentIndex() + 1;
      p.setWidth(h);
      QMatrix m;
      double s = (double)h / (double)ima.height();
      m.scale(s, s);
      m.translate((double)ima.width()/2., (double)ima.height()/2.);
      b.setMatrix(m);
      p.setBrush(b);
      painter.setPen(p);

      painter.save();
      painter.translate(rec.topLeft());
      painter.drawLine(QPoint(0, 0), QPoint(rec.width()-1, 0));
      painter.restore();

      painter.save();
      painter.translate(rec.bottomLeft());
      painter.drawLine(QPoint(0, 0), QPoint(rec.width()-1, 0));
      painter.restore();

      painter.save();
      painter.translate(rec.bottomLeft());
      painter.rotate(-90);
      painter.drawLine(QPoint(0, 0), QPoint(rec.height()-1, 0));
      painter.restore();

      painter.save();
      painter.translate(rec.bottomRight());
      painter.rotate(-90);
      painter.drawLine(QPoint(0, 0), QPoint(rec.height()-1, 0));
      painter.restore();
    }
  }

  if(m_polygonContourColor.getAlpha() != 0)
  {
    p.setBrush(Qt::NoBrush);
    QColor color(m_polygonContourColor.getRed(), m_polygonContourColor.getGreen(), m_polygonContourColor.getBlue(), m_polygonContourColor.getAlpha());
    p.setColor(color);

    int w = m_polygonContourWidthComboBox->currentIndex() + 1;
    p.setWidth(w);
    painter.setPen(p);
    painter.drawRect(rec);
  }
}

void PolygonStyle::onOkPushButtonClicked(bool)
{
  accept();
  hide();
}

void PolygonStyle::onCancelPushButtonClicked(bool)
{
  reject();
  hide();
}

void PolygonStyle::closeEvent(QCloseEvent *event)
{
  event->accept();
  onCancelPushButtonClicked(true);
}
