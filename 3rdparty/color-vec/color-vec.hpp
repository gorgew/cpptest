#pragma once

#include <iosfwd>
#include <concepts>
#include <exception>
#include <stdexcept>
#include <array>
#include <cmath>


namespace color_vec {

    /**
     * @brief Concept to check bounds?
    */
    template<typename T>
    concept to_float = requires(T c){
        
        requires std::convertible_to<T, float>;
    };

    struct rgb_bound_exception : public std::exception {
        const char* what () const throw() {
            return "RGB values must be between 0.0f and 255.0f";
        }
    };

    struct hsv_hue_bound_exception : public std::exception {
        const char* what () const throw() {
            return "HSV hue must be between 0.0f and 360.0f";
        }
    };

    struct hsv_sat_bound_exception : public std::exception {
        const char* what () const throw() {
            return "HSV saturation must be between 0.0f and 100.0f";
        }
    };

    struct hsv_val_bound_exception : public std::exception {
        const char* what () const throw() {
            return "HSV value must be between 0.0f and 100.0f";
        }
    };

    struct alpha_bound_exception : public std::exception {
        const char* what () const throw() {
            return "alpha must be between 0.0f and 1.0f";
        }
    };

    void check_rgb_bound(const to_float auto& f) {
        if (f < 0.0f || f > 255.0f) {
            throw rgb_bound_exception();
        }
    }

    void check_hsv_hue_bound(const to_float auto& f) {
        if (f < 0.0f || f > 360.0f) {
            throw hsv_hue_bound_exception();
        }
    }

    void check_hsv_sat_bound(const to_float auto& f) {
        if (f < 0.0f || f > 100.0f) {
            throw hsv_sat_bound_exception();
        }
    }

    void check_hsv_val_bound(const to_float auto& f) {
        if (f < 0.0f || f > 100.0f) {
            throw hsv_val_bound_exception();
        }
    }

    void check_alpha_bound(const to_float auto& f) {
        if (f < 0.0f || f > 1.0f) {
            throw alpha_bound_exception();
        }
    }

    /**
     * @brief Converts hsv values to float 0.0f - 1.0f rgb colors
     * @param h Hue
     * @param s Satuaration
     * @param v Value
     * @returns std::array<float,3> where [0] = r, [1] = g, [2] = b
    */
    std::array<float, 3> hsv_to_float(const to_float auto& h, const to_float auto& s, 
            const to_float auto& v) {
        
        check_hsv_hue_bound(h);
        check_hsv_sat_bound(s);
        check_hsv_val_bound(v);
        //float h_dec = static_cast<float>(h);
        float s_dec = static_cast<float>(s) * .01;
        float v_dec = static_cast<float>(v) * .01;

        float c = s_dec * v_dec;
        float X = c * (1.0f - std::abs( std::fmod(h / 60.0f, 2) - 1 ) );
        float m = v_dec - c;

        if (h < 60.0f) {

            return {c + m, X + m, m};
        }
        else if (h < 120.0f) {
            return {X + m, c + m, m};
        }
        else if (h < 180.0f) {
            return {m, c + m, X + m};
        }
        else if (h < 240.0f) {
            return {m, X + m, c + m};
        }
        else if (h < 300.0f) {
            return {X + m, m, c + m};
        }
        else {
            return {c + m, m, X + m};
        }
    }

    template <typename T, int Index>
    class rgb_access{
        float arr[3];
        public:
            
            float operator=(const float f) {
                check_rgb_bound(f);
                arr[Index] = f / 255.0f;
                return f;
            }

            operator float() const {
                return arr[Index] * 255.0f;
            }

            float operator+(const to_float auto& c) {
                return arr[Index] * 255.0f + static_cast<float>(c);
            } 

            float operator+=(const to_float auto& c) {
                float val = arr[Index] + static_cast<float>(c) / 255.0f;
                float scaled = val * 255.0f;
                check_rgb_bound(scaled);
                arr[Index] = val;
                return scaled;
            }

            /**
             * @brief Increments r/g/b by 1 on 0-255 scale
            */
            float operator++() {
                return operator+=(1);
            }

            float operator++(int) {
                float temp = 255.0f * arr[Index];
                operator+=(1);
                return temp;
            }

            float operator-(const to_float auto& c) {
                return arr[Index] * 255.0f - static_cast<float>(c);
            } 

            float operator-=(const to_float auto& c) {
                float val = arr[Index] - static_cast<float>(c) / 255.0f;
                float scaled = val * 255.0f;
                check_rgb_bound(scaled);
                arr[Index] = val;
                return scaled;
            }

            /**
             * @brief Increments r/g/b by 1 on 0-255 scale
            */
            float operator--() {
                return operator-=(1);
            }

            float operator--(int) {
                float temp = 255.0f * arr[Index];
                operator-=(1);
                return temp;
            }

            float operator*(const to_float auto& scale) {
                return arr[Index] * 255.0f * static_cast<float>(scale);
            }

            float operator/(const to_float auto& scale) {
                return arr[Index] * 255.0f / static_cast<float>(scale);
            }

            float operator*=(const to_float auto& scale) {
                float val = 255.0f * arr[Index] * static_cast<float>(scale);
                check_rgb_bound(val);
                arr[Index] = val / 255.0f;
                return val;
            }

            float operator/=(const to_float auto& scale) {
                float val = arr[Index] / static_cast<float>(scale);
                float scaled = 255.0f * val;
                check_rgb_bound(scaled);
                arr[Index] = val;
                return scaled;
            }
    };

    enum class hsv_type {
        hue,
        saturation,
        value
    };
    
    template<hsv_type t>
    struct hsv_access {

        float arr[3];
        public:

            float operator=(const to_float auto& f) {
                if (t == hsv_type::hue) {
                    
                    check_hsv_hue_bound(f);
                    auto current_sat = calc_sat();
                    auto current_val = calc_val();

                    auto new_floats = hsv_to_float(f, current_sat, current_val);
                    arr[0] = new_floats[0];
                    arr[1] = new_floats[1];
                    arr[2] = new_floats[2];
                    return f;
                }

                if (t == hsv_type::saturation) {
                    check_hsv_sat_bound(f);
                    auto current_hue = calc_hue();
                    auto current_val = calc_val();

                    auto new_floats = hsv_to_float(current_hue, f, current_val);
                    arr[0] = new_floats[0];
                    arr[1] = new_floats[1];
                    arr[2] = new_floats[2];
                    return f;
                }

                if (t == hsv_type::value) {
                    check_hsv_val_bound(f);
                    auto current_hue = calc_hue();
                    auto current_sat = calc_sat();

                    auto new_floats = hsv_to_float(current_hue, current_sat, f);
                    arr[0] = new_floats[0];
                    arr[1] = new_floats[1];
                    arr[2] = new_floats[2];
                    return f;
                }
            }

            operator float() const {

                if (t == hsv_type::hue) {
                    return calc_hue();
                }

                if (t == hsv_type::saturation) {
                    return calc_sat();
                }

                if (t == hsv_type::value) {
                    return calc_val();
                }
            }

            float operator+(const to_float auto& f) {
                if (t == hsv_type::hue) {
                    return calc_hue() + static_cast<float>(f);
                }

                if (t == hsv_type::saturation) {
                    return calc_sat() + static_cast<float>(f);
                }

                if (t == hsv_type::value) {
                    return calc_sat() + static_cast<float>(f);
                }
            }

            float operator-(const to_float auto& f) {
                return operator+(-1.0 * f);
            }

            float operator*(const to_float auto& f) {
                if (t == hsv_type::hue) {
                    return calc_hue() * static_cast<float>(f);
                }

                if (t == hsv_type::saturation) {
                    return calc_sat() * static_cast<float>(f);
                }

                if (t == hsv_type::value) {
                    return calc_sat() * static_cast<float>(f);
                }
            }

            float operator/(const to_float auto& f) {
                if (t == hsv_type::hue) {
                    return calc_hue() / static_cast<float>(f);
                }

                if (t == hsv_type::saturation) {
                    return calc_sat() / static_cast<float>(f);
                }

                if (t == hsv_type::value) {
                    return calc_sat() / static_cast<float>(f);
                }
            }

            float operator+=(const to_float auto& f) {
                if (t == hsv_type::hue) {
                    float h = calc_hue() + static_cast<float>(f);
                    check_hsv_hue_bound(h);
                    return operator=(h);
                }

                if (t == hsv_type::saturation) {
                    float s = calc_sat() + static_cast<float>(f);
                    check_hsv_sat_bound(s);
                    return operator=(s);
                }

                if (t == hsv_type::value) {
                    float v = calc_val() + static_cast<float>(f);
                    check_hsv_val_bound(v);
                    return operator=(v);
                }
            }

            float operator-=(const to_float auto& f) {
                if (t == hsv_type::hue) {
                    float h = calc_hue() - static_cast<float>(f);
                    check_hsv_hue_bound(h);
                    return operator=(h);
                }

                if (t == hsv_type::saturation) {
                    float s = calc_sat() - static_cast<float>(f);
                    check_hsv_sat_bound(s);
                    return operator=(s);
                }

                if (t == hsv_type::value) {
                    float v = calc_val() - static_cast<float>(f);
                    check_hsv_val_bound(v);
                    return operator=(v);
                }
            }

            float operator++() {
                return operator+=(1);
            }

            float operator++(int) {
                if (t == hsv_type::hue) {
                    float old = calc_hue();
                    operator+= (1);
                    return old;
                }

                if (t == hsv_type::saturation) {
                    float old = calc_sat();
                    operator+= (1);
                    return old;
                }

                if (t == hsv_type::value) {
                    float old = calc_val();
                    operator+= (1);
                    return old;
                }
            }

            float operator--() {
                return operator-=(1);
            }

            float operator--(int) {
                if (t == hsv_type::hue) {
                    float old = calc_hue();
                    operator-= (1);
                    return old;
                }

                if (t == hsv_type::saturation) {
                    float old = calc_sat();
                    operator-= (1);
                    return old;
                }

                if (t == hsv_type::value) {
                    float old = calc_val();
                    operator-= (1);
                    return old;
                }
            }

            float operator*=(const to_float auto& f) {
                if (t == hsv_type::hue) {
                    float h = calc_hue() * static_cast<float>(f);
                    check_hsv_hue_bound(h);
                    return operator=(h);
                }

                if (t == hsv_type::saturation) {
                    float s = calc_sat() * static_cast<float>(f);
                    check_hsv_sat_bound(s);
                    return operator=(s);
                }

                if (t == hsv_type::value) {
                    float v = calc_val() * static_cast<float>(f);
                    check_hsv_val_bound(v);
                    return operator=(v);
                }
            }

            float operator/=(const to_float auto& f) {
                if (t == hsv_type::hue) {
                    float h = calc_hue() / static_cast<float>(f);
                    check_hsv_hue_bound(h);
                    return operator=(h);
                }

                if (t == hsv_type::saturation) {
                    float s = calc_sat() / static_cast<float>(f);
                    check_hsv_sat_bound(s);
                    return operator=(s);
                }

                if (t == hsv_type::value) {
                    float v = calc_val() / static_cast<float>(f);
                    check_hsv_val_bound(v);
                    return operator=(v);
                }
            }

        private:
            struct hsv_calc_data {
                float max;
                float delta;
            };
            
            struct hsv_calc_data float_to_hsv_helper() const{
                float max = arr[0];
                if (arr[1] > max) {
                    max = arr[1];
                } 
                if (arr[2] > max) {
                    max = arr[2];
                }

                float min = arr[0];
                if (arr[1] < min) {
                    min = arr[1];
                }
                if (arr[2] < min) {
                    min = arr[2];
                }
                float delta = max - min;
                return {max, delta};
            }

            float calc_hue() const{
                auto data = float_to_hsv_helper();

                if (data.delta == 0.0f) {
                        return 0.0f;
                    }
                else if (data.max == arr[0]) {
                    return std::fmod(60.0f * (arr[1] - arr[2]) / data.delta + 360.0f, 360.0f);
                }

                else if (data.max == arr[1]) {
                    return std::fmod(60.0f * (arr[2] - arr[0]) / data.delta + 120.0f, 360.0f);
                }
                else {
                    return std::fmod(60.0f * (arr[0] - arr[1]) / data.delta + 240.0f, 360.0f);
                }
            }

            float calc_sat() const{
                auto data = float_to_hsv_helper();
                if (data.max == 0.0f) {
                        return 0.0f;
                    }
                return data.delta/data.max * 100.0f;
            }

            float calc_val() const{
                auto data = float_to_hsv_helper();
                return data.max * 100.0f;
            }
            
    };

    /**
     * @brief Color vector
    */
    struct color {
        union {
            float data[4];
            struct {float x, y, z, w;};
            struct {float float_r, float_g, float_b, a;};
            rgb_access<color, 0> r; // Red  from 0 - 255 RGB scale
            rgb_access<color, 1> g; // Green from 0 - 255 RGB scale
            rgb_access<color, 2> b; // Blue from 0 - 255 RGB scale

            hsv_access<hsv_type::hue> h; // Hue from 0 - 360 scale
            hsv_access<hsv_type::saturation> s; // Saturation from 0 - 100 scale
            hsv_access<hsv_type::value> v; // Value from 0 - 100 scale
        };

        constexpr color() {}
        constexpr color(const float _v) : x(_v), y(_v), z(_v), w(1.0f) {};
        constexpr color(const float _v, const float _a) : x(_v), y(_v), z(_v), w(_a) {};
        constexpr color(const float _x, const float _y, const float _z, const float _w) : 
                x(_x), y(_y), z(_z), w(_w) {};
        
        operator float*() { return data; };
        operator const float*() const { return static_cast<const float*>(data); };
        
    };

    inline std::ostream &operator<<(std::ostream &os, const color &vec) {
        os << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
        return os;
    }

    /**
     * @brief Make color from rgb
     * @param r Red from 0-255.0f
     * @param g Green from 0-255.0f
     * @param b Blue from 0-255.0f
     * @param alpha Alpha from 0-1.0f
     * @return color made from  params
    */
    struct color make_rgb(const to_float auto& r, const to_float auto& g, 
            const to_float auto& b, const float& alpha) {

        check_rgb_bound(r);
        check_rgb_bound(g);
        check_rgb_bound(b);

        return color(static_cast<float>(r) / 255.0f, 
                     static_cast<float>(g) / 255.0f, 
                     static_cast<float>(b) / 255.0f, 
                     alpha);
    }

    /**
     * @brief Make color from rgb
     * @param v Value to assign r, g, and b from 0-255.0f
     * @param alpha Alpha from 0-1.0f
     * @return color made from  params
    */
    struct color make_rgb(const to_float auto& v, const float& alpha) {

        check_rgb_bound(v);
        float scaled = static_cast<float>(v) / 255.0f;
        return color(scaled, alpha);
    }

    /**
     * @brief Make color from hsv
     * @param h Hue from 0-360.0f
     * @param s Saturation from 0-100.0f
     * @param v Value from 0-100.0f
     * @param alpha Alpha from 0-1.0f
     * @return color made from params
    */
    struct color make_hsv(const to_float auto& h, const to_float auto& s, const to_float auto& v, 
        const to_float auto& alpha) {
        
        auto arr = hsv_to_float(h, s, v);
        return color(arr[0], arr[1], arr[2], alpha);
    }

    /**
     * @brief Make color from 6-digit hex color
     * @param hex Hex number
     * @return color
    */
    inline struct color make_hex(int32_t hex) {
        int temp_1 = (hex %= 16);
        int temp_2 = (hex %= 16);
        int b = temp_1 + 16 * temp_2;
        temp_1 = (hex %= 16);
        temp_2 = (hex %= 16);
        int g = temp_1 + 16 * temp_2;
        temp_1 = (hex %= 16);
        temp_2 = (hex %= 16);
        int r = temp_1 + 16 * temp_2;
        return color(r, g, b, 1.0f);
    }

    /**
     * @brief Make color from 6 digit hex color string or 3 digit triplet
     * @param str String to use
     * @return color
    */
   inline struct color make_hex(const std::string& s) {

       int32_t hex;

        if (s.size() == 6) {
            hex = (int32_t)std::strtol(s.c_str(), NULL, 16);
        }
        else if (s.size() == 3) {
            std::string six_digit = std::string("0");
            hex = (int32_t)std::strtol(six_digit.c_str(), NULL, 16);
        }
        else {
            throw std::invalid_argument(std::string("color-vec::make_hex takes in a 6 digit str") 
                + std::string("or 3 digit triplet"));
        }
        return make_hex(hex);
   }
}
