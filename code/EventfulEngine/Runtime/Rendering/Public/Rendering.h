#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

#include "Common/Interfaces/IPipelineState.h"

namespace EventfulEngine::Rendering{
    class ICommandBuffer;
    class ITexture;
    class ICommandQueue;
    class IDevice;
    class ISwapChain;
    enum class RendererAPI;
    struct RenderPassDescriptor;
    class IShaderModule;
    class IPipelineState;

    class Rendering{
    public:
        using RenderFunc = std::function<void(ICommandBuffer&, ITexture&)>;

        Rendering();

        ~Rendering();

        bool Initialize(RendererAPI graphicsAPI);

        void ShutDown();

        bool HotSwap(RendererAPI newGraphicsAPI);

        // Views
        int AddView(void* nativeWindow, uint32_t w, uint32_t h);

        void RemoveView(int viewId);

        void ResizeView(int viewId, uint32_t w, uint32_t h);

        // User-facing resource creation (use these instead of raw device calls)
        std::shared_ptr<IShaderModule> CreateShaderModule(const std::string& source, uint32_t stageFlags);

        std::shared_ptr<IPipelineState> CreatePipelineState(const PipelineStateDescriptor& desc);

        // Render dispatch
        void RenderView(int viewId, RenderFunc renderFunc);

        void RenderAll(RenderFunc renderFunc);

    private:
        bool CreateAPI(RendererAPI graphicsAPI);

        void DestroyAPI();

        void RecreateResources();

        RendererAPI _currentAPI{};
        std::unique_ptr<IDevice> _device;
        std::unique_ptr<ICommandQueue> _commandQueue;

        // track views
        struct View{
            std::unique_ptr<ISwapChain> swapChain;
            uint32_t width;
            uint32_t height;
        };

        std::unordered_map<int, View> _views;
        int _nextViewId{0};

        // track shader sources and modules
        struct ShaderRec{
            std::string src;
            uint32_t stage;
            std::shared_ptr<IShaderModule> shaderModule;
        };

        std::vector<ShaderRec> _shaders;

        // track PSO descriptors & objects
        struct PSORec{
            PipelineStateDescriptor desc;
            std::shared_ptr<IPipelineState> pso;
        };

        std::vector<PSORec> _psos;
    };
}
