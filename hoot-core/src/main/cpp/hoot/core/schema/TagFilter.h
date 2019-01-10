
#ifndef TAG_FILTER_H
#define TAG_FILTER_H

// Hoot
#include <hoot/core/schema/OsmSchema.h>

// Qt
#include <QString>

// Boost
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

namespace hoot
{

/**
 * A tag filter linked to the Hootenanny schema that can be applied to feature data with a criterion.
 * Multiple tag filters can be specified in the filter JSON.
 */
class TagFilter
{

public:

  TagFilter(const QString key, const QString value,
            const OsmSchemaCategory& category = OsmSchemaCategory::Empty);

  QString getKey() const { return _key; }
  void setKey(QString key);

  QString getValue() const { return _value; }
  void setValue(QString val);

  bool getAllowAliases() const { return _allowAliases; }
  void setAllowAliases(bool allow) { _allowAliases = allow; }

  double getSimilarityThreshold() const { return _similarityThreshold; }
  void setSimilarityThreshold(double threshold);

  bool getAllowChildren() const { return _allowChildren; }
  void setAllowChildren(bool allow) { _allowChildren = allow; }

  bool getAllowAncestors() const { return _allowAncestors; }
  void setAllowAncestors(bool allow) { _allowAncestors = allow; }

  bool getAllowAssociations() const { return _allowAssociations; }
  void setAllowAssociations(bool allow) { _allowAssociations = allow; }

  OsmSchemaCategory getCategory() const { return _category; }
  void setCategory(OsmSchemaCategory category);

  /**
   * Creates a tag filter from a JSON node
   *
   * @param tagFilterPart a "must", "must_not", "should" boost prop tree node
   * @return a tag filter
   */
  static TagFilter fromJson(const pt::ptree::value_type& tagFilterPart);

  QString toString() const;

private:

  friend class TagFilterTest;

  // allows for retrieving features by tag content (optional if category is used)
  QString _key;
  QString _value;

  // allows retrieving features similar to those brought back by the tag filter, if their
  // similarity score is above the threshold, where similarity is defined in the hoot schema; must
  // be a positive number <= 1.0 for use; -1.0 is the default value and is ignored (optional)
  double _similarityThreshold;

  // allows for retrieving related features with to those brought back by the tag filter;
  // See OsmSchema or the user documentation on tag filtering for more info (optional)
  bool _allowAliases;
  bool _allowChildren;
  bool _allowAncestors;
  bool _allowAssociations;

  // allows for retrieving features by a category, where categories are defined in the hoot schema
  // (optional if a tag filter is used)
  OsmSchemaCategory _category;
};

}

#endif // TAG_FILTER_H
