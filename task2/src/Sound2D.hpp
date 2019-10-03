//=======================================================================
/** @file Sound2D.hpp
 *  @author Artem Oshchepkov (semitro_8@mail.ru)
 * This file contains class to generate volumetric sound
 */
//=======================================================================
#ifndef AUDIO2D_HPP
#define AUDIO2D_HPP

#include <functional>
#include <utility>
#include <math.h>
/*
 * This class can transform sound generating by external generator
 * to make an illusion that the source of the sound has distance and
 * angle regarding to the listener
*/
class Sound2D
{
public:
    /* head radius of the listener and speed of sound are counted in meters */
    Sound2D(double head_radius, double sound_speed);
    /* @Returns sound by the generator shifted at left and right channels
     * that makes it sound 2D */
    std::pair<double,double> makeSound(double angle, double distance, double time);
    /* Sets the function that generate sound wave from time, ex. sin(440*2pi*t) */
    void setSoundGenerator(const std::function<double(double)> &generator);

private:
    /* @Returns signal delay in seconds */
    double calculateDelay(double angle, double distance);
    double head_radius;
    double sound_speed;
    /* gets time, returns amplitude */
    std::function<double(double)> generator;
};

#endif // AUDIO2D_HPP
