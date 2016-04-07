#ifndef CHANGSETPROVIDER_H
#define CHANGSETPROVIDER_H

// GDAL
#include <ogr_spatialref.h>

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Represents an individual OSM change in a changeset
 */
class Change
{

public:

  /**
   * Enumerates the allowable changeset types
   */
  enum ChangeType
  {
    Create,
    Modify,
    Delete,
    Unknown
  };

  static QString changeTypeToString(const ChangeType changeType)
  {
    switch (changeType)
    {
      case Create:
        return "Create";
      case Modify:
        return "Modify";
      case Delete:
        return "Delete";
      case Unknown:
        return "Unknown";
    }
    throw HootException("Invalid change type.");
  }

  ElementPtr e;
  ChangeType type;

  Change() : type(Unknown) {}

  QString toString() const
  {
    return changeTypeToString(type) + ": " +
      e->getElementId().toString() //+
      //e->toString()
      ;
  }
};


/**
 * Interface for classes implementing OSM changeset capabilities
 */
class ChangeSetProvider
{

public:

  /**
   * @brief getProjection
   * @return
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const = 0;

  /**
   * @brief ~ElementInputStream
   *
   * If the stream is open when the destructor is called, closeStream must be called in the destructor
   */
  virtual ~ChangeSetProvider() {}

  /**
   * @brief closeStream
   *
   * Releases all resources associated with the stream, if any
   */
  virtual void close() = 0;

  /**
   * Determines if the changeset has any unparsed changes
   *
   * @return true if the changeset has more changes; false otherwise
   */
  virtual bool hasMoreChanges() = 0;

  /**
   * Returns the next change in the changeset
   *
   * @return a changeset change
   */
  virtual Change readNextChange() = 0;

};

typedef shared_ptr<ChangeSetProvider> ChangeSetProviderPtr;

}

#endif // CHANGSETPROVIDER_H
