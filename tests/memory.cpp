#include "../RTree.h"
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>
#include <limits>
#include <memory.h>
#include <stdio.h>
#ifdef WIN32
#include <crtdbg.h>
#endif // WIN32

using namespace ::testing;

// Use CRT Debug facility to dump memory leaks on app exit
#ifdef WIN32
// These two are for MSVS 2005 security consciousness until safe std lib funcs
// are available
#pragma warning(disable : 4996)  // Deprecated functions
#define _CRT_SECURE_NO_DEPRECATE // Allow old unsecure standard library
                                 // functions, Disable some 'warning C4996 -
                                 // function was deprecated'

// The following macros set and clear, respectively, given bits
// of the C runtime library debug flag, as specified by a bitmask.
#ifdef _DEBUG
#define SET_CRT_DEBUG_FIELD(a)                                                 \
  _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define CLEAR_CRT_DEBUG_FIELD(a)                                               \
  _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define SET_CRT_DEBUG_FIELD(a) ((void)0)
#define CLEAR_CRT_DEBUG_FIELD(a) ((void)0)
#endif
#endif // WIN32

//
// Get a random float b/n two values
// The returned value is >= min && < max (exclusive of max)
// Note this is a low precision random value since it is generated from an int.
//
static float RandFloat(float a_min, float a_max) {
  const float ooMax = 1.0f / (float)RAND_MAX;

  float retValue = ((float)rand() * ooMax * (a_max - a_min) + a_min);

  RTREE_ASSERT(retValue >= a_min && retValue < a_max); // Paranoid check

  return retValue;
}

/// Simplify handling of 3 dimensional coordinate
struct Vec3 {
  /// Default constructor
  Vec3() {}

  /// Construct from three elements
  Vec3(float a_x, float a_y, float a_z) {
    v[0] = a_x;
    v[1] = a_y;
    v[2] = a_z;
  }

  /// Add two vectors and return result
  Vec3 operator+(const Vec3 &a_other) const {
    return Vec3(v[0] + a_other.v[0], v[1] + a_other.v[1], v[2] + a_other.v[2]);
  }

  float v[3]; ///< 3 float components for axes or dimensions
};

static bool BoxesIntersect(const Vec3 &a_boxMinA, const Vec3 &a_boxMaxA,
                           const Vec3 &a_boxMinB, const Vec3 &a_boxMaxB) {
  for (int axis = 0; axis < 3; ++axis) {
    if (a_boxMinA.v[axis] > a_boxMaxB.v[axis] ||
        a_boxMaxA.v[axis] < a_boxMinB.v[axis]) {
      return false;
    }
  }
  return true;
}

/// A user type to test with, instead of a simple type such as an 'int'
struct SomeThing {
  SomeThing() { ++s_outstandingAllocs; }
  ~SomeThing() { --s_outstandingAllocs; }

  int m_creationCounter; ///< Just a number for identifying within test program
  Vec3 m_min, m_max;     ///< Minimal bounding rect, values must be known and
                         ///< constant in order to remove from RTree

  static int s_outstandingAllocs; ///< Count how many outstanding objects remain
};

/// Init static
int SomeThing::s_outstandingAllocs = 0;

TEST(MemoryTest, NoLeak) {
  // Number of objects in test set, must be > FRAC_OBJECTS for this test
  const int NUM_OBJECTS = 40;
  const int FRAC_OBJECTS = 4;
  const float MAX_WORLDSIZE = 10.0f;
  const float FRAC_WORLDSIZE = MAX_WORLDSIZE / 2;

  // typedef the RTree useage just for conveniance with iteration
  typedef RTree<SomeThing *, float, 3> SomeThingTree;

  EXPECT_GT(NUM_OBJECTS, FRAC_OBJECTS);

  int index; // general purpose counter, declared here because MSVC 6 is not
             // ansi compliant with 'for' loops.
  SomeThing
      *thingArray[NUM_OBJECTS * 2]; // Store objects in another container to
                                    // test with, sized larger than we need

  memset(thingArray, 0,
         sizeof(thingArray)); // Nullify array, size is known here

  // Create intance of RTree
  SomeThingTree tree;

  // Add some nodes
  int counter = 0;
  for (index = 0; index < NUM_OBJECTS; ++index) {
    SomeThing *newThing = new SomeThing;

    newThing->m_creationCounter = counter++;
    newThing->m_min = Vec3(RandFloat(-MAX_WORLDSIZE, MAX_WORLDSIZE),
                           RandFloat(-MAX_WORLDSIZE, MAX_WORLDSIZE),
                           RandFloat(-MAX_WORLDSIZE, MAX_WORLDSIZE));
    Vec3 extent =
        Vec3(RandFloat(0, FRAC_WORLDSIZE), RandFloat(0, FRAC_WORLDSIZE),
             RandFloat(0, FRAC_WORLDSIZE));
    newThing->m_max = newThing->m_min + extent;

    thingArray[counter - 1] = newThing;

    tree.Insert(newThing->m_min.v, newThing->m_max.v, newThing);
  }

  EXPECT_EQ(tree.Count(), NUM_OBJECTS);

  int numToDelete = NUM_OBJECTS / FRAC_OBJECTS;
  int numToStep = FRAC_OBJECTS;

  // Delete some nodes
  for (index = 0; index < NUM_OBJECTS; index += numToStep) {
    SomeThing *curThing = thingArray[index];

    if (curThing) {
      tree.Remove(curThing->m_min.v, curThing->m_max.v, curThing);

      delete curThing;
      thingArray[index] = NULL;
    }
  }

  EXPECT_EQ(tree.Count(), NUM_OBJECTS - numToDelete);

  // Add some more nodes
  for (index = 0; index < numToDelete; ++index) {
    SomeThing *newThing = new SomeThing;

    newThing->m_creationCounter = counter++;
    newThing->m_min = Vec3(RandFloat(-MAX_WORLDSIZE, MAX_WORLDSIZE),
                           RandFloat(-MAX_WORLDSIZE, MAX_WORLDSIZE),
                           RandFloat(-MAX_WORLDSIZE, MAX_WORLDSIZE));
    Vec3 extent =
        Vec3(RandFloat(0, FRAC_WORLDSIZE), RandFloat(0, FRAC_WORLDSIZE),
             RandFloat(0, FRAC_WORLDSIZE));
    newThing->m_max = newThing->m_min + extent;

    thingArray[counter - 1] = newThing;

    tree.Insert(newThing->m_min.v, newThing->m_max.v, newThing);
  }

  EXPECT_EQ(tree.Count(), NUM_OBJECTS);

  MockFunction<bool(SomeThing *)> mock_function;
  ON_CALL(mock_function, Call).WillByDefault(Return(true));
  EXPECT_CALL(mock_function, Call(_)).Times(tree.Count());

  Vec3 searchMin(-MAX_WORLDSIZE, -MAX_WORLDSIZE, -MAX_WORLDSIZE);
  Vec3 searchMax(MAX_WORLDSIZE, MAX_WORLDSIZE, MAX_WORLDSIZE);
  tree.Search(searchMin.v, searchMax.v, mock_function.AsStdFunction());

  // List values.  Iterator is NOT delete safe
  int findCounter = 0;
  SomeThingTree::Iterator it;
  for (tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it)) {
    SomeThing *curThing = tree.GetAt(it);

    if (BoxesIntersect(searchMin, searchMax, curThing->m_min,
                       curThing->m_max)) {
      findCounter++;
    }
  }

  EXPECT_EQ(tree.Count(), findCounter);

  // Delete our nodes, NOTE, we are NOT deleting the tree nodes, just our data
  // of course the tree will now contain invalid pointers that must not be used
  // any more.
  for (tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it)) {
    SomeThing *removeElem = tree.GetAt(it);
    if (removeElem) {
      delete removeElem;
    }
  }

  // Remove all contents (This would have happened automatically during
  // destructor)
  tree.RemoveAll();

  if (SomeThing::s_outstandingAllocs > 0) {
    FAIL() << "Memory leak! "
           << "s_outstandingAllocs = " << SomeThing::s_outstandingAllocs;
  }

#ifdef WIN32
  // Use CRT Debug facility to dump memory leaks on app exit
  SET_CRT_DEBUG_FIELD(_CRTDBG_LEAK_CHECK_DF);
#endif // WIN32
}
