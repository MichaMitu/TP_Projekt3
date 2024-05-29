#include <iostream>
#include <matplot/matplot.h>
#include <cmath>
#include <algorithm>
#include <complex>
#include "AudioFile/AudioFile.h"

using namespace std;
using namespace matplot;

struct wave {
    vector<double> waveform;
    vector<double> length;
};

namespace create {
    wave gen_wave(double, double, function<void(wave*, double)>, int);
    void sin_wave(wave*, double);
    void cos_wave(wave*, double);
    void saw_wave(wave*, double);
    void square_wave(wave*, double);
    void noise_wave(wave*, double);
    wave get_from_audio(string, int);
}

namespace operations {
    void change_amplitude(wave*, double);
    void add_noise(wave*, double);
    wave get_DFT(wave);
    wave get_IDFT(wave);
    void show_waveform(wave);
}

wave create::gen_wave(double frequency, double length, function<void(wave*, double)> func, int samples = 100) {
    wave wave1;
    wave1.length = linspace(0, length, samples);
    func(&wave1, frequency);
    return wave1;
}

void create::sin_wave(wave *target, double frequency) {
    target->waveform = transform(target->length, [frequency](double x){ return sin(x * frequency * 2 * pi);});
}

void create::cos_wave(wave *target, double frequency) {
    target->waveform = transform(target->length, [frequency](double x){ return cos(x * frequency * 2 * pi);});
}

void create::saw_wave(wave *target, double frequency) {
    target->waveform = transform(target->length, [frequency](double x){ return fmod(x * frequency, 1);});
}

void create::square_wave(wave *target, double frequency) {
    target->waveform = transform(target->length, [frequency](double x){ return sin(copysign(1, sin(x * frequency * 2 * pi)));});
}

//NIE POTRZEBNE
void create::noise_wave(wave *target, double amplitude) {
    target->waveform = transform(target->length, [amplitude](double x){ return rand(-amplitude, amplitude);});
}

wave create::get_from_audio(string path, int sampleNum) {
    wave output;
    AudioFile<double> loadedFile;
    loadedFile.load (path);
    output.length = linspace(0, sampleNum, sampleNum);
    for(int i = 0; i < sampleNum; i++) {
        output.waveform.push_back(loadedFile.samples[0][i]);
    }
    return output;
}

// NIE DZIAŁA (nie wymagane)
// działa bardzo źle przez implementację deklaracji linspace przez dewelopera matplotplusplus
// jest na to kilka rozwiązań:
// np. rozpatrywanie skali próbkowania obu sygnałów
// to co mamy zrobić da się jednak zrobić bez tego
/*
wave operations::add_waves(wave source1, wave source2) {
    wave output;
    if(source1.length.back() >= source2.length.back()) {
        output = source1;
        std::transform(output.waveform.begin(), output.waveform.end(), source2.waveform.begin(), output.waveform.begin(), std::plus<double>());
    } else {
        output = source2;
        std::transform(output.waveform.begin(), output.waveform.end(), source1.waveform.begin(), output.waveform.begin(), std::plus<double>());
    }
    return output;
}
*/

// NIE POTRZEBNE
// byłoby przydatne jakby funkcja wyżej (add_waves()) działała
void operations::change_amplitude(wave *target, double new_amp) {
    double old_amp = 0;
    old_amp += *max_element(target->waveform.begin(), target->waveform.end());
    old_amp -= *min_element(target->waveform.begin(), target->waveform.end());
    old_amp /= 2;
    if (old_amp != 0) {
        target->waveform = transform(target->waveform, [=](double x){ return x * new_amp / old_amp;});
    } else {
        cout << "err: can't change amplitude of flatline" << endl;
    }
}

void operations::add_noise(wave* target, double amount) {
    target->waveform = transform(target->waveform, [=](double x){ return x + rand( -amount , amount);});
}

wave operations::get_DFT(wave input) {
    wave result;
    result.length = input.length;
    for(int i = 0; i < input.length.size(); i++) {
        complex<double> sum = 0;
        for (int j = 0; j < input.length.size(); j++) {
            complex<double> exp_holder (0, -2 * pi * i / input.length.size() * j);
            sum += input.waveform[j] * exp(exp_holder);
        }
        result.waveform.push_back(sum.real());
    }
    return result;
}

wave operations::get_IDFT(wave input) {
    wave result;
    result.length = input.length;
    for(int i = 0; i < input.length.size(); i++) {
        complex<double> sum = 0;
        for (int j = 0; j < input.length.size(); j++) {
            complex<double> exp_holder (0, 2 * pi * i / input.length.size() * j);
            sum += input.waveform[j] * exp(exp_holder);
        }
        result.waveform.push_back(sum.real() / result.length.size());
    }
    return result;
}

void operations::show_waveform(wave wave_to_plot) {
    plot(wave_to_plot.length, wave_to_plot.waveform);
    show();
}

int main() {
    return 0;
}