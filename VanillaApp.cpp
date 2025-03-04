// ============================================================================
// Copyright Deere & Company. For more information,
// please see COPYRIGHT file in root of source repository.
// ============================================================================
#include "VanillaApp.h"

#include <IPCCommon.h>
#include <IPCPackets.h>
#include <ServiceFactory/ServiceFactory.h>
#include <ServiceIO/DefaultValues.h>
#include <Utils/ARMComputeFunctions/CLColorConvertFactory.h>
#include <Utils/Thread.h>
#include <Utils/IpcFinalizeMessage.h>

#include <structs/structICDMessage.h>
#include <structs/structImageGroup.h>

NEW_LOG_CATEGORY(static VanillaAppLog, "VanillaApp")

namespace {
constexpr int ImageBufferSize = 1;
constexpr int ImageBufferWaitTime = 500;
}

VanillaApp::VanillaApp(const std::string &path,
                       std::atomic<bool> &killed,
                       bool sendHeartbeat)
    : ApplicationBase(path, killed, g3::VanillaAppLog, sendHeartbeat)
    , Ipc(path, *IoService, g3::VanillaAppLog)
    , IcdSendThread()
    , ImageSendThread()
    , ImageBuffer(ImageBufferWaitTime, ImageBufferSize, killed)
    , ClrFactoryMan()
    , NV12ConversionWidth(params().get("NV12ConversionWidth", 0))
    , NV12ConversionHeight(params().get("NV12ConversionHeight", 0))
    , Diagnostics(nullptr, path, killed, IoService, nullptr, nullptr)
{
    if (params().name() != "VanillaApp")
    {
        LOG_C(FATAL, VanillaAppLog) << "No VanillaApp in XML. ";
    }
    else if (params().empty())
    {
        LOG_C(FATAL, VanillaAppLog) << "Empty VanillaApp section in XML. ";
    }

    // Load apps under Vanilla as threads
    ServiceFactory::LoadLibraries(path, Killed, g3::VanillaAppLog);

    // initialize the Rx connections
    Ipc.Receive<StructImageGroup>([this](std::shared_ptr<StructImageGroup> data, const std::string &)
                                  {
                                      ImageBuffer.PushData(data);
                                  });
    Ipc.Receive<StructICDMessage>([](std::shared_ptr<StructICDMessage> packet, const std::string)
                                  {
                                      (void)packet;
                                  });

    IcdSendThread = Utils::createThreadOther(std::bind(&VanillaApp::IcdAsyncSend, this), "VA_IcdAsyncSend");
    ImageSendThread = Utils::createThreadOther(std::bind(&VanillaApp::ImageAsyncSend, this), "VA_ImgAsyncSend");

    Diagnostics->StartA53Diagnostics();
}

// -------------------------------------------------------------------------------
VanillaApp::~VanillaApp()
{
    IcdSendThread->join();
    ImageSendThread->join();
    Cleanup();
}

// -------------------------------------------------------------------------------
ApplicationBase* entryPointFunction(std::string arg,
                                    std::atomic<bool> &killed)
{
    VanillaApp* service = new VanillaApp(arg, killed, false);
    return service;
}

// -------------------------------------------------------------------------------
void VanillaApp::Cleanup()
{
    ServiceFactory::UnloadLibraries();
    ApplicationBase::Cleanup();
}

// -------------------------------------------------------------------------------
void VanillaApp::IcdAsyncSend()
{
    LOG_C(DBUG, VanillaAppLog) << "Entering IcdSend thread";

    UInt8_T VanillaAppDiagnosticCounter = 0;

    while (!Killed)
    {
        UInt8_T CANDiagnosticAppCounterCmdByte = 0x62;
        UInt8_T InvalidCommandByte = 0xFF;

        MsgShortCANDiagnostics_T shortDiagnosticMessage = {
            .CmdByte1 = CANDiagnosticAppCounterCmdByte,
            .CANData1 = { 0 },
            .CmdByte2 = InvalidCommandByte,
            .CANData2 = { 0 },
            .CmdByte3 = InvalidCommandByte,
            .CANData3 = { 0 },
            .CmdByte4 = InvalidCommandByte,
            .CANData4 = { 0 },
            .CmdByte5 = InvalidCommandByte,
            .CANData5 = { 0 },
            .CmdByte6 = InvalidCommandByte,
            .CANData6 = { 0 }
        };
        shortDiagnosticMessage.CANData1[0] = VanillaAppDiagnosticCounter++;

        auto packet = std::make_shared<StructICDMessage>();
        packet->messageID = MSGS_SHORTCANDIAGNOSTICS;
        packet->destinationID = IcdProducerConsumer_E::ICDPRODUCERCONSUMER_R50;
        packet->length = sizeof(MsgShortCANDiagnostics_T);
        memcpy(packet->data, &shortDiagnosticMessage, sizeof(MsgShortCANDiagnostics_T));
        icd::FinalizeICDMessage(packet);

        Ipc.Send<StructICDMessage>(packet);

        std::this_thread::sleep_for(std::chrono::seconds(1));
        continue;
    }
    LOG_C(DBUG, VanillaAppLog) << "Exiting IcdSend thread";
}

// -------------------------------------------------------------------------------
void VanillaApp::ImageAsyncSend()
{
    LOG_C(DBUG, VanillaAppLog) << "Entering Image send thread";

    while (!Killed)
    {
        auto image = ImageBuffer.PopData();

        if ((image != nullptr) && (image->at(0).imageFormat() != ImageFormat::ImageFormatBayerGBRG_12u))
        {
            image = ClrFactoryMan.ConvertImageToNV12(image, NV12ConversionHeight, NV12ConversionWidth);
            Ipc.Send<StructImageGroup>(image);
        }
    }

    LOG_C(DBUG, VanillaAppLog) << "Exiting Image send thread";
}

bool CustomCameraCheckWillPass(std::shared_ptr<StructICDMessage> packet)
{
    (void)packet;
    return true;
}

bool CustomCameraCheckWillFail(std::shared_ptr<StructICDMessage> packet)
{
    (void)packet;
    return false;
}

// ===============================================================================
