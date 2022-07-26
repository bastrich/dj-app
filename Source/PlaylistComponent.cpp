/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent() {

    addAndMakeVisible(tableComponent);

    tableComponent.getHeader().addColumn("Track title", 1, 600);
    tableComponent.getHeader().addColumn("", 2, 200);

    tableComponent.setModel(this);

    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

//    g.setColour (Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("DeckGUI", getLocalBounds(),
//                Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows ()
{
    return trackTitles.size();
}
void PlaylistComponent::paintRowBackground (Graphics & g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
// just highlight selected rows
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else{
        g.fillAll(Colours::darkgrey);
    }
}
void PlaylistComponent::paintCell (Graphics & g,
                                   int rowNumber,
                                   int columnId,
                                   int width,
                                   int height,
                                   bool rowIsSelected)
{
    g.drawText (trackTitles[rowNumber], // the important bit
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
}

Component* PlaylistComponent::refreshComponentForCell (
        int rowNumber,
        int columnId,
        bool isRowSelected,
        Component *existingComponentToUpdate)
{
    if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("play");
            existingComponentToUpdate = btn;
            btn->addListener(this);
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked " << trackTitles[id]);
}
