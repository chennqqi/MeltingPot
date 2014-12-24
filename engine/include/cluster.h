#ifndef _CLUSTER_H_
#define _CLUSTER_H_


#include <stdint.h>
#include <stdbool.h>


/* The messages for debugging. */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define FAIL_OPT_PARSE_CONF      "Fail to parse \"conf\" option"
#define FAIL_NO_SAMPLE           "No samples in the given folder"
#define FAIL_PTN_CREATE          "Insufficient pattern buffer size"
#define SLC_GEN_FAIL    ANSI_COLOR_RED"[Step 1/4] Slice Generation  **FAIL"ANSI_COLOR_RESET
#define SLC_GEN_SUCC    ANSI_COLOR_GREEN"[Step 1/4] Slice Generation  **SUCC"ANSI_COLOR_RESET
#define SLC_CRL_FAIL    ANSI_COLOR_RED"[Step 2/4] Slice Correlation  **FAIL"ANSI_COLOR_RESET
#define SLC_CRL_SUCC    ANSI_COLOR_GREEN"[Step 2/4] Slice Correlation  **SUCC"ANSI_COLOR_RESET
#define PTN_GEN_FAIL    ANSI_COLOR_RED"[Step 3/4] Pattern Crafting  **FAIL"ANSI_COLOR_RESET
#define PTN_GEN_SUCC    ANSI_COLOR_GREEN"[Step 3/4] Pattern Crafting  **SUCC"ANSI_COLOR_RESET
#define PTN_OUT_FAIL    ANSI_COLOR_RED"[Step 4/4] Pattern Output  **FAIL"ANSI_COLOR_RESET
#define PTN_OUT_SUCC    ANSI_COLOR_GREEN"[Step 4/4] Pattern Output  **SUCC"ANSI_COLOR_RESET


/* The constants for command option parsing. */
#define BUF_SIZE_OPT          (64)    /* The size of option order buffer. */
#define OPT_LONG_HELP         "help"  /* The option for help message. */
#define OPT_LONG_PATH_CONF    "conf"  /* The abbreviated character. */
#define OPT_HELP              'h'     /* The option for the path of configuration file. */
#define OPT_PATH_CONF         'c'     /* The abbreviated character. */


/* The tags for each kind of configuration. */
#define C_COUNT_THREAD              "COUNT_THREAD"
#define C_THRESHOLD_SIMILARITY      "THRESHOLD_SIMILARITY"
#define C_COUNT_HEX_BLOCK           "COUNT_HEX_BLOCK"
#define C_SIZE_HEX_BLOCK            "SIZE_HEX_BLOCK"
#define C_SIZE_SLICE                "SIZE_SLICE"
#define C_RATIO_NOISE               "RATIO_NOISE"
#define C_RATIO_WILDCARD            "RATIO_WILDCARD"
#define C_IO_BANDWIDTH              "IO_BANDWIDTH"
#define C_SIZE_TRUNCATE_GROUP       "TRUNCATE_GROUP_SIZE_LESS_THAN"
#define C_PATH_ROOT_INPUT           "PATH_ROOT_INPUT"
#define C_PATH_ROOT_OUTPUT          "PATH_ROOT_OUTPUT"
#define C_PATH_PLUGIN_SLICE         "PATH_PLUGIN_SLICE"
#define C_PATH_PLUGIN_SIMILARITY    "PATH_PLUGIN_SIMILARITY"


enum {
    CLS_SUCCESS = 0,
    CLS_FAIL_FILE_IO = -1,
    CLS_FAIL_MEM_ALLOC = -2,
    CLS_FAIL_OPT_PARSE = -3,
    CLS_FAIL_CONF_PARSE = -4,
    CLS_FAIL_PLUGIN_RESOLVE = -5,
    CLS_FAIL_PLUGIN_INTERACT = -6,
    CLS_FAIL_PROCESS = -7,
    CLS_FAIL_PTN_CREATE = -8,
};


/* This ds records the paramters to control the clustering process.
   Note the reason that we do not follow the definition of standard integer 
   is due to the type definition of libconfig. */
typedef struct _CONFIG_T {
    uint8_t ucCntThrd;        /* The number of threads used for parallel processing. */
    uint8_t ucScoreSim;       /* The threshold to group similar binary slices. */
    uint8_t ucCntBlk;         /* The number of hex byte blocks in pattern. */
    uint8_t ucSizeBlk;        /* The size of the hex byte block. */
    uint8_t ucSizeTruncGrp;   /* The threshold to truncate groups with size less than it. */
    uint8_t ucRatNoise;       /* The ratio of meaningless bytes in one hex block. */
    uint8_t ucRatWild;        /* The ratio of wildcard characters in one hex block. */
    uint8_t ucIoBand;         /* The number of files a thread can simultaneously open. */
    uint16_t usSizeSlc;       /* The size of the binary slice extracted from file
                                 which is the basic unit of clustering process. */
    char *szPathRootIn;       /* The root pathname of the input sample set. */
    char *szPathRootOut;      /* The root pathname of the output patterns. */
    char *szPathPluginSlc;    /* The pathname of the plugin which is to be used for 
                                 file slicing. */
    char *szPathPluginSim;    /* The pathname of the plugin which is to be used for 
                                 similarity computation. */
} CONFIG;

/* The structure recording the entire process context. */
typedef struct _CONTEXT_T {
    CONFIG *p_Conf;
    MELT_POT *p_Pot;
    PLUGIN_SLICE *plg_Slc;
    PLUGIN_SIMILARITY *plg_Sim;
} CONTEXT;


/**
 * This function initializes the clustering engine with the configuration
 * specified by user.
 *
 * @param szPathCfg     The pathname of the task configuration.
 *
 * @return status code
 */
int8_t
ClsInit(char *szPathCfg);


/**
 * This function releases the resources allocated by clustering engine.
 *
 * @return status code
 */
int8_t
ClsDeinit();


/**
 * This function launches the major clustering workflow.
 *
 * @return status code
 */
int8_t
ClsRunTask();


#endif
