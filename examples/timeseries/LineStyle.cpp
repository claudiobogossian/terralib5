#include "LineStyle.h"

// TerraLib
#include <terralib/maptools.h>
#include <terralib/qt/widgets/Utils.h>
#include <terralib/dataaccess.h>

//QT
#include <QColorDialog>
#include <QPainter>
#include <QImage>

LineStyle::LineStyle(te::map::DataGridOperation* op, QWidget* parent) : QDialog(parent)
{
  QString title = op->getDataSetType()->getTitle().c_str();
  title += ": Default Style";
  setWindowTitle(title);
  m_op = op;

  m_pattern = m_op->getLinePatternIcon();
  m_imageType = m_op->getLinePatternIconImageType();
  m_patternSize = m_op->getLinePatternIconSize();

  QVBoxLayout* vbox = new QVBoxLayout(this);

  m_color = m_op->getLineColor();
  QGroupBox* gb = new QGroupBox("", this); 
  m_colorPushButton = new QPushButton("Color...", gb);
  QHBoxLayout* hbox = new QHBoxLayout(gb);
  hbox->addWidget(m_colorPushButton);
  hbox->addStretch();

  m_widthComboBox = new QComboBox(gb);
  for(int i = 0; i < 32; ++i)
  {
    QString s;
    s.setNum(i+1);
    m_widthComboBox->addItem(s);
  }
  int width = op->getLineWidth();
  m_widthComboBox->setCurrentIndex(width - 1);
  hbox->addWidget(m_widthComboBox);
  hbox->addStretch();

  m_iconPushButton = new QPushButton("Icon...", gb);
  hbox->addWidget(m_iconPushButton);
  hbox->addStretch();

  m_noIconPushButton = new QPushButton("No Icon", gb);
  hbox->addWidget(m_noIconPushButton);

  //render area
  QGroupBox* renderAreaGroupBox = new QGroupBox("", this);
  m_lineRenderArea = new RenderArea(renderAreaGroupBox);
  QHBoxLayout* hbox2 = new QHBoxLayout(renderAreaGroupBox);
  hbox2->addStretch();
  hbox2->addWidget(m_lineRenderArea);
  hbox2->addStretch();

  QGroupBox* gb3 = new QGroupBox(this); 
  m_okPushButton = new QPushButton("Ok", gb3);
  m_cancelPushButton = new QPushButton("Cancel", gb3);
  QHBoxLayout* hbox3 = new QHBoxLayout(gb3);
  hbox3->addStretch();
  hbox3->addWidget(m_okPushButton);
  hbox3->addStretch();
  hbox3->addWidget(m_cancelPushButton);
  hbox3->addStretch();

  vbox->addWidget(gb);
  vbox->addWidget(renderAreaGroupBox);
  vbox->addWidget(gb3);

  QObject::connect(m_widthComboBox, SIGNAL(activated(int)), this, SLOT(onWidthComboBoxActivated(int))); 
  QObject::connect(m_colorPushButton, SIGNAL(clicked(bool)), this, SLOT(onColorPushButtonClicked(bool))); 
  QObject::connect(m_iconPushButton, SIGNAL(clicked(bool)), this, SLOT(onIconPushButtonClicked(bool))); 
  QObject::connect(m_noIconPushButton, SIGNAL(clicked(bool)), this, SLOT(onNoIconPushButtonClicked(bool))); 
  QObject::connect(m_okPushButton, SIGNAL(clicked(bool)), this, SLOT(onOkPushButtonClicked(bool))); 
  QObject::connect(m_cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(onCancelPushButtonClicked(bool))); 

  setLayout(vbox);
}

LineStyle::~LineStyle()
{
}

void LineStyle::onColorPushButtonClicked(bool)
{
  QColor color, oldColor(m_color.getRed(), m_color.getGreen(), m_color.getBlue(), m_color.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Polygon Contour Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
  {
    m_color.setColor(color.red(), color.green(), color.blue(), color.alpha());
    update();
  }
}

void LineStyle::onIconPushButtonClicked(bool)
{
  m_patternIconFileName = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "C:/lixo", tr("Image Files (*.png *.jpg *.bmp)"));

  update();
}

void LineStyle::onNoIconPushButtonClicked(bool)
{
  m_patternIconFileName = "";
  m_pattern = 0;
  m_patternSize = 0;
  update();
}

void LineStyle::onWidthComboBoxActivated(int)
{
  update();
}

void LineStyle::drawRenderArea()
{
  QPixmap* pix = m_lineRenderArea->m_pixmap;
  pix->fill(Qt::white);
  QPainter painter(pix);

  QRect rec = pix->rect();
  QPoint pc = rec.center();
  int w = rec.width() * 4 / 5;
  int h = rec.height() * 4 / 5;
  rec.setWidth(w);
  rec.setHeight(h);
  rec.moveCenter(pc);

  painter.setBrush(Qt::NoBrush);
  QPen p;
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
      QBrush b;
      b.setTextureImage(ima);
      QPen p;
      p.setBrush(b);

      int h = m_widthComboBox->currentIndex() + 1;
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

  if(m_color.getAlpha() != 0)
  {
    QColor color(m_color.getRed(), m_color.getGreen(), m_color.getBlue(), m_color.getAlpha());
    p.setColor(color);

    int w = m_widthComboBox->currentIndex() + 1;
    p.setWidth(w);
    painter.setPen(p);
    painter.drawRect(rec);
  }
}

void LineStyle::paintEvent(QPaintEvent* e)
{
  QWidget::paintEvent(e);
  drawRenderArea();
}

void LineStyle::onOkPushButtonClicked(bool)
{
  accept();
  hide();
}

void LineStyle::onCancelPushButtonClicked(bool)
{
  reject();
  hide();
}

void LineStyle::closeEvent(QCloseEvent *event)
{
  event->accept();
  onCancelPushButtonClicked(true);
}
