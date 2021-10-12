#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(AudioProcessorEditor *editor) : editor_(editor) {
    if (editor_ != nullptr) addAndMakeVisible(editor);
    setSize(600, 400);

//    setSize(editor_->getWidth(),editor_->getHeight());
}

//==============================================================================
void MainComponent::paint(juce::Graphics &) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setFont (juce::Font (16.0f));
//    g.setColour (juce::Colours::white);
//    g.drawText ("Hello VstInstrApp !", getLocalBounds(), juce::Justification::centred, true)
//    ;
//    if(editor_) editor_->paint(g);
}

void MainComponent::resized() {
    if(editor_) editor_->setSize(getWidth(),getHeight());

}
