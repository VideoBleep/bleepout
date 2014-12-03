//
//  Player.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Player__
#define __bleepout__Player__

typedef int PlayerId;

class Player {
public:
  PlayerId id() const { return _id; }
private:
  PlayerId _id;
};

#endif /* defined(__bleepout__Player__) */
