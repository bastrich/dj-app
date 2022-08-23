#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

using std::string;

class TrackDeck : public Component,
                  public Button::Listener,
                  public Slider::Listener,
                  public FileDragAndDropTarget,
                  public Timer,
                  public LookAndFeel_V4 {
public:
    TrackDeck(DJAudioPlayer *player,
              AudioFormatManager &formatManagerToUse,
              AudioThumbnailCache &cacheToUse);

    ~TrackDeck() override;

    void loadFile(string filePath);

    void paint(Graphics &) override;

    void resized() override;

    void buttonClicked(Button *) override;

    void sliderValueChanged(Slider *slider) override;

    bool isInterestedInFileDrag(const StringArray &files) override;

    void filesDropped(const StringArray &files, int x, int y) override;

    void timerCallback() override;

    void drawRotarySlider(
            juce::Graphics &g,
            int x,
            int y,
            int width,
            int height,
            float sliderPos,
            float rotaryStartAngle,
            float rotaryEndAngle,
            juce::Slider &
    ) override;

private:
    FileChooser fChooser{"Select a file..."};

    DJAudioPlayer *player;

    Label trackLabel;
    WaveformDisplay waveformDisplay;
    ImageButton playButton;
    ImageButton stopButton;
    ImageButton loadButton;
    Slider positionSlider;
    Slider volumeSlider;
    Slider speedSlider;
    Slider reverbSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackDeck)
};
