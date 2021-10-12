#include "MainComponent.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/juce_audio_devices.h>

using namespace juce;

//==============================================================================
class VstInstrApp : public juce::JUCEApplication {
public:
    //==============================================================================
    VstInstrApp() {}

    // We inject these as compile definitions from the CMakeLists.txt
    // If you've enabled the juce header with `juce_generate_juce_header(<thisTarget>)`
    // you could `#include <JuceHeader.h>` and use `ProjectInfo::projectName` etc. instead.
    const juce::String getApplicationName() override { return JUCE_APPLICATION_NAME_STRING; }

    const juce::String getApplicationVersion() override { return JUCE_APPLICATION_VERSION_STRING; }

    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise(const juce::String &commandLine) override {
        // This method is where you should put your application's initialisation code..
        juce::ignoreUnused(commandLine);

        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted(const juce::String &commandLine) override {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
        juce::ignoreUnused(commandLine);
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow : public juce::DocumentWindow {
    public:
        explicit MainWindow(juce::String name)
            : DocumentWindow(name,
                             juce::Desktop::getInstance().getDefaultLookAndFeel()
                                 .findColour(ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons) {
            setUsingNativeTitleBar(true);

            AudioProcessorEditor *editor = nullptr;

            // vst 2 does not have groups
//            String path = "/Users/kodiak/Library/Audio/Plug-Ins/VST/pmix.vst";
//            String path = "/Users/kodiak/Library/Audio/Plug-Ins/VST/clds.vst";

            // vst 3 has groups
            String path = "/Users/kodiak/Library/Audio/Plug-Ins/VST3/pmix.vst3";
            formatManager.addDefaultFormats();
            for (auto fmt : formatManager.getFormats()) {
                Logger::writeToLog("Plugin format : " + fmt->getName());
                Logger::writeToLog("checking : " + path);
                knownPluginList.scanAndAddFile(path, true, pluginDescriptions, *fmt);
            }

            if (pluginDescriptions.size() > 0) {
                auto &pluginDesc = *pluginDescriptions[0];
                String msg("Oh no!");
                auto instance = formatManager.createPluginInstance(pluginDesc,
                                                                   48000,
                                                                   1024,
                                                                   msg);
//                auto plist= instance->getParameters();
//                for(auto p : plist) {
//
//                    Logger::writeToLog("Param : " + p->getName(40));
//                }


                Logger::writeToLog("PluginName: " + pluginDesc.name);
                auto &pt = instance->getParameterTree();
                printParamTree(&pt, 0);


//                editor = instance->createEditor();
//                auto bc = editor->getConstrainer();
//                editor->setBounds(0, 0, bc->getMinimumWidth(), bc->getMinimumHeight());
//                addAndMakeVisible(editor);
            } else {
                Logger::writeToLog("unable to load : " + path);
            }

            setContentOwned(new MainComponent(editor), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif

            setVisible(true);
        }

        void printParamTree(const AudioProcessorParameterGroup *pg, int lvl) {
            Logger::writeToLog(String(lvl) + " -> Name : " + pg->getName());
            auto pl = pg->getParameters(false);
            for (auto p: pl) {
                Logger::writeToLog(String(lvl) + " -> Param : "
                                   + String(p->getParameterIndex()) + "/" + p->getName(40) + "/" + p->getLabel());
            }

            auto sg = pg->getSubgroups(false);
            for (auto g: sg) {
                printParamTree(g, lvl + 1);
            }


        }

        void closeButtonPressed() override {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:

        AudioDeviceManager deviceManager;
        AudioPluginFormatManager formatManager;

        KnownPluginList knownPluginList;
        OwnedArray<PluginDescription> pluginDescriptions;
//        Array<PluginDescription> pluginDescriptions;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (VstInstrApp)
