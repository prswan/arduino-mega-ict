#ifndef __82S100_H__
#define __82S100_H__

#include <stdint.h>


#pragma pack(push, 1)

// Fuses for one single LogicTerm (16 pairs for min terms and 8 fuses for max terms)
typedef struct LogicTerm
{
    bool AND[16][2];
    bool OR[8];
}
LogicTerm;

// Fuse bits for 82S100/PLS100 match to layout of bits from JEDEC file
typedef struct PLS100
{
    LogicTerm minTerms[48];
    bool NEG[8];
}
PLS100;

// Dirty trick to bring life (values) to the structures
typedef union PLA
{
    uint8_t fusesFromJedecFile[1928];
    PLS100 fusesInPLA;
}
PLA;

// Function to convert the PLA fuse map into flat EPROM binary
int convert_82S100(PLA *pla);

#pragma pack(pop)

#endif // __82S100_H__
