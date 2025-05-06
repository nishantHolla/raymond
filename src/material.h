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

#endif //!MATERIAL_H_
