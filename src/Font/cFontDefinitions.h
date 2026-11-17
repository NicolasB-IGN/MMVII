#include "MMVII_Image2D.h"

namespace MMVII
{

    /**
     * Font container
     */
    class cFontDefinitions
    {
        class cFontDefinition;

    public:
        /**
         * Read the default font defintion file in `<MMVII-RessourceDir>/Fonts/Defintions.xml`
         */
        static cFontDefinitions Default(cMMVII_Appli &app);
        static cFontDefinitions Default();
        /**
         * Read a font definition file
         */
        static cFontDefinitions Read(const std::string &path);

        /**
         * Return the font if present in the definition file.
         */
        std::optional<cFontDefinition> GetFont(const std::string &name) const;

        /**
         * Serialization
         */
        void AddData(const cAuxAr2007 &anAux);

    private:
        std::vector<cFontDefinition> data;

        const std::string path;
        explicit cFontDefinitions(const std::string &path);
    };

    /**
     * Please use cFontDefinitions
     */
    class cFontDefinitions::cFontDefinition
    {
        using tFontData = cIm2D<tU_INT1>;

    public:
        cFontDefinition() = default;

        /**
         * Name of the font
         */
        std::string GetName() const;
        /**
         * Return the 2D image of the character
         */
        tFontData CharacterData(const char c);
        /**
         * Return the 2D image of the string.
         * Can be multiline.
         */
        tFontData ImageString(const std::string &text, const cPt2di spacing, const cPt2di padding = {0, 0}, const bool center = false);

        /**
         * Serialization
         */
        void AddData(const cAuxAr2007 &anAux, const std::string &path);

        /**
         * Print to stdout the characters in ASCII art or in ANSI escape codes.
         */
        void Debug(const std::string chars, const bool useEscapeCodes = false);
        /**
         * Print to stdout all printable character in ASCII art or in ANSI escape codes.
         */
        void Debug(const bool useEscapeCodes = false);

        /**
         * Print to stdout in ASCII art the 2D image data.
         */
        void DebugImage(const tFontData image) const;

    private:
        std::optional<tFontData> fontData;
        /**
         * Read image from `imagePath` relative to `fontDefinitionPath`
         */
        void ReadImage();

        static std::string LightLevelToChar(const int level);

        std::string name;
        std::string imagePath;
        std::string fontDefinitionPath;
        tU_INT2 characterWidth;
        tU_INT2 characterHeight;
    };

} // namespace MMVII
