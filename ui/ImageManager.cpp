#include "ui/ImageManager.h"
#include "Device/device.h"

namespace ui
{
const char* g_ImagePathKT[] = {
#define IMAGEPATHKT(CommandId, ImagePathKT, ImagePathKP) \
    ":/res/"IMAGEPATHKT,
#undef IMAGEPATHKT
};

const char* g_ImagePathKP[] = {
#define IMAGEPATHKP(CommandId, ImagePathKT, ImagePathKP) \
    ":/res/"IMAGEPATHKP,
#undef IMAGEPATHKP
};

const char* ImageManger::GetImagePath(ImageIndex index)
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
