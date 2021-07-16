#include "render_material.h"

bool32 render_material::SetUseColorTexture(bool32 Use)
{
    bool32 Succces = FALSE;
    if (Use == TRUE)
    {
        if (ColorTexture == nullptr)
        {
            UseColorTexture = FALSE;
            Succces = FALSE;
        }
        else
        {
            UseColorTexture = TRUE;
            Succces = TRUE;
        }
    }
    else
    {
        UseColorTexture = FALSE;
        Succces = TRUE;
    }
    return Succces;
}

bool32 render_material::ActivateTextureWithSampler(uint32 Slot)
{
    ColorTexture->Bind(Slot);
    ColorSampler->Bind(Slot);
    return TRUE;
}