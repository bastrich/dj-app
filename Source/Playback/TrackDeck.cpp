#include "TrackDeck.h"
#include "../Library/TrackInfo.h"
#include "../Utils.h"

using std::stringstream;
using std::string;
using std::setprecision;
using std::fixed;

TrackDeck::TrackDeck(DJAudioPlayer *_player,
                     AudioFormatManager &formatManagerToUse,
                     AudioThumbnailCache &cacheToUse
) : player(_player),
    waveformDisplay(formatManagerToUse, cacheToUse) {

    Utils::setupImageButton(playButton, BinaryData::play_png, BinaryData::play_pngSize);
    Utils::setupImageButton(stopButton, BinaryData::stop_png, BinaryData::stop_pngSize);
    Utils::setupImageButton(loadButton, BinaryData::load_png, BinaryData::load_pngSize);

    addAndMakeVisible(trackLabel);
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
    speedSlider.setRange(0.1, 3.0);
    positionSlider.setRange(0.0, 1.0);
    reverbSlider.setRange(0.0, 1.0);

    volumeSlider.setNumDecimalPlacesToDisplay(3);
    speedSlider.setNumDecimalPlacesToDisplay(3);
    positionSlider.setNumDecimalPlacesToDisplay(3);

    volumeSlider.setColour(Slider::ColourIds::textBoxTextColourId, Colours::orchid);
    speedSlider.setColour(Slider::ColourIds::textBoxTextColourId, Colours::orchid);
    positionSlider.setColour(Slider::ColourIds::textBoxTextColourId, Colours::orchid);

    reverbSlider.setSliderStyle(Slider::Rotary);
    reverbSlider.setLookAndFeel(this);
    reverbSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    startTimer(100);
}

TrackDeck::~TrackDeck() {
    stopTimer();
}

void TrackDeck::paint(Graphics &g) {
    g.setColour(Colours::darkorchid);
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
    stopButton.setBounds(
            getWidth() * 2 / 5 + getHeight() / 5 + 10,
            0,
            getHeight() / 5,
            getHeight() / 5
    );
    loadButton.setBounds(
            getWidth() * 2 / 5 + getHeight() / 5 * 2 + 20,
            0,
            getHeight() / 5,
            getHeight() / 5
    );
    positionSlider.setBounds(
            getWidth() * 2 / 5 + getWidth() * 1 / 10,
            getHeight() / 5,
            getWidth() * 3 / 10,

            getHeight() * 4 / 15
    );
    volumeSlider.setBounds(
            getWidth() * 2 / 5 + getWidth() * 1 / 10,
            getHeight() / 5 + getHeight() * 4 / 15,

            getWidth() * 3 / 10,
            getHeight() * 4 / 15
    );
    speedSlider.setBounds(
            getWidth() * 2 / 5 + getWidth() * 1 / 10,
            getHeight() / 5 + getHeight() * 4 / 15 * 2,
            getWidth() * 3 / 10,
            getHeight() * 4 / 15
    );
    reverbSlider.setBounds(
            getWidth() * 4 / 5,
            getHeight() / 5,
            getWidth() * 1 / 5,
            getHeight() * 4 / 5
    );
}

void TrackDeck::buttonClicked(Button *button) {
    if (button == &playButton) {
        Logger::writeToLog("Play button was clicked ");
        player->start();
    }
    if (button == &stopButton) {
        Logger::writeToLog("Stop button was clicked ");
        player->stop();
    }
    if (button == &loadButton) {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser) {
            loadFile(chooser.getResult().getFullPathName().toStdString());
        });
    }
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
    Logger::writeToLog("TrackDeck::isInterestedInFileDrag");
    return true;
}

void TrackDeck::loadFile(string filePath) {
    File file{filePath};
    URL url{file};

    trackLabel.setColour(Label::ColourIds::textColourId, Colours::darkorchid);
    trackLabel.setText(file.getFileName(), NotificationType::dontSendNotification);

    player->loadURL(url);
    waveformDisplay.loadURL(url);

    speedSlider.setValue(1);
    volumeSlider.setValue(0.5);
    positionSlider.setValue(0);
    reverbSlider.setValue(0);
}

void TrackDeck::filesDropped(const StringArray &files, int x, int y) {
    Logger::writeToLog("TrackDeck::filesDropped");
    if (files.size() == 1) {
        loadFile(files[0].toStdString());
    }
}

void TrackDeck::timerCallback() {
    double relativePosition = player->getPositionRelative();
    if (!isnan(relativePosition)) {
        positionSlider.setValue(relativePosition, NotificationType::dontSendNotification);
        waveformDisplay.setPositionRelative(relativePosition);
    }
}


void TrackDeck::drawRotarySlider(
        Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        const float rotaryStartAngle,
        const float rotaryEndAngle,
        Slider &
) {
    //init required calculation variables
    int markSide = 30;
    auto markAddRadius = static_cast<float>(sqrt(pow(markSide / 2, 2) + pow(markSide / 2, 2)));
    auto radius = (float) juce::jmin(width / 2, height / 2) - markAddRadius;
    auto centreX = (float) x + (float) width * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    float markRadius = radius + markAddRadius;

    //draw knob marks
    g.setColour(juce::Colours::orchid);
    g.setFont(16.0f);
    drawMark(g,centreX,centreY, markRadius, markSide, rotaryStartAngle, rotaryEndAngle, 0);
    drawMark(g,centreX,centreY, markRadius, markSide, rotaryStartAngle, rotaryEndAngle, 0.25);
    drawMark(g,centreX,centreY, markRadius, markSide, rotaryStartAngle, rotaryEndAngle, 0.5);
    drawMark(g,centreX,centreY, markRadius, markSide, rotaryStartAngle, rotaryEndAngle, 0.75);
    drawMark(g,centreX,centreY, markRadius, markSide, rotaryStartAngle, rotaryEndAngle, 1);

    //draw knob
    g.addTransform(
            juce::AffineTransform::rotation(angle).translated(centreX, centreY)
    );

    g.setGradientFill(ColourGradient(
            Colours::lavender, rx, ry,
            Colours::mediumslateblue, centreX + rw, centreY + rw,
            true
    ));
    g.fillEllipse(-radius, -radius, rw, rw);

    auto pointerLength = radius * 0.5f;
    auto pointerThickness = 10.0f;
    g.setColour(Colours::mediumslateblue);
    g.fillRoundedRectangle(-pointerThickness * 0.5f, -0.85f * radius, pointerThickness, pointerLength, 5);
}

void TrackDeck::drawMark(
        Graphics &g,
        const float centreX,
        const float centreY,
        const float markRadius,
        const int markSide,
        const float rotaryStartAngle,
        const float rotaryEndAngle,
        const float piece
) {
    float angle = 2.5f * MathConstants<float>::pi - (rotaryStartAngle + piece * (rotaryEndAngle - rotaryStartAngle));
    stringstream stream;
    stream << fixed << setprecision(2) << piece;
    string pieceString = stream.str();
    g.drawText(
            pieceString,
            static_cast<int>(centreX + markRadius * cos(angle) - static_cast<float>(markSide) / 2),
            static_cast<int>(centreY - markRadius * sin(angle) - (piece == 0.5 ? 0 : static_cast<float>(markSide) / 2)),
            markSide,
            markSide,
            Justification::centred
    );
}


    

