#pragma once

#include <JuceHeader.h>

class Utils {
public:
    static void setupImageButton(ImageButton &imageButton, const void *imageData, const int dataSize) {
        Image image = ImageCache::getFromMemory(imageData, dataSize);
        imageButton.setImages(
                true,
                true,
                true,
                image,
                1,
                Colours::transparentWhite,
                image,
                1,
                Colour{0x6fffffff},
                image,
                1,
                Colour{0x1f0000ff}
        );
    }
};