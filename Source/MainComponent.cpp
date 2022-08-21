#include "MainComponent.h"

MainComponent::MainComponent() {
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
        && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio)) {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
                                    [&](bool granted) { if (granted) setAudioChannels(2, 2); });
    } else {
        setAudioChannels(0, 2);
    }



    addAndMakeVisible(headers);
    addAndMakeVisible(trackDeck1);
    addAndMakeVisible(trackDeck2);
    addAndMakeVisible(library);

    setSize(1200, 800);


    if (formatManager.getNumKnownFormats() == 0) {
        formatManager.registerBasicFormats();
    }



}

MainComponent::~MainComponent() {
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) {
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(Graphics &g) {
//    setAlpha(1);
//    setOpaque(true);

//    g.fillAll(Colour{0x00ffffff});

    Path path{};
    path.addRoundedRectangle(0, 0, getWidth(), getHeight(), 20);
    g.reduceClipRegion(path);

    g.drawImage(backgroundImage, getLocalBounds().toFloat());




//    g.fillAll(Colours::red);
}

void MainComponent::resized() {




    headers.setBounds(0, 0, getWidth(), getHeight() / 10);
    trackDeck1.setBounds(0, getHeight() / 10, getWidth(), getHeight() * 1 / 4);
    trackDeck2.setBounds(0, getHeight() * 7 / 20, getWidth(), getHeight() * 1 / 4);
    library.setBounds(0, getHeight() * 3 / 5, getWidth(), getHeight() * 2 / 5);
}

