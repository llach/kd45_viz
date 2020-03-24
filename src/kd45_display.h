#ifndef KD45_DISPLAY_H
#define KD45_DISPLAY_H

#ifndef Q_MOC_RUN
#include <boost/circular_buffer.hpp>

#include <rviz/message_filter_display.h>
#include <tactile_msgs/TactileState.h>
#include <rviz/ogre_helpers/arrow.h>
#endif

namespace Ogre
{
class SceneNode;
}

namespace rviz
{
class ColorProperty;
class FloatProperty;
class IntProperty;
}

// All the source in this plugin is in its own namespace.  This is not
// required but is good practice.
namespace kd45_viz
{

class KD45Visual;

// Here we declare our new subclass of rviz::Display.  Every display
// which can be listed in the "Displays" panel is a subclass of
// rviz::Display.
//
// KD45Display will show a 3D arrow showing the direction and magnitude
// of the KD45 force sensor.

class KD45Display: public rviz::MessageFilterDisplay<tactile_msgs::TactileState>
{
Q_OBJECT
public:
  // Constructor.  pluginlib::ClassLoader creates instances by calling
  // the default constructor, so make sure you have one.
  KD45Display();
  virtual ~KD45Display();

protected:
  virtual void onInitialize();
  virtual void reset();

  // These Qt slots get connected to signals indicating changes in the user-editable properties.
private Q_SLOTS:
  void updateColorAndAlpha();
  void updateArrowScale();

  // Function to handle an incoming ROS message.
private:
  void processMessage( const tactile_msgs::TactileState::ConstPtr& msg );
    Ogre::SceneNode* right_frame_;
    Ogre::SceneNode* left_frame_;

    std::shared_ptr<rviz::Arrow> right_arrow_;
    std::shared_ptr<rviz::Arrow> left_arrow_;

    float arrow_scale_;

  // User-editable property variables.
  rviz::ColorProperty* color_property_;
  rviz::FloatProperty* alpha_property_;
    rviz::FloatProperty* arrow_scale_property_;
};

}

#endif
