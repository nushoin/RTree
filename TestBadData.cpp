//
// TestBadData.cpp
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "RTree.h"

using namespace std;

typedef int ValueType;
typedef long long CoordType;

struct Rect
{
  Rect()  {}

  Rect(CoordType a_minX, CoordType a_minY, CoordType a_maxX, CoordType a_maxY)
  {
    min[0] = a_minX;
    min[1] = a_minY;

    max[0] = a_maxX;
    max[1] = a_maxY;
  }


  CoordType min[2];
  CoordType max[2];
};


bool MySearchCallback(ValueType id, void* arg)
{
  cout << "Hit data rect " << id << "\n";
  return true; // keep going
}


int main( int argc, char* argv[] )
{
  if ( argc < 2 ) {
     std::cout << "Usage: " << argv[0] << " inFile\n";
     return -1;
  }

  typedef std::vector< Rect > RectVector;
  RectVector rectVector;

  // read the data
  {
     ifstream inFile( argv[1] );
     while( !inFile.eof() ) {
        // security and robustness be damned
        CoordType xmin, ymin, xmax, ymax;
        string dummy;
        inFile >> xmin >> ymin >> xmax >> ymax;
        cout << xmin << " " << ymin << " " << xmax << " " << ymax << "\n";
        rectVector.push_back( Rect( xmin, ymin, xmin + xmax, ymin + ymax ) );
     }
  }

  typedef RTree<ValueType, CoordType, 2, float> MyTree;
  MyTree tree;

  int i, nhits;
  cout << "number of rectangles is " << rectVector.size() << "\n";

  for(i=0; i<rectVector.size(); i++)
  {
    tree.Insert(rectVector[i].min, rectVector[i].max, i); // Note, all values including zero are fine in this version
  }

  Rect search_rect(6, 4, 10, 6);
  nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);

  cout << "Search resulted in " << nhits << " hits\n";

  // Iterator test
  int itIndex = 0;
  MyTree::Iterator it;
  for( tree.GetFirst(it);
       !tree.IsNull(it);
       tree.GetNext(it) )
  {
    int value = tree.GetAt(it);

    CoordType boundsMin[2] = {0,0};
    CoordType boundsMax[2] = {0,0};
    it.GetBounds(boundsMin, boundsMax);
    cout << "it[" << itIndex++ << "] " << value << " = (" << boundsMin[0] << "," << boundsMin[1] << "," << boundsMax[0] << "," << boundsMax[1] << ")\n";
  }

  // Iterator test, alternate syntax
  itIndex = 0;
  tree.GetFirst(it);
  while( !it.IsNull() )
  {
    CoordType value = *it;
    ++it;
    cout << "it[" << itIndex++ << "] " << value << "\n";
  }

  return 0;
}

