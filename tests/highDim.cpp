#include "../RTree.h"
#include "util.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>
#include <vector>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> range(0, 1);

class RectND
{
public:
	RectND(int n)
	{
    for (int i = 0; i < n; i++)
    {
      m_min.push_back(range(gen));
      m_max.push_back(range(gen));
    }
	}

	std::vector<float> m_min, m_max;
};

using namespace ::testing;

TEST(HighDim, HighDim) {
  constexpr uint NDIM = 100;
  typedef RTree<int, float, NDIM> MyTree;
  MyTree tree;

  int nrects = 1000;

  for (int i = 0; i < nrects; i++) {
    const RectND rect(NDIM);
    tree.Insert(rect.m_min.data(), rect.m_max.data(), i);
  }

  MyTree::Iterator it;
  int counter = 0;
  for (tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it)) {
    counter++;
  }

  ASSERT_EQ(counter, nrects);
}
