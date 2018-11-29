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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ApiEntityDisplayer.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/SingleStatistic.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/conflate/extractors/FeatureExtractor.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/conflate/merging/MergerCreator.h>
#include <hoot/core/schema/TagMerger.h>
#include <hoot/core/algorithms/StringDistance.h>
#include <hoot/core/algorithms/aggregator/ValueAggregator.h>
#include <hoot/core/info/ApiEntityInfo.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

template<typename ApiEntity>
class ApiEntityNameComparator
{
public:

  ApiEntityNameComparator() {}

  bool operator()(const std::string& name1, const std::string& name2)
  {
    return
      QString::fromStdString(name1).replace("hoot::", "") <
      QString::fromStdString(name2).replace("hoot::", "");
  }
};

static const int MAX_NAME_SIZE = 45;
static const int MAX_TYPE_SIZE = 18;

template<typename ApiEntity>
void printApiEntities(const std::string& apiEntityBaseClassName, const QString apiEntityType,
                      const bool displayType,
                      //the size of the longest names plus a 3 space buffer; the value passed in
                      //here by callers may have to be adjusted over time for some entity types
                      const int maxNameSize)
{
  LOG_VARD(apiEntityBaseClassName);
  std::vector<std::string> classNames =
    Factory::getInstance().getObjectNamesByBase(apiEntityBaseClassName);
  LOG_VARD(classNames);
  ApiEntityNameComparator<ApiEntity> apiEntityNameComparator;
  std::sort(classNames.begin(), classNames.end(), apiEntityNameComparator);

  for (size_t i = 0; i < classNames.size(); i++)
  {
    const std::string className = classNames[i];
    LOG_VARD(className);

    boost::shared_ptr<ApiEntity> apiEntity(
      Factory::getInstance().constructObject<ApiEntity>(className));

    boost::shared_ptr<ApiEntityInfo> apiEntityInfo =
      boost::dynamic_pointer_cast<ApiEntityInfo>(apiEntity);
    if (!apiEntityInfo.get())
    {
      throw HootException(
        "Calls to printApiEntities must be made with classes that implement ApiEntityInfo.");
    }

    LOG_VARD(apiEntityInfo->getDescription());
    if (!apiEntityInfo->getDescription().isEmpty())
    {
      bool supportsSingleStat = false;
      boost::shared_ptr<SingleStatistic> singleStat =
        boost::dynamic_pointer_cast<SingleStatistic>(apiEntity);
      if (singleStat.get())
      {
        supportsSingleStat = true;
      }

      QString name = QString::fromStdString(className).replace("hoot::", "");
      //append '*' to the names of visitors that support the SingleStatistic interface
      if (supportsSingleStat)
      {
        name += "*";
      }
      const int indentAfterName = maxNameSize - name.size();
      const int indentAfterType = MAX_TYPE_SIZE - apiEntityType.size();
      QString line = "  " + name + QString(indentAfterName, ' ');
      if (displayType)
      {
        line += apiEntityType + QString(indentAfterType, ' ');
      }
      line += apiEntityInfo->getDescription();
      LOG_VARD(line);
      std::cout << line << std::endl;
    }
  }
  std::cout << std::endl;
}

QString ApiEntityDisplayer::_apiEntityTypeForBaseClass(const QString className)
{
  LOG_VARD(className);
  if (className.toStdString() == OsmMapOperation::className() ||
      Factory::getInstance().hasBase<OsmMapOperation>(className.toStdString()))
  {
    return "operation";
  }
  else if (className.toStdString() == ElementVisitor::className() ||
           Factory::getInstance().hasBase<ElementVisitor>(className.toStdString()))
  {
    return "visitor";
  }
  else if (className.toStdString() == ConstElementVisitor::className() ||
           Factory::getInstance().hasBase<ConstElementVisitor>(className.toStdString()))
  {
    return "visitor (const)";
  }
  return "";
}

//matchers/mergers have a more roundabout way to get at the description, so we'll create a new
//display method for them
template<typename ApiEntity>
void printApiEntities2(const std::string& apiEntityClassName)
{
  //the size of the longest names plus a 3 space buffer
  const int maxNameSize = 48;

  std::vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(apiEntityClassName);
  ApiEntityNameComparator<ApiEntity> apiEntityNameComparator;
  std::sort(names.begin(), names.end(), apiEntityNameComparator);
  LOG_VARD(names);
  QStringList output;
  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator
    boost::shared_ptr<ApiEntity> mc(
      Factory::getInstance().constructObject<ApiEntity>(names[i]));
    std::vector<CreatorDescription> creators = mc->getAllCreators();
    LOG_VARD(creators.size());

    for (std::vector<CreatorDescription>::const_iterator itr = creators.begin();
         itr != creators.end(); ++itr)
    {
      CreatorDescription description = *itr;
      LOG_VARD(description);
      const QString name = QString::fromStdString(description.className).replace("hoot::", "");
      LOG_VARD(name);
      //this suppresses test and auxiliary rules files
      if (!name.endsWith("Test.js") && !name.endsWith("Rules.js"))
      {
        const int indentAfterName = maxNameSize - name.size();
        QString line = "  " + name + QString(indentAfterName, ' ');
        line += description.description;
        if (description.experimental)
        {
          line += " (experimental)";
        }
        LOG_VARD(line);
        output.append(line);
      }
    }
  }

  output.sort();
  for (int i = 0; i < output.size(); i++)
  {
    std::cout << output.at(i) << std::endl;
  }
}

void ApiEntityDisplayer::displayCleaningOps()
{
  ConfigOptions opts = ConfigOptions(conf());
  const QStringList cleaningOps = opts.getMapCleanerTransforms();
  for (int i = 0; i < cleaningOps.size(); i++)
  {   
    QString className = cleaningOps[i];
    LOG_VARD(className);

    // There's a lot of duplicated code in here when compared with printApiEntities.  Haven't
    // figured out a good way to combine the two yet.

    boost::shared_ptr<ApiEntityInfo> apiEntityInfo;
    const QString apiEntityType = _apiEntityTypeForBaseClass(className);
    boost::shared_ptr<SingleStatistic> singleStat;
    // :-( this is messy...
    if (Factory::getInstance().hasBase<OsmMapOperation>(className.toStdString()))
    {
      boost::shared_ptr<OsmMapOperation> apiEntity(
        Factory::getInstance().constructObject<OsmMapOperation>(className.toStdString()));
      apiEntityInfo = boost::dynamic_pointer_cast<ApiEntityInfo>(apiEntity);
      singleStat = boost::dynamic_pointer_cast<SingleStatistic>(apiEntity);
    }
    else if (Factory::getInstance().hasBase<ElementVisitor>(className.toStdString()))
    {
      boost::shared_ptr<ElementVisitor> apiEntity(
        Factory::getInstance().constructObject<ElementVisitor>(className.toStdString()));
      apiEntityInfo = boost::dynamic_pointer_cast<ApiEntityInfo>(apiEntity);
      singleStat = boost::dynamic_pointer_cast<SingleStatistic>(apiEntity);
    }
    else if (Factory::getInstance().hasBase<ConstElementVisitor>(className.toStdString()))
    {
      boost::shared_ptr<ConstElementVisitor> apiEntity(
        Factory::getInstance().constructObject<ConstElementVisitor>(className.toStdString()));
      apiEntityInfo = boost::dynamic_pointer_cast<ApiEntityInfo>(apiEntity);
      singleStat = boost::dynamic_pointer_cast<SingleStatistic>(apiEntity);
    }

    if (!apiEntityInfo.get())
    {
      throw HootException(
        "Calls to displayCleaningOps must be made with classes that implement ApiEntityInfo.");
    }
    const bool supportsSingleStat = singleStat.get();

    QString name = className.replace("hoot::", "");
    //append '*' to the names of visitors that support the SingleStatistic interface
    if (supportsSingleStat)
    {
      name += "*";
    }
    const int indentAfterName = MAX_NAME_SIZE - name.size();
    const int indentAfterType = MAX_TYPE_SIZE - apiEntityType.size();
    QString line = "  " + name + QString(indentAfterName, ' ');
    line += apiEntityType + QString(indentAfterType, ' ');
    line += apiEntityInfo->getDescription();
    LOG_VARD(line);
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
}

void ApiEntityDisplayer::display(const QString apiEntityType)
{
  DisableLog dl;
  QString msg = " (prepend 'hoot::' before using";
  if (apiEntityType == "operators")
  {
    msg += "; * = implements SingleStatistic):";
    msg.prepend("Operators");
    std::cout << msg << std::endl << std::endl;

    printApiEntities<ElementCriterion>(
      ElementCriterion::className(), "criterion", true, MAX_NAME_SIZE);
    printApiEntities<OsmMapOperation>(
      OsmMapOperation::className(), "operation", true, MAX_NAME_SIZE);
    //would like to combine these visitors into one method call somehow
    printApiEntities<ElementVisitor>(ElementVisitor::className(), "visitor", true, MAX_NAME_SIZE);
    printApiEntities<ConstElementVisitor>(
      ConstElementVisitor::className(), "visitor (const)", true, MAX_NAME_SIZE);
  }
  // this is pretty repetitive :-(
  else if (apiEntityType == "feature-extractors")
  {
    msg += "):";
    msg.prepend("Feature Extractors");
    std::cout << msg << std::endl << std::endl;
    printApiEntities<FeatureExtractor>(
      FeatureExtractor::className(), "feature extractor", false, MAX_NAME_SIZE);
  }
  else if (apiEntityType == "matchers")
  {
    msg += "):";
    msg.prepend("Conflate Matchers");
    std::cout << msg << std::endl << std::endl;
    printApiEntities2<MatchCreator>(MatchCreator::className());
  }
  else if (apiEntityType == "mergers")
  {
    msg += "):";
    msg.prepend("Conflate Mergers");
    std::cout << msg << std::endl << std::endl;
    printApiEntities2<MergerCreator>(MergerCreator::className());
  }
  else if (apiEntityType == "tag-mergers")
  {
    msg += "):";
    msg.prepend("Tag Mergers");
    std::cout << msg << std::endl << std::endl;
    printApiEntities<TagMerger>(TagMerger::className(), "tag merger", false, MAX_NAME_SIZE - 10);
  }
  else if (apiEntityType == "string-comparators")
  {
    msg += "):";
    msg.prepend("String Comparators");
    std::cout << msg << std::endl << std::endl;
    printApiEntities<StringDistance>(
      StringDistance::className(), "string comparator", false, MAX_NAME_SIZE - 15);
  }
  else if (apiEntityType == "value-aggregators")
  {
    msg += "):";
    msg.prepend("Value Aggregators");
    std::cout << msg << std::endl << std::endl;
    printApiEntities<ValueAggregator>(
      ValueAggregator::className(), "value aggregator", false, MAX_NAME_SIZE - 10);
  }
}

}
