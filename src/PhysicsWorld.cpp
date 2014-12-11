//
//  PhysicsWorld.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#define USE_BULLET_COLLISIONS 0

#include "PhysicsWorld.h"
#include "PhysicsObject.h"
#include "OrbitalTrajectory.h"

#if USE_BULLET_COLLISIONS

#include <ofxBullet.h>

class PhysicsImpl {
public:
    std::map<PhysicsObject*, btCollisionObject*> objectMap;
    
    btCollisionConfiguration* bt_collision_configuration;
    btCollisionDispatcher* bt_dispatcher;
    btBroadphaseInterface* bt_broadphase;
    btCollisionWorld* bt_collision_world;
    
    double scene_size = 500;
    unsigned int max_objects = 16000;
    
    PhysicsImpl() {
        bt_collision_configuration = new btDefaultCollisionConfiguration();
        bt_dispatcher = new btCollisionDispatcher(bt_collision_configuration);
        
        btScalar sscene_size = (btScalar) scene_size;
        btVector3 worldAabbMin(-sscene_size, -sscene_size, -sscene_size);
        btVector3 worldAabbMax(sscene_size, sscene_size, sscene_size);
        bt_broadphase = new bt32BitAxisSweep3(worldAabbMin, worldAabbMax, max_objects, 0, true);
        
        bt_collision_world = new btCollisionWorld(bt_dispatcher, bt_broadphase, bt_collision_configuration);
    }

    void addObject(PhysicsObject* obj) {
        ofVec3f pos = obj->getPosition();

        btCollisionObject* co = new btCollisionObject;
        co->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
        
        if (obj->collisionShape == CollisionSphere) {
            btSphereShape* shape = new btSphereShape(obj->getSize().x);
            shape->setMargin(10);
            co->setCollisionShape(shape);
        } else if (obj->collisionShape == CollisionBox) {
            ofVec3f size = obj->getSize();
            btBoxShape* shape = new btBoxShape(btVector3(size.x, size.y, size.z));
            co->setCollisionShape(shape);
        }
        co->setUserPointer(obj);
        bt_collision_world->addCollisionObject(co);
        objectMap.insert(std::pair<PhysicsObject*, btCollisionObject*>(obj, co));
    }
    
    void updateCollisionObject(PhysicsObject* obj) {
        auto it = objectMap.find(obj);
        if (it != objectMap.end()) {
            auto co = it->second;
            ofVec3f pos = obj->getPosition();
            ofVec3f size = obj->getSize();
            co->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
            co->getCollisionShape()->setLocalScaling(btVector3(size.x, size.y, size.z));
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
    
    ~PhysicsImpl() {
        for (auto const &it : objectMap) {
            removeObject(it.first);
        }
    }

    void update() {
        for (auto const &it : objectMap) {
            auto obj = it.first;
            if (obj->isDynamic()) {
                obj->trajectory->tick();
                obj->setPosition(obj->trajectory->getPosition());
            }
            ofPushStyle();
            ofNoFill();
            ofSetColor(255, 0, 0);
            btVector3 pos = it.second->getWorldTransform().getOrigin();
            btVector3 size = it.second->getCollisionShape()->getLocalScaling();
            ofRect(pos.x()-3, pos.y()-3, size.x()+6, size.y()+6);
            ofPopStyle();
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
    
    class MyContactCallback : public btCollisionWorld::ContactResultCallback {
    public:
        virtual btScalar addSingleResult(btManifoldPoint& cp,
                                         const btCollisionObjectWrapper* colObj0,int partId0,int index0,
                                         const btCollisionObjectWrapper* colObj1,int partId1,int index1)
        {
            btVector3 pt1 = cp.m_localPointA;
            btVector3 pt2 = cp.m_localPointB;
            return 0; // There was a planned purpose for the return value of addSingleResult, but it is not used so you can ignore it.
        }
    };
    
    void performCollisionDetection() {
        bt_collision_world->performDiscreteCollisionDetection();
        
        if (objectMap.size()) {
            MyContactCallback contactCallback;
            bt_collision_world->contactTest(objectMap.begin()->second, contactCallback);
        }
        
        int numManifolds = bt_collision_world->getDispatcher()->getNumManifolds();
        for (int i = 0; i < numManifolds; i++) {
            btPersistentManifold* contactManifold = bt_collision_world->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
            const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
            contactManifold->refreshContactPoints(obA->getWorldTransform(), obB->getWorldTransform());
            int numContacts = contactManifold->getNumContacts();
            
            for (int j = 0; j < numContacts; j++) {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                btVector3 ptA = pt.getPositionWorldOnA();
                btVector3 ptB = pt.getPositionWorldOnB();
                double ptdist = pt.getDistance();
            }
        }
    }
    
};

#else


class PhysicsImpl {
public:
    std::set<PhysicsObject*> objects;

    typedef std::set<std::pair<PhysicsObject*, PhysicsObject*> > ContactSet;
    ContactSet* newContacts;
    ContactSet* oldContacts;
    
    PhysicsImpl() {
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
                if (BoundingBox::testCollision(obj1->getBoundingBox(), obj2->getBoundingBox())) {
                    auto contact = std::make_pair(obj1, obj2);
                    newContacts->insert(contact);
                    if (oldContacts->find(contact) == oldContacts->end()) {
                        static CollisionArgs args;
                        args.a = obj1->thisGameObject;
                        args.b = obj2->thisGameObject;
                        ofNotifyEvent(PhysicsWorld::collisionEvent, args);
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
    _impl.reset(new PhysicsImpl());
}

void PhysicsWorld::addObject(PhysicsObject* obj) {
    if (_impl.get()) {
        _impl->addObject(obj);
        obj->world = this;
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

void PhysicsWorld::notifyCollision(GameObject* a, GameObject* b) {
    static CollisionArgs args;
    args.a = a;
    args.b = b;
    ofNotifyEvent(collisionEvent, args, this);
}

ofEvent<CollisionArgs> PhysicsWorld::collisionEvent;

