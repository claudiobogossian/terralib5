/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/progress/AbstractProgress.cpp

  \brief The AbstractProgress is an abstract class used to define the main progress functions.
*/

// TerraLib
#include "../Translator.h"
#include "ConsoleProgress.h"

// STL
#include <iostream>

te::common::ConsoleProgress::ConsoleProgress()
  : AbstractProgress()
{
}

te::common::ConsoleProgress::~ConsoleProgress()
{
}

void te::common::ConsoleProgress::setCurrentStep(const int& step)
{
  te::common::AbstractProgress::setCurrentStep(step);

  if(this->hasToUpdate())
  {
    // Show the percentage complete.
    std::cout << this->getMessage() << TR_COMMON(" - Percent: ") << this->getCurrentProportionalStep() << "%" << std::endl;
  }
}
