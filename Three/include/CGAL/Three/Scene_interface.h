#ifndef SCENE_INTERFACE_H
#define SCENE_INTERFACE_H

#include <QString>
#include <QColor>
#include <QList>
#include <algorithm>
#include <cmath>

class Scene_item;

// OpenGL rendering mode
/** \file Scene_interface.h
 * Contains the class Scene_interface and the enum RenderingMode.
 */
/*!
 * \brief The RenderingMode enum
 * Can be Points, PointsPlusNormals, Wireframe, Flat, FlatPlusEdges, or Gouraud.
 * - Points, PointsPlusNormals, and Wireframe have no light model.
 * - Flat and FlatPlusEdges use a basic light model with one normal per facet.
 * - Gouraud use the same light model but with one normal per vertex.
 */
enum RenderingMode { Points = 0,
                     PointsPlusNormals,
                     Splatting,
                     Wireframe, 
                     Flat,
                     FlatPlusEdges,
                     Gouraud,
                     LastRenderingMode = Gouraud,
                     NumberOfRenderingMode = LastRenderingMode+1 };


namespace CGAL {
namespace Three{
/*!
 * This is the class given to the plugins to interact with the scene.
 * */
class Scene_interface {
public:
    //!Contains the coordinates for the two extremities of the diagonal of a bounding box.
    //! Those points are respectively the left bottom front vertex and the right upper back vertex.
  struct Bbox {
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;
    Bbox(const double _xmin,const double _ymin,const double _zmin,
         const double _xmax,const double _ymax,const double _zmax)
      : xmin(_xmin), ymin(_ymin), zmin(_zmin),
        xmax(_xmax), ymax(_ymax), zmax(_zmax)
    {
    }
    //!The default constructor of a bounding box.
    //! The two vertices are (0.0,0.0,0.0) and (1.0,1.0,1.0).
    Bbox()
      : xmin(0.0), ymin(0.0), zmin(0.0),
        xmax(1.0), ymax(1.0), zmax(1.0)
    {
    }
    //!Sums the coordinates of each vertex one by one.
    Bbox operator+(const Bbox& b) const {
      return Bbox((std::min)(xmin, b.xmin),
                  (std::min)(ymin, b.ymin),
                  (std::min)(zmin, b.zmin),
                  (std::max)(xmax, b.xmax),
                  (std::max)(ymax, b.ymax),
                  (std::max)(zmax, b.zmax));
    }

    //!@returns true if all the coordinates of the two bounding boxes are the same.
    bool operator==(const Bbox&b) const{
      return
        xmin==b.xmin && xmax==b.xmax &&
        ymin==b.ymin && ymax==b.ymax &&
        zmin==b.zmin && zmax==b.zmax;
    }
    //!@returns the opposite of ==.
    //!@see #operator==
    bool operator!=(const Bbox& b) const{
      return !(*this == b);
    }
    //!@returns the difference between xmax and xmin.
    double width() const { return xmax-xmin; }
    //!@returns the difference between ymax and ymin.
    double height() const { return ymax-ymin; }
    //!@returns the difference between zmax and zmin.
    double depth() const { return zmax-zmin; }
    //!@returns the length of the diagonal of the bounding box.
    double diagonal_length() const
    {
      return std::sqrt(width()*width() + height()*height() + depth()*depth());
    }

  }; // struct BBox (ad hoc class, does not depend on CGAL kernels

  //!Integer used for the index of an Scene_item.
  //!@see Scene_item
  typedef int Item_id;
  //!Virtual destructor
  virtual ~Scene_interface() {};
  //!Adds an item to the list of items. Must be overloaded.
  //!@returns the index of the new item.
  virtual Item_id addItem(Scene_item* item) = 0;
  //!Sets an item as the one at the specified index .
  //!If emit_item_about_to_be_destroyed is set to true, emits
  //!an itemAboutToBeDestroyed signal. Must be overloaded.
  //!@returns a pointer to the old item.
  virtual Scene_item* replaceItem(Item_id, Scene_item*, bool emit_item_about_to_be_destroyed = false) = 0;


  /*! Erases an item in the list. Must be overloaded.
   * @returns the index of the item just before the one that is erased,
   * or just after.
   * @returns -1 if the list is empty.*/
  virtual Item_id erase(Item_id) = 0;

  /*! Duplicate a scene item. Must be overloaded.
   * @returns the ID of the new item (-1 on error).
   */
    virtual Item_id duplicate(Item_id) = 0;

  // Accessors (getters)
  //!Must be overloaded.
  //!@returns the number of items.
  virtual int numberOfEntries() const = 0;
  //!Must be overloaded.
  //! @returns the item at the target index.
  virtual Scene_item* item(Item_id) const = 0;
  //!Must be overloaded.
  //! @returns the id of the target item.
  virtual Item_id item_id(Scene_item*) const = 0;
  //!Must be overloaded.
  //!@returns the currently selected item's index.
  virtual Item_id mainSelectionIndex() const = 0;
  //!Must be overloaded.
  //!@returns the list of currently selected items indices.
  virtual QList<Item_id> selectionIndices() const = 0;
  //!Must be overloaded.
  //!@returns the index of the Item_A
  virtual Item_id selectionAindex() const = 0;
  //!Must be overloaded.
  //!@returns the index of the Item_B
  virtual Item_id selectionBindex() const = 0;

  //! Must be overloaded.
  //!@returns the scene bounding box
  virtual Bbox bbox() const = 0;
  //!Must be overloaded.
  //!@returns the length of the bounding box's diagonal.
  virtual double len_diagonal() const = 0;

public:
  /*! Notifies the scene that an item was modified. Must be overloaded.*/
  virtual void itemChanged(Item_id i) = 0; 
  /*! Notifies the scene that an item was modified. Must be overloaded.*/
  virtual void itemChanged(Scene_item*) = 0;

  /*! Selects an item. Must be overloaded.*/
  virtual void setSelectedItem(Item_id) = 0;
  
}; // end interface Scene_interface
}
}

#endif // SCENE_INTERFACE_H
