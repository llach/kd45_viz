#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>

#include <rviz/ogre_helpers/arrow.h>

#include "kd45_visual.h"

namespace kd45_viz
{

KD45Visual::KD45Visual( Ogre::SceneManager* scene_manager, Ogre::SceneNode* parent_node )
{
  scene_manager_ = scene_manager;

  // Ogre::SceneNode s form a tree, with each node storing the
  // transform (position and orientation) of itself relative to its
  // parent.  Ogre does the math of combining those transforms when it
  // is time to render.
  //
  // Here we create a node to store the pose of the force frame
  // relative to the RViz fixed frame.
  frame_node_ = parent_node->createChildSceneNode();

  // We create the arrow object within the frame node so that we can
  // set its position and direction relative to its header frame.
  acceleration_arrow_.reset(new rviz::Arrow( scene_manager_, frame_node_ ));
}

KD45Visual::~KD45Visual()
{
  // Destroy the frame node since we don't need it anymore.
  scene_manager_->destroySceneNode( frame_node_ );
}

void KD45Visual::setMessage( const tactile_msgs::TactileState::ConstPtr& msg )
{
//    todo
//  const geometry_msgs::Vector3& a = msg->linear_acceleration;

  // Convert the geometry_msgs::Vector3 to an Ogre::Vector3.
//  Ogre::Vector3 acc( a.x, a.y, a.z );
//
//  // Find the magnitude of the acceleration vector.
//  float length = acc.length();
//
//  // Scale the arrow's thickness in each dimension along with its length.
//  Ogre::Vector3 scale( length, length, length );
//  acceleration_arrow_->setScale( scale );
//
//  // Set the orientation of the arrow to match the direction of the
//  // acceleration vector.
//  acceleration_arrow_->setDirection( acc );
}

// Position and orientation are passed through to the SceneNode.
void KD45Visual::setFramePosition( const Ogre::Vector3& position )
{
  frame_node_->setPosition( position );
}

void KD45Visual::setFrameOrientation( const Ogre::Quaternion& orientation )
{
  frame_node_->setOrientation( orientation );
}

// Color is passed through to the Arrow object.
void KD45Visual::setColor( float r, float g, float b, float a )
{
  acceleration_arrow_->setColor( r, g, b, a );
}

}

