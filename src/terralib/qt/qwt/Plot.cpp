#include "Plot.h"
#include <zoom_in_cursor.xpm>
#include <zoom_in_cursor_mask.xpm>

//QWT
#include <qwt_scale_div.h>
#include <qwt_scale_draw.h>

//TerraLib
#include "../../datatype.h"

//QT
#include <QBitmap>

te::qt::qwt::Plot::Plot(QString type, te::map::DataGridOperation* op, QWidget *parent):
  QwtPlot(parent),
  m_type(type),
  m_xStringScaleDraw(0),
  m_yStringScaleDraw(0),
  m_op(op),
  m_numberOfBars(50),
  m_tableChanged(true),
  m_yType(-1),
  m_zoomer(0),
  m_panner(0),
  m_zoomCursor(0),
  m_legend(0),
  m_maxNumberOfHorizontalLabels(30)
{
  m_xMin = m_XMIN = m_yMin = m_YMIN = std::numeric_limits<double>::max();
  m_xMax = m_XMAX = m_yMax = m_YMAX = -(std::numeric_limits<double>::max());
}

te::qt::qwt::Plot::~Plot()
{
  if(m_zoomer)
    delete m_zoomer;
  if(m_panner)
    delete m_panner;
  if(m_zoomCursor)
    delete m_zoomCursor;
  //if(m_legend)
  //  delete m_legend;
}

void te::qt::qwt::Plot::init()
{
  //QBitmap zoom("../../../../images/zoom_in_cursor.bmp");
  //QBitmap zoomMask("../../../../images/zoom_in_cursor_mask.bmp");

  QPixmap pz((const char**)zoom_in_cursor);
  QPixmap pzm((const char**)zoom_in_cursor_mask);
  QBitmap bz(pz);
  QBitmap bzm(pzm);
  m_zoomCursor = new QCursor(bz, bzm, 12, 12);

  //zoom
  m_zoomer = new QwtPlotZoomer(canvas());

  // define a cor do retangulo (area de zoom)
  m_zoomer->setRubberBandPen( QColor( Qt::red ) );

  m_zoomer->setTrackerMode(QwtPicker::AlwaysOff);
  //define a cor do texto que mostra as coordenadas para o zoom
  //m_zoomer->setTrackerPen( QColor( Qt::magenta ) );

  //mouseSelect1 define qual combinacao de botoes desenha a area de zoom.
  //neste caso: Control_key + left button start a area de zoom.
  m_zoomer->setMousePattern( QwtEventPattern::MouseSelect1,
      Qt::LeftButton, Qt::ControlModifier );

  // mouseSelect2 (RightButton) foi modificado para nao resetar o zoom facilmente
  // mouseSelect2 define qual combinacao de botoes reseta o zoom.
  // neste caso: Control + right button reseta o zoom.
  m_zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
      Qt::RightButton, Qt::ControlModifier );

  // mouseSelect3 (MidButton) foi modificado para permitir PAN usando MidButton
  // mouseSelect3 define qual combinacao de botoes retorna ao zoom anterior.
  // neste caso: Alt + right button volta o zoom.
  m_zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
      Qt::RightButton, Qt::AltModifier );

  //Controle para UNDO e REDO usando teclas.
  //por default a tecla "-" faz undo funcionar, mas, a tecla "+" que deveria fazer redo nao funciona
  //acho que e' porque usa a tecla shift para gerar o "+".

  //KeyUndo define qual tecla faz o undo do zoom.
  //neste caso: a tecla F1 (Nao se pode usar shift ou alt ou control).
  m_zoomer->setKeyPattern( QwtEventPattern::KeyUndo,
      Qt::Key_F1);

  //KeyRedo define qual tecla faz o redo do zoom.
  //neste caso: a tecla F2 (Nao se pode usar shift ou alt ou control).
  m_zoomer->setKeyPattern( QwtEventPattern::KeyRedo,
      Qt::Key_F2);

  m_panner = new QwtPlotPanner( canvas() );
  //define qual botao do mouse controla o voo
//  m_panner->setMouseButton( Qt::LeftButton, Qt::ShiftModifier);
  m_panner->setMouseButton( Qt::MidButton);

  //define o shape do cursor quando o pan esta sendo feito
  //  m_panner->setCursor(QCursor(Qt::OpenHandCursor));

  //define a cor de fundo
  canvas()->setPalette( Qt::white );
  //canvas()->setPalette( Qt::darkGray );
  canvas()->setBorderRadius( 10 );

  //Quando da' zoom e' preciso ajustar a quantidade de labels do eixo x
  QObject::connect(m_zoomer, SIGNAL(zoomed(const QRectF&)), this, SLOT(zoomedSlot(const QRectF&)));

  //Quando da' pan e' preciso ajustar a quantidade de labels do eixo x
  QObject::connect(m_panner, SIGNAL(panned(int, int)), this, SLOT(pannedSlot(int, int)));  
}

void te::qt::qwt::Plot::setVerticalTitle(QString title)
{
  setAxisTitle(QwtPlot::yLeft, title);
}

void te::qt::qwt::Plot::setHorizontalTitle(QString title)
{
  setAxisTitle(QwtPlot::xBottom, title);
}

void te::qt::qwt::Plot::setTableChanged(bool b)
{
  m_tableChanged = b;
}

void te::qt::qwt::Plot::zoomedSlot(const QRectF&)
{
  const QwtScaleDiv& sdx = axisScaleDraw(QwtPlot::xBottom)->scaleDiv();
  m_xMin = sdx.lowerBound();
  m_xMax = sdx.upperBound();
  const QwtScaleDiv& sdy = axisScaleDraw(QwtPlot::yLeft)->scaleDiv();
  m_yMin = sdy.lowerBound();
  m_yMax = sdy.upperBound();

  if(m_yType == -1) // if Histogram, recalculate horizontal scale.
  {
    double d = m_xMax - m_xMin;
    double labelInterval = m_barInterval;
    while(d/labelInterval > m_numberOfBars)
      labelInterval += 1.;

    setAxisScale(QwtPlot::xBottom, m_xMin, m_xMax, (double)labelInterval);
  }
  else // if scatter
    adjustScatterLabels();
  
  replot();
}

void te::qt::qwt::Plot::pannedSlot(int, int)
{
  const QwtScaleDiv& xsd = axisScaleDraw(QwtPlot::xBottom)->scaleDiv();
  double xmin = xsd.lowerBound();
  double xmax = xsd.upperBound();

  const QwtScaleDiv& ysd = axisScaleDraw(QwtPlot::yLeft)->scaleDiv();
  double ymin = ysd.lowerBound();
  double ymax = ysd.upperBound();

  QRectF rec(xmin, ymin, xmax-xmin, ymax-ymin);
  m_zoomer->zoom(rec);
}

te::map::DataGridOperation* te::qt::qwt::Plot::getDataGridOperation()
{
  return m_op;
}

void te::qt::qwt::Plot::adjustHistogramHorizontalAxis() // somente para numeros e datetime
{
  m_barInterval = (m_XMAX - m_XMIN) / (double)m_numberOfBars;

  m_XMIN = (int)(m_XMIN / m_barInterval);
  m_XMIN *= m_barInterval;
  m_XMAX = (int)(m_XMAX / m_barInterval + .5);
  m_XMAX *= m_barInterval;

  // ajuste o m_xMin e m_xMax em funcao do intervalo
  if(m_xMin < m_XMIN)
    m_xMin = m_XMIN;
  if(m_xMax > m_XMAX)
    m_xMax = m_XMAX;

  double xmin = m_xMin;
  double xmax = m_xMax;

  int nb = (int)(((xmax- xmin)/(m_XMAX - m_XMIN)) * (double)m_numberOfBars);
  int nl = (int)(((xmax- xmin)/(m_XMAX - m_XMIN)) * (double)m_maxNumberOfHorizontalLabels);

  xmin = (int)(xmin / m_barInterval);
  xmin *= m_barInterval;
  xmax = (int)(xmax / m_barInterval + .5);
  xmax *= m_barInterval;

  m_XMIN = (int)(m_XMIN / m_barInterval);
  m_XMIN *= m_barInterval;
  m_XMAX = (int)(m_XMAX / m_barInterval + .5);
  m_XMAX *= m_barInterval;
  m_xMin = (int)(xmin / m_barInterval);
  m_xMin *= m_barInterval;
  m_xMax = (int)(xmax / m_barInterval + .5);
  m_xMax *= m_barInterval;

  // se o numero de barras for maior que o numero maximo de labels
  // recalcule os labelInterval para nao exceder o numero maximo de labels

  double labelInterval = m_barInterval;

  while((m_xMax-m_xMin)/labelInterval > nl)
    labelInterval += m_barInterval;
  setAxisScale(QwtPlot::xBottom, m_xMin-m_barInterval/2., m_xMax+m_barInterval, labelInterval);
}


void te::qt::qwt::Plot::adjustScatterLabels()
{
  if(m_xType == te::dt::STRING_TYPE)
  {
    int labelInterval = (m_xMax - m_xMin) / m_maxNumberOfHorizontalLabels;
    if(labelInterval < 1)
      labelInterval = 1;
    setAxisScale(QwtPlot::xBottom, m_xMin, m_xMax, (double)labelInterval);
  }

  if(m_yType == te::dt::STRING_TYPE)
  {
    int labelInterval = (m_yMax - m_yMin) / m_maxNumberOfHorizontalLabels;
    if(labelInterval < 1)
      labelInterval = 1;
    setAxisScale(QwtPlot::yLeft, m_yMin, m_yMax, (double)labelInterval);
  }
}

te::qt::qwt::Legend* te::qt::qwt::Plot::getLegend()
{
  return m_legend;
}