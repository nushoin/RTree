#include "../RTree.h"
#include "util.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <vector>

using namespace ::testing;

typedef int ValueType;

typedef RectTemplate<int> Rect;

const std::vector<Rect> rects = {
    Rect(0, 0, 2, 2), // xmin, ymin, xmax, ymax (for 2 dimensional RTree)
    Rect(5, 5, 7, 7),
    Rect(8, 5, 9, 6),
    Rect(7, 1, 9, 2),
};

const std::vector<ValueType> values = {0, 2, 3, 5};

int nrects = rects.size();

Rect search_rect(6, 4, 10,
                 6); // search will find above rects that this one overlaps

TEST(BasicTests, BasicTests) {
  typedef RTree<ValueType, int, 2, float> MyTree;
  MyTree tree;

  int i, nhits;

  for (i = 0; i < nrects; i++) {
    tree.Insert(
        rects[i].min, rects[i].max,
        values[i]); // Note, all values including zero are fine in this version
  }

  MockFunction<bool(ValueType)> mock_function;
  ON_CALL(mock_function, Call).WillByDefault(Return(true));
  EXPECT_CALL(mock_function, Call(_)).Times(2);

  nhits = tree.Search(search_rect.min, search_rect.max,
                      mock_function.AsStdFunction());

  ASSERT_EQ(nhits, 2);

  std::vector<Rect> collectedRects = std::vector<Rect>();
  std::vector<ValueType> collectedValues = std::vector<ValueType>();

  // Iterator test
  MyTree::Iterator it;
  for (tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it)) {
    int value = tree.GetAt(it);

    int boundsMin[2] = {0, 0};
    int boundsMax[2] = {0, 0};
    it.GetBounds(boundsMin, boundsMax);
    collectedRects.push_back(
        Rect(boundsMin[0], boundsMin[1], boundsMax[0], boundsMax[1]));
    collectedValues.push_back(value);
  }

  EXPECT_THAT(rects, UnorderedElementsAreArray(collectedRects));
  EXPECT_THAT(values, UnorderedElementsAreArray(collectedValues));
  collectedValues.clear();


  // Iterator test, alternate syntax
  tree.GetFirst(it);
  while (!it.IsNull()) {
    int value = *it;
    ++it;
    collectedValues.push_back(value);
  }

  EXPECT_THAT(values, UnorderedElementsAreArray(collectedValues));

  // test copy constructor
  MyTree copy = tree;

  collectedRects.clear();
  collectedValues.clear();

  // Iterator test
  for (copy.GetFirst(it); !copy.IsNull(it); copy.GetNext(it)) {
    int value = copy.GetAt(it);

    int boundsMin[2] = {0, 0};
    int boundsMax[2] = {0, 0};
    it.GetBounds(boundsMin, boundsMax);
    collectedRects.push_back(
        Rect(boundsMin[0], boundsMin[1], boundsMax[0], boundsMax[1]));
    collectedValues.push_back(value);
  }
  EXPECT_THAT(values, UnorderedElementsAreArray(collectedValues));

  EXPECT_THAT(rects, UnorderedElementsAreArray(collectedRects));

  collectedValues.clear();

  // Iterator test, alternate syntax
  copy.GetFirst(it);
  while (!it.IsNull()) {
    int value = *it;
    ++it;
    collectedValues.push_back(value);
  }

  EXPECT_THAT(values, UnorderedElementsAreArray(collectedValues));
}


TEST(BasicTests, RemoveTests) {
  typedef RTree<ValueType, int, 2, float> MyTree;
  MyTree tree;

  std::vector<ValueType> v;

  for (size_t i = 0; i < 1000; i++) {
    const Rect rect(i, i, i, i);
    tree.Insert(rect.min, rect.max, i);
    v.push_back(i);
  }

  for (size_t i = 0; i < v.size(); i++) {
    tree.Remove(v.back());

    std::vector<ValueType> collectedValues = std::vector<ValueType>();
    MyTree::Iterator it;
    tree.GetFirst(it);

    while (!it.IsNull()) {
      int value = *it;
      ++it;
      collectedValues.push_back(value);
    }

    v.pop_back();

    ASSERT_EQ(v.size(), collectedValues.size());
    EXPECT_THAT(v, UnorderedElementsAreArray(collectedValues));
  }

}
