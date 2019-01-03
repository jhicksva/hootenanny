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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "UnionPolygonsVisitor.h"

// geos
#include <geos/geom/GeometryFactory.h>

// hoot
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/AreaCriterion.h>
using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, UnionPolygonsVisitor)

UnionPolygonsVisitor::UnionPolygonsVisitor()
{
  _result.reset(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
}

void UnionPolygonsVisitor::visit(const boost::shared_ptr<const Element>& e)
{
  if (e->getElementType() == ElementType::Node)
  {
    return;
  }

  if (AreaCriterion().isSatisfied(e))
  {
    boost::shared_ptr<Geometry> g = ElementConverter(_map->shared_from_this()).convertToGeometry(e);
    _result.reset(g->Union(_result.get()));
  }
}

}
