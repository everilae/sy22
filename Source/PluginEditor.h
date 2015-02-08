/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class Sy22PanelAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Sy22PanelAudioProcessorEditor (Sy22PanelAudioProcessor&);
    ~Sy22PanelAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Sy22PanelAudioProcessor& processor;
    
    // Common voice controls
    Slider effectDepth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sy22PanelAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
