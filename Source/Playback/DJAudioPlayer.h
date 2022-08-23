#pragma once

#include <JuceHeader.h>

using std::unique_ptr;

class DJAudioPlayer : public AudioSource {
public:

    DJAudioPlayer(AudioFormatManager &_formatManager);

    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;

    void releaseResources() override;

    void loadURL(URL audioURL);

    void setGain(double gain);

    void setSpeed(double ratio);

    void setPosition(double posInSecs);

    void setPositionRelative(double pos);

    void setReverb(double reverb);

    void start();

    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();

private:
    AudioFormatManager &formatManager;
    unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    ReverbAudioSource reverbAudioSource{&resampleSource, false};
};




