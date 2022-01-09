#include <iostream>
#include "RTree.h"

using namespace std;

typedef int ValueType;

struct Rect
{
  Rect()  {}

  Rect(int a_minX, int a_minY, int a_maxX, int a_maxY)
  {
    min[0] = a_minX;
    min[1] = a_minY;

    max[0] = a_maxX;
    max[1] = a_maxY;
  }


  int min[2];
  int max[2];
};

struct Rect rects[] =
{
  Rect(0,0,0,0),
  Rect(1,1,1,1),
  Rect(2,2,2,2),
  Rect(3,3,3,3),
  Rect(4,4,4,4),
  Rect(5,5,5,5),
  Rect(6,6,6,6),
  Rect(7,7,7,7),
  Rect(8,8,8,8),
  Rect(9,9,9,9),
};

int nrects = sizeof(rects) / sizeof(rects[0]);


int main()
{
  typedef RTree<ValueType, int, 2, float, 4> MyTree;
  MyTree tree;

  for(int i=0; i<nrects; i++)
  {
    tree.Insert(rects[i].min, rects[i].max, i);
  }


  auto list = tree.ListTree();
  int counter = 0;
  for (auto aabb : list) {
    cout << "TreeList [" << counter++ << "]: "
      << aabb.m_min[0] << ", "
      << aabb.m_min[1] << "; "
      << aabb.m_max[0] << ", "
      << aabb.m_max[1] << endl;
  }

  return 0;

}
