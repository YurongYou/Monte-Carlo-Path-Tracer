#include "Texture.h"

class Material {
public:
    Material() : emission(BLACK), intrinsic_color(WHITE),  diffuse_prob(0.8f),
                 reflection_prob(0.0f), refraction_prob(0.0f), base_reflection_rate(0.5f),
                 refraction_index(0.0f), texture(NULL) {}

    const Color &getEmission() const {
        return emission;
    }

    ~Material(){
        if (texture) {
            delete texture;
        }
    }

    void clear(){
        emission = BLACK;
        intrinsic_color = WHITE;
        diffuse_prob = 1.0f;
        reflection_prob = 0.0f;
        refraction_prob = 0.0f;
        base_reflection_rate = 0.5f;
        refraction_index = 0.0f;
        if (texture) {
            delete texture;
        }
    }

public:
    void setEmission(const Color &emission) {
        Material::emission = emission;
    }

    const Color &getIntrinsic_color() const {
        return intrinsic_color;
    }

    void setIntrinsic_color(const Color &intrinsic_color) {
        Material::intrinsic_color = intrinsic_color;
    }

    float getDiffuse_prob() const {
        return diffuse_prob;
    }

    void setDiffuse_prob(float diffuse_prob) {
        Material::diffuse_prob = diffuse_prob;
    }

    float getReflection_prob() const {
        return reflection_prob;
    }

    void setReflection_prob(float reflection_prob) {
        Material::reflection_prob = reflection_prob;
    }

    float getRefraction_prob() const {
        return refraction_prob;
    }

    void setRefraction_prob(float refraction_prob) {
        Material::refraction_prob = refraction_prob;
    }

    float getBase_reflection_rate() const {
        return base_reflection_rate;
    }

    void setBase_reflection_rate(float base_reflection_rate) {
        Material::base_reflection_rate = base_reflection_rate;
    }

    float getRefraction_index() const {
        return refraction_index;
    }

    void setRefraction_index(float refraction_index) {
        Material::refraction_index = refraction_index;
    }

    float getKs() const {
        return Ks;
    }

    void setKs(float Ks) {
        Material::Ks = Ks;
    }

    Texture *getTexture() const {
        return texture;
    }

    void setTexture(Texture *texture) {
        Material::texture = texture;
    }

private:
    Color emission;
    Color intrinsic_color;
    float diffuse_prob;
    float reflection_prob;
    float refraction_prob;
    float base_reflection_rate;
    float refraction_index;
    Texture* texture;

    // for casting
    float Ks;
};