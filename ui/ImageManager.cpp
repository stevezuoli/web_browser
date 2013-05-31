#include "ui/ImageManager.h"
#include "Device/device.h"

namespace ui
{
static const char* g_ImagePathKT[] = {
#define IMAGEPATHKT(CommandId, ImagePathKT, ImagePathKP) \
    ":/res/"ImagePathKT,
    IMAGE_LIST(IMAGEPATHKT)
#undef IMAGEPATHKT
};

static const char* g_ImagePathKP[] = {
#define IMAGEPATHKP(CommandId, ImagePathKT, ImagePathKP) \
    ":/res/"ImagePathKP,
    IMAGE_LIST(IMAGEPATHKP)
#undef IMAGEPATHKP
};

const char* ImageManager::GetImagePath(ImageIndex index)
{
    if (index < 0 || index >= ImageCount)
    {
        return "";
    }
    if (Device::getModel() == Device::KPW)
    {
        return g_ImagePathKP[index];
    }

    return g_ImagePathKT[index];
}
}//ui
