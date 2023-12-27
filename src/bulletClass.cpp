#include "bulletClass.hpp"

PhysicsObjectClass::PhysicsObjectClass()
{
    initializePhysics(); // I like to keep the initialization separate from the constructor
}

// This is the destructor for the PhysicsObjectClass to clean up the memory
PhysicsObjectClass::~PhysicsObjectClass()
{
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
}

void PhysicsObjectClass::initializePhysics()
{
    // Basic setup for Bullet Physics
    // Perhaps the most important part of this for N64 applications is the pool size
    // Due to the limited ram, we need to keep the pool size small
    // This should keep the size of the pool to 1MB total.
    // The default pool size is way too large for N64 applications and will throw an error.
    btDefaultCollisionConstructionInfo constructionInfo;
    constructionInfo.m_defaultMaxCollisionAlgorithmPoolSize = 512;
    constructionInfo.m_defaultMaxPersistentManifoldPoolSize = 512;

    collisionConfiguration = new btDefaultCollisionConfiguration(constructionInfo);
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    setGravity(btVector3(0, -30, 0)); // Set gravity to -30 on the Y axis to start
}

void PhysicsObjectClass::addRigidBody(btRigidBody *body)
{
    dynamicsWorld->addRigidBody(body);
}

void PhysicsObjectClass::stepSimulation(float deltaTime)
{
    dynamicsWorld->stepSimulation(1.f / deltaTime, 10); // Step the simulation at 30fps
}

void PhysicsObjectClass::setGravity(btVector3 gravity)
{
    dynamicsWorld->setGravity(gravity);
}

btVector3 PhysicsObjectClass::getPrismRigidBodyPosition()
{
    btTransform trans;
    prismRigidBody->getMotionState()->getWorldTransform(trans); // Use the passed prismBody instead of GetPrismRigidBody()

    btVector3 pos = trans.getOrigin();

    return pos;
}

btQuaternion PhysicsObjectClass::getPrismRigidBodyRotation()
{
    btTransform trans;

    prismRigidBody->getMotionState()->getWorldTransform(trans);

    btQuaternion rot = trans.getRotation();

    return rot;
}

void PhysicsObjectClass::createPrismRigidBody(int rigidBodySize, int startingHeight)
{
    // Similar to CreatePrism, but use the x, y, z parameters for the position
    // Create prism shape and body
    btConvexHullShape *prismShape = new btConvexHullShape();
    // Define vertices for the prism shape here

    // Front Face
    prismShape->addPoint(btVector3(0, rigidBodySize * 3, 0));
    prismShape->addPoint(btVector3(-rigidBodySize, -rigidBodySize, rigidBodySize));
    prismShape->addPoint(btVector3(rigidBodySize, -rigidBodySize, rigidBodySize));

    // Right Face
    prismShape->addPoint(btVector3(0, rigidBodySize * 3, 0));
    prismShape->addPoint(btVector3(rigidBodySize, -rigidBodySize, rigidBodySize));
    prismShape->addPoint(btVector3(rigidBodySize, -rigidBodySize, -rigidBodySize));

    // Back Face
    prismShape->addPoint(btVector3(0, rigidBodySize * 3, 0));
    prismShape->addPoint(btVector3(rigidBodySize, -rigidBodySize, -rigidBodySize));
    prismShape->addPoint(btVector3(-rigidBodySize, -rigidBodySize, -rigidBodySize));

    // Left Face
    prismShape->addPoint(btVector3(0, rigidBodySize * 3, 0));
    prismShape->addPoint(btVector3(-rigidBodySize, -rigidBodySize, -rigidBodySize));
    prismShape->addPoint(btVector3(-rigidBodySize, -rigidBodySize, rigidBodySize));

    // btCollisionShape *prismShape = new btBoxShape(btVector3(rigidBodySize, rigidBodySize, rigidBodySize)); // Example for a box-shaped prism

    // Set the starting position of the prism
    btDefaultMotionState *prismMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, startingHeight, 0)));

    btScalar mass = 5; // Mass of the prism (in kg) - raise to make the prism heavier
    btVector3 prismInertia(0, 0, 0);
    prismShape->calculateLocalInertia(mass, prismInertia);                                                       // Calculate the inertia of the prism
    btRigidBody::btRigidBodyConstructionInfo prismRigidBodyCI(mass, prismMotionState, prismShape, prismInertia); // I made the mass 100 so the prism is heavier
    prismRigidBody = new btRigidBody(prismRigidBodyCI);                                                          // Create the prism rigid body
    dynamicsWorld->addRigidBody(prismRigidBody);                                                                 // Add the prism to the physics world
    prismRigidBody->setRestitution(0.3f);                                                                        // Make the prism bouncy
    prismRigidBody->setFriction(0.5f);                                                                           // Make the prism slippery
    prismRigidBody->applyTorque(btVector3(1, 1, 5));                                                             // Zero out the torque to start

    // Apply an initial angular velocity to make the prism spin
    prismRigidBody->setAngularVelocity(btVector3(1, 2, 1));
}

void PhysicsObjectClass::resetPrismRigidBody()
{
    // Reset the prism rigid body to the starting position
    prismRigidBody->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 100, 0)));
}

void PhysicsObjectClass::createGroundRigidBody()
{
    // Define ground shape and body
    btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);                                                 // Create the ground shape
    btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))); // Set the starting position of the ground
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));             // Zero mass for static objects
    groundRigidBody = new btRigidBody(groundRigidBodyCI);                                                                          // Create the ground rigid body
    groundRigidBody->setRestitution(1.0f);                                                                                         // Make the ground bouncy
    dynamicsWorld->addRigidBody(groundRigidBody);                                                                                  // Add the ground to the world
}

void PhysicsObjectClass::updatePlaneRotation(float plane_rotationX, float plane_rotationY)
{

    // Set the prismRigidBody to active if it is not already
    prismRigidBody->activate();
    groundRigidBody->activate();
    // Update the motion state of the ground based on the plane_rotationX and plane_rotationY
    // convert the rotation to a quaternion
    btQuaternion quat;
    quat.setEuler(plane_rotationX, 0, plane_rotationY);
    // set the rotation of the ground rigid body
    groundRigidBody->setWorldTransform(btTransform(quat, btVector3(0, 0, 0)));
}
