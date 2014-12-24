//
//  PhysicsWorld.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "GameObject.h"
#include "OrbitalTrajectory.h"
#include "Logging.h"

#ifdef USE_BULLET_COLLISIONS

#include <ofxBullet.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

#define SCENE_SIZE 500
#define MAX_OBJECTS 16000

class PhysicsImpl {
public:
    std::map<PhysicsObject*, btCollisionObject*> objectMap;

    typedef std::set<std::pair<PhysicsObject*, PhysicsObject*> > ContactSet;
    ContactSet* newContacts;
    ContactSet* oldContacts;
  
    PhysicsWorld& world;
    btCollisionConfiguration* bt_collision_configuration;
    btCollisionDispatcher* bt_dispatcher;
    btBroadphaseInterface* bt_broadphase;
    btCollisionWorld* bt_collision_world;
    
    PhysicsImpl(PhysicsWorld& w) : world(w) {
        bt_collision_configuration = new btDefaultCollisionConfiguration();
        bt_dispatcher = new btCollisionDispatcher(bt_collision_configuration);
        
        btScalar sscene_size = (btScalar) SCENE_SIZE;
        btVector3 worldAabbMin(-sscene_size, -sscene_size, -sscene_size);
        btVector3 worldAabbMax(sscene_size, sscene_size, sscene_size);
        bt_broadphase = new bt32BitAxisSweep3(worldAabbMin, worldAabbMax, MAX_OBJECTS, 0, true);
        
        bt_collision_world = new btCollisionWorld(bt_dispatcher, bt_broadphase, bt_collision_configuration);
        
        newContacts = new ContactSet();
        oldContacts = new ContactSet();
    }

    void addObject(PhysicsObject* obj) {

        btCollisionObject* co = new btCollisionObject;
        co->setUserPointer(obj);

        if (obj->collisionShape == CollisionSphere) {
            btSphereShape* shape = new btSphereShape(obj->getSize().x * 0.5);
            shape->setMargin(2.0);
            co->setCollisionShape(shape);
        } else if (obj->collisionShape == CollisionBox) {
            ofVec3f size = obj->getSize();
            btBoxShape* shape = new btBoxShape(btVector3(size.x * 0.5, size.y * 0.5, size.z * 0.5));
            shape->setMargin(2.0);
            co->setCollisionShape(shape);
        }
        
        updateCollisionObject(obj, co);
        
        bt_collision_world->addCollisionObject(co);
        objectMap.insert(std::pair<PhysicsObject*, btCollisionObject*>(obj, co));
    }
    
    void updateCollisionObject(PhysicsObject* obj) {
        auto it = objectMap.find(obj);
        if (it != objectMap.end()) {
            auto co = it->second;
            updateCollisionObject(obj, co);
        }
    }
    
    void updateCollisionObject(PhysicsObject* obj, btCollisionObject* co) {
        ofVec3f pos = obj->getPosition();
        ofVec3f size = obj->getSize();
        
        co->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
        // setLocalScaling is the wrong place.
        // if we want to have size-changing objects, we'll need to figure out the right place
        //co->getCollisionShape()->setLocalScaling(btVector3(size.x, size.y, size.z));
        
        btQuaternion btq;
        btq.setRotation(btVector3(0, 1, 0), obj->getRotation() * PI/180.0);
        co->getWorldTransform().setRotation(btq);
        
        if (obj->isDynamic()) {
            co->setCollisionFlags(co->getCollisionFlags() | ~btCollisionObject::CF_STATIC_OBJECT);
        } else {
            co->setCollisionFlags(co->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        }
    }
    
    void removeObject(PhysicsObject* obj) {
        auto it = objectMap.find(obj);
        if (it != objectMap.end()) {
            auto co = it->second;
            bt_collision_world->removeCollisionObject(co);
            delete co->getCollisionShape();
            delete co;
            objectMap.erase(it);
        }
    }
    
    BoundingBox getObjBoundingBox(PhysicsObject* obj) {
        BoundingBox bbox;
        
        auto it = objectMap.find(obj);
        if (it != objectMap.end()) {
            auto co = it->second;
            
            btTransform tr = co->getWorldTransform();
            btVector3 aabbMin, aabbMax;
            co->getCollisionShape()->getAabb(tr, aabbMin, aabbMax);
            
            btVector3 center;
            center = (aabbMin + aabbMax) * btScalar(0.5);
            bbox.center = ofVec3f(center.x(), center.y(), center.z());
            bbox.halfwidths = ofVec3f((aabbMax.x() - aabbMin.x()) * 0.5,
                                      (aabbMax.y() - aabbMin.y()) * 0.5,
                                      (aabbMax.z() - aabbMin.z()) * 0.5);
        }
        
        return bbox;
    }
    
    ~PhysicsImpl() {
        for (auto const &it : objectMap) {
            removeObject(it.first);
        }
        
        delete newContacts;
        delete oldContacts;
        
        delete bt_collision_world;
        delete bt_broadphase;
        delete bt_dispatcher;
        delete bt_collision_configuration;
    }

    void update() {
        for (auto const &it : objectMap) {
            auto obj = it.first;
            if (obj->isDynamic()) {
                obj->tick();
            }
        }
        performCollisionDetection();
    }
    
    void drawDebug() {
        for (auto const &it : objectMap) {
            ofSetColor(255, 0, 0);
            btVector3 pos = it.second->getWorldTransform().getOrigin();
            btVector3 size = it.second->getCollisionShape()->getLocalScaling();
            ofRect(pos.x()-3, pos.y()-3, size.x()+6, size.y()+6);
        }
    }
    
    void performCollisionDetection() {
        bt_collision_world->performDiscreteCollisionDetection();
        
        int numManifolds = bt_collision_world->getDispatcher()->getNumManifolds();
        for (int i = 0; i < numManifolds; i++) {
            btPersistentManifold* contactManifold = bt_collision_world->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
            const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
            contactManifold->refreshContactPoints(obA->getWorldTransform(), obB->getWorldTransform());
            int numContacts = contactManifold->getNumContacts();
            
            if (numContacts > 0) {
            
                auto obj1 = (PhysicsObject*)obA->getUserPointer();
                auto obj2 = (PhysicsObject*)obB->getUserPointer();

                
                if ((obj1->isDynamic() || obj2->isDynamic()) &&
                    obj1->thisGameObject->alive() &&
                    obj2->thisGameObject->alive())
                {
                    auto contact = std::make_pair(obj1, obj2);
                    newContacts->insert(contact);
                    if (oldContacts->find(contact) == oldContacts->end()) {
                        
                        ofVec3f normalFromBtoA;
                        ofVec3f contactPointOnA;
                        ofVec3f contactPointOnB;
                        double penDist = -9999;
                        
                        for (int j = 0; j < numContacts; j++) {
                            btManifoldPoint& pt = contactManifold->getContactPoint(j);
                            double ptdist = pt.getDistance();
                            if (ptdist > penDist) {
                                penDist = ptdist;
                                const btVector3& n = pt.m_normalWorldOnB;
                                normalFromBtoA = ofVec3f(n.x(), n.y(), n.z());
                                
                                const btVector3& posA = pt.getPositionWorldOnA();
                                btVector3 relA = obA->getWorldTransform().inverse() * posA;
                                contactPointOnA = ofVec3f(relA.x(), relA.y(), relA.z());

                                const btVector3& posB = pt.getPositionWorldOnB();
                                btVector3 relB = obB->getWorldTransform().inverse() * posB;
                                contactPointOnB = ofVec3f(relB.x(), relB.y(), relB.z());
                            }
                        }

                        CollisionArgs args;
                        args.a = obj1->thisGameObject;
                        args.b = obj2->thisGameObject;
                        args.normalOnA = -normalFromBtoA;
                        args.pointOnA = contactPointOnA;
                        args.pointOnB = contactPointOnB;
                        
                        ofLog(OF_LOG_VERBOSE) << "* Collision detected\n" << *obj1 << *obj2;
                        world.notifyCollision(args);
                    }
                }
            }
        }
        
        std::swap(oldContacts, newContacts);
        newContacts->clear();
    }
    
};

#else


class PhysicsImpl {
public:
    std::set<PhysicsObject*> objects;

    typedef std::set<std::pair<PhysicsObject*, PhysicsObject*> > ContactSet;
    ContactSet* newContacts;
    ContactSet* oldContacts;
    PhysicsWorld& world;
  
    PhysicsImpl(PhysicsWorld& w) : world(w) {
        newContacts = new ContactSet();
        oldContacts = new ContactSet();
    }
    
    ~PhysicsImpl() {
        delete newContacts;
        delete oldContacts;
    }
    
    void addObject(PhysicsObject* obj) {
        objects.insert(obj);
    }
    
    void updateCollisionObject(PhysicsObject* obj) {
    }
    
    void removeObject(PhysicsObject* obj) {
        auto it = objects.find(obj);
        if (it != objects.end()) {
            objects.erase(it);
        }
    }
    
    void update() {
        for (auto &obj : objects) {
            if (obj->isDynamic()) {
                obj->tick();
            }
        }
        performCollisionDetection();
    }
    
    BoundingBox getObjBoundingBox(PhysicsObject* obj) {
        return obj->getBoundingBox();
    }
    
    void performCollisionDetection() {
        
        // inefficient O(n^2) AABB for now
        // replace with sweep-and-prune or quadtrees or whatever if Bullet never works right
        auto iter1 = objects.begin();
        while (iter1 != objects.end()) {
            auto iter2 = iter1;
            ++iter2;
            while (iter2 != objects.end()) {
                auto obj1 = *iter1;
                auto obj2 = *iter2;
                
                // speed things up considerably by ignoring static-static collisions.
                // will need to adjust this if collisions that involve a paddle and a non-ball become necessary
                // (paddles are technically static, even though they move.)
                if ((obj1->isDynamic() || obj2->isDynamic()) &&
                    obj1->thisGameObject->physical() &&
                    obj2->thisGameObject->physical())
                {
                    
                    CollisionManifold m;
                    if (BoundingBox::testCollision(obj1->getBoundingBox(), obj2->getBoundingBox(), &m)) {
                        auto contact = std::make_pair(obj1, obj2);
                        newContacts->insert(contact);
                        if (oldContacts->find(contact) == oldContacts->end()) {
                            static CollisionArgs args;
                            args.a = obj1->thisGameObject;
                            args.b = obj2->thisGameObject;
                            args.normal = m.normal;

                            
                            world.notifyCollision(args);
                        }
                    }
                }
                ++iter2;
            }
            ++iter1;
        }
        std::swap(oldContacts, newContacts);
        newContacts->clear();
    }
    
};




#endif

void PhysicsWorld::setup() {
    _impl.reset(new PhysicsImpl(*this));
}

void PhysicsWorld::addObject(PhysicsObject* obj) {
    if (_impl.get()) {
        _impl->addObject(obj);
        obj->world = this;
        obj->isCollidable = true;
    }
}

void PhysicsWorld::removeObject(PhysicsObject* obj) {
    if (_impl.get()) {
        _impl->removeObject(obj);
    }
}

void PhysicsWorld::update() {
    if (_impl.get()) {
        _impl->update();
    }
}

void PhysicsWorld::updateCollisionObject(PhysicsObject* obj) {
    if (_impl.get()) {
        _impl->updateCollisionObject(obj);
    }
}

void PhysicsWorld::notifyCollision(CollisionArgs& args) {
    ofNotifyEvent(collisionEvent, args);
}

BoundingBox PhysicsWorld::getObjBoundingBox(PhysicsObject* obj) {
    if (_impl.get()) {
        return _impl->getObjBoundingBox(obj);
    }
}

