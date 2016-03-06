#include "DrawThread.h"

// TerraLib
#include "../Utils.h"
#include "../../../common/Exception.h"
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../srs/Config.h"
#include "Canvas.h"

te::qt::widgets::DrawThread::DrawThread(QPaintDevice* dev, te::map::AbstractLayer* layer, te::gm::Envelope* env, const QColor& bckGround, int srid, double scale,
                                        te::map::AlignType hAlign, te::map::AlignType vAlign):
QObject(),
QRunnable(),
m_device(dev),
m_layer(layer),
m_envelope(env),
m_bckGround(bckGround),
m_srid(srid),
m_scale(scale),
m_hAlign(hAlign),
m_vAlign(vAlign),
m_cancel(false)
{
  setAutoDelete(false);
}

te::qt::widgets::DrawThread::~DrawThread()
{
}

void te::qt::widgets::DrawThread::run()
{
  m_finished = false;

  try
  {
    Canvas canvas(m_device);
    canvas.clear();

    m_layer->draw(&canvas, *m_envelope, m_srid, m_scale, &m_cancel);

    if(!m_cancel)
      m_finished = true;
  }
  catch(const te::common::Exception& e)
  {
    if(e.code() == te::common::NO_CONNECTION_AVAILABLE)
    {
      //try again
      //msleep(100);
    }
    else
      m_errorMessage = QString(tr("The layer") + " %1 " + tr("could not be drawn! Details:") + " %2").arg(m_layer->getTitle().c_str()).arg(e.what());
  }
  catch(const std::exception& e)
  {
    m_errorMessage = QString(tr("The layer") + " %1 " + tr("could not be drawn! Details:") + " %2").arg(m_layer->getTitle().c_str()).arg(e.what());
  }
  catch(...)
  {
    m_errorMessage = QString(tr("The layer") + " %1 " + tr("could not be drawn!")).arg(m_layer->getTitle().c_str());
  }

  emit finished();
}

bool te::qt::widgets::DrawThread::hasFinished() const
{
  return m_finished;
}

QString te::qt::widgets::DrawThread::errorMessage() const
{
  return m_errorMessage;
}

QString te::qt::widgets::DrawThread::layerId() const
{
  return QString::fromStdString(m_layer->getId());
}
