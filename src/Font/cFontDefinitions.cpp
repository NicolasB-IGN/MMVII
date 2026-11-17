#include "MMVII_2Include_Serial_Tpl.h"
#include "cFontDefinitions.h"

namespace MMVII
{

    //
    // cFontDefinitions
    //

    cFontDefinitions::cFontDefinitions(const std::string &path) : path(path)
    {
    }

    std::optional<cFontDefinitions::cFontDefinition> cFontDefinitions::GetFont(const std::string &name) const
    {
        for (const auto &fontDef : data)
        {
            if (fontDef.GetName() == name)
                return fontDef;
        }

        return {};
    }

    cFontDefinitions cFontDefinitions::Default()
    {
        return Default(cMMVII_Appli::CurrentAppli());
    }

    cFontDefinitions cFontDefinitions::Default(cMMVII_Appli &app)
    {
        return cFontDefinitions::Read(app.DirRessourcesMMVII() + "/Fonts/Definitions.xml");
    }

    cFontDefinitions cFontDefinitions::Read(const std::string &path)
    {
        auto def = cFontDefinitions(path);
        ReadFromFile(def, path);

        return def;
    }

    void cFontDefinitions::AddData(const cAuxAr2007 &anAux)
    {
        const auto &defs = cAuxAr2007("FontDefinitions", anAux);
        defs.SetType(eTAAr::eCont);
        int nb = data.size();
        AddDataSizeCont(nb, defs);

        if (nb != int(data.size()))
        {
            data = std::vector<cFontDefinition>(nb);
        }
        // now read the elements
        for (auto &el : data)
        {
            el.AddData(cAuxAr2007("FontDefinition", defs, eTAAr::eElemCont), path);
        }
    }

    //
    // cFontDefinition
    //

    std::string cFontDefinitions::cFontDefinition::GetName() const
    {
        return name;
    }

    void cFontDefinitions::cFontDefinition::AddData(const cAuxAr2007 &anAux, const std::string &path)
    {
        MMVII::AddData(
            cAuxAr2007("Name", anAux), name);
        MMVII::AddData(
            cAuxAr2007("ImagePath", anAux), imagePath);
        MMVII::AddData(
            cAuxAr2007("CharacterWidth", anAux), characterWidth);
        MMVII::AddData(
            cAuxAr2007("CharacterHeight", anAux), characterHeight);
        fontDefinitionPath = path;
    }

    void cFontDefinitions::cFontDefinition::ReadImage()
    {
        if (fontData)
            return;

        fontData = tFontData::FromFile(DirOfPath(fontDefinitionPath, false) + "/" + imagePath);
    }

    cFontDefinitions::cFontDefinition::tFontData cFontDefinitions::cFontDefinition::CharacterData(char c)
    {
        if (!fontData)
            ReadImage();

        tFontData result(cPt2di(characterWidth, characterHeight), nullptr, eModeInitImage::eMIA_Null);

        const auto &imageData = fontData.value().DIm();

        const auto x = (c & 0x0f) * characterWidth;
        const auto y = ((c & 0x70) >> 4) * characterHeight;

        for (tU_INT2 dy = 0; dy < characterHeight; dy++)
        {
            for (tU_INT2 dx = 0; dx < characterWidth; dx++)
            {
                const auto value = imageData.GetV(cPt2di(x + dx, y + dy));
                result.DIm().SetV(cPt2di(dx, dy), value);
            }
        }

        return result;
    }

    std::string cFontDefinitions::cFontDefinition::LightLevelToChar(const int level)
    {
        if (level < 50)
            return " ";
        if (level < 100)
            return "░";
        if (level < 150)
            return "▒";
        if (level < 200)
            return "▓";
        return "█";
    }

    void cFontDefinitions::cFontDefinition::Debug(const std::string chars, const bool useEscapeCodes)
    {
        StdOut() << "FontDefinition(" << name << ") {\n";
        for (const unsigned char c : chars)
        {
            StdOut() << " CharacterData('" << c << "') {\n";
            const auto &data = CharacterData(c);
            const auto &im = data.DIm();
            for (size_t y = 0; y < im.SzY(); y++)
            {
                StdOut() << "  ";
                for (size_t x = 0; x < im.SzX(); x++)
                {
                    const auto value = im.GetV(cPt2di(x, y));
                    if (useEscapeCodes)
                    {
                        const auto valueString = std::to_string(value);
                        StdOut() << "\x1B[48;2;" << valueString << ";" << valueString << ";" << valueString << "m ";
                    }
                    else
                    {
                        StdOut() << LightLevelToChar(value);
                    }
                }
                if (useEscapeCodes)
                {
                    StdOut() << "\x1B[0m";
                }
                StdOut() << "\n";
            }
            StdOut() << " }\n";
        }
        StdOut() << "}\n";
    }

    void cFontDefinitions::cFontDefinition::Debug(const bool useEscapeCodes)
    {
        std::string printable_chars = "";
        for (unsigned char c = 33; c < 127; c++)
        {
            printable_chars += c;
        }

        Debug(printable_chars, useEscapeCodes);
    }

    void cFontDefinitions::cFontDefinition::DebugImage(const tFontData image) const
    {
        const auto size = image.DIm().Sz();

        for (size_t y = 0; y < size.y(); y++)
        {
            StdOut() << "|";

            for (size_t x = 0; x < size.x(); x++)
            {
                StdOut() << LightLevelToChar(image.DIm().GetV(cPt2di(x, y)));
            }

            StdOut() << "|\n";
        }
    }

    cFontDefinitions::cFontDefinition::tFontData cFontDefinitions::cFontDefinition::ImageString(const std::string &text, const cPt2di spacing, const cPt2di padding, const bool center)
    {
        if (text == "")
        {
            return CharacterData(' ');
        }

        cPt2di imageDimensions(0, 0);
        std::vector<int> widths;
        int width = 0;

        for (const char &c : text)
        {
            const auto &charImage = CharacterData(c);
            const auto &charDimensions = charImage.DIm().Sz();

            if (imageDimensions.y() == 0)
            {
                imageDimensions.y() = charDimensions.y();
            }
            if (c == '\n')
            {
                imageDimensions.y() += charDimensions.y() + spacing.y();
                widths.push_back(width);
                width = 0;
                continue;
            }

            width += charDimensions.x() + spacing.x();
            if (width > imageDimensions.x())
            {
                imageDimensions.x() = width;
            }
        }
        widths.push_back(width);

        imageDimensions += padding * 2;

        int line = 0;
        tFontData result(imageDimensions, nullptr, eModeInitImage::eMIA_Null);
        cPt2di offset(padding.x(), padding.y());
        if (center)
        {
            offset.x() = (imageDimensions.x() - widths[line]) / 2;
        }

        for (const char &c : text)
        {
            const auto &charImage = CharacterData(c);
            if (c == '\n')
            {
                line++;
                offset.y() += charImage.DIm().SzY() + spacing.y();
                if (center)
                {
                    offset.x() = (imageDimensions.x() - widths[line]) / 2;
                }
                else
                {
                    offset.x() = padding.x();
                }
                continue;
            }

            for (const auto &point : charImage.DIm())
            {
                result.DIm().SetV(
                    point + offset,
                    charImage.DIm().GetV(point));
            }
            offset.x() += spacing.x() + charImage.DIm().SzX();
        }

        return result;
    }

    //
    // Serialisation
    //

    void AddData(const cAuxAr2007 &anAux, cFontDefinitions &value)
    {
        value.AddData(anAux);
    }

} // namespace MMVII
