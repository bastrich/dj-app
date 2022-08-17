#include "TrackDeck.h"
#include "Library/TrackInfo.h"

//==============================================================================
TrackDeck::TrackDeck(DJAudioPlayer *_player,
                     AudioFormatManager &formatManagerToUse,
                     AudioThumbnailCache &cacheToUse
) : player(_player),
    waveformDisplay(formatManagerToUse, cacheToUse) {

    addAndMakeVisible(trackLabel);

    Image playButtonImage = ImageFileFormat::loadFrom(File("/Users/bastrich/dev/study/otodecks/Source/play.png"));
    playButton.setImages(
            true,
            true,
            true,
            playButtonImage,
            0.5,
            Colours::transparentWhite,
            playButtonImage,
            0.5,
            Colours::transparentWhite,
            playButtonImage,
            0.5,
            Colours::transparentWhite
    );

    Image stopButtonImage = ImageFileFormat::loadFrom(File("/Users/bastrich/dev/study/otodecks/Source/stop.png"));
    stopButton.setImages(
            true,
            true,
            true,
            stopButtonImage,
            0.5,
            Colours::transparentWhite,
            stopButtonImage,
            0.5,
            Colours::transparentWhite,
            stopButtonImage,
            0.5,
            Colours::transparentWhite
    );

    Image loadButtonImage = ImageFileFormat::loadFrom(File("/Users/bastrich/dev/study/otodecks/Source/load.png"));
    loadButton.setImages(
            true,
            true,
            true,
            loadButtonImage,
            0.5,
            Colours::transparentWhite,
            loadButtonImage,
            0.5,
            Colours::transparentWhite,
            loadButtonImage,
            0.5,
            Colours::transparentWhite
    );

    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(reverbSlider);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);
    reverbSlider.addListener(this);


    volumeSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    positionSlider.setRange(0.0, 1.0);
    reverbSlider.setRange(0.0, 1.0);

    reverbSlider.setSliderStyle(Slider::Rotary);
    reverbSlider.setLookAndFeel(this);

    startTimer(500);
}

TrackDeck::~TrackDeck() {
    stopTimer();
}

void TrackDeck::paint(Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);

    g.drawText(
            "Position",
            getWidth() * 2 / 5,
            getHeight() / 5,
            getWidth() * 1 / 10,
            getHeight() * 4 / 15,
            Justification::centred,
            true
    );
    g.drawText(
            "Volume",
            getWidth() * 2 / 5,
            getHeight() / 5 + getHeight() * 4 / 15,
            getWidth() * 1 / 10,
            getHeight() * 4 / 15,
            Justification::centred,
            true
    );
    g.drawText(
            "Speed",
            getWidth() * 2 / 5,
            getHeight() / 5 + getHeight() * 4 / 15 * 2,
            getWidth() * 1 / 10,
            getHeight() * 4 / 15,
            Justification::centred,
            true
    );
    g.drawText(
            "Reverb",
            getWidth() * 4 / 5,
            0,
            getWidth() * 1 / 5,
            getHeight() / 5,
            Justification::centred,
            true
    );
}

void TrackDeck::resized() {
    trackLabel.setBounds(0, 0, getWidth() * 2 / 5, getHeight() / 5);
    waveformDisplay.setBounds(0, getHeight() / 5, getWidth() * 2 / 5, getHeight() * 4 / 5);
    playButton.setBounds(getWidth() * 2 / 5, 0, getHeight() / 5, getHeight() / 5);
    stopButton.setBounds(getWidth() * 2 / 5 + getHeight() / 5, 0, getHeight() / 5, getHeight() / 5);
    loadButton.setBounds(getWidth() * 2 / 5 + getHeight() / 5 * 2, 0, getHeight() / 5, getHeight() / 5);
    positionSlider.setBounds(getWidth() * 2 / 5 + getWidth() * 1 / 10, getHeight() / 5, getWidth() * 3 / 10, getHeight() * 4 / 15);
    volumeSlider.setBounds(getWidth() * 2 / 5 + getWidth() * 1 / 10, getHeight() / 5 + getHeight() * 4 / 15, getWidth() * 3 / 10, getHeight() * 4 / 15);
    speedSlider.setBounds(getWidth() * 2 / 5 + getWidth() * 1 / 10, getHeight() / 5 + getHeight() * 4 / 15 * 2, getWidth() * 3 / 10, getHeight() * 4 / 15);
    reverbSlider.setBounds(getWidth() * 4 / 5, getHeight() / 5, getWidth() * 1 / 5, getHeight() * 4 / 5);
}

void TrackDeck::buttonClicked(Button *button) {
    if (button == &playButton) {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton) {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton) {
        auto fileChooserFlags =
                FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser) {
            player->loadURL(URL{chooser.getResult()});
            // and now the waveformDisplay as well
            waveformDisplay.loadURL(URL{chooser.getResult()});
        });
    }
    // if (button == &loadButton)
    // {
    //     FileChooser chooser{"Select a file..."};
    //     if (chooser.browseForFileToOpen())
    //     {
    //         player->loadURL(URL{chooser.getResult()});
    //         waveformDisplay.loadURL(URL{chooser.getResult()});

    //     }


    // }
}

void TrackDeck::sliderValueChanged(Slider *slider) {
    if (slider == &volumeSlider) {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());
    }

    if (slider == &positionSlider) {
        player->setPositionRelative(slider->getValue());
    }

    if (slider == &reverbSlider) {
        player->setReverb(slider->getValue());
    }
}

bool TrackDeck::isInterestedInFileDrag(const StringArray &files) {
    std::cout << "TrackDeck::isInterestedInFileDrag" << std::endl;
    return true;
}

void TrackDeck::loadFile(string filePath) {
    File file{filePath};
    URL url{file};
    trackLabel.setText(file.getFileName(), NotificationType::dontSendNotification);
    player->loadURL(url);
    waveformDisplay.loadURL(url);
}

void TrackDeck::filesDropped(const StringArray &files, int x, int y) {
    std::cout << "TrackDeck::filesDropped" << std::endl;
    if (files.size() == 1) {
        loadFile(files[0].toStdString());
    }
}

void TrackDeck::timerCallback() {
    //std::cout << "TrackDeck::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}

//https://docs.juce.com/master/tutorial_look_and_feel_customisation.html
void TrackDeck::drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos,
                                 const float rotaryStartAngle, const float rotaryEndAngle, Slider &) {
    auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour (juce::Colours::orange);
    g.fillEllipse (rx, ry, rw, rw);

    // outline
    g.setColour (juce::Colours::red);
    g.drawEllipse (rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));

    // pointer
    g.setColour (juce::Colours::yellow);
    g.fillPath (p);
}


    

