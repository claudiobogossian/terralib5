
/*!
\file terralib/mnt/core/Enums.h

\brief General enumerations.

*/

#ifndef __TERRALIB_MNT_INTERNAL_ENUMS_H
#define __TERRALIB_MNT_INTERNAL_ENUMS_H

namespace te
{
  namespace mnt
  {
    /*!
    \enum LType
    \brief Line types.
    */
    enum Ltype{
      Deletedline,
      Normalline,
      Isoline,
      Breakline
    };

    /*!
    \enum NType
    \brief Node types.
    */
    enum Ntype
    {
      Deletednode,
      Normalnode,
      First,
      Last,
      Breaklinenormal,
      Breaklinefirst,
      Breaklinelast,
      Sample,
      Box
    };

    /*!
    \enum InputType
    \brief Input types.
    */
    enum InputType
    {
      Isolines,
      Samples
    };

     enum Interpolator
    {
      MediaCotaQuad,   /*!< Weighted Average/Z Value/Quadrant */
      MediaQuad,      /*!< Weighted Average/Quadrant */
      MediaPonderada, /*!< Weighted Average */
      Media,          /*!< Simple Average */
      Vizinho,        /*!< Nearest Neighbor */
      SplineBilinear, /*!< Bilinear Spline */
      SplineBicubico, /*!< Bicubic Spline */
      Mitasova,       /*!< Mitasova Spline */
      Bilinear,       /*!< Bilinear */
      Bicubico,       /*!< Bicubic */
      Linear,         /*!< Linear */
      QuinticoBrkLine,  /*!< Quintic with breaklines */
      Quintico        /*!< Quintic without breaklines */
    };

     enum Curve
     {
       LINEARCURVE,
       BSPLINECURVE,
       CATMULLROMCURVE
     };

     enum mntType
     {
       SAMPLE,
       TIN,
       GRID
     };

     enum Simplify
     {
       AccumulatedDistance,
       Spline,
       DouglasPeucker,
       None
     };

  }
}
#endif
