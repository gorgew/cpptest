#include <cereal/archives/json.hpp>
#include <sstream>
#include <iostream>
#include <cassert>

#include <glm/glm.hpp>

struct position {
    int x, y, z;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(x, y, z);
    }
};

struct health {
    unsigned int points;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(points);
    }    
};

struct my_type_1 {
    int a;
    int b;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(a, b);
    }  
};

struct color {
    glm::fvec3 vec;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(vec.x, vec.y, vec.z);
    }  
};

int main(void) {

    std::stringstream ss;

    {
        cereal::JSONOutputArchive oarchive(ss);

        health hp {10};
        position pos {10, 2, 3};
        my_type_1 my_type = {5, 6};
        color my_color = {glm::vec3(1.0, 2.0, 3.0)};
        oarchive(CEREAL_NVP(hp), CEREAL_NVP(pos), CEREAL_NVP(my_type), CEREAL_NVP(my_color));
    }
    {
        cereal::JSONInputArchive in(ss);

        health hp;
        position pos;
        my_type_1 my_type;
        color my_color;
        cereal::make_optional_nvp(in, "my_type", my_type);
        CEREAL_OPTIONAL_NVP(in, my_color);
        in(CEREAL_NVP(hp), CEREAL_NVP(pos));
        //assert(hp.points == 10);
        //assert(pos.x == 1);
        //assert(pos.y == 2);
        std::cout << hp.points << " " << pos.x << " " << my_type.a << "\n" << my_color.vec.z;
        
    }
    std::cout << ss.str();
    return 0;
}