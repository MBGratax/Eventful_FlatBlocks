#pragma once

#include "Rendering.h"
#include "OpenGL/EventfulOpenGL.h"
#include "Public/Common/RenderingCommon.h"
#include "Public/Common/EventfulHeadless.h"
#include <stdexcept>

namespace EventfulEngine::Rendering{

    Rendering::Rendering(){
        throw std::logic_error("Not implemented");
    }

    Rendering::~Rendering(){
        throw std::logic_error("Not implemented");
    }

    bool Rendering::Initialize(RendererAPI graphicsAPI){
        if (!CreateAPI(graphicsAPI)){
            return false;
        }
        RecreateResources();
        return true;
    }

    void Rendering::ShutDown(){
        DestroyAPI();
        _shaders.clear();
        _psos.clear();
        _views.clear();
    }

    bool Rendering::HotSwap(RendererAPI newGraphicsAPI){
        // destroy current
        DestroyAPI();
        // recreate device + queue
        if (!CreateAPI(newGraphicsAPI)){
            return false;
        }
        // recreate swapchains
        for (auto& [id, view] : _views){
            view.swapChain = _device->CreateSwapChain(view.swapChain->GetNativeWindow(), view.width, view.height);
        }
        // recreate shaders & pipelines
        RecreateResources();
        return true;
    }

    int Rendering::AddView(void* nativeWindow, uint32_t w, uint32_t h){
        int id = _nextViewId++;
        _views[id] = {_device->CreateSwapChain(nativeWindow, w, h), w, h};
        return id;
    }

    void Rendering::RemoveView(int viewId){
        _views.erase(viewId);
    }

    void Rendering::ResizeView(int viewId, uint32_t w, uint32_t h){
        auto& v = _views.at(viewId);
        v.swapChain->Resize(w, h);
        v.width = w;
        v.height = h;
    }

    std::shared_ptr<IShaderModule> Rendering::CreateShaderModule(const std::string& source, uint32_t stageFlags){
        std::shared_ptr mod = std::make_shared<IShaderModule>(_device->CreateShaderModule(source, stageFlags).release(),
                                                              [](IShaderModule* p){
                                                                  p->~IShaderModule();
                                                                  delete p;
                                                              });
        _shaders.push_back({source, stageFlags, mod});
        return mod;
    }

    std::shared_ptr<IPipelineState> Rendering::CreatePipelineState(const PipelineStateDescriptor& desc){
        std::shared_ptr<IPipelineState> pso = std::make_shared<IPipelineState>(
            _device->CreatePipelineState(desc).release(), [](IPipelineState* p){ delete p; });
        _psos.emplace_back({desc, pso});
        return pso;
    }

    void Rendering::RenderView(int viewId, RenderFunc renderFunc){
        auto& v = _views.at(viewId);
        auto cmd = _device->CreateCommandBuffer();
        cmd->Begin();
        auto rp = _device->CreateRenderPass({});
        cmd->BeginRenderPass(*rp);
        cmd->SetViewport(0, 0, v.width, v.height, 0.f, 1.f);
        cmd->SetScissor(0, 0, v.width, v.height);
        fn(*cmd, v.swapChain->GetCurrentBackBuffer());
        cmd->EndRenderPass();
        cmd->End();
        _commandQueue->Execute(*cmd);
        v.swapChain->Present(1);
    }

    void Rendering::RenderAll(RenderFunc renderFunc){
        throw std::logic_error("Not implemented");
    }

    bool Rendering::CreateAPI(RendererAPI graphicsAPI){
        throw std::logic_error("Not implemented");
    }

    void Rendering::DestroyAPI(){
        throw std::logic_error("Not implemented");
    }

    void Rendering::RecreateResources(){
        throw std::logic_error("Not implemented");
    }

}
