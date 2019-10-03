#include <iostream>
#include <utility>
#include <string>
#include <math.h>
#include "AudioFile.hpp"
#include "Sound2D.hpp"

using namespace std;

/* got from external library */
AudioFile<double> audio;
AudioFile<double>::AudioBuffer buffer;

#define BIT_DEPTH 16
#define SAMPLE_RATE 42100
#define SAMPLES_NUM 700000

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "usage: " << argv[0] << " distance-in-meters speed-rad/sec\n";
        return 0;
    }
    /* init audio things */
    const size_t numSamples = SAMPLES_NUM;
    audio.setBitDepth(BIT_DEPTH);
    audio.setSampleRate(SAMPLE_RATE);
    buffer.resize(2);
    buffer[0].resize(numSamples);
    buffer[1].resize(numSamples);

    /* init listener parameters */
    const double head_radius = 0.102; // meters
    const double sound_speed = 340.29;
    double distance_source;
    double speed_rotating;
    try{
        distance_source = stoi(argv[1]);
        /* rad/sec */
        speed_rotating  = stoi(argv[2]); // rad / sec
    }catch(std::invalid_argument& e){
        cout << "the parameters should be numbers!\n";
        cout << "example: " << argv[0] << " 1. 6.14\n";
        return 0;
    }
    cout << "Generating sound with parameters:\n";
    cout << "Head radius = " << head_radius << " meters\n";
    cout << "Speed of sound = " << sound_speed << " meters\n";
    cout << "Distance to the source = " << distance_source << " meters\n";
    cout << "Source is rotating at speed = " << speed_rotating << " radians per second\n";
    /* create sound in space */
    Sound2D sound2d(head_radius, sound_speed);
    /* make it produce the "A" note */
    sound2d.setSoundGenerator([](auto t){return sin(440*2*M_PI*t);});

    double angle = 0.;
    double time;
    for (int i = 0; i < numSamples; i++)
    {
        time  = i/(double)SAMPLE_RATE; /* current time */
        angle = time*speed_rotating;   /* make it rotate */
        auto sample = sound2d.makeSound(angle, distance_source, time);
        buffer[0][i] = sample.first;
        buffer[1][i] = sample.second;
    }
    /* save result to FS */
    audio.setAudioBuffer(buffer);
    cout << "generating file result.waw\n";
    audio.printSummary();
    audio.save("result.wav");
}
