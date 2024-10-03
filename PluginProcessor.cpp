#include "PluginProcessor.h"

MaqamRastAudioProcessor::MaqamRastAudioProcessor()
    : forwardFFT(juce::dsp::FFT::Order::order2048) // Use a 2048-point FFT
{}

MaqamRastAudioProcessor::~MaqamRastAudioProcessor() {}

void MaqamRastAudioProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock) {
    sampleRate = newSampleRate;
}

void MaqamRastAudioProcessor::releaseResources() {}

// Real-time pitch detection using autocorrelation (simple algorithm)
float MaqamRastAudioProcessor::detectPitch(const float* samples, int numSamples) {
    // Autocorrelation-based pitch detection
    float maxAutocorr = 0;
    int maxLag = 0;

    for (int lag = 1; lag < numSamples / 2; ++lag) {
        float autocorr = 0;
        for (int i = 0; i < numSamples - lag; ++i) {
            autocorr += samples[i] * samples[i + lag];
        }
        if (autocorr > maxAutocorr) {
            maxAutocorr = autocorr;
            maxLag = lag;
        }
    }

    if (maxLag == 0) return 0.0f; // No pitch detected

    // Convert lag to frequency
    float detectedFreq = sampleRate / maxLag;
    return detectedFreq;
}

// Compare the pitch to the Maqam Rast notes
juce::String MaqamRastAudioProcessor::comparePitchToMaqam(float pitch) {
    // Adding the Jawab (C in the next octave)
    const float maqamNotes[] = { 261.63f, 293.66f, 315.0f, 349.23f, 392.00f, 440.0f, 470.0f, 523.25f }; // C4, D4, E-half-flat, F4, G4, A4, B-half-flat, C5
    const juce::String maqamNames[] = { "C", "D", "E-half-flat", "F", "G", "A", "B-half-flat", "C (Jawab)" };

    float minDiff = std::numeric_limits<float>::max();
    int closestNoteIndex = 0;

    // Compare the detected pitch to the notes in Maqam Rast
    for (int i = 0; i < 8; ++i) {
        float diff = std::abs(pitch - maqamNotes[i]);
        if (diff < minDiff) {
            minDiff = diff;
            closestNoteIndex = i;
        }
    }

    return maqamNames[closestNoteIndex];
}

void MaqamRastAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    auto* channelData = buffer.getReadPointer(0); // Mono audio (use the first channel)
    int numSamples = buffer.getNumSamples();

    // Detect pitch from the audio input
    float detectedPitch = detectPitch(channelData, numSamples);

    // If a pitch is detected, compare it to the Maqam Rast notes
    if (detectedPitch > 0.0f) {
        juce::String closestNote = comparePitchToMaqam(detectedPitch);
        DBG("Detected pitch: " << detectedPitch << " Hz, Closest Maqam Rast note: " << closestNote);
    }
}

const juce::String MaqamRastAudioProcessor::getName() const {
    return JucePlugin_Name;
}
