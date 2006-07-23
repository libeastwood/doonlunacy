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

int main(int argc, char* argv[])
{
    FILE* file = fopen("new10p.fnt", "rb");

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

    word cdata[header.nchars+1];

    fread(cdata, sizeof(word), header.nchars+1, file);
    
    byte wchar[header.nchars+1];

    printf("wpos %d\n", header.wpos);
    fseek(file, header.wpos, SEEK_SET);
    long br = fread(cdata, sizeof(byte), header.nchars+1, file);
    printf("br %ld\n", br);

    if (wchar[0] != 8) printf("bad!!\n");

    //for (int i=0; i!=header.nchars+1; i++)
    //    printf("%d %hd\n", i, wchar[i]);

    fclose(file);
};
