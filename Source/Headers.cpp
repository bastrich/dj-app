#include "Headers.h"
#include "Utils.h"

Headers::Headers() = default;

void Headers::paint(Graphics &g) {
    g.setColour(Colours::orchid);
    g.setFont(20.0f);

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