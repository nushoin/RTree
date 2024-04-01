template<class CoordType = float>
struct RectTemplate {
  RectTemplate() {}

  RectTemplate(CoordType a_minX, CoordType a_minY, CoordType a_maxX, CoordType a_maxY) {
    min[0] = a_minX;
    min[1] = a_minY;

    max[0] = a_maxX;
    max[1] = a_maxY;
  }

  bool operator==(const RectTemplate &o) const {
    return min[0] == o.min[0] && min[1] == o.min[1] && max[0] == o.max[0] &&
           max[1] == o.max[1];
  }

  CoordType min[2];
  CoordType max[2];
};