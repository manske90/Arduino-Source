/*  Post-Catch Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSV_PostCatchDetector_H
#define PokemonAutomation_PokemonSV_PostCatchDetector_H

#include "Common/Cpp/Color.h"
#include "CommonFramework/ImageTools/ImageBoxes.h"
#include "CommonFramework/Inference/VisualDetector.h"
#include "CommonFramework/InferenceInfra/VisualInferenceCallback.h"
#include "PokemonSV_DialogDetector.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{



class AddToPartyDetector : public StaticScreenDetector{
public:
    AddToPartyDetector(Color color = COLOR_RED);

    virtual void make_overlays(VideoOverlaySet& items) const override;
    virtual bool detect(const ImageViewRGB32& screen) const override;

protected:
    Color m_color;
    ImageFloatBox m_top_left;
    ImageFloatBox m_top_right;
    ImageFloatBox m_bottom_left;
    ImageFloatBox m_bottom_right;
    PromptDialogDetector m_dialog;
};
class AddToPartyFinder : public DetectorToFinder_ConsecutiveDebounce<AddToPartyDetector>{
public:
    AddToPartyFinder(Color color = COLOR_RED)
         : DetectorToFinder_ConsecutiveDebounce("AddToPartyFinder", 5, color)
    {}
};






}
}
}
#endif
