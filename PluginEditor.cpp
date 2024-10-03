#include "PluginProcessor.h"
#include "PluginEditor.h"

MaqamRastAudioProcessorEditor::MaqamRastAudioProcessorEditor (MaqamRastAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p) {
    setSize (400, 300);
    startTimerHz(10); // Update the UI every 100ms
}

void MaqamRastAudioProcessorEditor::paint (juce::Graphics& g) {
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Maqam Rast Pitch Detection", 10, 10, getWidth() - 20, 30, juce::Justification::centred);

    g.setFont (15.0f);
    g.drawText ("Closest Note: " + lastDetectedNote, 10, 50, getWidth() - 20, 30, juce::Justification::centred);
}

void MaqamRastAudioProcessorEditor::timerCallback() {
    lastDetectedNote = audioProcessor.getLastDetectedNote(); // Custom function to return the last note
    repaint();
}

void MaqamRastAudioProcessorEditor::resized() {
}
