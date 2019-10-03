#include "Sound2D.hpp"

Sound2D::Sound2D(double head_radius,  double sound_speed)
    : head_radius(head_radius), sound_speed(sound_speed)
{

}

void Sound2D::setSoundGenerator(const std::function<double(double)> &generator)
{
    this->generator = generator;
}

double Sound2D::calculateDelay(double angle, double distance)
{
    // calculate distance to one ear of the listener
    const double r = sqrt(head_radius*head_radius + distance*distance
                          + 2*head_radius*distance*cos(angle));
    // time = path / speed;
    return r/sound_speed;
}

std::pair<double, double> Sound2D::makeSound(double angle, double distance, double t)
{
    const double left_ear  = generator(t + calculateDelay(angle, distance));
    const double right_ear = generator(t + calculateDelay(M_PI - angle, distance));
    return std::pair<double, double>(left_ear, right_ear);
}
