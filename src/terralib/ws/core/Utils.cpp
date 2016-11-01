#include "Utils.h"

#include "../../common/StringUtils.h"
#include "../../core/translator/Translator.h"
#include "Exception.h"

//BOOST
#include <boost/algorithm/string.hpp>

te::map::ImageType te::ws::core::FormatToImageType(const std::string& format)
{
  te::map::ImageType imageType = te::map::ImageType::UNKNOWN;

  std::vector<std::string> formatSplit = te::common::SplitString(format, '/');

  if(formatSplit.size() != 2)
  {
    throw te::ws::core::Exception() << te::ErrorDescription(TE_TR("Format conversion failed. Image format not supported."));
  }

  std::string posfix = formatSplit[1];

  if(boost::iequals(posfix, "png"))
  {
    imageType = te::map::ImageType::PNG;
  }
  else if(boost::iequals(posfix, "tiff"))
  {
    imageType = te::map::ImageType::TIFF;
  }
  else if(boost::iequals(posfix, "bmp"))
  {
    imageType = te::map::ImageType::BMP;
  }
  else if(boost::iequals(posfix, "jpeg") || boost::iequals(posfix, "jpg"))
  {
    imageType = te::map::ImageType::JPEG;
  }

  return imageType;
}
