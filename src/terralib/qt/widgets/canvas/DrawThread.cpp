#include "DrawThread.h"

// TerraLib
#include "../Utils.h"
#include "../../../common/Exception.h"
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/Enums.h"
#include "Canvas.h"

te::qt::widgets::DrawThread::DrawThread(QPaintDevice* dev, te::map::AbstractLayer* layer, te::gm::Envelope* env, const QColor& bckGround, int srid, double scale,
                                        te::map::AlignType hAlign, te::map::AlignType vAlign):
QRunnable(),
m_device(dev),
m_layer(layer),
m_envelope(env),
m_bckGround(bckGround),
m_srid(srid),
m_scale(scale),
m_hAlign(hAlign),
m_vAlign(vAlign)
{
  setAutoDelete(false);
}

te::qt::widgets::DrawThread::~DrawThread()
{
}

void te::qt::widgets::DrawThread::run()
{
  // Prepares the canvas
  Canvas canvas(m_device);

//  canvas.setBackgroundColor(te::qt::widgets::Convert2TerraLib(m_bckGround));
  canvas.calcAspectRatio(m_envelope->m_llx, m_envelope->m_lly, m_envelope->m_urx, m_envelope->m_ury, m_hAlign, m_vAlign);
  canvas.setWindow(m_envelope->m_llx, m_envelope->m_lly, m_envelope->m_urx, m_envelope->m_ury);

  canvas.clear();

//  canvas.calcAspectRatio(m_envelope->m_llx, m_envelope->m_lly, m_envelope->m_urx, m_envelope->m_ury);
//  canvas.setWindow(m_envelope->m_llx, m_envelope->m_lly, m_envelope->m_urx, m_envelope->m_ury);
//  canvas.clear();

  try
  {
    m_layer->draw(&canvas, *m_envelope, m_srid, m_scale);

    //QPainter p(canvas.getPixmap());

//    canvas.save("teste.png", te::map::PNG);
  }
  catch(const te::common::Exception&)
  {
    //if(e.code() == te::common::NO_CONNECTION_AVAILABLE)
    //{
    //  //try again
    //  //msleep(100);
    //}
    //else
    //{
    //  m_finishedWithSuccess = false;
    //  m_errorMessage = QString(tr("The layer") + " %1 " + tr("could not be drawn! Details:") + " %2").arg(m_layer->getTitle().c_str()).arg(e.what());
    //  //        break; // finish with error
    //}
  }
  catch(const std::exception&)
  {
    //m_finishedWithSuccess = false;
    //m_errorMessage = QString(tr("The layer") + " %1 " + tr("could not be drawn! Details:") + " %2").arg(m_layer->getTitle().c_str()).arg(e.what());
    ////      break; // finish with error
  }
  catch(...)
  {
    //m_finishedWithSuccess = false;
    //m_errorMessage = QString(tr("The layer") + " %1 " + tr("could not be drawn!")).arg(m_layer->getTitle().c_str());
    ////      break; // finish with error
  }
}
