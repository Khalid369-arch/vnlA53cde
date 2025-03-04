// ============================================================================
// Copyright Deere & Company. For more information,
// please see COPYRIGHT file in root of source repository.
// ============================================================================
#pragma once

#include <ApplicationBase/ApplicationBase.h>
#include <Utils/RingBufferWithLocks.h>
#include <Utils/ColorConvertFactoryManager.h>

#include <boost/thread/thread.hpp>
#include <memory>
#include <string>

#include "IpmA53Diagnostics/A53Diagnostics.h"

struct StructImageGroup;
struct StructICDMessage;

class VanillaApp : public ApplicationBase
{
    public:
    VanillaApp(const std::string &path,
               std::atomic<bool> &killed,
               bool sendHeartbeat = true);
    virtual ~VanillaApp() override;

    protected:
    virtual void Cleanup() override;

    // ServiceIO handlers
    IPC<StructImageGroup, StructICDMessage> Ipc;

    std::unique_ptr<boost::thread> IcdSendThread;
    std::unique_ptr<boost::thread> ImageSendThread;

    void Timeout(const boost::system::error_code&);
    void IcdAsyncSend();
    void ImageAsyncSend();

    private:
    RingBufferWithLocks<std::shared_ptr<StructImageGroup>> ImageBuffer;
    Utils::ColorConvertFactoryManager ClrFactoryMan;
    const int NV12ConversionWidth;
    const int NV12ConversionHeight;
    IPMA53Diagnostics::Mockable Diagnostics;
};

extern "C"
{
bool CustomCameraCheckWillFail(std::shared_ptr<StructICDMessage> packet);
bool CustomCameraCheckWillPass(std::shared_ptr<StructICDMessage> packet);
}
