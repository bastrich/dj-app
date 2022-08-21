/*
  ==============================================================================

    TrackDeck.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

using std::string;

//==============================================================================
/*
*/
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

    ~TrackDeck();

    void loadFile(string filePath);

    void paint(Graphics &) override;

    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider *slider) override;

    bool isInterestedInFileDrag(const StringArray &files) override;

    void filesDropped(const StringArray &files, int x, int y) override;

    void timerCallback() override;

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &) override;

private:
    FileChooser fChooser{"Select a file..."};
    DJAudioPlayer *player;

    Label trackLabel{"trackLabel"};
    WaveformDisplay waveformDisplay;
    ImageButton playButton{"Play"};
    ImageButton stopButton{"Stop"};
    ImageButton loadButton{"Load"};
    Slider positionSlider;
    Slider volumeSlider;
    Slider speedSlider;
    Slider reverbSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackDeck)
};
