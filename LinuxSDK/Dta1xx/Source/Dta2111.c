//-.-.-.-.-.-.-.-.-.-.-.-.- Dta2111FrequencyResponseCompensation -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Apply frequency-response compensation, given the frequency stored in the channel data.

#include "../Include/Dta1xx.h"

// frequnecy compensation table
//
FreqCompTableEntry Dta2111FreqCompTable[] = {
//  {m_Freq, m_Fsc, m_Attn},
//  {   MHz,   cBm,    cBm},
//  {------,------,-------},
    {    34,    24,      0},
    {   106,    29,      0},
    {   202,    69,      0},
    {   402,   204,      0},
    {   506,   312,      0},
    {   578,   352,      0},
    {   602,   389,      0},
    {   626,   371,      0},
    {   650,   401,      0},
    {   674,   392,      0},
    {   706,   429,      0},
    {   730,   458,      0},
    {   754,   520,      0},
    {   778,   574,      0},
    {   810,   696,      0},
    {   834,   736,      0},
    {   858,   823,      0},
    {   882,   844,      0},
    {   906,   903,      0},
    {   938,   875,      0},
    {   962,  1010,      0},
    {   994,   955,      0},
    {  1002,  1010,      0},
};

void Dta2111Init(Channel * pCh) {
    pCh->m_OutpLevelOffset   = -422; // cBm
    pCh->m_OutpLevelGain     = 2871; // 1024/cBm = 1024 * delta(DACvalue) / delta(outputlevel(cBm))
    pCh->m_FreqCompTable     = Dta2111FreqCompTable;
    pCh->m_FreqCompTableSize = sizeof(Dta2111FreqCompTable)/sizeof(FreqCompTableEntry);
}
