#pragma once

#include <JuceHeader.h>

class WaveformDisplay : public Component,
                        public ChangeListener {
public:
    WaveformDisplay(AudioFormatManager &formatManagerToUse,
                    AudioThumbnailCache &cacheToUse);

    ~WaveformDisplay() override;

    void paint(Graphics &) override;

    void changeListenerCallback(ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
