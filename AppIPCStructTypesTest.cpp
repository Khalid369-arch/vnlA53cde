// ============================================================================
// Copyright Deere & Company. For more information,
// please see COPYRIGHT file in root of source repository.
// ============================================================================

#include <ServiceIO/IPC.h>
#include <AppIpcStructTypes.h>

// -------------------------------------------------------------------------------
// This test won't actually run due to a fake XML.
// Rather, we're just trying to verify we can instantiate IPC objects
// with struct types that we've defined in our AppIpcStructTypes.h header.
// -------------------------------------------------------------------------------

int main()
{
    boost::asio::io_context ioservice;
    IPC<StructGeoImage> ipc("FAKE_XML_THIS_DOESNT_EXECUTE", ioservice, 0);
    // It should complain if was unable to compile due to a missing explicit template instantiation, but
    // call 1 IPC function anyways so the compiler doesn't try to optimize the construction away.
    ipc.Send(std::make_shared<StructGeoImage>());
}
