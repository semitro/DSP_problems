#include "AudioFile.hpp"
#include <iostream>
#include <math.h>

using namespace std;

AudioFile<double> audio;
AudioFile<double>::AudioBuffer buffer;

double sami(double distance_e, double distance_source, double sound_speed, double angle){
    const double r = sqrt(distance_e*distance_e + distance_source*distance_source
                          - 2*distance_e*distance_source*cos(angle));
    return r/sound_speed;
}
double samr(double distance_e, double distance_source,  double sound_speed, double angle){
    const double r = sqrt(distance_e*distance_e + distance_source*distance_source
                          + 2*distance_e*distance_source*cos(angle));
    return r/sound_speed;
}

#define BIT_DEPTH 16
#define SAMPLE_RATE 42100
#define SAMPLES_NUM 1000000

int main(){
    audio.setBitDepth(BIT_DEPTH);
    audio.setSampleRate(SAMPLE_RATE);

    const size_t numSamples = SAMPLES_NUM;
    const double distance_e  = 0.102; // meters
    const double speed_sound = 340.29; // meters
    const double distance_source = 50.5; // meters
    const double speed_rotating  = 1.14; // rad / sec

    double angle = 0.;

    double time;
    buffer.resize(2);
    buffer[0].resize(numSamples);
    buffer[1].resize(numSamples);

    for (int i = 0; i < numSamples; i++)
    {
        time = i/(double)SAMPLE_RATE;
        angle = time*speed_rotating;
        double delay_r = sami(distance_e, distance_source, speed_sound, angle);
        double delay_l = samr(distance_e, distance_source, speed_sound, angle);
        buffer[0][i] = sin(440*6.18*(time+delay_l));//audioFile.samples[0][i];
        buffer[1][i] = sin(440*6.18*(time+delay_r));//audioFile.samples[1][i];
    }
    audio.setAudioBuffer(buffer);
    audio.save("result.wav");
    audio.printSummary();
}
