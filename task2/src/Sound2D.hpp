#ifndef AUDIO2D_HPP
#define AUDIO2D_HPP

#include <functional>
#include <utility>
#include <math.h>


class Sound2D
{
public:
    Sound2D(double head_radius,  double sound_speed);
    std::pair<float,float> makeSound(float angle, float distance, float time);
    void setSoundGenerator(const std::function<float(float)> &generator);

private:
    float calculateDelay(float angle, float distance);
    double head_radius;
    double sound_speed;
    std::function<float(float)> generator;
};

#endif // AUDIO2D_HPP
