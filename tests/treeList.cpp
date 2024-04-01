#include "../RTree.h"
#include "util.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>

using namespace ::testing;

typedef int ValueType;

typedef RectTemplate<int> Rect;

Rect rects[] = {
    Rect(0, 0, 0, 0), Rect(1, 1, 1, 1), Rect(2, 2, 2, 2), Rect(3, 3, 3, 3),
    Rect(4, 4, 4, 4), Rect(5, 5, 5, 5), Rect(6, 6, 6, 6), Rect(7, 7, 7, 7),
    Rect(8, 8, 8, 8), Rect(9, 9, 9, 9),
};

int nrects = sizeof(rects) / sizeof(rects[0]);

TEST(TreeList, TreeList) {
  typedef RTree<ValueType, int, 2, float, 4> MyTree;
  MyTree tree;

  for (int i = 0; i < nrects; i++) {
    tree.Insert(rects[i].min, rects[i].max, i);
  }

  auto list = tree.ListTree();

  // there are at least nrects internal rectangles, because we use at most 4
  // items per node
  EXPECT_GT(list.size(), nrects);
}
