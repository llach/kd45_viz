#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>

#include <tf/transform_listener.h>

#include <rviz/visualization_manager.h>
#include <rviz/properties/color_property.h>
#include <rviz/properties/float_property.h>
#include <rviz/properties/int_property.h>
#include <rviz/frame_manager.h>

#include "kd45_visual.h"

#include "kd45_display.h"

namespace kd45_viz
{

KD45Display::KD45Display()
{
  color_property_ = new rviz::ColorProperty( "Color", QColor( 204, 51, 204 ),
                                             "Color to draw the force arrows.",
                                             this, SLOT( updateColorAndAlpha() ));

  alpha_property_ = new rviz::FloatProperty( "Alpha", 1.0,
                                             "0 is fully transparent, 1.0 is fully opaque.",
                                             this, SLOT( updateColorAndAlpha() ));
}

// After the top-level rviz::Display::initialize() does its own setup,
// it calls the subclass's onInitialize() function.  This is where we
// instantiate all the workings of the class.  We make sure to also
// call our immediate super-class's onInitialize() function, since it
// does important stuff setting up the message filter.
//
//  Note that "MFDClass" is a typedef of
// ``MessageFilterDisplay<message type>``, to save typing that long
// templated class name every time you need to refer to the
// superclass.
void KD45Display::onInitialize()
{
  MFDClass::onInitialize();
}

KD45Display::~KD45Display()
{
}

// Clear the visuals by deleting their objects.
void KD45Display::reset()
{
  MFDClass::reset();
    visual_left_.reset();
    visual_right_.reset();
}

// Set the current color and alpha values for each visual.
void KD45Display::updateColorAndAlpha()
{
  float alpha = alpha_property_->getFloat();
  Ogre::ColourValue color = color_property_->getOgreColor();

    visual_left_->setColor( color.r, color.g, color.b, alpha );
    visual_right_->setColor( color.r, color.g, color.b, alpha );
}

// This is our callback to handle an incoming message.
void KD45Display::processMessage( const tactile_msgs::TactileState::ConstPtr& msg )
{
    for (auto& c: msg->sensors) {

        std::shared_ptr<KD45Visual> v = nullptr;
        std::string sensor_frame = "";
        if (c.name == "left_gripper_tactile") {
            std::cout << "left" << std::endl;
            v = visual_left_;
            sensor_frame = "kd45_left_finger_link";
        } else {
            v = visual_right_;
            std::cout << "right" << std::endl;
            sensor_frame = "kd45_right_finger_link";
        }

        Ogre::Quaternion orientation;
        Ogre::Vector3 position;
        if (!context_->getFrameManager()->getTransform(sensor_frame,
                                                       msg->header.stamp,
                                                       position, orientation)) {
            ROS_DEBUG("Error transforming from frame '%s' to frame '%s'",
                      msg->header.frame_id.c_str(), qPrintable(fixed_frame_));
            // std::cout << "Error transforming from frame " << sensor_frame << " to frame " << qPrintable(fixed_frame_) << std::endl;
            return;
        }
        std::cout << position << " | " << orientation <<std::endl;

        v.reset(new KD45Visual(context_->getSceneManager(), scene_node_));
        std::cout << v << std::endl;

        // Now set or update the contents of the chosen visual.
        v->setMessage(msg);
        v->setFramePosition(position);
        v->setFrameOrientation(orientation);

        float alpha = alpha_property_->getFloat();
        Ogre::ColourValue color = color_property_->getOgreColor();
        v->setColor(color.r, color.g, color.b, alpha);
    }
}

}

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(kd45_viz::KD45Display,rviz::Display )
