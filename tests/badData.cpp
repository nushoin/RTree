#include "../RTree.h"
#include "util.hpp"
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace ::testing;

typedef int ValueType;
typedef long long CoordType;

typedef RectTemplate<CoordType> Rect;

TEST(BadDataTest, BadData) {
  typedef std::vector<Rect> RectVector;
  RectVector rects;
  std::vector<ValueType> values;

  // read the data
  {
    std::ifstream inFile("baddata.txt");
    while (!inFile.eof()) {
      // security and robustness be damned
      CoordType xmin, ymin, xmax, ymax;
      std::string dummy;
      inFile >> xmin >> ymin >> xmax >> ymax;
      rects.push_back(Rect(xmin, ymin, xmin + xmax, ymin + ymax));
    }
  }

  EXPECT_EQ(rects.size(), 174);

  typedef RTree<ValueType, CoordType, 2, float> MyTree;
  MyTree tree;

  unsigned int nhits;

  for (unsigned int i = 0; i < rects.size(); i++) {
    tree.Insert(rects[i].min, rects[i].max, i);
    values.push_back(i);
  }

  MockFunction<bool(ValueType)> mock_function;
  ON_CALL(mock_function, Call).WillByDefault(Return(true));
  EXPECT_CALL(mock_function, Call(_)).Times(0);

  Rect search_rect(6, 4, 10, 6);
  nhits = tree.Search(search_rect.min, search_rect.max, mock_function.AsStdFunction());

  EXPECT_EQ(nhits, 0);

  std::vector<Rect> collectedRects = std::vector<Rect>();
  std::vector<ValueType> collectedValues = std::vector<ValueType>();

  // Iterator test
  MyTree::Iterator it;
  for (tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it)) {
    int value = tree.GetAt(it);

    CoordType boundsMin[2] = {0, 0};
    CoordType boundsMax[2] = {0, 0};
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
    CoordType value = *it;
    ++it;
    collectedValues.push_back(value);
  }

  EXPECT_THAT(values, UnorderedElementsAreArray(collectedValues));
}
