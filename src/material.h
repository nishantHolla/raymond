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
    Metal(const Color& albedo) :
      albedo(albedo) {
      }

    bool scatter(
        const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered
        ) const override {
      Vector3 reflected = reflect(r_in.direction(), record.normal);
      scattered = Ray(record.p, reflected);
      attenuation = albedo;

      return true;
    }

  private:
    Color albedo;
};

#endif //!MATERIAL_H_
