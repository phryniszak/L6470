#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <pigpio.h>

#include "params.h"
#include "xnucleoihm02a1.h"
#include "utils.h"

#define MPR_1 4       //!< Motor Movements Per Revolution 1st option
#define MPR_2 8       //!< Motor Movements Per Revolution 2nd option
#define DELAY_1 1000  //!< Delay time 1st option
#define DELAY_2 2500  //!< Delay time 2nd option
#define DELAY_3 10000 //!< Delay time 3rd option

/**
 * @brief  Example no.1 for X-NUCLEO-IHM02A1.
 * @note	Perform a complete motor axis revolution as MPR_1 equal movements,
 *			for each L6470 mounted on all stacked X-NUCLEO-IHM02A1.
 *			At the end of each movement there is a delay of DELAY_1 ms.
 *     	After each motor has performed a complete revolution there is a
 *			delay of DELAY_2 ms.
 *			Now all motors for each X-NUCLEO-IHM02A1 will start at the same
 *			time.
 *			They are going to run at INIT_SPEED for DELAY_3 ms.
 *			After that all motors for each X-NUCLEO-IHM02A1 will get a HardStop
 *			at the same time.
 *			Perform a complete motor axis revolution as MPR_2 equal movements,
 *			for each L6470 mounted on all stacked X-NUCLEO-IHM02A1.
 *			At the end of each movement there is a delay of DELAY_1 ms.
 *			After that all motors for each X-NUCLEO-IHM02A1 will get a HardHiZ
 *			at the same time.
 */
void MicrosteppingMotor_Example_01(void)
{

    uint32_t Step;
    uint32_t Speed;
    uint8_t MovementPerRevolution;
    uint8_t i;
    uint8_t board, device;

    uint8_t id;

    StepperMotorBoardHandle_t *StepperMotorBoardHandle;
    MotorParameterData_t *MotorParameterDataGlobal, *MotorParameterDataSingle;

    printf("Initial values for registers:\n\r");
    PrintAllRegisterValues();

    /* Setup each X-NUCLEO-IHM02A1 Expansion Board ******************************/

    /* Get the parameters for the motor connected with the 1st stepper motor driver of the 1st stepper motor expansion board */
    MotorParameterDataGlobal = GetMotorParameterInitData();

    for (id = 0; id < EXPBRD_MOUNTED_NR; id++)
    {
        StepperMotorBoardHandle = BSP_GetExpansionBoardHandle(EXPBRD_ID(id));
        MotorParameterDataSingle = MotorParameterDataGlobal + (id * L6470DAISYCHAINSIZE);
        StepperMotorBoardHandle->Config(MotorParameterDataSingle);
    }

    printf("Custom values for registers:\n\r");
    PrintAllRegisterValues();

    /****************************************************************************/

    MovementPerRevolution = MPR_1;
    for (board = EXPBRD_ID(0); board <= EXPBRD_ID(EXPBRD_MOUNTED_NR - 1); board++)
    {
        StepperMotorBoardHandle = BSP_GetExpansionBoardHandle(board);

        for (device = L6470_ID(0); device <= L6470_ID(L6470DAISYCHAINSIZE - 1); device++)
        {
            /* Get the parameters for the motor connected with the actual stepper motor driver of the actual stepper motor expansion board */
            MotorParameterDataSingle = MotorParameterDataGlobal + ((board * L6470DAISYCHAINSIZE) + device);
            Step = ((uint32_t)MotorParameterDataSingle->fullstepsperrevolution * pow(2, MotorParameterDataSingle->step_sel)) / MovementPerRevolution;

            for (i = 0; i < MovementPerRevolution; i++)
            {
                printf("board: %d  device: %d  move L6470_DIR_FWD_ID steps: %d\n\r", board, device, Step);
                StepperMotorBoardHandle->Command->Move(board, device, L6470_DIR_FWD_ID, Step);
                while (StepperMotorBoardHandle->Command->CheckStatusRegisterFlag(board, device, BUSY_ID) == 0)
                    ;
                printf("sleep %d[ms]\n\r", DELAY_1);
                msleep(DELAY_1);
            }
        }
    }

    printf("sleep %d[ms]\n\r", DELAY_2);
    msleep(DELAY_2);

    for (board = EXPBRD_ID(0); board <= EXPBRD_ID(EXPBRD_MOUNTED_NR - 1); board++)
    {

        for (device = L6470_ID(0); device <= L6470_ID(L6470DAISYCHAINSIZE - 1); device++)
        {
            /* Get the parameters for the motor connected with the actual stepper motor driver of the actual stepper motor expansion board */
            MotorParameterDataSingle = MotorParameterDataGlobal + ((board * L6470DAISYCHAINSIZE) + device);

            /* Set Speed */
            Speed = Step_s_2_Speed(MotorParameterDataSingle->speed);

            printf("board: %d  device: %d  run L6470_DIR_FWD_ID speed: %d\n\r", board, device, Speed);

            /* Prepare the stepper driver to be ready to perform a command */
            StepperMotorBoardHandle->StepperMotorDriverHandle[device]->Command->PrepareRun(device, L6470_DIR_FWD_ID, Speed);
        }

        StepperMotorBoardHandle->Command->PerformPreparedApplicationCommand();
    }

    printf("sleep %d[ms]\n\r", DELAY_3);
    msleep(DELAY_3);

    for (board = EXPBRD_ID(0); board <= EXPBRD_ID(EXPBRD_MOUNTED_NR - 1); board++)
    {

        for (device = L6470_ID(0); device <= L6470_ID(L6470DAISYCHAINSIZE - 1); device++)
        {

            printf("board: %d  device: %d  hard stop\n\r", board, device);

            /* Prepare the stepper driver to be ready to perform a command */
            StepperMotorBoardHandle->StepperMotorDriverHandle[device]->Command->PrepareHardStop(device);
        }

        StepperMotorBoardHandle->Command->PerformPreparedApplicationCommand();
    }

    printf("sleep %d[ms]\n\r", DELAY_2);
    msleep(DELAY_2);

    MovementPerRevolution = MPR_2;
    for (board = EXPBRD_ID(0); board <= EXPBRD_ID(EXPBRD_MOUNTED_NR - 1); board++)
    {
        StepperMotorBoardHandle = BSP_GetExpansionBoardHandle(board);

        for (device = L6470_ID(0); device <= L6470_ID(L6470DAISYCHAINSIZE - 1); device++)
        {
            /* Get the parameters for the motor connected with the actual stepper motor driver of the actual stepper motor expansion board */
            MotorParameterDataSingle = MotorParameterDataGlobal + ((board * L6470DAISYCHAINSIZE) + device);
            Step = ((uint32_t)MotorParameterDataSingle->fullstepsperrevolution * pow(2, MotorParameterDataSingle->step_sel)) / MovementPerRevolution;

            for (i = 0; i < MovementPerRevolution; i++)
            {
                printf("board: %d  device: %d  move L6470_DIR_FWD_ID steps: %d\n\r", board, device, Step);
                StepperMotorBoardHandle->Command->Move(board, device, L6470_DIR_FWD_ID, Step);
                while (StepperMotorBoardHandle->Command->CheckStatusRegisterFlag(board, device, BUSY_ID) == 0)
                    ;
                printf("sleep %d[ms]\n\r", DELAY_1);
                msleep(DELAY_1);
            }
        }
    }

    for (board = EXPBRD_ID(0); board <= EXPBRD_ID(EXPBRD_MOUNTED_NR - 1); board++)
    {

        for (device = L6470_ID(0); device <= L6470_ID(L6470DAISYCHAINSIZE - 1); device++)
        {

            printf("board: %d  device: %d  hard stop HiZ\n\r", board, device);
            /* Prepare the stepper driver to be ready to perform a command */
            StepperMotorBoardHandle->StepperMotorDriverHandle[device]->Command->PrepareHardHiZ(device);
        }

        StepperMotorBoardHandle->Command->PerformPreparedApplicationCommand();
    }

    printf("example finished\n\r\n\r");
}

// This pin is an open drain output which can be used as the busy flag or synchronization
// signal according to the SYNC_EN bit value (STEP_MODE register).
// The pin works as busy signal when the SYNC_EN bit is set low (default condition). In this
// mode the output is forced low while a constant speed, absolute positioning or motion
// command is under execution. The BUSY pin is released when the command has been
// executed (target speed or target position reached). The STATUS register includes a BUSY
// flag that is the BUSY pin mirror

void busy_edge(int gpio, int level, uint32_t tick)
{
    if (level == 0)
    {
        printf("BUSY_SYNC command executed start\n\r");
    }
    else if (level == 1)
    {
        printf("BUSY_SYNC command executed stopped\n\r");
    }
}

// Status flag pin. An internal open drain transistor can
// pull the pin to GND when a programmed alarm
// condition occurs (step loss, OCD, thermal pre warning or shutdown, UVLO, wrong command, non performable command)

void flag_edge(int gpio, int level, uint32_t tick)
{
    if (level == 0)
    {
        printf("FLAG pin alarm set\n\r");
    }
    else if (level == 1)
    {
        printf("FLAG pin alarm reset\n\r");
    }
}

/*
 * 
 */
int main(int argc, char **argv)
{

    // https://stackoverflow.com/questions/1716296/why-does-printf-not-flush-after-the-call-unless-a-newline-is-in-the-format-strin
    setbuf(stdout, NULL);
    printf("Program started\n\r");

    int euid = geteuid();

    printf("The EFFECTIVE UID = %d\n\r", euid);
    if (euid != 0)
    {
        printf("Did you forget to run as sudo?\n\r");
        return (-1);
    }

    /* RPI initialization */
    int res = RPI_Board_Init();
    if (res < 0)
    {
        printf("Failed to init SPI\n\r");
        return (-1);
    }

    /* X-NUCLEO-IHM02A1 initialization */
    BSP_Init();

    set_nBUSY_SYNC_Handler(busy_edge);
    set_nFLAG_Handler(flag_edge);

    /* Perform a batch commands for X-NUCLEO-IHM02A1 */
    MicrosteppingMotor_Example_01();

    /* RPI deinitialization */
    RPI_Board_Close();

    printf("program is finished\n\r");

    return (0);
}
