// ============================================================================
// Copyright Deere & Company. For more information,
// please see COPYRIGHT file in root of source repository.
// ============================================================================
#pragma once

// -------------------------------------------------------------------------------
// Vanilla does not use StructGeoImage or StructGeoImageDetections at runtime.
// This is just an example showing how applications can add other struct types
// to be supported by the Core IPC mechanism.
// -------------------------------------------------------------------------------
#include <structs/structGeoImage.h>
#include <structs/structGeoImageDetections.h>

#define APP_DEFINED_STRUCTS \
    (StructGeoImage)        \
    (StructGeoImageDetections)
