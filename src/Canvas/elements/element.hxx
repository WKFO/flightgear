// Interface for 2D canvas element
//
// Copyright (C) 2012  Thomas Geymayer <tomgey@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef CANVAS_ELEMENT_HXX_
#define CANVAS_ELEMENT_HXX_

#include <simgear/props/props.hxx>
#include <osg/MatrixTransform>
#include <simgear/misc/stdint.hxx> // for uint32_t

namespace osg
{
  class Drawable;
}

namespace canvas
{

  class Element:
    private SGPropertyChangeListener
  {
    public:
      virtual ~Element() = 0;
      SGPropertyNode* getPropertyNode();

      /**
       * Called every frame to update internal state
       *
       * @param dt  Frame time in seconds
       */
      virtual void update(double dt);

      osg::ref_ptr<osg::MatrixTransform> getMatrixTransform();

    protected:

      enum Attributes
      {
        COLOR           = 0x0001,
        COLOR_FILL      = 0x0002,
        BOUNDING_BOX    = 0x0004
      };

      enum TransformType
      {
        TT_NONE,
        TT_MATRIX,
        TT_TRANSLATE,
        TT_ROTATE,
        TT_SCALE
      };

      SGPropertyNode *_node;
      osg::Drawable  *_drawable;

      uint32_t _attributes_used;
      uint32_t _attributes_dirty;

      bool _transform_dirty;
      osg::ref_ptr<osg::MatrixTransform>    _transform;
      std::vector<TransformType>            _transform_types;

      SGPropertyNode    *_bounding_box[4]; ///<! x-min, y-min, x-max, y-max
      SGPropertyNode    *_color[3];
      SGPropertyNode    *_color_fill[3];

      Element(SGPropertyNode* node, uint32_t attributes_used = 0);

      virtual void childAdded(SGPropertyNode * child)  {}
      virtual void childRemoved(SGPropertyNode * child){}
      virtual void childChanged(SGPropertyNode * child){}

      virtual void colorChanged(const osg::Vec4& color)  {}
      virtual void colorFillChanged(const osg::Vec4& color){}

      void linkColorNodes( const char* name,
                           SGPropertyNode** nodes,
                           const osg::Vec4& def = osg::Vec4(1,1,0,1) );

    private:
      Element(const Element&);// = delete

      virtual void childAdded( SGPropertyNode * parent,
                               SGPropertyNode * child );
      virtual void childRemoved( SGPropertyNode * parent,
                                 SGPropertyNode * child );
      virtual void valueChanged(SGPropertyNode * child);
  };

}  // namespace canvas

#endif /* CANVAS_ELEMENT_HXX_ */