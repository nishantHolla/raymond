#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "entity.h"

class Material {
  public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray& r_in, const HitRecord& record, Color& Attenuation, Ray& scattered
        ) const {
      return false;
    }
};

class Lambertian : public Material {
  public:
    Lambertian(const Color &albedo) :
      albedo(albedo) {
      }

    bool scatter(
        const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered
        ) const override {
      Vector3 scatter_direction = record.normal + random_unit_vector();

      if (scatter_direction.near_zero()) {
        scatter_direction = record.normal;
      }

      scattered = Ray(record.p, scatter_direction);
      attenuation = albedo;

      return true;
    }

  private:
    Color albedo;
};

class Metal : public Material {
  public:
    Metal(const Color& albedo, double fuzz) :
      albedo(albedo),
      fuzz(fuzz < 1 ? fuzz: 1) {
      }

    bool scatter(
        const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered
        ) const override {
      Vector3 reflected = reflect(r_in.direction(), record.normal);
      reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
      scattered = Ray(record.p, reflected);
      attenuation = albedo;

      return (dot(scattered.direction(), record.normal) > 0);
    }

  private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
  public:
    Dielectric(double refraction_index) :
      refraction_index(refraction_index) {
      }

    bool scatter(
        const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered
        ) const override {
      attenuation = Color(1.0, 1.0, 1.0);
      double ri = record.front_face ? (1.0/refraction_index) : refraction_index;

      Vector3 unit_direction = unit_vector(r_in.direction());
      double cos_theta = std::fmin(dot(-unit_direction, record.normal), 1.0);
      double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

      bool cannot_refract = ri * sin_theta > 1.0;
      Vector3 direction;

      if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
        direction = reflect(unit_direction, record.normal);
      }
      else {
        direction = refract(unit_direction, record.normal, ri);
      }

      scattered = Ray(record.p, direction);
      return true;
    }

  private:
    double refraction_index;


    static double reflectance(double cosine, double refraction_index) {
      // Schlick's approximation for reflectiance
      double r0 = (1 - refraction_index) / (1 + refraction_index);
      r0 = r0 * r0;
      return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif //!MATERIAL_H_
