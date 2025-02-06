#include <switch.h>
#include <pu/Plutonium>
#include "gui/screen/TitleLoadScreen.hpp"
#include "data/mock/MockTitleDataProvider.hpp"
#include "data/mock/MockSaveDataProvider.hpp"
#include "gui/UIConstants.hpp"
#include "gui/FontManager.hpp"
#include "utils/Logger.hpp"
#include <sstream>

// Main application class
class PKSMApplication : public pu::ui::Application {
private:
    std::shared_ptr<TitleLoadScreen> titleLoadScreen;

    // Initialize renderer options with basic configuration
    static pu::ui::render::RendererInitOptions CreateRendererOptions() {
        LOG_DEBUG("Creating renderer options...");
        LOG_MEMORY();
        
        // Initialize SDL with hardware acceleration and vsync
        // This enables proper display, audio, and controller support
        auto renderer_opts = pu::ui::render::RendererInitOptions(SDL_INIT_EVERYTHING, pu::ui::render::RendererHardwareFlags);

        // Enable PNG/JPG loading for UI assets
        renderer_opts.init_img = true;
        renderer_opts.sdl_img_flags = IMG_INIT_PNG | IMG_INIT_JPG;

        // Enable romfs for loading assets bundled with the NRO
        renderer_opts.init_romfs = true;

        LOG_DEBUG("Renderer options created successfully");
        return renderer_opts;
    }

    // Configure font settings
    static void ConfigureFonts(pu::ui::render::RendererInitOptions& renderer_opts) {
        LOG_DEBUG("Configuring fonts...");
        
        // Register default (light) font
        renderer_opts.AddDefaultFontPath("romfs:/gfx/fonts/dinnextw1g_light.ttf");

        // Register all custom font sizes
        gui::FontManager::ConfigureRendererFontSizes(renderer_opts);
        
        LOG_DEBUG("Fonts configured successfully");
    }

    // Configure input settings
    static void ConfigureInput(pu::ui::render::RendererInitOptions& renderer_opts) {
        LOG_DEBUG("Configuring input...");
        
        renderer_opts.SetInputPlayerCount(1);  // Accept input from one player
        renderer_opts.AddInputNpadStyleTag(HidNpadStyleSet_NpadStandard);  // Accept standard controller input
        renderer_opts.AddInputNpadIdType(HidNpadIdType_Handheld);  // Accept handheld mode input
        renderer_opts.AddInputNpadIdType(HidNpadIdType_No1);  // Accept controller 1 input
        
        LOG_DEBUG("Input configured successfully");
    }

    // Register additional fonts that require romfs to be mounted
    static void RegisterAdditionalFonts() {
        LOG_DEBUG("Registering additional fonts...");
        
        try {
            // Register heavy font for all custom sizes
            gui::FontManager::RegisterFont(
                "romfs:/gfx/fonts/dinnextw1g_heavy.ttf",
                UIConstants::MakeHeavyFontName
            );

            // Register medium font for all custom sizes
            gui::FontManager::RegisterFont(
                "romfs:/gfx/fonts/dinnextw1g_medium.ttf",
                UIConstants::MakeMediumFontName
            );

            // Register switch button font for all custom sizes
            gui::FontManager::RegisterFont(
                "romfs:/gfx/fonts/NintendoExtLE003-M.ttf",
                UIConstants::MakeSwitchButtonFontName
            );
            
            LOG_DEBUG("Additional fonts registered successfully");
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to register additional fonts: " + std::string(e.what()));
            throw;
        }
    }

public:
    using Application::Application;  // Inherit constructor

    PU_SMART_CTOR(PKSMApplication)

    // Initialize the application with all necessary configuration
    static std::shared_ptr<PKSMApplication> Initialize() {
        try {
            // Initialize logger first
            utils::Logger::Initialize();
            LOG_INFO("Initializing PKSM...");
            LOG_MEMORY();  // Initial memory state
            
            // Initialize renderer with all configurations
            auto renderer_opts = CreateRendererOptions();
            ConfigureFonts(renderer_opts);
            ConfigureInput(renderer_opts);
            
            LOG_DEBUG("Creating renderer...");
            auto renderer = pu::ui::render::Renderer::New(renderer_opts);
            
            LOG_DEBUG("Initializing renderer...");
            renderer->Initialize();
            LOG_MEMORY();  // Memory after renderer initialization
            
            // Register additional fonts after romfs is mounted
            RegisterAdditionalFonts();
            
            // Create and prepare application
            LOG_DEBUG("Creating application...");
            auto app = PKSMApplication::New(renderer);
            
            LOG_DEBUG("Preparing application...");
            app->Prepare();
            
            LOG_INFO("PKSM initialization complete");
            LOG_MEMORY();  // Final initialization memory state
            return app;
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to initialize application: " + std::string(e.what()));
            throw;
        }
    }

    void OnLoad() override {
        try {
            LOG_DEBUG("Loading title screen...");
            LOG_MEMORY();  // Initial title screen memory state
            
            // Create data providers
            LOG_DEBUG("Creating data providers...");
            auto titleProvider = std::make_shared<MockTitleDataProvider>();
            auto saveProvider = std::make_shared<MockSaveDataProvider>();
            
            // Create and load the title screen
            LOG_DEBUG("Creating title screen...");
            
            try {
                this->titleLoadScreen = TitleLoadScreen::New(titleProvider, saveProvider);
                LOG_DEBUG("Title screen object created");
                LOG_MEMORY();  // Memory after title screen creation
            } catch (const std::exception& e) {
                LOG_ERROR("Failed to create title screen object: " + std::string(e.what()));
                throw;
            }
            
            LOG_DEBUG("Loading title screen layout...");
            try {
                this->LoadLayout(this->titleLoadScreen);
                LOG_DEBUG("Layout loaded");
                LOG_MEMORY();  // Final title screen memory state
            } catch (const std::exception& e) {
                LOG_ERROR("Failed to load title screen layout: " + std::string(e.what()));
                throw;
            }
            
            LOG_DEBUG("Title screen loaded successfully");
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to load title screen: " + std::string(e.what()));
            throw;
        }
    }
};

int main(int argc, char* argv[]) {
    try {
        // Initialize and run application
        auto app = PKSMApplication::Initialize();
        app->ShowWithFadeIn();
        
        // Cleanup
        utils::Logger::Finalize();
        return 0;
    } catch (const std::exception& e) {
        // Make sure we log any fatal errors
        LOG_ERROR("Fatal error: " + std::string(e.what()));
        utils::Logger::Finalize();
        return 1;
    }
} 