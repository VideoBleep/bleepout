//
//  Geometry.cpp
//  bleepout
//
//  Created by Dewb on 12/18/14.
//
//

#include "Geometry.h"

void addFaceToMesh(ofPtr<ofMesh>& mesh, const vector<ofVec3f>& points) {
    int count = mesh->getVertices().size();
    mesh->addVertices(points);
    mesh->addIndex(count + points.size() - 1);
    mesh->addIndex(count);
    mesh->addIndex(count + 1);
    for (int i = count + 1; i < count + points.size() - 2; i += 2) {
        mesh->addIndex(i);
        mesh->addIndex(i + 1);
        mesh->addIndex(i + 2);
    }
}

void addLineToMesh(ofPtr<ofMesh>& mesh, const ofVec3f& pt1, const ofVec3f& pt2) {
    int count = mesh->getVertices().size();
    mesh->addVertex(pt1);
    mesh->addVertex(pt2);
    mesh->addIndex(count);
    mesh->addIndex(count + 1);
}

void addLinesToMesh(ofPtr<ofMesh>& mesh, const vector<ofVec3f>& points, bool close = false) {
    int count = mesh->getVertices().size();
    mesh->addVertices(points);
    for (int i = count; i < count + points.size() - 1; i++) {
        mesh->addIndex(i);
        mesh->addIndex(i + 1);
    }
    if (close) {
        mesh->addIndex(count + points.size() - 1);
        mesh->addIndex(count);
    }
}

void Sweep::generate(bool merge) {
    mesh.reset(new ofMesh());
    outline.reset(new ofMesh());
    
    mesh->setMode(OF_PRIMITIVE_TRIANGLES);
    mesh->enableIndices();
    outline->setMode(OF_PRIMITIVE_LINES);
    outline->enableIndices();
    
    addFaceToMesh(mesh, startFace.points);
    addLinesToMesh(outline, startFace.points, true);
    
    vector<ofVec3f> currRing(startFace.points);
    vector<ofVec3f> nextRing(startFace.points);
    vector<ofVec3f> face;
    face.resize(4);
    for (int pathstep = 1; pathstep < path.points.size(); pathstep++) {
        for (int pt = 0; pt < nextRing.size(); pt++) {
            nextRing[pt] += path.points[pathstep] - path.points[pathstep - 1];
        }
        for (int pt = 0; pt < nextRing.size(); pt += 1) {
            face[0] = currRing[pt];
            face[1] = currRing[(pt+1) % currRing.size()];
            face[2] = nextRing[(pt+1) % nextRing.size()];
            face[3] = nextRing[pt];
            
            addFaceToMesh(mesh, face);
            addLineToMesh(outline, currRing[pt], nextRing[pt]);
        }
        currRing = nextRing;
    }
    
    addFaceToMesh(mesh, nextRing);
    addLinesToMesh(outline, nextRing, true);
    if (merge) {
        mesh->mergeDuplicateVertices();
    }
    return mesh;
}
