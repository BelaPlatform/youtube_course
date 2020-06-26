/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

http://bela.io

C++ Real-Time Audio Programming with Bela - Lecture 12: Envelopes
ramp-filter: linear envelope to control the frequency of a resonant filter
*/

#include <Bela.h>
#include <cmath>
#include "Wavetable.h"
#include "Filter.h"

// Variables for linear envelope
const float kRampDuration = 2.0;
const float kFilterFrequencyMin = 200.0;
const float kFilterFrequencyMax = 4000.0;
float gFilterFrequency = kFilterFrequencyMin;  
// TODO: declare any other variables for linear envelope (ramp)

// Oscillator frequency
const float kOscillatorFrequency = 110.0;

// Oscillator and filter objects
Wavetable gOscillator;
Filter gFilter;

// setup() only runs one time
bool setup(BelaContext *context, void *userData)
{
	std::vector<float> wavetable;
	const unsigned int wavetableSize = 512;
		
	// Populate a buffer with the first 32 harmonics of a sawtooth wave
	wavetable.resize(wavetableSize);
	for(unsigned int n = 0; n < wavetable.size(); n++) {
		wavetable[n] = 0;
		for(unsigned int harmonic = 1; harmonic <= 32; harmonic++) {
			wavetable[n] += sinf(2.0 * M_PI * (float)harmonic * (float)n / 
								 (float)wavetable.size()) / (float)harmonic;
		}
	}
	
	// Initialise the wavetable, passing the sample rate and the buffer
	gOscillator.setup(context->audioSampleRate, wavetable);
	gOscillator.setFrequency(kOscillatorFrequency);
	
	// Initialise the filter
	gFilter.setSampleRate(context->audioSampleRate);
	gFilter.setFrequency(kFilterFrequencyMin);
	gFilter.setQ(4.0);
	
	// TODO: calculate ramp increment
	
    return true;
}

// render() is called every time there is a new block to calculate
void render(BelaContext *context, void *userData)
{
   	// This for() loop goes through all the samples in the block
	for (unsigned int n = 0; n < context->audioFrames; n++) {
		// TODO: increment the frequency to create a linear ramp
		
		// Update the filter frequency
		gFilter.setFrequency(gFilterFrequency);
		
		// Generate and filter the signal
		float in = gOscillator.process();
		float out = 0.2 * gFilter.process(in);

		// This part is done for you: store the sample in the
		// audio output buffer
		for(unsigned int channel = 0; channel <context->audioOutChannels; channel++) {
		    audioWrite(context, n, channel, out);
		}
    }
}

// cleanup() runs once when the program stops
void cleanup(BelaContext *context, void *userData)
{
	// nothing to do here
}
