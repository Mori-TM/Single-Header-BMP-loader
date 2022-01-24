#include <stdint.h>

#pragma pack(push, 1)

typedef struct
{
	uint16_t Type;
	uint32_t Size;
	uint16_t Resv1;
	uint16_t Resv2;
	uint32_t OffBits;
} BitmapFileHeader;

typedef struct
{
	uint32_t Size;
	int32_t Width;
	int32_t Height;
	uint16_t Planes;
	uint16_t Bits;
	uint32_t Compression;
	uint32_t SizeImage;
	int32_t XPelsPerMeter;
	int32_t YPelsPerMeter;
	uint32_t ClrUsed;
	uint32_t ClrImportant;
} BitmapInfoHeader;

typedef struct
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
	uint8_t Alpha;
} BitmapColorHeader;

#pragma pack(pop)

uint8_t* ImLoadImage(const int8_t* Filename, uint32_t* Width, uint32_t* Height, uint32_t* ChannelCount)
{
	BitmapFileHeader ImageHeader;
	BitmapInfoHeader ImageInfo;
	uint8_t* ImageData;

	FILE* File = fopen(Filename, "rb");
	if (!File)
		printf("Failed to open image: %s\n", Filename);

	fread(&ImageHeader, sizeof(BitmapFileHeader), 1, File);
	fread(&ImageInfo, sizeof(BitmapInfoHeader), 1, File);

	fseek(File, ImageHeader.OffBits, SEEK_SET);

	uint32_t ImageSize = ImageInfo.SizeImage * (ImageInfo.Bits == 32 ? 4 : 3);

	ImageData = malloc(ImageSize);
	fread(ImageData, ImageSize, 1, File);

	fclose(File);

	/*
	printf("Width: %d\n", ImageInfo.Width);
	printf("Height: %d\n", ImageInfo.Height);
	printf("Size: %d\n", ImageInfo.SizeImage);
	printf("Bits: %d\n", ImageInfo.Bits);
	*/

	for (int32_t i = 0; i < ImageInfo.Width * ImageInfo.Height; i++)
	{
		int32_t Index = i * (ImageInfo.Bits == 32 ? 4 : 3);

		uint8_t B = ImageData[Index];
		uint8_t R = ImageData[Index + 2];

		ImageData[Index] = R;
		ImageData[Index + 2] = B;
	}

	*Width = ImageInfo.Width;
	*Height = ImageInfo.Height;
	*ChannelCount = (ImageInfo.Bits == 32 ? 4 : 3);

	return ImageData;
}