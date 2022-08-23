#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(AudioFormatManager &formatManagerToUse,
                                 AudioThumbnailCache &cacheToUse) :
        audioThumb(1000, formatManagerToUse, cacheToUse),
        fileLoaded(false),
        position(0) {
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() = default;

void WaveformDisplay::paint(Graphics &g) {
    g.setColour(Colours::hotpink);
    if (fileLoaded) {
        audioThumb.drawChannel(g,
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f
        );
        g.setColour(Colour{0xaf90ee90});
        g.fillRect(static_cast<int>(position * getWidth()), 0, 3, getHeight());
    } else {
        g.setColour(Colours::firebrick);
        g.setFont(16.0f);
        g.drawText("File not loaded...", getLocalBounds(), Justification::centred,
                   true);

    }
}
void WaveformDisplay::loadURL(URL audioURL) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded) {
        Logger::writeToLog("wfd: loaded!");
        repaint();
    } else {
        Logger::writeToLog("wfd: not loaded!");
    }

}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source) {
    Logger::writeToLog("wfd: change received! ");

    repaint();

}

void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position) {
        position = pos;
        repaint();
    }


}





