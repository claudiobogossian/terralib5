/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file RPExamples.h

  \brief These routines show how to use the RP (raster processing) module.
 */

#ifndef __TERRALIB_EXAMPLES_INTERNAL_RP_EXAMPLES_H
#define __TERRALIB_EXAMPLES_INTERNAL_RP_EXAMPLES_H

#include "Config.h"

/*! \brief It loads the data source drivers. */
void LoadModules();

/* \brief Segmentation example using Raster Processing module. */
void Segmenter();

/* \brief Contrast example using Raster Processing module. */
void Contrast();

/* \brief Classification example using SAM, Dummy (example), ISOSeg, MAP, EM, KMeans modules. */
void Classifier();

/* \brief Raster decomposition using Mixture Model module. */
void MixtureModel();

/* \brief Mosaic of images. */
void Mosaic();

/* \brief Mosaic of images. */
void Register();

/* \brief IHS Fusion example. */
void IHSFusion();

/* \brief Filter examples (Sobel, Mean, Median, Low/High-pass). */
void Filter();

/* \brief Arithmetic operations examples (band division band sum). */
void ArithmeticOperations();

/* \brief Texture metrics example. */
void Texture();

#endif
