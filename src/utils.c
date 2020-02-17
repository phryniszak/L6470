#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "xnucleoihm02a1.h"
#include "utils.h"

/**
 * @brief  Print on the PC screen the values of the L6470 Registers.
 * @param  ExpBrd    The addressed Expansion Board
 * @param  L6470_Id  The addressed L6470
 *         
 */
void PrintRegisterValues(uint8_t ExpBrd, uint8_t L6470_Id)
{
    uint8_t r;
    uint32_t value;

    /* Initialize the used GPIO for the L6470 nCS related to the addressed X-NUCLEO-IHM02A1 */
    if (BSP_Select(ExpBrd))
    {
        printf("\n\rValues of the registers of L6470_# 0x%02x of ExpBrd_# 0x%02x\n\r\n\r", L6470_Id, ExpBrd);

        for (r = 0; r < L6470REGIDSIZE; r++)
        {
            value = L6470_GetParam(L6470_Id, (eL6470_RegId_t)r);
            printf("%s: 0x%04x\n\r", L6470_GetRegisterName((eL6470_RegId_t)r), value);
        }

        printf("\n\r");
    }
}

/**
 * @brief  Print on the PC screen the values of the L6470 Registers for all
 *         devices mounted on all stacked X-NUCLEO-IHM02A1.
 */
void PrintAllRegisterValues(void)
{
    uint8_t ExpBrd;   /* to index the expansion board to be addressed */
    uint8_t L6470_Id; /* to index the L6470 into the daisy chain to be addressed */

    for (ExpBrd = EXPBRD_ID(0); ExpBrd <= EXPBRD_ID(EXPBRD_MOUNTED_NR - 1);
         ExpBrd++)
    {
        for (L6470_Id = L6470_ID(0);
             L6470_Id <= L6470_ID(L6470DAISYCHAINSIZE - 1); L6470_Id++)
        {
            PrintRegisterValues(ExpBrd, L6470_Id);
        }
    }
}

// https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}