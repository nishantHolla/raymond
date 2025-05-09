#ifndef TEXTURE_H_
#define TEXTURE_H_

// ==============================
// Texture class
// ==============================

class Texture {
  public:
    virtual ~Texture() = default;

    virtual Color value(double u, double v, const Point3& p) const = 0;
};

// ==============================
// SolidColor class
// (derived from Texture class)
// ==============================

class SolidColor : public Texture {
  public:
    /*
     * Constructs the solid texture with the given color.
     */
    SolidColor(const Color& albedo) :
      albedo(albedo) {
      }

    /*
     * Constructs the solid texture with the color values given separately.
     */
    SolidColor(double r, double g, double b) :
      albedo(Color(r, g, b)) {
      }

    /*
     * Returns the single solid color of the texture for any point on the entity.
     */
    Color value(double u, double v, const Point3& p) const override {
      return albedo;
    }

  private:
      Color albedo; // single color of the texture

};

// ==============================
// CheckerTexture class
// (derived from Texture)
// ==============================

class CheckerTexture : public Texture {
  public:
    /*
     * Constructs the texture with the scale of the squares, the even texture and the odd texture.
     */
    CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) :
      inv_scale(1.0 / scale),
      even(even),
      odd(odd) {
      }

    /*
     * Constructs the texture with the scale of the squares and two colors which constructs a SolidColor
     * texture.
     */
    CheckerTexture(double scale, const Color& c1, const Color& c2) :
      CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {
      }

    /*
     * Returns the color of the texture for the given point on the entity.
     */
    Color value(double u, double v, const Point3& p) const override {
      int xi = int(std::floor(inv_scale * p.x()));
      int yi = int(std::floor(inv_scale * p.y()));
      int zi = int(std::floor(inv_scale * p.z()));

      bool isEven = (xi + yi + zi) % 2 == 0;

      return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    double inv_scale;           // inverse of the scale of the squares of the checker pattern
    shared_ptr<Texture> even;   // texture of even squares
    shared_ptr<Texture> odd;    // texture of odd squares
};

#endif //!TEXTURE_H_
