
#include "except.h"
#include "spew.h"
#include "data.h"


void
DsDeleteHashKey(gpointer gp_Key)
{
    return;
}


void
DsDeleteString(gpointer gp_Str)
{
    if (gp_Str)
        free(gp_Str);

    return;
}


void
DsDeleteBind(gpointer gp_Bind)
{
    if (gp_Bind)
        free(gp_Bind);

    return;
}


void
DsDeleteArrayString(gpointer gp_AStr)
{
    if (gp_AStr) {
        GPtrArray *aStr = (GPtrArray*)gp_AStr;
        g_ptr_array_free(aStr, true);
    }

    return;
}


void
DsDeleteContentAddr(gpointer gp_CtnAddr)
{
    if (gp_CtnAddr)
        free(gp_CtnAddr);

    return;
}


void
DsDeleteBlkCand(gpointer gp_BlkCand)
{
    if (gp_BlkCand) {
        BLOCK_CAND *p_BlkCand = (BLOCK_CAND*)gp_BlkCand;    
        if (p_BlkCand->a_CtnAddr)
            g_array_free(p_BlkCand->a_CtnAddr, true);
        if (p_BlkCand->t_CtnAddr)
            g_tree_destroy(p_BlkCand->t_CtnAddr);    
        if (p_BlkCand->a_usCtn)
            free(p_BlkCand->a_usCtn);    
        free(p_BlkCand);
    }

    return;
}


void
DsDeleteSlice(gpointer gp_Slc)
{
    if (gp_Slc)
        free(gp_Slc);

    return;
}


void
DsDeleteGroup(gpointer gp_Grp)
{
    if (gp_Grp) {
        GROUP *p_Grp = (GROUP*)gp_Grp;
        if (p_Grp->a_Mbr)
            g_array_free(p_Grp->a_Mbr, true);
        if (p_Grp->a_BlkCand)
            g_ptr_array_free(p_Grp->a_BlkCand, true);
        free(p_Grp);
    }

    return;
}


void
DsDeleteMeltPot(gpointer gp_Pot)
{
    if (gp_Pot) {
        MELT_POT *p_Pot = (MELT_POT*)gp_Pot;
        if (p_Pot->a_Path)
            g_ptr_array_free(p_Pot->a_Path, true);
        if (p_Pot->a_Hash)
            g_ptr_array_free(p_Pot->a_Hash, true);
        if (p_Pot->a_Slc)        
            g_ptr_array_free(p_Pot->a_Slc, true);
        if (p_Pot->h_Grp)
            g_hash_table_destroy(p_Pot->h_Grp);
        free(p_Pot);    
    }

    return;
}


int8_t
DsNewBlockCand(BLOCK_CAND **pp_BlkCand, uint8_t usSizeCtn)
{
    int8_t cRtnCode = SUCCESS;

    *pp_BlkCand = (BLOCK_CAND*)malloc(sizeof(BLOCK_CAND));
    if (!(*pp_BlkCand))
        EXIT1(FAIL_MEM_ALLOC, EXIT, "Error: %s.", strerror(errno));

    BLOCK_CAND *p_BlkCand = *pp_BlkCand;
    p_BlkCand->a_CtnAddr = g_array_new(false, false, sizeof(CONTENT_ADDR));
    if (!p_BlkCand->a_CtnAddr)
        EXIT1(FAIL_MEM_ALLOC, FREEBLK, "Error: %s.", strerror(errno));

    p_BlkCand->t_CtnAddr = g_tree_new_full(DsCompContentAddrPlus, NULL, 
                                           DsDeleteContentAddr, DsDeleteArrayString);
    if (!p_BlkCand->t_CtnAddr)
        EXIT1(FAIL_MEM_ALLOC, FREEBLK, "Error: %s.", strerror(errno));

    p_BlkCand->a_usCtn = (uint16_t*)malloc(sizeof(uint16_t) * usSizeCtn);
    if (!p_BlkCand->a_usCtn)
        EXIT1(FAIL_MEM_ALLOC, FREEADDR, "Error: %s.", strerror(errno));

    p_BlkCand->ucCntNoise = 0;
    p_BlkCand->ucCntWild = 0;
    goto EXIT;

FREEADDR:
    if (p_BlkCand->a_CtnAddr)
        g_array_free(p_BlkCand->a_CtnAddr, true);
FREEBLK:
    if (*pp_BlkCand)
        free(*pp_BlkCand);
EXIT:
    return cRtnCode;
}


int8_t
DsNewGroup(GROUP **pp_Grp)
{
    int8_t cRtnCode = SUCCESS;

    *pp_Grp = (GROUP*)malloc(sizeof(GROUP));
    if (!(*pp_Grp))
        EXIT1(FAIL_MEM_ALLOC, EXIT, "Error: %s.", strerror(errno));

    GROUP *p_Grp = *pp_Grp;
    p_Grp->a_Mbr = g_array_new(false, false, sizeof(uint64_t));
    if (!p_Grp->a_Mbr)
        EXIT1(FAIL_MEM_ALLOC, FREEGRP, "Error: %s.", strerror(errno));

    p_Grp->a_BlkCand = g_ptr_array_new_with_free_func(DsDeleteBlkCand);
    if (!p_Grp->a_BlkCand)
        EXIT1(FAIL_MEM_ALLOC, FREEMBR, "Error: %s.", strerror(errno));    
    goto EXIT;

FREEMBR:
    if (p_Grp->a_Mbr)
        g_array_free(p_Grp->a_Mbr, true);
FREEGRP:
    if (*pp_Grp)
        free(*pp_Grp);
EXIT:
    return cRtnCode;
}


int8_t
DsNewMeltPot(MELT_POT **pp_Pot)
{
    int8_t cRtnCode = SUCCESS;

    *pp_Pot = (MELT_POT*)malloc(sizeof(MELT_POT));
    if (!(*pp_Pot))
        EXIT1(FAIL_MEM_ALLOC, EXIT, "Error: %s.", strerror(errno));    

    MELT_POT *p_Pot = *pp_Pot;
    p_Pot->a_Path = g_ptr_array_new_with_free_func(DsDeleteString);    
    if (!p_Pot->a_Path)
        EXIT1(FAIL_MEM_ALLOC, FREEPOT, "Error: %s.", strerror(errno));

    p_Pot->a_Hash = g_ptr_array_new_with_free_func(DsDeleteString);
    if (!p_Pot->a_Hash)
        EXIT1(FAIL_MEM_ALLOC, FREENAME, "Error: %s.", strerror(errno));

    p_Pot->a_Slc = g_ptr_array_new_with_free_func(DsDeleteSlice);
    if (!p_Pot->a_Slc)
        EXIT1(FAIL_MEM_ALLOC, FREEHASH, "Error: %s.", strerror(errno));

    p_Pot->h_Grp = g_hash_table_new_full(g_int64_hash, g_int64_equal,
                                         DsDeleteHashKey, DsDeleteGroup);
    if (!p_Pot->h_Grp)
        EXIT1(FAIL_MEM_ALLOC, FREESLC, "Error: %s.", strerror(errno));
    goto EXIT;

FREESLC:
    if (p_Pot->a_Slc)
        g_ptr_array_free(p_Pot->a_Slc, true);
FREEHASH:
    if (p_Pot->a_Hash)
        g_ptr_array_free(p_Pot->a_Hash, true);
FREENAME:
    if (p_Pot->a_Path)
        g_ptr_array_free(p_Pot->a_Path, true);
FREEPOT:
    if (*pp_Pot)
        free(*pp_Pot);
EXIT:
    return cRtnCode;    
}


int
DsCompBlockCandNoise(const void *vp_Src, const void *vp_Tge)
{
    BLOCK_CAND *p_Src = *(BLOCK_CAND**)vp_Src;
    BLOCK_CAND *p_Tge = *(BLOCK_CAND**)vp_Tge;
    return p_Src->ucCntNoise - p_Tge->ucCntNoise;
}


int
DsCompBlockCandWildCard(const void *vp_Src, const void *vp_Tge)
{
    BLOCK_CAND *p_Src = *(BLOCK_CAND**)vp_Src;
    BLOCK_CAND *p_Tge = *(BLOCK_CAND**)vp_Tge;
    if (p_Src->ucCntWild == p_Tge->ucCntWild)
        return (int16_t)p_Src->ucCntNoise - (int16_t)p_Tge->ucCntNoise;
    return (int16_t)p_Src->ucCntWild - (int16_t)p_Tge->ucCntWild;
}


int
DsCompContentAddr(const void *vp_Src, const void *vp_Tge)
{
    CONTENT_ADDR *p_Src = (CONTENT_ADDR*)vp_Src;
    CONTENT_ADDR *p_Tge = (CONTENT_ADDR*)vp_Tge;
    if (p_Src->iIdSec == p_Tge->iIdSec)
        return (double)p_Src->ulOfstRel - (double)p_Tge->ulOfstRel;
    return p_Src->iIdSec - p_Tge->iIdSec;
}


int
DsCompContentAddrPlus(const void *vp_Src, const void *vp_Tge, void *vp_Data)
{
    CONTENT_ADDR *p_Src = (CONTENT_ADDR*)vp_Src;
    CONTENT_ADDR *p_Tge = (CONTENT_ADDR*)vp_Tge;
    if (p_Src->iIdSec == p_Tge->iIdSec)
        return (double)p_Src->ulOfstRel - (double)p_Tge->ulOfstRel;
    return p_Src->iIdSec - p_Tge->iIdSec;
}


int8_t
DsInsertContentAddr(GTree *t_CtnAddr, CONTENT_ADDR *p_Addr, char *szPathFile)
{
    int8_t cRtnCode = SUCCESS;

    GPtrArray *aStr = g_tree_lookup(t_CtnAddr, p_Addr);
    if (!aStr) {
        aStr = g_ptr_array_new();
        if (!aStr)
            EXIT1(FAIL_MEM_ALLOC, EXIT, "Error: %s.", strerror(errno));
        g_tree_insert(t_CtnAddr, p_Addr, aStr);
    }
    g_ptr_array_add(aStr, szPathFile);

EXIT:
    return cRtnCode;
}


gboolean
DsTravContentAddrCopy(gpointer gp_Key, gpointer gp_Val, gpointer gp_Tge)
{
    int8_t cRtnCode = SUCCESS;

    GTree *t_CtnAddr = (GTree*)gp_Tge;
    CONTENT_ADDR *p_AddrSrc = (CONTENT_ADDR*)gp_Key;
    GPtrArray *aStrTge = g_tree_lookup(t_CtnAddr, p_AddrSrc);
    if (!aStrTge) {
        CONTENT_ADDR *p_AddrTge = (CONTENT_ADDR*)malloc(sizeof(CONTENT_ADDR));
        if (!p_AddrTge)
            EXIT1(FAIL_MEM_ALLOC, EXIT, "Error: %s.", strerror(errno));
        aStrTge = g_ptr_array_new();
        if (!aStrTge)
            EXIT1(FAIL_MEM_ALLOC, EXIT, "Error: %s.", strerror(errno));
        p_AddrTge->iIdSec = p_AddrSrc->iIdSec;
        p_AddrTge->ulOfstRel = p_AddrSrc->ulOfstRel;
        g_tree_insert(t_CtnAddr, p_AddrTge, aStrTge);
    }

    GPtrArray *aStrSrc = (GPtrArray*)gp_Val;
    uint64_t ulIdx;
    for (ulIdx = 0 ; ulIdx < aStrSrc->len ; ulIdx++) {
        char *szPathFile = g_ptr_array_index(aStrSrc, ulIdx);
        g_ptr_array_add(aStrTge, szPathFile);
    }

EXIT:
    return (cRtnCode == SUCCESS)? false : true;
}