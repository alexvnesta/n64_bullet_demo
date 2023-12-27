#pragma once
#include <bullet/btBulletDynamicsCommon.h> // This is the main Bullet include file, contains most common include files.
#include <vector>                          // We will use a vector to store the rigid bodies
class PhysicsObjectClass
{
public:
    PhysicsObjectClass();                                                                                                                                                 // Constructor
    ~PhysicsObjectClass();                                                                                                                                                // Destructor
    void initializePhysics();                                                                                                                                             // Initialize Bullet Physics
    void setGravity(btVector3 gravity);                                                                                                                                   // Set gravity
    void createPrismRigidBody(int rigidBodySize = 3, int startingHeight = 100, btVector3 startPosition = btVector3(std::rand() % 140 - 70, 100, std::rand() % 140 - 70)); // Create a prism rigid body - x and z are random between -70 and +70, y is 100
    void createGroundRigidBody();                                                                                                                                         // Create a ground rigid body
    void stepSimulation(float deltaTime = 30.0f);                                                                                                                         // Step the simulation
    void updatePlaneRotation(float plane_rotationX, float plane_rotationY);                                                                                               // Update the rotation of the plane
    void resetPrismRigidBodies();                                                                                                                                         // Reset the prism rigid body to the starting position
    btVector3 getPrismRigidBodyPosition(btRigidBody *rigidBody);                                                                                                          // Get the position of the rigid body
    btQuaternion getPrismRigidBodyRotation(btRigidBody *rigidBody);                                                                                                       // Get the rotation of the rigid body - we are using a quaternion here
    const std::vector<btRigidBody *> getPrismRigidBodies() const
    {
        return prismRigidBodies;
    };                                   // Get the vector of prism rigid bodies
    btDynamicsWorld *GetDynamicsWorld(); // Get the dynamics world - example if you want to get the world from inside the class

private:
    btCollisionConfiguration *collisionConfiguration;
    btDispatcher *dispatcher;
    btBroadphaseInterface *broadphase;
    btConstraintSolver *solver;
    btDynamicsWorld *dynamicsWorld;
    // btRigidBody *prismRigidBody; // Legacy - we will use a vector of rigid bodies instead so that we can draw multiple prisms
    btRigidBody *groundRigidBody;
    std::vector<btRigidBody *> prismRigidBodies; // Vector of prism rigid bodies - we will use this to draw multiple prisms
};
