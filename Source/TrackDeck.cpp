#include "TrackDeck.h"
#include "Library/TrackInfo.h"
#include "Utils.h"

//==============================================================================
TrackDeck::TrackDeck(DJAudioPlayer *_player,
                     AudioFormatManager &formatManagerToUse,
                     AudioThumbnailCache &cacheToUse
) : player(_player),
    waveformDisplay(formatManagerToUse, cacheToUse) {

    addAndMakeVisible(trackLabel);

    Utils::setupImageButton(
            playButton,
            ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize)
    );
    Utils::setupImageButton(
            stopButton,
            ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize)
    );
    Utils::setupImageButton(
            loadButton,
            ImageCache::getFromMemory(BinaryData::load_png, BinaryData::load_pngSize)
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

    volumeSlider.setNumDecimalPlacesToDisplay(3);
    speedSlider.setNumDecimalPlacesToDisplay(3);
    positionSlider.setNumDecimalPlacesToDisplay(3);

    reverbSlider.setSliderStyle(Slider::Rotary);
    reverbSlider.setLookAndFeel(this);
    reverbSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    startTimer(500);
}

TrackDeck::~TrackDeck() {
    stopTimer();
}

void TrackDeck::paint(Graphics &g) {
//    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

//    g.setColour(Colours::grey);
//    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

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
    stopButton.setBounds(getWidth() * 2 / 5 + getHeight() / 5 + 10, 0, getHeight() / 5, getHeight() / 5);
    loadButton.setBounds(getWidth() * 2 / 5 + getHeight() / 5 * 2 + 20, 0, getHeight() / 5, getHeight() / 5);
    positionSlider.setBounds(getWidth() * 2 / 5 + getWidth() * 1 / 10, getHeight() / 5, getWidth() * 3 / 10,
                             getHeight() * 4 / 15);
    volumeSlider.setBounds(getWidth() * 2 / 5 + getWidth() * 1 / 10, getHeight() / 5 + getHeight() * 4 / 15,
                           getWidth() * 3 / 10, getHeight() * 4 / 15);
    speedSlider.setBounds(getWidth() * 2 / 5 + getWidth() * 1 / 10, getHeight() / 5 + getHeight() * 4 / 15 * 2,
                          getWidth() * 3 / 10, getHeight() * 4 / 15);
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


void TrackDeck::drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos,
                                 const float rotaryStartAngle, const float rotaryEndAngle, Slider &) {


    float markSide = 30;
    float markAddRadius = sqrt(pow(markSide / 2, 2) + pow(markSide / 2, 2));

    auto radius = (float) juce::jmin(width / 2, height / 2) - markAddRadius;
    auto centreX = (float) x + (float) width * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);


    g.setColour(juce::Colours::orchid);
    g.setFont(16.0f);

    float markRadius = radius + markAddRadius;

    g.drawText(
            "0.00",
            centreX + markRadius * cos(2.5f * MathConstants<float>::pi - rotaryStartAngle) - markSide / 2,
            centreY - markRadius * sin(2.5f * MathConstants<float>::pi - rotaryStartAngle) - markSide / 2,
            markSide,
            markSide,
            Justification::centred
    );
    g.drawText(
            "0.25",
            centreX + markRadius * cos(2.5f * MathConstants<float>::pi -
                                       (rotaryStartAngle + 0.25f * (rotaryEndAngle - rotaryStartAngle))) - markSide / 2,
            centreY - markRadius * sin(2.5f * MathConstants<float>::pi -
                                       (rotaryStartAngle + 0.25f * (rotaryEndAngle - rotaryStartAngle))) - markSide / 2,
            markSide,
            markSide,
            Justification::centred
    );
    g.drawText(
            "0.50",
            centreX + markRadius * cos(2.5f * MathConstants<float>::pi -
                                       (rotaryStartAngle + 0.5f * (rotaryEndAngle - rotaryStartAngle))) - markSide / 2,
            centreY - markRadius * sin(2.5f * MathConstants<float>::pi -
                                       (rotaryStartAngle + 0.5f * (rotaryEndAngle - rotaryStartAngle))),
            markSide,
            markSide,
            Justification::centred
    );
    g.drawText(
            "0.75",
            centreX + markRadius * cos(2.5f * MathConstants<float>::pi -
                                       (rotaryStartAngle + 0.75f * (rotaryEndAngle - rotaryStartAngle))) - markSide / 2,
            centreY - markRadius * sin(2.5f * MathConstants<float>::pi -
                                       (rotaryStartAngle + 0.75f * (rotaryEndAngle - rotaryStartAngle))) - markSide / 2,
            markSide,
            markSide,
            Justification::centred
    );
    g.drawText(
            "1.00",
            centreX + markRadius * cos(2.5f * MathConstants<float>::pi - rotaryEndAngle) - markSide / 2,
            centreY - markRadius * sin(2.5f * MathConstants<float>::pi - rotaryEndAngle) - markSide / 2,
            markSide,
            markSide,
            Justification::centred
    );


    // fill
//    g.setColour(juce::Colours::orange);
//    g.setGradientFill(ColourGradient::horizontal(Colours::lavender, rx, Colours::mediumslateblue, rx+rw));
//    g.setGradientFill(ColourGradient::vertical(Colours::lavender, ry, Colours::mediumslateblue, ry+rw));

    g.addTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
//    g.setGradientFill(ColourGradient(
//            Colours::lavender, centreX + radius * cos(angle), centreY + radius * sin(angle),
//            Colours::mediumslateblue, centreX + radius * cos(angle), centreY + radius * sin(angle),
//            true
//    ));
    g.setGradientFill(ColourGradient(
            Colours::lavender, rx, ry,
            Colours::mediumslateblue, centreX + rw, centreY + rw,
            true
    ));
//    g.addTransform(AffineTransform::translation(centreX/2, centreY/2));
    g.fillEllipse(-radius, -radius, rw, rw);


//    g.addTransform(juce::AffineTransform::translation(-centreX, -centreY));

//    g.addTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // outline
//    g.setColour(juce::Colours::red);
//    g.drawEllipse(rx, ry, rw, rw, 1.0f);

//    juce::Path p;
    auto pointerLength = radius * 0.5f;
    auto pointerThickness = 10.0f;
    g.setColour(Colours::mediumslateblue);
    g.fillRoundedRectangle(-pointerThickness * 0.5f, -0.85f * radius, pointerThickness, pointerLength, 5);
//    p.applyTransform(juce::AffineTransform::translation(centreX, centreY));

    // pointer
//    g.setColour(juce::Colours::darkcyan);
//    g.fillPath(p);
}


    

