/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGFILTEREDDIFFERENCER_H
#define TAGFILTEREDDIFFERENCER_H

#include "TagDifferencer.h"

#include <hoot/core/schema/SchemaVertex.h>

namespace hoot
{

class TagFilteredDifferencer : public TagDifferencer
{
public:
  TagFilteredDifferencer();

  virtual double diff(const ConstOsmMapPtr& map, const ConstElementPtr& e1,
    const ConstElementPtr& e2) const;

protected:

  virtual bool isValidTag(const SchemaVertex& sv) const = 0;
};

}

#endif // TAGFILTEREDDIFFERENCER_H