#include "MMVII_2Include_Serial_Tpl.h"
#include "cFontDefinitions.h"

namespace MMVII
{
    class cFontTest : public cMMVII_Appli
    {
    public:
        cFontTest(const std::vector<std::string> &aVArgs, const cSpecMMVII_Appli &aSpec);

    private:
        int Exe() override;
        cCollecSpecArg2007 &ArgObl(cCollecSpecArg2007 &anArgObl) override;
        cCollecSpecArg2007 &ArgOpt(cCollecSpecArg2007 &anArgOpt) override;
        
        std::string fontName = "default";
        bool useEscapeCodes = false;
    };

    cFontTest::cFontTest(
        const std::vector<std::string> &aVArgs,
        const cSpecMMVII_Appli &aSpec) : cMMVII_Appli(aVArgs, aSpec)
    {
    }

    cCollecSpecArg2007 &cFontTest::ArgObl(cCollecSpecArg2007 &anArgObl)
    {
        return anArgObl;
    }

    cCollecSpecArg2007 &cFontTest::ArgOpt(cCollecSpecArg2007 &anArgOpt)
    {
        return anArgOpt
            << AOpt2007(
                fontName, "FontName", "Name of the font to display in Fonts/Definitions.xml", { eTA2007::HDV }
            )
            << AOpt2007(
                useEscapeCodes, "UseEscapeCode", "Use AINSI escape codes", { eTA2007::HDV }
            );
    }

    int cFontTest::Exe()
    {
        const auto &fontdefs = cFontDefinitions::Default();
        auto def = fontdefs.GetFont(fontName);

        if (!def.has_value()) {
            StdOut() << "Font '" << fontName << "' not found.\n";
            return EXIT_FAILURE;
        }
        
        def.value().Debug(useEscapeCodes);

        return EXIT_SUCCESS;
    }

    tMMVII_UnikPApli Alloc_cFontTest(const std::vector<std::string> &aVArgs, const cSpecMMVII_Appli &aSpec)
    {
        return tMMVII_UnikPApli(new cFontTest(aVArgs, aSpec));
    }

    cSpecMMVII_Appli TheSpec_FontTest(
        "FontTest",
        Alloc_cFontTest,
        "Debug fonts stored in Fonts/Definitions.xml",
        {eApF::Project},
        {eApDT::None},
        {eApDT::Xml},
        __FILE__);

    /*
     */

};
