#include "DJAudioPlayer.h"

#include <utility>

using std::unique_ptr;

DJAudioPlayer::DJAudioPlayer(AudioFormatManager &_formatManager)
        : formatManager(_formatManager) {
}

DJAudioPlayer::~DJAudioPlayer() = default;

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    reverbAudioSource.setBypassed(true);

    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) {
    reverbAudioSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbAudioSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL) {
    auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
                                                                                  true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource = std::move(newSource);
    }
}

void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 1.0) {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    } else {
        transportSource.setGain(gain);
    }

}

void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio > 100.0) {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");
    } else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos > 1.0) {
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    } else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::setReverb(double reverb) {
    if (reverb > 0) {
        reverbAudioSource.setBypassed(false);
    } else {
        reverbAudioSource.setBypassed(true);
        return;
    }

    Reverb::Parameters parameters = reverbAudioSource.getParameters();
    parameters.wetLevel = static_cast<float>(reverb);
    parameters.dryLevel = static_cast<float>(1 - reverb);
    reverbAudioSource.setParameters(parameters);
}


void DJAudioPlayer::start() {
    transportSource.start();
}

void DJAudioPlayer::stop() {
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative() {
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}