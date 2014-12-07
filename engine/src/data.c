
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
DsDeleteBlkCand(gpointer gp_BlkCand)
{
    if (gp_BlkCand) {
        BLOCK_CAND *p_BlkCand = (BLOCK_CAND*)gp_BlkCand;    
        if (p_BlkCand->a_ContAddr)
            g_array_free(p_BlkCand->a_ContAddr, true);
    }

    return;
}


void
DsDeleteGroup(gpointer gp_Val)
{
    if (gp_Val) {
        GROUP *p_Grp = (GROUP*)gp_Val;
        if (p_Grp->a_Mbr)
            g_array_free(p_Grp->a_Mbr, true);
        if (p_Grp->a_BlkCand)
            g_ptr_array_free(p_Grp->a_BlkCand, true);
    }

    return;
}


int8_t
DsNewGroup(GROUP **pp_Grp)
{
    int8_t cRtnCode = CLS_SUCCESS;

    *pp_Grp = (GROUP*)malloc(sizeof(GROUP));
    if (!(*pp_Grp))
        EXIT1(CLS_FAIL_MEM_ALLOC, EXIT, "Error: %s.", strerror(errno));

    GROUP *p_Grp = *pp_Grp;
    p_Grp->a_Mbr = g_array_new(false, false, sizeof(uint64_t));
    if (!p_Grp->a_Mbr)
        EXIT1(CLS_FAIL_MEM_ALLOC, FREEGRP, "Error: %s.", strerror(errno));

    p_Grp->a_BlkCand = g_ptr_array_new_with_free_func(DsDeleteBlkCand);
    if (!p_Grp->a_BlkCand)
        EXIT1(CLS_FAIL_MEM_ALLOC, FREEMBR, "Error: %s.", strerror(errno));    
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
