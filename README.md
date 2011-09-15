R-TREES: A DYNAMIC INDEX STRUCTURE FOR SPATIAL SEARCHING
--------------------------------------------------------

DESCRIPTION

    A C++ templated version of the RTree algorithm.
    The code it now generally compatible with the STL and Boost C++ libraries.
    For more information please read the comments in RTree.h

AUTHORS

- 1983 Original algorithm and test code by Antonin Guttman and Michael Stonebraker, UC Berkely
- 1994 ANCI C ported from original test code by Melinda Green - melinda@superliminal.com
- 1995 Sphere volume fix for degeneracy problem submitted by Paul Brook
- 2004 Templated C++ port by Greg Douglas
- 2011 Modified the container to support more data types, by Yariv Barkan

LICENSE

    Original code was taken from http://www.superliminal.com/sources/sources.htm 
    and is stored as git revision 0. This revision is entirely free for all
    uses. Enjoy!

    Due to restrictions on public domain in certain jurisdictions, code
    contributed by Yariv Barkan is released in these jurisdictions under the
    BSD, MIT or the GPL - you may choose one or more, whichever that suits you
    best. 
    
    In jurisdictions where public domain property is recognized, the user of
    this software may choose to accept it either 1) as public domain, 2) under
    the conditions of the BSD, MIT or GPL or 3) any combination of public
    domain and one or more of these licenses.

    Thanks [Baptiste Lepilleur](http://jsoncpp.sourceforge.net/LICENSE) for the
    licensing idea.

FILES

    - RTree.h            The C++ templated RTree implementation. Well commented.
    - Test.cpp           A simple test program, ported from the original C version.
    - MemoryTest.cpp     A more rigourous test to validate memory use.
    - README             This file.

TO BUILD

    To build a test, compile only one of the test files with RTree.h.  
    Both test files contain a main() function.

RECENT CHANGE LOG

02 Sep 2011

- Modified the container to support more data types. The code it now generally
  compatible with the STL and Boost C++ libraries.

05 Jan 2010

- Fixed Iterator GetFirst() - Previous fix was not incomplete

03 Dec 2009

- Added Iteartor GetBounds()
- Added Iterator usage to simple test
- Fixed Iterator GetFirst() - Thanks Mathew Riek
- Minor updates for MSVC 2005/08 compilers

