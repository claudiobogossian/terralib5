%apply unsigned int *INPUT{size_t& size}

#ifdef SWIGJAVA
// ---------------------------------
// Java Documentation
// ---------------------------------
%javamethodmodifiers GetLayersImage(const std::vector<te::map::AbstractLayer*>& layers, te::gm::Envelope& bbox, int width, int height, size_t& size, const QColor& bckGround) "
/**
 * Returns a PNG image of the given layers.
 * <p>
 * Note that the layers will be rendered at reverse order, it means that the last layer on the layers set will be the first to be rendered.
 *
 * @param layers Layers to be rendered.
 *
 * @param bbox Bounding rectangle of the area to be rendered (return parameter). The rectangle must change to suit to the image dimensions.
 *
 * @param width Image width.
 *
 * @param height Image height.
 *
 * @param size Size of the image (return parameter).
 *
 * @param bckGround Background color of the image.
 *
 * @return A PNG image of the selected layers with the desired bounding rectangle.
 */
  public";

%javamethodmodifiers GetTransformation(const te::gm::Envelope& vport, const te::gm::Envelope& win) "
/** 
 * Returns the transformation matrix to map world coordinates to device coordinates.
 *
 * @param vport Bounding rectangle of the device (pixel coordinate).
 *
 * @param win Bounding rectangle of the data (world coordinate).
 *
 * @return Transformation matrix.
 *
 * @see java.awt.geom.AffineTransform
 */
  public";
  
#endif

%{
// TerraLib
#include "terralib/dataaccess/utils/Utils.h"
#include "terralib/maptools/DataSetLayer.h"
#include "terralib/maptools/Utils.h"
#include "terralib/qt/widgets/canvas/Canvas.h"
#include "terralib/qt/widgets/canvas/MultiThreadMapDisplay.h"
#include "terralib/qt/widgets/Utils.h"
#include "terralib/raster/Grid.h"
#include "terralib/raster/Raster.h"
#include "terralib/raster/RasterProperty.h"
#include "terralib/se/CoverageStyle.h"
#include "terralib/se/Utils.h"
#include "terralib/geometry/Coord2D.h"
#include "terralib/geometry/GeometryProperty.h"
#include "terralib/binding/swig/java/MultiImager.h"
#include "terralib/qt/widgets/datasource/core/DataSourceType.h"
#include "terralib/qt/widgets/datasource/core/DataSourceTypeManager.h"
#include "terralib/qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "terralib/qt/widgets/layer/selector/AbstractLayerSelector.h"

// Qt 
#include "QBuffer"
#include "QPixmap"
#include "QSize"

// STL
#include <memory>

//Forward declarations
namespace te
{
	namespace da { class DataSet; }
	namespace gm { class Envelope; }
}

void Config2Draw(te::da::DataSet* dset, te::qt::widgets::Canvas*);

static char* GetLayersImage(const std::vector<te::map::AbstractLayer*>& layers, te::gm::Envelope& bbox, int width, int height, size_t& size, const QColor& bckGround)
{
  std::vector<te::map::AbstractLayer*>::const_iterator it; 
	std::list<te::map::AbstractLayerPtr> lst;
	std::list<te::map::AbstractLayerPtr>::iterator it2;

	for(it=layers.begin(); it!=layers.end(); ++it)
	{
		te::map::AbstractLayerPtr ptr(0);
		ptr.reset(*it);
		lst.push_back(ptr);
	}
	  
  QSize s(width, height);

  te::qt::widgets::MultiThreadMapDisplay* disp = new te::qt::widgets::MultiThreadMapDisplay(s, false);

  disp->setBackgroundColor(bckGround);
  disp->setExtent(bbox);
  disp->setLayerList(lst);
  disp->setSRID(lst.begin()->get()->getSRID(), false);

  MultiImager imgr;
  imgr.setMapDisplay(disp);
  imgr.refresh();
  
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);

  QPixmap* img = disp->getDisplayPixmap();

	img->save(&buffer, te::qt::widgets::Globals::sm_pngFmt);
	int nbytes = bytes.size();
	char* result = new char[nbytes];
	memcpy(result, bytes.data(), nbytes);
	size = nbytes;
	return result;
}

QTransform GetTransformation(const te::gm::Envelope& vport, const te::gm::Envelope& win)
{
  double xScale = vport.getWidth() / win.getWidth();
  double yScale = vport.getHeight() / win.getHeight();

  QTransform trans;
  
  trans.scale(xScale, -yScale);
  trans.translate(-win.getLowerLeftX(), -win.getUpperRightY());
  
  return trans;
}

%}

// Wrap function
char* GetLayersImage(const std::vector<te::map::AbstractLayer*>& layers, te::gm::Envelope& bbox, int width, int height, size_t& size, const QColor& bckGround);
QTransform GetTransformation(const te::gm::Envelope& vport, const te::gm::Envelope& win);