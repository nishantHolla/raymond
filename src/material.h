#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "entity.h"
#include "texture.h"

// ==============================
// Material class
// ==============================

class Material {
  public:
    virtual ~Material() = default;

    /*
     * Scatters the given ray depending on its hit record and properties of the material
     * Stores the scattered ray in the given scattered parameter.
     * Returns true if the ray is scattered, else returns false.
     */
    virtual bool scatter(
        const Ray& r_in, const HitRecord& record, Color& Attenuation, Ray& scattered
        ) const {
      (void) r_in, (void) record, (void) Attenuation, (void) scattered;
      return false;
    }

    /*
     * Emit no color by default
     */
    virtual Color emitted(double u, double v, const Point3& p) const {
      (void) u, (void) v, (void) p;
      return Color(0, 0, 0);
    }
};

// ==============================
// Lambertian class
// (derived from Material class)
// Diffused material
// ==============================

class Lambertian : public Material {
  public:
    /*
     * Constructs the Lambertian material with the given albedo.
     */
    Lambertian(const Color &albedo) :
      tex(make_shared<SolidColor>(albedo)) {
      }

    /*
     * Constructs the Lambertian material with the given texture.
     */
    Lambertian(shared_ptr<Texture> tex) :
      tex(tex) {
      }

    /*
     * Scatters the given ray depending on its hit record and properties of the material
     * Stores the scattered ray in the given scattered parameter.
     * Returns true if the ray is scattered, else returns false.
     */
    bool scatter(
        const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered
        ) const override {
      // scattered ray has a random direction from the normal
      Vector3 scatter_direction = record.normal + random_unit_vector();

      // if the scattered ray is close to the normal, make is same as normal
      if (scatter_direction.near_zero()) {
        scatter_direction = record.normal;
      }

      // set the scattered ray
      scattered = Ray(record.p, scatter_direction, r_in.time());

      // get the color of the ray from the texture of the material
      attenuation = tex->value(record.u, record.v, record.p);

      return true;
    }

  private:
    shared_ptr<Texture> tex; // texture of the material
};

// ==============================
// Metal class
// (derived from Material class)
// Reflective material
// ==============================

class Metal : public Material {
  public:
    /*
     * Constructs the Metal material with the given albedo and fuzz.
     */
    Metal(const Color& albedo, double fuzz) :
      albedo(albedo),
      fuzz(fuzz < 1 ? fuzz: 1) {
      }

    /*
     * Scatters the given ray depending on its hit record and properties of the material
     * Stores the scattered ray in the given scattered parameter.
     * Returns true if the ray is scattered, else returns false.
     */
    bool scatter(
        const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered
        ) const override {

      // Calculate the reflected ray
      Vector3 reflected = reflect(r_in.direction(), record.normal);
      reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

      // Set the scattered ray as the reflected ray
      scattered = Ray(record.p, reflected, r_in.time());

      // color of the ray is same as albedo of the material
      attenuation = albedo;

      // the ray is reflected if the scattered ray is in the same hemisphere as the normal ray
      return (dot(scattered.direction(), record.normal) > 0);
    }

  private:
    Color albedo; // percentage of light reflected represented as color
                  // Color(1, 1, 1) represents 100% reflection and
                  // Color(0, 0, 0) represents 0% reflection

    double fuzz;  // Amount of fuzz finish to the metal texture
                  // 0 gives a smooth and shiny metal material
                  // positive value gives a brushed metal surface
};

// ==============================
// Dielectric class
// (derived from Material class)
// Refractive material
// ==============================

class Dielectric : public Material {
  public:
    /*
     * Constructs the Dielectric material with the given refraction_index.
     */
    Dielectric(double refraction_index) :
      refraction_index(refraction_index) {
      }

    /*
     * Scatters the given ray depending on its hit record and properties of the material
     * Stores the scattered ray in the given scattered parameter.
     * Returns true if the ray is scattered, else returns false.
     */
    bool scatter(
        const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered
        ) const override {
      // Attenuation has the color white
      attenuation = Color(1.0, 1.0, 1.0);

      // Sets the refraction_index base on the surface that the ray hit(inside or outside)
      double ri = record.front_face ? (1.0/refraction_index) : refraction_index;

      // Snell's law to determine the angle of refraction
      Vector3 unit_direction = unit_vector(r_in.direction());
      double cos_theta = std::fmin(dot(-unit_direction, record.normal), 1.0);
      double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

      // Edge case of Snell's law that results in totat internal reflection
      bool cannot_refract = ri * sin_theta > 1.0;
      Vector3 direction;

      // Check whether to reflect the ray or to refract the ray
      if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
        direction = reflect(unit_direction, record.normal);
      }
      else {
        direction = refract(unit_direction, record.normal, ri);
      }

      // Record the scatterd ray
      scattered = Ray(record.p, direction, r_in.time());

      return true;
    }

  private:
    double refraction_index; // refraction index of the material

    /*
     * Schlick's approximation for reflectiance
     */
    static double reflectance(double cosine, double refraction_index) {
      double r0 = (1 - refraction_index) / (1 + refraction_index);
      r0 = r0 * r0;
      return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

// ==============================
// DiffuseLight class
// (derived from Material class)
// ==============================

class DiffuseLight : public Material {
  public:
    DiffuseLight(shared_ptr<Texture> tex) :
      tex(tex) {
      }

    DiffuseLight(const Color& emit) :
      tex(make_shared<SolidColor>(emit)) {
      }

    Color emitted(double u, double v, const Point3& p) const override {
      return tex->value(u, v, p);
    }

  private:
    shared_ptr<Texture> tex;
};

#endif //!MATERIAL_H_
