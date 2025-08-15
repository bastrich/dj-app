#include <memory>

#include "MainComponent.h"
#include "Utils.h"

using std::unique_ptr;
using std::make_unique;
using std::exception;

class DjAppApplication : public JUCEApplication {
public:

    DjAppApplication() = default;

    const String getApplicationName() override { return JUCE_APPLICATION_NAME_STRING; }

    const String getApplicationVersion() override { return JUCE_APPLICATION_VERSION_STRING; }

    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String &commandLine) override {
        //init window
        mainWindow = make_unique<MainWindow>(getApplicationName());

        //init global font
        const Font font = Typeface::createSystemTypefaceFor(
                BinaryData::StoryElement_ttf,
                BinaryData::StoryElement_ttfSize
        );
        LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(font.getTypefacePtr());
    }

    void shutdown() override {}

    class MainWindow : public DocumentWindow,
                       public LookAndFeel_V4 {
    public:
        explicit MainWindow(const String &name) : DocumentWindow(
                name,
                Colours::transparentWhite,
                DocumentWindow::allButtons,
                true
        ) {
            setContentOwned(new MainComponent(), true);
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
            addMouseListener(this, true);
            setOpaque(false);
            setLookAndFeel(this);
        }

        ~MainWindow() override = default;

        void paint(Graphics &g) override {
            Path backgroundPath{};
            backgroundPath.addRoundedRectangle(0, 0, getWidth(), getHeight(), 20);
            g.reduceClipRegion(backgroundPath);
            g.drawImage(backgroundImage, getLocalBounds().toFloat());
        }

        Button *createDocumentWindowButton(int buttonType) override {
            switch (buttonType) {
                case DocumentWindow::closeButton:
                    return createButton(BinaryData::close_png, BinaryData::close_pngSize);
                case DocumentWindow::minimiseButton:
                    return createButton(BinaryData::minimize_png, BinaryData::minimize_pngSize);
                case DocumentWindow::maximiseButton:
                    return createButton(BinaryData::maximize_png, BinaryData::maximize_pngSize);
                default:
                    throw exception();
            }
        }

        void positionDocumentWindowButtons(DocumentWindow &,
                                           int titleBarX, int titleBarY,
                                           int titleBarW, int titleBarH,
                                           Button *minimiseButton,
                                           Button *maximiseButton,
                                           Button *closeButton,
                                           bool positionTitleBarButtonsOnLeft) override {
            closeButton->setBounds(10, 5, 20, 20);
            minimiseButton->setBounds(35, 5, 20, 20);
            maximiseButton->setBounds(60, 5, 20, 20);
        }

        void closeButtonPressed() override {
            JUCEApplicationBase::quit();
        }

        void mouseMove(const MouseEvent &e) override {
            setDraggable(e.getScreenY() - getBounds().getY() < 30);
        }

    private:
        Image backgroundImage = ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);

        Button *createButton(const void *imageData, const int &dataSize) {
            auto *button = new ImageButton();
            Utils::setupImageButton(*button, imageData, dataSize);
            return button;
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (DjAppApplication)
