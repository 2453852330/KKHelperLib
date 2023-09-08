#pragma once

#include "KKHelperLibType.generated.h"

// custom 
enum class EKKFontImportFlags
{
	None = 0x0,						// No flags
	EnableAntialiasing = 0x1,		// Whether the font should be antialiased or not.  Usually you should leave this enabled.
	EnableBold = 0x2,				// Whether the font should be generated in bold or not
	EnableItalic = 0x4,				// Whether the font should be generated in italics or not
	EnableUnderline = 0x8,			// Whether the font should be generated with an underline or not
	AlphaOnly = 0x10,				// Forces PF_G8 and only maintains Alpha value and discards color
	CreatePrintableOnly = 0x20,		// Skips generation of glyphs for any characters that are not considered 'printable'
	IncludeASCIIRange = 0x40,		// When specifying a range of characters and this is enabled, forces ASCII characters (0 thru 255) to be included as well
	EnableDropShadow = 0x80,		// Enables a very simple, 1-pixel, black colored drop shadow for the generated font
	EnableLegacyMode = 0x100,		// Enables legacy font import mode.  This results in lower quality antialiasing and larger glyph bounds, but may be useful when debugging problems
	UseDistanceFieldAlpha = 0x200	// Alpha channel of the font textures will store a distance field instead of a color mask
};

ENUM_CLASS_FLAGS(EKKFontImportFlags)


USTRUCT(BlueprintType)
struct FFontImportFlagsBP
{
	GENERATED_BODY()

	EKKFontImportFlags Flags;
};
