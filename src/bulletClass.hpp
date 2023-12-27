#pragma once
#include <bullet/btBulletDynamicsCommon.h> // This is the main Bullet include file, contains most common include files.

class PhysicsObjectClass
{
public:
    PhysicsObjectClass();                                                       // Constructor
    ~PhysicsObjectClass();                                                      // Destructor
    void initializePhysics();                                                   // Initialize Bullet Physics
    void addRigidBody(btRigidBody *body);                                       // Add a rigid body to the world
    void setGravity(btVector3 gravity);                                         // Set gravity
    void createPrismRigidBody(int rigidBodySize = 3, int startingHeight = 100); // Create a prism rigid body
    void createGroundRigidBody();                                               // Create a ground rigid body
    void stepSimulation(float deltaTime = 60.0f);                               // Step the simulation
    void updatePlaneRotation(float plane_rotationX, float plane_rotationY);     // Update the rotation of the plane
    void resetPrismRigidBody();                                                 // Reset the prism rigid body to the starting position
    btVector3 getPrismRigidBodyPosition();                                      // Get the position of the rigid body
    btQuaternion getPrismRigidBodyRotation();                                   // Get the rotation of the rigid body - we are using a quaternion here

    btDynamicsWorld *GetDynamicsWorld(); // Get the dynamics world - example if you want to get the world from inside the class

private:
    btCollisionConfiguration *collisionConfiguration;
    btDispatcher *dispatcher;
    btBroadphaseInterface *broadphase;
    btConstraintSolver *solver;
    btDynamicsWorld *dynamicsWorld;
    btRigidBody *prismRigidBody;
    btRigidBody *groundRigidBody;
};
