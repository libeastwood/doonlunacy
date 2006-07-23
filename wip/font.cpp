#include <stdio.h>

// http://xccu.sourceforge.net/documents/ccfonts.txt

typedef unsigned short word;
typedef unsigned char byte;

struct FNTHeader
{
    word fsize;    /* Size of the file              */
    word unknown1; /* Unknown entry (always 0x0500) */
    word unknown2; /* Unknown entry (always 0x000e) */
    word unknown3; /* Unknown entry (always 0x0014) */
    word wpos;     /* Offset of char. widths array  (abs. from beg. of file) */
    word cdata;    /* Offset of char. graphics data (abs. from beg. of file) */
    word hpos;     /* Offset of char. heights array (abs. from beg. of file) */
    word unknown4; /* Unknown entry (always 0x1012) */
    byte unknown5; // dunk- had to add this to get nchars read correctly 
    byte nchars;   /* Number of characters in font minus 1*/ // dunk- the doc says word 
    byte height;   /* Font height                   */
    byte maxw;     /* Max. character width          */
};

struct FNTCharacter
{
    byte width;
    byte height;
    byte y_offset;
    byte *bitmap;
};

int main(int argc, char* argv[])
{
    FILE* file = fopen("intro.fnt", "rb");

    FNTHeader header;

    printf("sizeof word %ld, sizeof byte %ld\n", sizeof(word), sizeof(byte));

    printf("fsize %d\n", header.fsize);

    fread(&header, sizeof(FNTHeader), 1, file);

    printf("fsize %d\n", header.fsize);

    // this checks if its a valid font
    if (header.unknown1 != 0x0500) printf("failed unknown1\n");
    if (header.unknown2 != 0x000e) printf("failed unknown2\n");
    if (header.unknown3 != 0x0014) printf("failed unknown3\n");

    printf("nchars %u\n", header.nchars);

    word* dchar = new word[header.nchars+1];

    fread(dchar, sizeof(word), header.nchars+1, file);

    byte* wchar = new byte[header.nchars+1];

    printf("wpos %d\n", header.wpos);
    fseek(file, header.wpos, SEEK_SET);
    long br = fread(wchar, sizeof(byte), header.nchars+1, file);
    printf("br %ld %d\n", br, 1);

    if (wchar[0] != 8) printf("bad!!\n");

    word* hchar = new word[header.nchars+1];

    fseek(file, header.hpos, SEEK_SET);
    fread(hchar, sizeof(word), header.nchars+1, file);

    fseek(file, header.cdata, SEEK_SET);

    FNTCharacter* characters = new FNTCharacter[header.nchars+1];    
    
    for (int i=0; i!=header.nchars+1; i++)
    {
        byte offset = hchar[i] & 0xFF;
        byte height = hchar[i] >> 8;
        byte width = (wchar[i] + 1) / 2;
        printf("%d %c width = %hd offset = %hd height = %hd\n", i, i, width, offset, height);
        
        characters[i].width = width;
        characters[i].height = height;
        characters[i].y_offset = offset;
        printf("size %hd\n", width * height);
        printf("dchar %hd\n", dchar[i]);

        fseek(file, dchar[i], SEEK_SET); 
        byte* bitmap = new byte[width * height];
        fread(bitmap, sizeof(byte), width * height, file);
        characters[i].bitmap = bitmap;       

        // 65 = A apparently
        if (i==65) printf("************************************************\n");

        for (byte y=0; y!=offset; y++)
        {
            for (byte x=0; x!=width; x++)
            {
                printf("  .  .");
            };
            printf("\n");
        };
        
        for (byte y=0; y!=height; y++)
        {
            for (byte x=0; x!=width; x++)
            {
                byte lobyte =  bitmap[x + (y*width)] >> 4;
                byte hibyte =  bitmap[x + (y*width)] & 0x0F;     

                {
                    printf("%02hd", hibyte);
                }

                printf(".");

                {
                    printf("%02hd", lobyte);
                };  

                printf(".");
            };
            printf("\n");
        };

        for (byte y=height; y!=header.height; y++)
        {
            for (byte x=0; x!=width; x++)
            {
                printf("00.00.");
            };
            printf("\n");
        };
        
    };
 
    fclose(file);

    for (int i=0; i!=header.nchars+1; i++)
    {
        delete [] characters[i].bitmap;
    };

    delete [] characters;

    delete [] dchar;
    delete [] wchar;
    delete [] hchar;
    //delete [] cdata;
};
