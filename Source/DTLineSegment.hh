#ifndef DTLINESEGMENT_HH
#define DTLINESEGMENT_HH

#include "DTPoint.hh"

class DTLineSegment
{
public:
  DTLineSegment(const DTPoint& pp1,const DTPoint& pp2) : p1(pp1), p2(pp2) {}

  const DTPoint p1;
  const DTPoint p2;

  bool Contains(const DTPoint& p) const;

  friend bool operator==(const DTLineSegment& l1,const DTLineSegment& l2)
  {
    return l1.p1 == l2.p1 && l1.p2 == l2.p2;
  }

  friend bool operator!=(const DTLineSegment& l1,const DTLineSegment& l2)
  {
    return !(l1 == l2);
  }

  friend bool operator<(const DTLineSegment& l1,const DTLineSegment& l2)
  {
    return (l1.p1 != l2.p1 ? l1.p1 < l2.p1 : l1.p2 < l2.p2);
  }

  friend bool operator>=(const DTLineSegment& l1,const DTLineSegment& l2)
  {
    return !(l1<l2);
  }

  friend bool operator>(const DTLineSegment& l1,const DTLineSegment& l2)
  {
    return (l1.p1 != l2.p1 ? l1.p1 > l2.p1 : l1.p2 > l2.p2);
  }

  friend bool operator<=(const DTLineSegment& l1,const DTLineSegment& l2)
  {
    return !(l1>l2);
  }

  friend std::ostream& operator<<(std::ostream& s,const DTLineSegment& l)
  {
    s<<"("<< l.p1<<","<<l.p2<<")";
    return s;
  }
};


#endif /* DTLINESEGMENT_HH */
