/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"


//==============================================================================
SyAmpAudioProcessor::SyAmpAudioProcessor()
{
	UserParams[kLow]=0.5f;
	UserParams[kMid]=0.5f;
	UserParams[kHigh] = 0.1f;
	UIUpdateFlag=true;//Request UI update
}

SyAmpAudioProcessor::~SyAmpAudioProcessor()
{
}

//==============================================================================
const String SyAmpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int SyAmpAudioProcessor::getNumParameters()
{
    return kNumParams;
}

float SyAmpAudioProcessor::getParameter (int index)
{
	switch(index)
	{
		case kLow://example nothing special
			return UserParams[kLow];
		case kMid://example update from internal
			return UserParams[kMid];
		case kHigh://example update from internal
			return UserParams[kHigh];
		default: return 0.0f;//invalid index
	}
}

void SyAmpAudioProcessor::setParameter (int index, float newValue)
{
	//dummy_.setParameter(index, newValue);
	switch(index) 
	{
	case kLow:
		UserParams[kLow]=newValue;
		bilTransform_.getLow(UserParams[kLow]);
		break; 
	case kMid:
		UserParams[kMid]=newValue;
		bilTransform_.getMid(UserParams[kMid]);
		break; 
	case kHigh:
		UserParams[kHigh]=newValue;
		bilTransform_.getHigh(UserParams[kHigh]);
		break; 
	default: return;
	}
}

const String SyAmpAudioProcessor::getParameterName (int index)
{
	switch(index)
	{
	case kLow: return "Low"; 
	case kMid: return "Mid"; 
	case kHigh: return "kHigh"; 
	default:return String::empty;
	}
}

const String SyAmpAudioProcessor::getParameterText (int index)
{
	if(index>=0 && index<kNumParams)
		return String(UserParams[index]);//return parameter value as string
	else return String::empty;
}

const String SyAmpAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String SyAmpAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool SyAmpAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool SyAmpAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool SyAmpAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SyAmpAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SyAmpAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double SyAmpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SyAmpAudioProcessor::getNumPrograms()
{
    return 0;
}

int SyAmpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SyAmpAudioProcessor::setCurrentProgram (int index)
{
}

const String SyAmpAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void SyAmpAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SyAmpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	bilTransform_.setSampleRate(sampleRate);
}

void SyAmpAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void SyAmpAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	if(getNumInputChannels()<2)
	{
		float* in = buffer.getSampleData(0);
		bilTransform_.process(*in);
	}
	else
	{
		float* inL = buffer.getSampleData(0);
		float* inR = buffer.getSampleData(1);
		bilTransform_.process(*inL);
		bilTransform_.process(*inR);
	}
}

//==============================================================================
bool SyAmpAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SyAmpAudioProcessor::createEditor()
{
    return 0;
}

//==============================================================================
void SyAmpAudioProcessor::getStateInformation (MemoryBlock& destData)
{//Save UserParams/Data to file
	XmlElement root("Root");
	XmlElement *el;
	el = root.createNewChildElement("Low"); 
	el->addTextElement(String(UserParams[kLow]));
	el = root.createNewChildElement("Mid");
	el->addTextElement(String(UserParams[kMid]));
	el = root.createNewChildElement("High");
	el->addTextElement(String(UserParams[kHigh]));
	copyXmlToBinary(root,destData);
}

void SyAmpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{//Load UserParams/Data from file
	XmlElement* pRoot = getXmlFromBinary(data,sizeInBytes);
	if(pRoot!=NULL)
	{
		forEachXmlChildElement((*pRoot),pChild)
		{
			if(pChild->hasTagName("Low"))
			{
				String text = pChild->getAllSubText();
				setParameter(kLow,text.getFloatValue());
			}
			else if(pChild->hasTagName("Mid"))
			{
				String text = pChild->getAllSubText();
				setParameter(kMid,text.getFloatValue());
			}
			else if(pChild->hasTagName("kHigh"))
			{
				String text = pChild->getAllSubText();
				setParameter(kHigh,text.getFloatValue());
			} 
		}
		delete pRoot;
		UIUpdateFlag=true;//Request UI update
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SyAmpAudioProcessor();
}
