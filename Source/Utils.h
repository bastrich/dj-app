#include <JuceHeader.h>

class Utils {
public:
    static void setupImageButton(ImageButton &imageButton, const Image &image) {
        imageButton.setImages(
                true,
                true,
                true,
                image,
                1,
                Colours::transparentWhite,
                image,
                1,
                Colour{0x2fffffff},
                image,
                1,
                Colour{0x1f0000ff}
        );
    }
};