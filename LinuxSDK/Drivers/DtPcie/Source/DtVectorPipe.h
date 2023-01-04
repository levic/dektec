// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorPipe.h *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
//

#ifndef __DT_VECTOR_PIPE_H
#define __DT_VECTOR_PIPE_H
// .-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtVectorPipe functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

#include "DtPipe.h"
#include "DtVector.h"

typedef struct _DtVector DtVectorPipe;

DtPipe* DtVectorPipe_At(const DtVectorPipe*, Int Index);
DtVectorPipe* DtVectorPipe_Create(Int Capacity, Int GrowSize);
void DtVectorPipe_Cleanup(DtVectorPipe*);
DtStatus DtVectorPipe_SetEmpty(DtVectorPipe*, Int Index);
Int DtVectorPipe_FindEmpty(DtVectorPipe*, const Int MaxPipes);
Int DtVectorPipe_FindPipeIndex(const DtVectorPipe*, DtPipe* pPipe);
DtStatus DtVectorPipe_Set(const DtVectorPipe*, Int Index, DtPipe*);
Int DtVectorPipe_Size(const DtVectorPipe*);
DtStatus DtVectorPipe_Reserve(DtVectorPipe*, Int NewCapacity);
DtStatus DtVectorPipe_Erase(DtVectorPipe* pTheVector, Int Index);
DtStatus DtVectorPipe_PushBack(DtVectorPipe* pTheVector, const DtPipe* pPipe);
DtPipe* DtVectorPipe_Begin(const DtVectorPipe*);
DtPipe* DtVectorPipe_Next(const DtVectorPipe*, DtPipe*);
DtPipe* DtVectorPipe_End(const DtVectorPipe*);
DtStatus DtVectorPipe_Clear(DtVectorPipe* pTheVector);

#endif
