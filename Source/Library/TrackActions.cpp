#include "TrackActions.h"

using std::function;

TrackActions::TrackActions(
        TrackInfo &trackInfo,
        const function<void(TrackInfo &)> &loadOnDeck1Impl,
        const function<void(TrackInfo &)> &loadOnDeck2Impl,
        const function<void(TrackInfo &)> &deleteTrackImpl
) : trackInfo(trackInfo),
    loadOnDeck1Impl(loadOnDeck1Impl),
    loadOnDeck2Impl(loadOnDeck2Impl),
    deleteTrackImpl(deleteTrackImpl) {

    addAndMakeVisible(loadOnDeck1Button);
    addAndMakeVisible(loadOnDeck2Button);
    addAndMakeVisible(deleteButton);

    loadOnDeck1Button.addListener(this);
    loadOnDeck2Button.addListener(this);
    deleteButton.addListener(this);

    loadOnDeck1Button.setColour(TextButton::ColourIds::buttonColourId, Colours::darkorchid);
    loadOnDeck1Button.setColour(TextButton::ColourIds::buttonOnColourId, Colours::orchid);
    loadOnDeck1Button.setColour(TextButton::ColourIds::textColourOffId, Colours::aliceblue);
    loadOnDeck1Button.setColour(TextButton::ColourIds::textColourOnId, Colours::aliceblue);

    loadOnDeck2Button.setColour(TextButton::ColourIds::buttonColourId, Colours::darkorchid);
    loadOnDeck2Button.setColour(TextButton::ColourIds::buttonOnColourId, Colours::orchid);
    loadOnDeck2Button.setColour(TextButton::ColourIds::textColourOffId, Colours::aliceblue);
    loadOnDeck2Button.setColour(TextButton::ColourIds::textColourOnId, Colours::aliceblue);

    deleteButton.setColour(TextButton::ColourIds::buttonColourId, Colours::crimson);
    deleteButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::red);
    deleteButton.setColour(TextButton::ColourIds::textColourOffId, Colours::aliceblue);
    deleteButton.setColour(TextButton::ColourIds::textColourOnId, Colours::aliceblue);
}

TrackActions::~TrackActions() = default;

void TrackActions::resized() {
    loadOnDeck1Button.setBounds(getWidth() / 20, 0, getWidth() / 4, getHeight());
    loadOnDeck2Button.setBounds(getWidth() / 4 + 2 * getWidth() / 20, 0, getWidth() / 4, getHeight());
    deleteButton.setBounds(2 * getWidth() / 4 + 3 * getWidth() / 20, 0, getWidth() / 4, getHeight());
}

void TrackActions::buttonClicked(Button *button) {
    if (button == &loadOnDeck1Button) {
        loadOnDeck1Impl(trackInfo);
    }
    if (button == &loadOnDeck2Button) {
        loadOnDeck2Impl(trackInfo);
    }
    if (button == &deleteButton) {
        deleteTrackImpl(trackInfo);
    }
}