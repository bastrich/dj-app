#include "WaveformDisplay.h"
#include "../Utils.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager &formatManagerToUse,
                                 AudioThumbnailCache &cacheToUse) :
        audioThumb(1000, formatManagerToUse, cacheToUse),
        fileLoaded(false),
        position(0) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() {
}

void WaveformDisplay::paint(Graphics &g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::hotpink);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component



    if (fileLoaded) {
        audioThumb.drawChannel(g,
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f
        );
        g.setColour(Colour{0xaf90ee90});
        g.fillRect(position * getWidth(), 0, 3, getHeight());
    } else {
        g.setColour(Colours::firebrick);
        g.setFont(16.0f);
        g.drawText("File not loaded...", getLocalBounds(), Justification::centred,
                   true);   // draw some placeholder text

    }
}

void WaveformDisplay::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..

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





