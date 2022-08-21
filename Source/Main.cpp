#include "MainComponent.h"
#include "Utils.h"

class OtoDecksApplication : public JUCEApplication {
public:

    OtoDecksApplication() {}

    const String getApplicationName() override { return JUCE_APPLICATION_NAME_STRING; }

    const String getApplicationVersion() override { return JUCE_APPLICATION_VERSION_STRING; }

    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const String &commandLine) override {
        mainWindow.reset(new MainWindow(getApplicationName()));
//        mainWindow->setUsingNativeTitleBar (false);
//        mainWindow->setTitleBarHeight(0);
    }

    void shutdown() override {
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override {
        quit();
    }

    void anotherInstanceStarted(const String &commandLine) override {

    }

    class CustomTitleBar : public LookAndFeel_V4 {
    public:
        void drawDocumentWindowTitleBar(
                DocumentWindow & documentWindow,
                Graphics & graphics,
                int w,
                int h,
                int titleSpaceX,
                int titleSpaceW,
                const Image * icon,
                bool drawTitleTextOnLeft
        ) override {
            graphics.fillAll(Colours::red);
        }
    };

class MainWindow : public ResizableWindow, public Button::Listener {
    public:
        MainWindow(String name) : ResizableWindow(name, true) {
            const Font font = Typeface::createSystemTypefaceFor(
                    BinaryData::StoryElement_ttf,
                    BinaryData::StoryElement_ttfSize
            );
            LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(font.getTypefacePtr());

//            setUsingNativeTitleBar(false);
            setContentOwned(new MainComponent(), true);

            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());

            setVisible(true);
            addMouseListener(this, true);

            Utils::setupImageButton(
                    closeButton,
                    ImageCache::getFromMemory(BinaryData::close_png, BinaryData::close_pngSize)
            );
            Utils::setupImageButton(
                    maximizeButton,
                    ImageCache::getFromMemory(BinaryData::maximize_png, BinaryData::maximize_pngSize)
            );
            Utils::setupImageButton(
                    minimizeButton,
                    ImageCache::getFromMemory(BinaryData::minimize_png, BinaryData::minimize_pngSize)
            );

            closeButton.setBounds(5, 5, 20, 20);
            minimizeButton.setBounds(30, 5, 20, 20);
            maximizeButton.setBounds(55, 5, 20, 20);

            Component::addAndMakeVisible (closeButton);
            Component::addAndMakeVisible (maximizeButton);
            Component::addAndMakeVisible (minimizeButton);

            closeButton.addListener(this);
            maximizeButton.addListener(this);
            minimizeButton.addListener(this);


//            setAlpha(0);
            setOpaque(false);

//            setLookAndFeel(new CustomTitleBar());
        }

//        void closeButtonPressed() override {
//            // This is called when the user tries to close this window. Here, we'll just
//            // ask the app to quit when this happens, but you can change this to do
//            // whatever you need.
//            JUCEApplication::getInstance()->systemRequestedQuit();
//        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */
//        MainWindow (String name)
//        {
//            mMainCompotnent = new MainComponent();
//            mMainCompotnent->setVisible (true);
//            mMainCompotnent->centreWithSize(mMainCompotnent->getWidth(), mMainCompotnent->getHeight());
//
//            mMainCompotnent->addToDesktop (ComponentPeer::windowHasDropShadow);
//        }

        ~MainWindow()
        {
        }

        void paint(Graphics& graphics) override {
//            setOpaque(true);
            graphics.fillAll(Colour{0x00ffffff});
        }

        void resized() override {
            ResizableWindow::resized();
            closeButton.setBounds(5, 5, 20, 20);
            minimizeButton.setBounds(30, 5, 20, 20);
            maximizeButton.setBounds(55, 5, 20, 20);
        }

        void mouseDown (const MouseEvent& e)
        {
            if (e.getMouseDownScreenY() < 30) {
                isDragging = true;
                componentDragger.startDraggingComponent (this, e);
            }

        }

        void mouseDrag (const MouseEvent& e)
        {
            if (isDragging) {
                componentDragger.dragComponent (this, e, nullptr);
            }

        }

        void mouseUp (const MouseEvent& e)
        {
            if (isDragging) {
                isDragging = false;
            }
        }

        void buttonClicked(Button* button) override {
            if (button == &closeButton) {
                JUCEApplicationBase::quit();
            }
            if (button == &minimizeButton) {
                setMinimised (true);
            }
            if (button == &maximizeButton) {
                setFullScreen (! isFullScreen());
            }
        }

    private:
        ImageButton closeButton{"Close"};
        ImageButton minimizeButton{"Minimize"};
        ImageButton maximizeButton{"Maximize"};

        bool isDragging = false;
        ComponentDragger componentDragger;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (OtoDecksApplication)
