#include "MMVII_Image2D.h"
#include "../Font/cFontDefinitions.h"

namespace MMVII {
// The public API ...

cIm2D<tU_INT1> ImageOfString_10x8(const std::string & aStr ,int aSpace)
{
    return cFontDefinitions::Default().GetFont("default").value().ImageString(aStr, cPt2di(aSpace, 0));
}

cIm2D<tU_INT1> ImageOfString_DCT(const std::string & aStr ,int aSpace)
{
    return cFontDefinitions::Default().GetFont("default").value().ImageString(aStr, cPt2di(aSpace, 0));
}

/**
 * @deprecated Please use the class @link MMVII::cFontDefinitions cFontDefinitions @endlink.
 */
cIm2D<tU_INT1> MultiLineImageOfString_10x8(const std::string & aStrInit,const cPt2di& aSpace,cPt2di aRab,int Centering)
{
    MMVII_DEV_WARNING("Deprecated MultiLineImageOfString_10x8: Please use cFontDefinition::ImageString");
    return cFontDefinitions::Default().GetFont("default").value().ImageString(aStrInit, aSpace, aRab, Centering == 0);
}

/**
 * @deprecated Please use the class @link MMVII::cFontDefinitions cFontDefinitions @endlink.
 */
cIm2D<tU_INT1> MultiLineImageOfString_DCT(const std::string & aStrInit,const cPt2di& aSpace,cPt2di aRab,int Centering)
{
    MMVII_DEV_WARNING("Deprecated MultiLineImageOfString_DCT: Please use cFontDefinition::ImageString");
    return cFontDefinitions::Default().GetFont("default").value().ImageString(aStrInit, aSpace, aRab, Centering == 0);
}

} // namespace MMVII
