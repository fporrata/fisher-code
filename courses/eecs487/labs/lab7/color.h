

#ifndef __COLOR_H__
#define __COLOR_H__

/*! Represents color as a triplet of floating points,
    like in OpenGL but without the alpha
*/
struct Color {

  double r, g, b;
  static Color const RED;
  static Color const GREEN;
  static Color const BLUE;
  static Color const WHITE;
  static Color const BLACK;

  Color(double r0=0, double g0=0, double b0=0) :
    r(r0), g(g0), b(b0) {}

  Color& operator =(Color const& c) {
    r=c.r; g=c.g; b=c.b;
    return *this;
  }

};

// Component-wise multiplication, for lighting calculations
inline Color operator *(Color const& a, Color const& b)
{
  return Color(a.r*b.r, a.g*b.g, a.b*b.b);
}

// Multiplication by scalar
inline Color operator *(double const& s, Color const& c)
{
  return Color(s*c.r, s*c.g, s*c.b);
}

// Ensure commutativity of multiplication by scalar
inline Color operator *(Color const& c, double const& s)
{
  return s*c;
}

#endif

