#pragma once

#include <JuceHeader.h>
#include "TrackInfo.h"

using std::string;
using std::vector;

class TrackStorage {
public:
    TrackStorage(string storageFilePath, AudioFormatManager &formatManager);

    void addTrack(File& file);

    void deleteTrack(string trackFilePath);

    vector<TrackInfo> getTracks(const string& nameFilter);

private:
    AudioFormatManager &formatManager;

    vector<TrackInfo> tracks;
    string storageFilePath;

    void readTracks();

    void saveTracks();
};
