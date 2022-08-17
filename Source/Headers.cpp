#include "Headers.h"

Headers::Headers() = default;

void Headers::paint(Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);

    g.drawText(
            "Track",
            0,
            0,
            getWidth() * 2 / 5,
            getHeight(),
            Justification::centred,
            true
    );
    g.drawText(
            "Playback Controls",
            getWidth() * 2 / 5,
            0,
            getWidth() * 2 / 5,
            getHeight(),
            Justification::centred,
            true
    );
    g.drawText(
            "Effects",
            getWidth() * 4 / 5,
            0,
            getWidth() / 5,
            getHeight(),
            Justification::centred,
            true
    );
}

void Headers::resized() {

}