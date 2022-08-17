#pragma once

#include <JuceHeader.h>

using std::string;

class TrackInfo {
public:
    TrackInfo(string name, string filePath, long durationSeconds);

    string name;
    string filePath;
    long durationSeconds;
};
