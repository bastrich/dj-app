#pragma once

#include <JuceHeader.h>
#include "TrackInfo.h"
#include "TrackStorage.h"
#include "TrackActions.h"

class Library : public Component,
                public TableListBoxModel,
                public Button::Listener,
                public TextEditor::Listener {
public:
    explicit Library(
            AudioFormatManager &formatManager,
            const std::function<void(TrackInfo &)> playOnDeck1Impl,
            const std::function<void(TrackInfo &)> playOnDeck2Impl
    );

    ~Library();

    void paint(Graphics &) override;

    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics &,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(Graphics &,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    Component *refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component *existingComponentToUpdate) override;

    void buttonClicked(Button *button) override;

    void textEditorTextChanged(TextEditor& textEditor) override;

private:
    AudioFormatManager &formatManager;
    TrackStorage trackStorage{"tracks.csv", formatManager};
    FileChooser fChooser{"Select a file..."};

    const std::function<void(TrackInfo &)> playOnDeck1Impl;
    const std::function<void(TrackInfo &)> playOnDeck2Impl;


    TextButton addTrackButton{"Add track"};
    TextEditor searchTrackTextEditor;

    TableListBox tableComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Library)
};