#pragma once

#include <JuceHeader.h>

// A utility to convert frequency (Hz) to MIDI note number
inline float frequencyToMidi(float frequency) {
    return 69 + 12 * std::log2(frequency / 440.0f); // A4 = 440Hz
}

class MaqamRastAudioProcessor  : public juce::AudioProcessor
{
public:
    MaqamRastAudioProcessor();
    ~MaqamRastAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void releaseResources() override;

    const juce::String getName() const override;
    
    float detectPitch(const float* samples, int numSamples);

    // Compare pitch to Maqam Rast notes
    juce::String comparePitchToMaqam(float pitch);

private:
    juce::dsp::FFT forwardFFT;
    std::vector<float> pitchBuffer;
    float sampleRate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MaqamRastAudioProcessor)
};
