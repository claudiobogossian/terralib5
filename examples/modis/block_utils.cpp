// MODIS
#include "block_utils.h"

// TerraLib
#include <terralib/datatype/Enums.h>

// STL
#include <cstring>

// Boost
#include <boost/cstdint.hpp>

void set_extract_pixel_data_strategy(extract_pixel_data_fnct_t* f, int data_type)
{
  switch(data_type)
  {
    case te::dt::R4BITS_TYPE:
      throw std::runtime_error("Invalid data type!");

    case te::dt::R2BITS_TYPE:
      throw std::runtime_error("Invalid data type!");

    case te::dt::R1BIT_TYPE:
      throw std::runtime_error("Invalid data type!");

    case te::dt::UCHAR_TYPE:
      *f = extract_pixel_data<unsigned char>;
    break;

    case te::dt::CHAR_TYPE:
      *f = extract_pixel_data<char>;
    break;

    case te::dt::UINT16_TYPE:
      *f = extract_pixel_data<boost::uint16_t>;
    break;

    case te::dt::INT16_TYPE:
      *f = extract_pixel_data<boost::int16_t>;
    break;

    case te::dt::UINT32_TYPE:
      *f = extract_pixel_data<boost::uint32_t>;
    break;

    case te::dt::INT32_TYPE:
      *f = extract_pixel_data<boost::int32_t>;
    break;

    case te::dt::FLOAT_TYPE:
      *f = extract_pixel_data<float>;
    break;

    case te::dt::DOUBLE_TYPE:
      *f = extract_pixel_data<double>;
    break;

    case te::dt::CINT16_TYPE:
      throw std::runtime_error("Invalid data type!");

    case te::dt::CINT32_TYPE:
      throw std::runtime_error("Invalid data type!");

    case te::dt::CFLOAT_TYPE:
      throw std::runtime_error("Invalid data type!");

    case te::dt::CDOUBLE_TYPE:
      throw std::runtime_error("Invalid data type!");

    default:
      throw std::runtime_error("Invalid data type!");
  }
}

