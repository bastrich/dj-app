#include "Library.h"
#include "TrackStorage.h"
#include "TrackInfo.h"


Library::Library(
        AudioFormatManager &formatManager,
        const std::function<void(TrackInfo &)> playOnDeck1Impl,
        const std::function<void(TrackInfo &)> playOnDeck2Impl
) : formatManager(formatManager), playOnDeck1Impl(playOnDeck1Impl), playOnDeck2Impl(playOnDeck2Impl) {

    addAndMakeVisible(addTrackButton);
    addAndMakeVisible(searchTrackTextEditor);
    addAndMakeVisible(tableComponent);

    tableComponent.getHeader().addColumn("Track Title", 1, 1);
    tableComponent.getHeader().addColumn("Duration", 2, 1);
    tableComponent.getHeader().addColumn("Actions", 3, 1);
    tableComponent.setModel(this);

    addTrackButton.addListener(this);

    tableComponent.setColour(ListBox::ColourIds::backgroundColourId, Colours::transparentWhite);
    tableComponent.setColour(ListBox::ColourIds::outlineColourId, Colours::transparentWhite);
//    tableComponent.setColour(ListBox::ColourIds::backgroundColourId, Colours::transparentWhite);
}

Library::~Library() {

}

void Library::paint(Graphics &g) {
//    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

//    g.setColour(Colours::grey);
//    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);

    g.drawText(
            "Music Library",
            0,
            0,
            getWidth() / 5,
            getHeight() / 10,
            Justification::centred,
            true
    );
}

void Library::resized() {
    addTrackButton.setBounds(getWidth() / 5, 0, getWidth() / 5, getHeight() / 10);
    searchTrackTextEditor.setBounds(getWidth() * 2 / 5, 0, getWidth() / 5, getHeight() / 10);
    tableComponent.setBounds(0, getHeight() / 10, getWidth(), getHeight() * 9 / 10);

    tableComponent.getHeader().setColumnWidth(1, getWidth() * 6 / 10);
    tableComponent.getHeader().setColumnWidth(2, getWidth() / 10);
    tableComponent.getHeader().setColumnWidth(3, getWidth() * 3/10);
}

int Library::getNumRows() {
    return trackStorage.getTracks().size();
}

void Library::paintRowBackground(Graphics &g,
                                 int rowNumber,
                                 int width,
                                 int height,
                                 bool rowIsSelected) {
// just highlight selected rows
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    } else {
        g.fillAll(Colours::transparentWhite);
    }
}

void Library::paintCell(Graphics &g,
                        int rowNumber,
                        int columnId,
                        int width,
                        int height,
                        bool rowIsSelected) {
    TrackInfo trackInfo = trackStorage.getTracks()[rowNumber];

    if (columnId == 1) {
        g.drawText(
                trackInfo.name, // the important bit
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true
        );
    }
    if (columnId == 2) {
        g.drawText(
                std::to_string(trackInfo.durationSeconds), // the important bit
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true
        );
    }
}

Component *Library::refreshComponentForCell(
        int rowNumber,
        int columnId,
        bool isRowSelected,
        Component *existingComponentToUpdate) {
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
//            TrackInfo trackInfo = trackStorage.getTracks()[rowNumber];

//            std::unique_ptr<TrackActions> trackActions{trackInfo, &loadOnDeck1Impl, &loadOnDeck2Impl, &Library::deleteTrack};

            TrackActions* trackActions = new TrackActions{
                    trackStorage.getTracks()[rowNumber],
                playOnDeck1Impl,
                playOnDeck2Impl,
                [this] (const TrackInfo& trackInfo)
                    { trackStorage.deleteTrack(trackInfo.filePath); }
            };

            existingComponentToUpdate = trackActions;
        }
    }
    return existingComponentToUpdate;
}

void Library::buttonClicked(Button *button) {
    if (button == &addTrackButton) {
        fChooser.launchAsync(FileBrowserComponent::canSelectFiles, [this](const FileChooser &chooser) {
            File audioFile = chooser.getResult();
            trackStorage.addTrack(audioFile);
        });
    }
}

void Library::textEditorTextChanged(TextEditor& textEditor) {
    int a = 5;
}