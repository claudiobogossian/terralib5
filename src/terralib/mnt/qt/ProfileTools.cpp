/*!
\file terralib/mnt/qt/ProfileTools.cpp

\brief This file defines the profile trajectory edition tools

*/

// TerraLib
#include "../../edit/qt/Utils.h"
#include "../../edit/Utils.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/canvas/MapDisplay.h"
#include "../../qt/widgets/Utils.h"

#include "ProfileTools.h"

#include <QMessageBox>


te::mnt::ProfileTools::ProfileTools(te::qt::widgets::MapDisplay* display, EditType t, QObject* parent)
  : AbstractTool(display, 0)
{
  m_type = t;
}

te::mnt::ProfileTools::~ProfileTools()
{
  m_lines.clear();
}

void te::mnt::ProfileTools::setType(EditType t)
{
  m_type = t;
}


bool te::mnt::ProfileTools::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
    return false;
  
  switch (m_type)
  {
  case LINE_SELECT:
    break;
  case LINE_CREATE:
    break;
  case VERTEX_MOVE:
  case VERTEX_ADD:
  case VERTEX_DELETE:
  case NONE:
    break;
  }

    return true;
}

bool te::mnt::ProfileTools::mouseMoveEvent(QMouseEvent* e)
{
  //switch (m_type)
  //{
  //case LINE_SELECT:
  //  break;
  //case LINE_CREATE:
  //  if (m_coords.size() < 1 || m_isFinished)
  //    return false;

  //  QPointF pos = te::edit::GetPosition(e);

  //  QPointF pw = m_display->transform(pos);

  //  te::gm::Coord2D coord = te::gm::Coord2D(pw.x(), pw.y());

  //  te::edit::TrySnap(coord, m_display->getSRID());

  //  m_coords.push_back(coord);

  //  m_lastPos = te::gm::Coord2D(coord.x, coord.y);

  //  if (e->buttons() & Qt::LeftButton)
  //    m_continuousMode = true;
  //  else
  //    m_continuousMode = false;

  //  //draw();
  //  break;
  //case VERTEX_MOVE:
  //case VERTEX_ADD:
  //case VERTEX_DELETE:
  //case NONE:
  //  break;
  //}

  return true;
}

bool te::mnt::ProfileTools::mouseReleaseEvent(QMouseEvent* e)
{
  try
  {
    switch (m_type)
    {
    case LINE_SELECT:
    {
      QPointF pos = te::edit::GetPosition(e);
      QPointF pw = m_display->transform(pos);
      te::gm::Point *coord = new te::gm::Point(pw.x(), pw.y());
      coord->setSRID(m_display->getSRID());
      for (size_t i = 0; i < m_lines.size(); i++)
      {
        if (m_lines[i]->intersects(coord))
        {
          m_selectline = m_lines[i];
          te::qt::widgets::Canvas canvas(m_display->getDisplayPixmap());
          canvas.setLineColor(Qt::green);
          te::qt::widgets::Config2DrawPoints(&canvas, "circle", 8, Qt::red, Qt::black, 1);

          canvas.draw(m_selectline);
          for (std::size_t j = 0; j < m_selectline->getNPoints(); ++j)
          {
            std::auto_ptr<te::gm::Point> point(m_selectline->getPointN(j));
            canvas.draw(point.get());
          }

          return true;
        }
      }
      return false;
    }
    break;

    case LINE_CREATE:
    case VERTEX_MOVE:
    case VERTEX_ADD:
    case VERTEX_DELETE:
    case NONE:
      break;
    }

  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(0, "Profile ", e.what());
    return EXIT_FAILURE;
  }
  return true;
}

bool te::mnt::ProfileTools::mouseDoubleClickEvent(QMouseEvent* e)
{
  switch (m_type)
  {
  case LINE_SELECT:
  case LINE_CREATE:
  case VERTEX_MOVE:
  case VERTEX_ADD:
  case VERTEX_DELETE:
  case NONE:
    break;
  }

  return true;
}

