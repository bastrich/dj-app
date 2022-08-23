#pragma once

#include <JuceHeader.h>

class Headers : public Component {
public:
    Headers();

    void paint(Graphics &) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Headers)
};
