

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class SyAmpAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SyAmpAudioProcessorEditor();
    ~SyAmpAudioProcessorEditor();

    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
};


#endif  // PLUGINEDITOR_H_INCLUDED
