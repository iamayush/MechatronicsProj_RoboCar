#include <std.h>
#include <log.h>
#include <clk.h>
#include <gbl.h>
#include <bcache.h>

#include <mem.h> // MEM_alloc calls
#include <que.h> // QUE functions
#include <sem.h> // Semaphore functions
#include <sys.h>
#include <tsk.h> // TASK functions
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <c6x.h> // register defines


#include "projectinclude.h"
#include "c67fastMath.h" // sinsp,cossp, tansp
#include "evmomapl138.h"
#include "evmomapl138_i2c.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_led.h"
#include "evmomapl138_dip.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_vpif.h"
#include "evmomapl138_spi.h"
#include "COECSL_edma3.h"
#include "COECSL_mcbsp.h"
#include "COECSL_registers.h"

#include "mcbsp_com.h"
#include "ColorVision.h"
#include "ColorLCD.h"
#include "sharedmem.h"
#include "LCDprintf.h"
#include "ladar.h"
#include "xy.h"
#include "MatrixMath.h"

#define FEETINONEMETER 3.28083989501312

extern EDMA3_CCRL_Regs *EDMA3_0_Regs;

volatile uint32_t index;

// test variables
extern float enc1;  // Left motor encoder
extern float enc2;  // Right motor encoder
extern float enc3;
extern float enc4;
extern float adcA0;  // ADC A0 - Gyro_X -400deg/s to 400deg/s  Pitch
extern float adcB0;  // ADC B0 - External ADC Ch4 (no protection circuit)
extern float adcA1;  // ADC A1 - Gyro_4X -100deg/s to 100deg/s  Pitch
extern float adcB1;  // ADC B1 - External ADC Ch1
extern float adcA2;  // ADC A2 -    Gyro_4Z -100deg/s to 100deg/s  Yaw
extern float adcB2;  // ADC B2 - External ADC Ch2
extern float adcA3;  // ADC A3 - Gyro_Z -400deg/s to 400 deg/s  Yaw
extern float adcB3;  // ADC B3 - External ADC Ch3
extern float adcA4;  // ADC A4 - Analog IR1
extern float adcB4;  // ADC B4 - USONIC1
extern float adcA5;  // ADC A5 -    Analog IR2
extern float adcB5;  // ADC B5 - USONIC2
extern float adcA6;  // ADC A6 - Analog IR3
extern float adcA7;  // ADC A7 - Analog IR4
extern float compass;
extern float switchstate;


//ColorVision
extern float blueobjectx;
extern float blueobjecty;
extern int bluenumpels;
extern float pinkobjectx;
extern float pinkobjecty;
extern int pinknumpels;
extern float greenobjectx;
extern float greenobjecty;
extern int greennumpels;
float objectx_b=0.0;
float objecty_b=0.0;
int numpels_b=0.0;
float objectx_p=0.0;
float objecty_p=0.0;
int numpels_p=0.0;
float objectx_g=0.0;
float objecty_g=0.0;
int numpels_g=0.0;
extern int new_coordata;
extern int flagcolor;
int current_color = 0;
float color_distance_p = 0.0;
float color_vision_error_p = 0;
float color_distance_b = 0.0;
float color_vision_error_b = 0;
float kp_vision = 0.1;
int  exterminate_timer = 0;



// A*
extern int pathLen;            //used to keep track of number of points in reconstructed path
extern int pathRow[400];           //reconstructed path in reverse order
extern int pathCol[400];
extern char map[176];
extern char mapCourseStart[176];
extern int mapRowSize;
extern int mapColSize;
int newpathRow[400];           //reconstructed path in reverse order
int newpathCol[400];
extern void astar(int rowStart, int colStart, int rowEnd, int colEnd);

float vref = 0;
float turn = 0;

int tskcount = 0;
char fromLinuxstring[LINUX_COMSIZE + 2];
char toLinuxstring[LINUX_COMSIZE + 2];

float LVvalue1 = 0;
float LVvalue2 = 0;
int new_LV_data = 0;

int newnavdata = 0;
float newvref = 0;
float newturn = 0;

extern sharedmemstruct *ptrshrdmem;

float x_pred[3][1] = {{0},{0},{0}};                 // predicted state

//more kalman vars
float B[3][2] = {{1,0},{1,0},{0,1}};            // control input model
float u[2][1] = {{0},{0}};          // control input in terms of velocity and angular velocity
float Bu[3][1] = {{0},{0},{0}}; // matrix multiplication of B and u
float z[3][1];                          // state measurement
float eye3[3][3] = {{1,0,0},{0,1,0},{0,0,1}};   // 3x3 identity matrix
float K[3][3] = {{1,0,0},{0,1,0},{0,0,1}};      // optimal Kalman gain
#define ProcUncert 0.0001
#define CovScalar 10
float Q[3][3] = {{ProcUncert,0,ProcUncert/CovScalar},
                 {0,ProcUncert,ProcUncert/CovScalar},
                 {ProcUncert/CovScalar,ProcUncert/CovScalar,ProcUncert}};   // process noise (covariance of encoders and gyro)
#define MeasUncert 1
float R[3][3] = {{MeasUncert,0,MeasUncert/CovScalar},
                 {0,MeasUncert,MeasUncert/CovScalar},
                 {MeasUncert/CovScalar,MeasUncert/CovScalar,MeasUncert}};   // measurement noise (covariance of LADAR)
float S[3][3] = {{1,0,0},{0,1,0},{0,0,1}};  // innovation covariance
float S_inv[3][3] = {{1,0,0},{0,1,0},{0,0,1}};  // innovation covariance matrix inverse
float P_pred[3][3] = {{1,0,0},{0,1,0},{0,0,1}}; // predicted covariance (measure of uncertainty for current position)
float temp_3x3[3][3];               // intermediate storage
float temp_3x1[3][1];               // intermediate storage
float ytilde[3][1];                 // difference between predictions

// deadreckoning
float vel1 = 0,vel2 = 0;
float vel1old = 0,vel2old = 0;
float enc1old = 0,enc2old = 0;

// SETTLETIME should be an even number and divisible by 3
#define SETTLETIME 6000
int settlegyro = 0;
float gyro_zero = 0;
float gyro_angle = 0;
float old_gyro = 0;
float gyro_drift = 0;
float gyro = 0;
int gyro_degrees = 0;
float gyro_radians = 0.0;
float gyro_x = 0,gyro_y = 0;
float gyro4x_gain = 1;

int statePos = 0;   // index into robotdest
//int robotdestSize = 8;    // number of positions to use out of robotdest
//pose robotdest[8];    // array of waypoints for the robot
int robotdestSize = 13;  // number of positions to use out of robotdest
pose robotdest[13];  // array of waypoints for the robot


extern float newLADARdistance[LADAR_MAX_DATA_SIZE];  //in mm
extern float newLADARangle[LADAR_MAX_DATA_SIZE];        // in degrees
float LADARdistance[LADAR_MAX_DATA_SIZE];
float LADARangle[LADAR_MAX_DATA_SIZE];
extern pose ROBOTps;
extern pose LADARps;
extern float newLADARdataX[LADAR_MAX_DATA_SIZE];
extern float newLADARdataY[LADAR_MAX_DATA_SIZE];
float LADARdataX[LADAR_MAX_DATA_SIZE];
float LADARdataY[LADAR_MAX_DATA_SIZE];
extern int newLADARdata;

// Optitrack Variables
int trackableIDerror = 0;
int firstdata = 1;
volatile int new_optitrack = 0;
volatile float previous_frame = -1;
int frame_error = 0;
volatile float Optitrackdata[OPTITRACKDATASIZE];
pose OPTITRACKps;
float temp_theta = 0.0;
float tempOPTITRACK_theta = 0.0;
volatile int temp_trackableID = -1;
int trackableID = -1;
int errorcheck = 1;
int Labview_flag=0;
float var1=0;//labview
float var2=0;
float var3=0;
float var4=0;

float vref_old = 0;
float turn_old = 0;
float RWdist = 450;
float LWdist = 450;
float FWdist = 2500;
float KpRW = -0.015;
float KpLW = 0.015;
float KpBW = -0.03;
float KpBLW = -0.03;
float KpFW = -0.002;
float Tmax = 2;
float Dvel = 1.5;
float FTvel = 0.4;
float BWdist = 1000;
float right_wall_error = 0;
float left_wall_error = 0;
float front_wall_error = 0;
char mode = 1;
long RW_timer = 0;
long LW_timer = 0;
float hit2goal_dist = 0;
float curr2goal_dist = 0;
float hit_x = 0;
float hit_y = 0;
float curr_angle = 0;
float dot_prod = 0;
float x_rcoord = 0;
float y_rcoord = 0;
float random = 0;
int newx = 0;
int newy = 0;
extern float Ladardata_x[LADAR_MAX_DATA_SIZE];
extern float Ladardata_y[LADAR_MAX_DATA_SIZE];
int Fminladar = 0;
int speed_factor = 1;

// weed stuff
int weednum = 0;
char astar_weed = 0;
char weed_centered = 0;
float x_blue[5] = {-1};
float y_blue[5] = {-1};
float x_pink[5] = {-1};
float y_pink[5] = {-1};
float blue_dist = 0;
int pink_tally = 0;
int pinkavg[20] = {0};
int avg = 0;
long timer_c = 0;
char color_lat = 0;
char dead_reckon = 0;
int dead_reckon_timer = 0;
int path_mode = 1;
char pink_have = 0;
char blue_have = 0;
char outside = 1;
char first_b_run = 1;
int b_find_timer = 0;
int avoid_b_timer = 0;
char first_p_run = 1;
int p_find_timer = 0;
int avoid_p_timer = 0;

// Mapping
int ladar_num = 0; //index = ladar_num*20 + 10;
typedef struct {
    int x ;
    int y ;
    int r ;
    int c ;
    char found;
    int tally;
    char orient;
} obs_location;
obs_location obs_loc[57];

char do_astar = 1;
char dont_astar = 0;
char go_direct = 1;
int j = 0;
int blue_count = 0;
int pink_count = 0;

float pwm3 = 0.0;
float pwm4 = 0.0;
float pwm3_b = 0.0;
float pwm4_b = 0.0;


pose UpdateOptitrackStates(pose localROBOTps, int * flag);

//int obs_Notnear(void){
//    int row = 11 - ROBOTps.y;
//    int col = 5 + ROBOTps.x;
//    if ((row == 0)||(row == 10)||(col == 0)||(col == 10)||(row == -1))
//        return 0;
//    else{
//        if (map[row*mapColSize + col + 1] == 'x') return 0;
//        else if (map[row*mapColSize + col - 1] == 'x') return 0;
//        else if (map[(row+1)*mapColSize + col] == 'x') return 0;
//        else if (map[(row-1)*mapColSize + col] == 'x') return 0;
//
//        else if (map[(row+1)*mapColSize + col - 1] == 'x') return 0;
//        else if (map[(row-1)*mapColSize + col - 1] == 'x') return 0;
//        else if (map[(row+1)*mapColSize + col + 1] == 'x') return 0;
//        else if (map[(row-1)*mapColSize + col + 1] == 'x') return 0;
//
//        //    else if (map[row*mapColSize + col + 2] == 'x') return 1;
//        //    else if (map[row*mapColSize + col - 2] == 'x') return 1;
//        //    else if (map[(row+2)*mapColSize + col] == 'x') return 1;
//        //    else if (map[(row-2)*mapColSize + col] == 'x') return 1;
//        else return 1;
//    }
//}

void astarFunc(void){

    int i = 0;
    int c = 0;
    int r = 0;
    int c_end = 0;
    int r_end = 0;
    while(1){
        SEM_pend(&SEM_astar,SYS_FOREVER);


        if(!GET_ASTAR_COMMAND) {
            //For now, Update the shared variables that need to be sent for astar
            for (i=0;i<176;i++) {
                ptrshrdmem->sharedAstarMap[i] = map[i];
            }

            BCACHE_wb((void *)ptrshrdmem,sizeof(sharedmemstruct),EDMA3_CACHE_WAIT);
            SET_ASTAR_COMMAND;
        }

        c = round(ROBOTps.x) + 5;
        r = 11- round(ROBOTps.y);
        c_end = round(robotdest[statePos].x) + 5;
        r_end = 11- round(robotdest[statePos].y);

        //        if (weed_centered == 1){
        //            c_end = round(x_weed_world[weednum]) + 5;
        //            r_end = 11- round(y_weed_world[weednum]);
        //            //            weednum++;
        //            astar_weed = 1;
        //        }

        vref = 0;
        turn = 0;
        astar(r,c,r_end,c_end);
        j = 0;

        for (i = 0;i<pathLen;i++){
            newpathRow[i] = pathRow[pathLen - 1 - i];
            newpathCol[i] = pathCol[pathLen - 1 - i];
        }

    }
}
void ComWithLinux(void) {

    int i = 0;
    TSK_sleep(100);

    while(1) {

        BCACHE_inv((void *)ptrshrdmem,sizeof(sharedmemstruct),EDMA3_CACHE_WAIT);

        if (GET_DATA_FROM_LINUX) {

            if (newnavdata == 0) {
                newvref = ptrshrdmem->Floats_to_DSP[0];
                newturn = ptrshrdmem->Floats_to_DSP[1];
                newnavdata = 1;
            }

            CLR_DATA_FROM_LINUX;

        }

        if (GET_LVDATA_FROM_LINUX) {

            if (ptrshrdmem->DSPRec_size > 256) ptrshrdmem->DSPRec_size = 256;
            for (i=0;i<ptrshrdmem->DSPRec_size;i++) {
                fromLinuxstring[i] = ptrshrdmem->DSPRec_buf[i];
            }
            fromLinuxstring[i] = '\0';

            if (new_LV_data == 0) {
                sscanf(fromLinuxstring,"%f%f",&LVvalue1,&LVvalue2);
                new_LV_data = 1;
            }

            CLR_LVDATA_FROM_LINUX;

        }

        if ((tskcount%6)==0) {
            if (GET_LVDATA_TO_LINUX) {

                if (1)
                {
                    var1 = ROBOTps.x;
                    var2 = ROBOTps.y;
                    var3 = ROBOTps.theta;
                    Labview_flag=0;
                }
                // Default
                //ptrshrdmem->DSPSend_size = sprintf(toLinuxstring,"1.0 1.0 1.0 1.0");
                // you would do something like this
                ptrshrdmem->DSPSend_size = sprintf(toLinuxstring,"%.1f %.1f %.1f %.1f",var1,var2,var3,var4);

                for (i=0;i<ptrshrdmem->DSPSend_size;i++) {
                    ptrshrdmem->DSPSend_buf[i] = toLinuxstring[i];
                }

                // Flush or write back source
                BCACHE_wb((void *)ptrshrdmem,sizeof(sharedmemstruct),EDMA3_CACHE_WAIT);

                CLR_LVDATA_TO_LINUX;

            }
        }

        if (GET_DATAFORFILE_TO_LINUX) {
            // First make sure all scratch elements are zero
            for (i=0;i<500;i++) {
                ptrshrdmem->scratch[i] = 0;
            }
            // Write LADARdataX to scratch
            for (i=0;i<228;i++) {
                ptrshrdmem->scratch[i] = LADARdataX[i];
            }
            // Write LADARdataY to scratch
            for (i=0;i<228;i++) {
                ptrshrdmem->scratch[228+i] = LADARdataY[i];
            }
            // Flush or write back source
            BCACHE_wb((void *)ptrshrdmem,sizeof(sharedmemstruct),EDMA3_CACHE_WAIT);

            CLR_DATAFORFILE_TO_LINUX;
        }

        tskcount++;
        TSK_sleep(40);
    }


}


/*
 *  ======== main ========
 */
Void main()
{

    int i = 0;

    // unlock the system config registers.
    SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
    SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;

    SYSCONFIG1->PUPD_SEL |= 0x10000000;  // change pin group 28 to pullup for GP7[12/13] (LCD switches)

    // Initially set McBSP1 pins as GPIO ins
    CLRBIT(SYSCONFIG->PINMUX[1], 0xFFFFFFFF);
    SETBIT(SYSCONFIG->PINMUX[1], 0x88888880);  // This is enabling the McBSP1 pins

    CLRBIT(SYSCONFIG->PINMUX[16], 0xFFFF0000);
    SETBIT(SYSCONFIG->PINMUX[16], 0x88880000);  // setup GP7.8 through GP7.13
    CLRBIT(SYSCONFIG->PINMUX[17], 0x000000FF);
    SETBIT(SYSCONFIG->PINMUX[17], 0x00000088);  // setup GP7.8 through GP7.13


    //Rick added for LCD DMA flagging test
    GPIO_setDir(GPIO_BANK0, GPIO_PIN8, GPIO_OUTPUT);
    GPIO_setOutput(GPIO_BANK0, GPIO_PIN8, OUTPUT_HIGH);

    GPIO_setDir(GPIO_BANK0, GPIO_PIN0, GPIO_INPUT);
    GPIO_setDir(GPIO_BANK0, GPIO_PIN1, GPIO_INPUT);
    GPIO_setDir(GPIO_BANK0, GPIO_PIN2, GPIO_INPUT);
    GPIO_setDir(GPIO_BANK0, GPIO_PIN3, GPIO_INPUT);
    GPIO_setDir(GPIO_BANK0, GPIO_PIN4, GPIO_INPUT);
    GPIO_setDir(GPIO_BANK0, GPIO_PIN5, GPIO_INPUT);
    GPIO_setDir(GPIO_BANK0, GPIO_PIN6, GPIO_INPUT);

    GPIO_setDir(GPIO_BANK7, GPIO_PIN8, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK7, GPIO_PIN9, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK7, GPIO_PIN10, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK7, GPIO_PIN11, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK7, GPIO_PIN12, GPIO_INPUT);
    GPIO_setDir(GPIO_BANK7, GPIO_PIN13, GPIO_INPUT);

    GPIO_setOutput(GPIO_BANK7, GPIO_PIN8, OUTPUT_HIGH);
    GPIO_setOutput(GPIO_BANK7, GPIO_PIN9, OUTPUT_HIGH);
    GPIO_setOutput(GPIO_BANK7, GPIO_PIN10, OUTPUT_HIGH);
    GPIO_setOutput(GPIO_BANK7, GPIO_PIN11, OUTPUT_HIGH);

    CLRBIT(SYSCONFIG->PINMUX[13], 0xFFFFFFFF);
    SETBIT(SYSCONFIG->PINMUX[13], 0x88888811); //Set GPIO 6.8-13 to GPIOs and IMPORTANT Sets GP6[15] to /RESETOUT used by PHY, GP6[14] CLKOUT appears unconnected

#warn GP6.15 is also connected to CAMERA RESET This is a Bug in my board design Need to change Camera Reset to different IO.

    GPIO_setDir(GPIO_BANK6, GPIO_PIN8, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK6, GPIO_PIN9, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK6, GPIO_PIN10, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK6, GPIO_PIN11, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK6, GPIO_PIN12, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK6, GPIO_PIN13, GPIO_INPUT);


    // on power up wait until Linux has initialized Timer1
    while ((T1_TGCR & 0x7) != 0x7) {
        for (index=0;index<50000;index++) {}  // small delay before checking again

    }

    USTIMER_init();

    // Turn on McBSP1
    EVMOMAPL138_lpscTransition(PSC1, DOMAIN0, LPSC_MCBSP1, PSC_ENABLE);

    // If Linux has already booted It sets a flag so no need to delay
    if ( GET_ISLINUX_BOOTED == 0) {
        USTIMER_delay(4*DELAY_1_SEC);  // delay allowing Linux to partially boot before continuing with DSP code
    }

    // init the us timer and i2c for all to use.
    I2C_init(I2C0, I2C_CLK_100K);
    init_ColorVision();
    init_LCD_mem(); // added rick

    EVTCLR0 = 0xFFFFFFFF;
    EVTCLR1 = 0xFFFFFFFF;
    EVTCLR2 = 0xFFFFFFFF;
    EVTCLR3 = 0xFFFFFFFF;

    init_DMA();
    init_McBSP();

    init_LADAR();

    CLRBIT(SYSCONFIG->PINMUX[1], 0xFFFFFFFF);
    SETBIT(SYSCONFIG->PINMUX[1], 0x22222220);  // This is enabling the McBSP1 pins

    CLRBIT(SYSCONFIG->PINMUX[5], 0x00FF0FFF);
    SETBIT(SYSCONFIG->PINMUX[5], 0x00110111);  // This is enabling SPI pins

    CLRBIT(SYSCONFIG->PINMUX[16], 0xFFFF0000);
    SETBIT(SYSCONFIG->PINMUX[16], 0x88880000);  // setup GP7.8 through GP7.13
    CLRBIT(SYSCONFIG->PINMUX[17], 0x000000FF);
    SETBIT(SYSCONFIG->PINMUX[17], 0x00000088);  // setup GP7.8 through GP7.13

    init_LCD();

    LADARps.x = 3.5/12; // 3.5/12 for front mounting
    LADARps.y = 0;
    LADARps.theta = 1;  // not inverted

    OPTITRACKps.x = 0;
    OPTITRACKps.y = 0;
    OPTITRACKps.theta = 0;

    for(i = 0;i<LADAR_MAX_DATA_SIZE;i++)
    { LADARdistance[i] = LADAR_MAX_READING; } //initialize all readings to max value.

    // ROBOTps will be updated by Optitrack during gyro calibration
    // TODO: specify the starting position of the robot
    ROBOTps.x = 0;          //the estimate in array form (useful for matrix operations)
    ROBOTps.y = 0;
    ROBOTps.theta = 0;  // was -PI: need to flip OT ground plane to fix this
    x_pred[0][0] = ROBOTps.x; //estimate in structure form (useful elsewhere)
    x_pred[1][0] = ROBOTps.y;
    x_pred[2][0] = ROBOTps.theta;

    // defined destinations that moves the robot around and outside the course

    robotdest[0].x = 0;    robotdest[0].y = -1;
    robotdest[1].x = -5;     robotdest[1].y = -3;
    robotdest[2].x = 0;     robotdest[2].y = -1;
    robotdest[3].x = 3;    robotdest[3].y = 7;
    robotdest[4].x = -3;     robotdest[4].y = 7;
    robotdest[5].x = 0;     robotdest[5].y = -1;
    robotdest[6].x = 5;     robotdest[6].y = -3;
    robotdest[7].x = 0;     robotdest[7].y = -1;
    robotdest[8].x = 0;     robotdest[8].y = 11;
    robotdest[9].x = 0;     robotdest[9].y = -1;
    robotdest[10].x = -2;     robotdest[10].y = -4;
    robotdest[11].x = 2;     robotdest[11].y = -4;
    robotdest[12].x = 2;     robotdest[12].y = -4;

    // possible obstacle locations
    //obc_loc

    //for all possible 57 obs_loc initialize found and tally to 0.
    // and for the appropriate range calculate the x and y coordinate, and the row and column coordinate
    //The equations can be found in an excel sheet in the team folder called obstacle_xandy_equations
    //in c://team5final/mylabs/final_v1.


    for(i=0; i<57; i++){
        obs_loc[i].found = 0;
        obs_loc[i].tally = 0;
        if(i<=5){
            obs_loc[i].x = 2*i-5;
            obs_loc[i].orient = 'h';
            obs_loc[i].y = 10;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=6) && (i<=10)){
            obs_loc[i].x =2*i-16;
            obs_loc[i].orient = 'v';
            obs_loc[i].y = 9;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=11) && (i<=16)){
            obs_loc[i].x =2*i-27;
            obs_loc[i].orient = 'h';
            obs_loc[i].y = 8;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=17) && (i<=21)){
            obs_loc[i].x =2*i-38;
            obs_loc[i].orient = 'v';
            obs_loc[i].y = 7;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=22) && (i<=27)){
            obs_loc[i].x =2*i-49;
            obs_loc[i].orient = 'h';
            obs_loc[i].y = 6;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=28) && (i<=32)){
            obs_loc[i].x =2*i-60;
            obs_loc[i].orient = 'v';
            obs_loc[i].y = 5;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=33) && (i<=38)){
            obs_loc[i].x =2*i-71;
            obs_loc[i].orient = 'h';
            obs_loc[i].y = 4;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=39) && (i<=43)){
            obs_loc[i].x =2*i-82;
            obs_loc[i].orient = 'v';
            obs_loc[i].y = 3;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=44) && (i<=49)){
            obs_loc[i].x =2*i-93;
            obs_loc[i].orient = 'h';
            obs_loc[i].y = 2;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if((i>=50) && (i<=54)){
            obs_loc[i].x =2*i-104;
            obs_loc[i].orient = 'v';
            obs_loc[i].y = 1;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else if(i==55){
            obs_loc[i].x =-1;
            obs_loc[i].orient = 'h';
            obs_loc[i].y = 0;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }
        else{
            obs_loc[i].x =1;
            obs_loc[i].orient = 'h';
            obs_loc[i].y = 0;
            obs_loc[i].r = 11-obs_loc[i].y;
            obs_loc[i].c = obs_loc[i].x + 5;
        }

    }


    // flag pins
    GPIO_setDir(IMAGE_TO_LINUX_BANK, IMAGE_TO_LINUX_FLAG, GPIO_OUTPUT);
    GPIO_setDir(OPTITRACKDATA_FROM_LINUX_BANK, OPTITRACKDATA_FROM_LINUX_FLAG, GPIO_OUTPUT);
    GPIO_setDir(DATA_TO_LINUX_BANK, DATA_TO_LINUX_FLAG, GPIO_OUTPUT);
    GPIO_setDir(DATA_FROM_LINUX_BANK, DATA_FROM_LINUX_FLAG, GPIO_OUTPUT);
    GPIO_setDir(DATAFORFILE_TO_LINUX_BANK, DATAFORFILE_TO_LINUX_FLAG, GPIO_OUTPUT);
    GPIO_setDir(LVDATA_FROM_LINUX_BANK, LVDATA_FROM_LINUX_FLAG, GPIO_OUTPUT);
    GPIO_setDir(LVDATA_TO_LINUX_BANK, LVDATA_TO_LINUX_FLAG, GPIO_OUTPUT);


    CLR_OPTITRACKDATA_FROM_LINUX;  // Clear = tell linux DSP is ready for new Opitrack data
    CLR_DATA_FROM_LINUX;  // Clear = tell linux that DSP is ready for new data
    CLR_DATAFORFILE_TO_LINUX;  // Clear = linux not requesting data
    SET_DATA_TO_LINUX;  // Set = put float array data into shared memory for linux
    SET_IMAGE_TO_LINUX;  // Set = put image into shared memory for linux
    CLR_LVDATA_FROM_LINUX;  // Clear = tell linux that DSP is ready for new LV data
    SET_LVDATA_TO_LINUX;  // Set = put LV char data into shared memory for linux

    // clear all possible EDMA
    EDMA3_0_Regs->SHADOW[1].ICR = 0xFFFFFFFF;

    // initializing map
    for(i = 0; i<mapRowSize*mapColSize; i++)        //copy m1 into map for solving
        map[i] = mapCourseStart[i];
}


long timecount= 0;
int whichled = 0;
// This SWI is Posted after each set of new data from the F28335
void RobotControl(void) {

    int newOPTITRACKpose = 0;
    int i = 0;

    if (0==(timecount%1000)) {
        switch(whichled) {
        case 0:
            SETREDLED;
            CLRBLUELED;
            CLRGREENLED;
            whichled = 1;
            break;
        case 1:
            CLRREDLED;
            SETBLUELED;
            CLRGREENLED;
            whichled = 2;
            break;
        case 2:
            CLRREDLED;
            CLRBLUELED;
            SETGREENLED;
            whichled = 0;
            break;
        default:
            whichled = 0;
            break;
        }
    }

    if (GET_OPTITRACKDATA_FROM_LINUX) {

        if (new_optitrack == 0) {
            for (i=0;i<OPTITRACKDATASIZE;i++) {
                Optitrackdata[i] = ptrshrdmem->Optitrackdata[i];
            }
            new_optitrack = 1;
        }

        CLR_OPTITRACKDATA_FROM_LINUX;

    }

    if (new_optitrack == 1) {
        OPTITRACKps = UpdateOptitrackStates(ROBOTps, &newOPTITRACKpose);
        new_optitrack = 0;
    }

    // using 400deg/s gyro
    gyro = adcA3*3.0/4096.0;
    if (settlegyro < SETTLETIME) {
        settlegyro++;
        if (settlegyro < (SETTLETIME/3)) {
            // do nothing
        } else if (settlegyro < (2*SETTLETIME/3)) {
            gyro_zero = gyro_zero + gyro/(SETTLETIME/3);
        } else {
            gyro_drift += (((gyro-gyro_zero) + old_gyro)*.0005)/(SETTLETIME/3);
            old_gyro = gyro-gyro_zero;
        }
        if(settlegyro%500 == 0) {
            LCDPrintfLine(1,"Cal Gyro -- %.1fSecs", (float)(SETTLETIME - settlegyro)/1000.0 );
            LCDPrintfLine(2,"");
        }
        enc1old = enc1;
        enc2old = enc2;
        newOPTITRACKpose = 0;

        SetRobotOutputs(0,0,0,0,0,0,0,0,0,0);
    } else {

        gyro_angle = gyro_angle - ((gyro-gyro_zero) + old_gyro)*.0005 + gyro_drift;
        old_gyro = gyro-gyro_zero;
        gyro_radians = (gyro_angle * (PI/180.0)*400.0*gyro4x_gain);

        // Kalman filtering
        vel1 = (enc1 - enc1old)/(193.0*0.001);  // calculate actual velocities
        vel2 = (enc2 - enc2old)/(193.0*0.001);
        if (fabsf(vel1) > 10.0) vel1 = vel1old; // check for encoder roll-over should never happen
        if (fabsf(vel2) > 10.0) vel2 = vel2old;
        enc1old = enc1; // save past values
        enc2old = enc2;
        vel1old = vel1;
        vel2old = vel2;

        // Step 0: update B, u
        B[0][0] = cosf(ROBOTps.theta)*0.001;
        B[1][0] = sinf(ROBOTps.theta)*0.001;
        B[2][1] = 0.001;
        u[0][0] = 0.5*(vel1 + vel2);    // linear velocity of robot
        u[1][0] = (gyro-gyro_zero)*(PI/180.0)*400.0*gyro4x_gain;    // angular velocity in rad/s (negative for right hand angle)

        // Step 1: predict the state and estimate covariance
        Matrix3x2_Mult(B, u, Bu);                   // Bu = B*u
        Matrix3x1_Add(x_pred, Bu, x_pred, 1.0, 1.0); // x_pred = x_pred(old) + Bu
        Matrix3x3_Add(P_pred, Q, P_pred, 1.0, 1.0); // P_pred = P_pred(old) + Q
        // Step 2: if there is a new measurement, then update the state
        if (1 == newOPTITRACKpose) {
            newOPTITRACKpose = 0;
            z[0][0] = OPTITRACKps.x;    // take in the LADAR measurement
            z[1][0] = OPTITRACKps.y;
            // fix for OptiTrack problem at 180 degrees
            if (cosf(ROBOTps.theta) < -0.99) {
                z[2][0] = ROBOTps.theta;
            }
            else {
                z[2][0] = OPTITRACKps.theta;
            }
            // Step 2a: calculate the innovation/measurement residual, ytilde
            Matrix3x1_Add(z, x_pred, ytilde, 1.0, -1.0);    // ytilde = z-x_pred
            // Step 2b: calculate innovation covariance, S
            Matrix3x3_Add(P_pred, R, S, 1.0, 1.0);                          // S = P_pred + R
            // Step 2c: calculate the optimal Kalman gain, K
            Matrix3x3_Invert(S, S_inv);
            Matrix3x3_Mult(P_pred,  S_inv, K);                              // K = P_pred*(S^-1)
            // Step 2d: update the state estimate x_pred = x_pred(old) + K*ytilde
            Matrix3x1_Mult(K, ytilde, temp_3x1);
            Matrix3x1_Add(x_pred, temp_3x1, x_pred, 1.0, 1.0);
            // Step 2e: update the covariance estimate   P_pred = (I-K)*P_pred(old)
            Matrix3x3_Add(eye3, K, temp_3x3, 1.0, -1.0);
            Matrix3x3_Mult(temp_3x3, P_pred, P_pred);
        }   // end of correction step

        // set ROBOTps to the updated and corrected Kalman values.
        ROBOTps.x = x_pred[0][0];
        ROBOTps.y = x_pred[1][0];
        ROBOTps.theta = x_pred[2][0];
        Labview_flag=1;

        // uses xy code to step through an array of positions
        //        if( xy_control(&vref, &turn, 1.0, ROBOTps.x, ROBOTps.y, robotdest[statePos].x, robotdest[statePos].y, ROBOTps.theta, 0.25, 0.5))
        //        { statePos = (statePos+1)%robotdestSize; }

        if (newLADARdata == 1) {
            newLADARdata = 0;
            for (i=0;i<228;i++) {
                LADARdistance[i] = newLADARdistance[i];
                LADARangle[i] = newLADARangle[i];
                LADARdataX[i] = newLADARdataX[i];
                LADARdataY[i] = newLADARdataY[i];
            }


            // update map here
            for (ladar_num = 0; ladar_num < 10; ladar_num++){
                for (i = 0;i < 57; i++){
                    if (((obs_loc[i].x < LADARdataX[ladar_num*20+10]+0.25)&&(obs_loc[i].x >= LADARdataX[ladar_num*20+10]-0.25)) && ((obs_loc[i].y < LADARdataY[ladar_num*20+10]+0.25)&&(obs_loc[i].y >= LADARdataY[ladar_num*20+10]-0.25))){
                        obs_loc[i].tally++;
                        if (obs_loc[i].tally > 8){

                            //if it's a new object, do astar again
                            if(obs_loc[i].found ==0){
                                obs_loc[i].found = 1;
                                //do_astar = 1;
                            }

                        }
                    }
                }
            }

            for (i = 0; i < 57; i++){
                if (obs_loc[i].found == 1){
                    if(map[obs_loc[i].r*mapColSize+obs_loc[i].c] == '0'){
                        map[obs_loc[i].r*mapColSize+obs_loc[i].c] = 'x';

                        if (obs_loc[i].orient == 'v'){
                            map[(obs_loc[i].r-1)*mapColSize+obs_loc[i].c] = 'x';
                            map[(obs_loc[i].r+1)*mapColSize+obs_loc[i].c] = 'x';
                        }
                        else{
                            map[obs_loc[i].r*mapColSize+obs_loc[i].c+1] = 'x';
                            map[obs_loc[i].r*mapColSize+obs_loc[i].c-1] = 'x';
                        }
                        do_astar =1;
                    }
                }
            }

            // post astar
            if (do_astar == 1){
                SEM_post(&SEM_astar);
                do_astar = 0;
            }
        }

        Fminladar = LADARdistance[113];
        for (i = 75;i<150;i++){
            if(LADARdistance[i]<Fminladar){
                Fminladar = LADARdistance[i];
            }
        }

        //New color vision data
        if (new_coordata == 1){
            objectx_b = blueobjectx;
            objecty_b = blueobjecty;
            numpels_b = bluenumpels;
            objectx_p = pinkobjectx;
            objecty_p = pinkobjecty;
            numpels_p = pinknumpels;
            objectx_g = greenobjectx;
            objecty_g = greenobjecty;
            numpels_g = greennumpels;
            current_color = flagcolor;
            new_coordata = 0;
        }


        color_vision_error_b = 0 - objectx_b;
        //Distance from center of robot to pink weed
        color_vision_error_p = 0 - objectx_p;

        color_distance_b = ((0.000425195*objecty_b*objecty_b*objecty_b)+(0.074152963*objecty_b*objecty_b)+(4.686246680*objecty_b)+127.692244805)/12.0;
        color_distance_p = ((0.000425195*objecty_p*objecty_p*objecty_p)+(0.074152963*objecty_p*objecty_p)+(4.686246680*objecty_p)+127.692244805)/12.0;


        //path_mode = 2;
        switch(path_mode){


        case 1:
            if (statePos == 0){
                go_direct = 1;
            }
            else if ((robotdest[statePos].y <= -1) && (robotdest[statePos-1].y <= -1)){
                go_direct = 1;
            }
            //        else if (obs_Notnear()){
            //            go_direct = 1;
            //        }


            if (go_direct == 1){
                newx = robotdest[statePos].x;
                newy = robotdest[statePos].y;
                outside = 1;
                speed_factor = 2;
                go_direct = 0;
            }
            //            else if(dont_astar == 1){
            //                //newx = x_weed_world[weednum];
            //                //newy = y_weed_world[weednum];
            //            }
            //        else if (weed_centered == 1){
            //            newx = round(x_weed_world);
            //            newy = round(y_weed_world);
            //        }
            else {
                newx = newpathCol[j] - 5;
                newy = 11 - newpathRow[j];
                outside = 0;
                speed_factor = 1;
            }
            if( xy_control(&vref, &turn, 1.0, ROBOTps.x, ROBOTps.y, newx, newy, ROBOTps.theta, 0.25, 0.5))
            { j = (j+1);
            if (j == pathLen){
                do_astar = 1;
                j = 0;
                if (statePos < 12)
                    statePos++;
                else
                    statePos = 12;


            }
            }

            // Weed Stuff
            if (blue_have == 1){
                avoid_b_timer++;
                numpels_b = 0;
                if (avoid_b_timer >= 3000){
                    blue_have = 0;
                }
            }

            if (pink_have == 1){
                avoid_p_timer++;
                numpels_p = 0;
                if (avoid_p_timer >= 3000){
                    pink_have = 0;
                }
            }

            if((color_distance_b < 3) && (numpels_b > 25)){

                if ((outside == 0) && (Fminladar > 600)){

                    if (blue_have == 0) {
                        path_mode = 2;
                        first_b_run = 1;
                        speed_factor = 1;
                        b_find_timer = 0;
                    }
                }
            }
            else if((color_distance_p < 3) && (numpels_p > 35)){
                if ((outside == 0) && (Fminladar > 600)){

                    if (pink_have == 0) {
                        path_mode = 3;
                        first_p_run = 1;
                        speed_factor = 1;
                        p_find_timer = 0;
                    }
                }
                //                if ((outside == 0) && (Fminladar > 600)){
                //                    path_mode = 3;
                //                    speed_factor = 1;
                //                }
            }

            break;


        case 2:
            b_find_timer++;
            if (b_find_timer < 2000) vref = 0;
            else vref=1;

            turn = kp_vision*color_vision_error_b;


            if (((color_distance_b < 2) && (color_distance_b > 1.6)) || (dead_reckon ==1))
            {
                if (((color_distance_b < 2) && (color_distance_b >= 1.9))){
                    float tempx = ROBOTps.x + 2.0*cos(ROBOTps.theta);
                    float tempy = ROBOTps.y + 2.0*sin(ROBOTps.theta);
                    if (first_b_run == 1){
                        blue_have = 0;
                        for (i = 0; i<blue_count; i++){
                            if ((fabs( tempx - x_blue[i]) < 1) && (fabs(tempy - y_blue[i]) < 1)) {
                                blue_have = 1;
                                do_astar = 1;
                                path_mode = 1;
                                avoid_b_timer = 0;

                                //break;
                            }
                        }
                        first_b_run = 0;
                    }
                    if (blue_have == 0){
                        x_blue[blue_count] = tempx;
                        y_blue[blue_count] = tempy;
                    }

                }

                if (blue_have == 0) dead_reckon_timer++;
                if ((dead_reckon_timer > 3000)&&(blue_have == 0))// || (dead_reckon ==1))
                {
                    vref = 0.0;
                    turn = 0.0;
                    exterminate_timer++;
                    if(exterminate_timer == 2000){
                        vref = 0.0;
                        dead_reckon = 0;

                        blue_count++;
                        exterminate_timer =0;
                        dead_reckon_timer =0;
                        do_astar = 1;
                        path_mode = 1;



                    }
                }
                else if (blue_have == 0){
                    vref = 0.6;
                    turn =0;
                    dead_reckon = 1;
                }
                else{
                    vref = 0;
                    turn = 0;
                }
            }

            break;

        case 3:

            p_find_timer++;
            if (p_find_timer < 2000) vref = 0;
            else vref=1;

            turn = kp_vision*color_vision_error_p;


            if (((color_distance_p < 2) && (color_distance_p > 1.6)) || (dead_reckon ==1))
            {
                if (((color_distance_p < 2) && (color_distance_p >= 1.9))){
                    float tempx = ROBOTps.x + 2.0*cos(ROBOTps.theta);
                    float tempy = ROBOTps.y + 2.0*sin(ROBOTps.theta);
                    if (first_p_run == 1){
                        pink_have = 0;
                        for (i = 0; i<pink_count; i++){
                            if ((fabs( tempx - x_pink[i]) < 1) && (fabs(tempy - y_pink[i]) < 1)) {
                                pink_have = 1;
                                do_astar = 1;
                                path_mode = 1;
                                avoid_p_timer = 0;

                                //break;
                            }
                        }
                        first_p_run = 0;
                    }
                    if (pink_have == 0){
                        x_pink[pink_count] = tempx;
                        y_pink[pink_count] = tempy;
                    }

                }

                if (pink_have == 0) dead_reckon_timer++;
                if ((dead_reckon_timer > 3000)&&(pink_have == 0))// || (dead_reckon ==1))
                {
                    vref = 0.0;
                    turn = 0.0;
                    exterminate_timer++;
                    if(exterminate_timer == 2000){
                        vref = 0.0;
                        dead_reckon = 0;

                        pink_count++;
                        exterminate_timer =0;
                        dead_reckon_timer =0;
                        do_astar = 1;
                        path_mode = 1;



                    }
                }
                else if (pink_have == 0){
                    vref = 0.6;
                    turn =0;
                    dead_reckon = 1;
                }
                else{
                    vref = 0;
                    turn = 0;
                }
            }

            //            vref=1;
            //            turn = kp_vision*color_vision_error_p;
            //
            //            if (((color_distance_p < 2) && (color_distance_p > 1.6)) || (dead_reckon ==1))
            //            {
            //                if (((color_distance_p < 2) && (color_distance_p >= 1.9))){
            //                    pink_have = 0;
            //                    for (i = 0; i<5; i++){
            //                        if ((fabs(ROBOTps.x + 1.9*cos(ROBOTps.theta) - x_pink[i]) < 1) && (fabs(ROBOTps.y + 1.9*sin(ROBOTps.theta) - y_pink[i]) < 1)) {
            //                            pink_have = 1;
            //                            do_astar = 1;
            //                            path_mode = 1;
            //                            break;
            //                        }
            //                    }
            //                    if (pink_have == 0){
            //                        x_pink[pink_count] = ROBOTps.x + 1.9*cos(ROBOTps.theta);
            //                        y_pink[pink_count] = ROBOTps.y + 1.9*sin(ROBOTps.theta);
            //                    }
            //
            //                }
            //                dead_reckon_timer++;
            //                if (dead_reckon_timer > 3000)// || (dead_reckon ==1))
            //                {
            //                    vref = 0.0;
            //                    turn = 0.0;
            //                    exterminate_timer++;
            //                    if(exterminate_timer == 2000){
            //                        vref = 0.0;
            //                        dead_reckon = 0;
            //                        pink_count++;
            //                        exterminate_timer =0;
            //                        dead_reckon_timer =0;
            //                        do_astar = 1;
            //                        path_mode = 1;
            //
            //                    }
            //                }
            //                else{
            //                    vref = 0.6;
            //                    turn =0;
            //                    dead_reckon = 1;
            //                }
            //            }
            break;

        default:
            vref= 0;
            turn =0;
            break;
        }

        if ((timecount%200)==0) {
            switch((int)switchstate){
            case 1:
                LCDPrintfLine(1, "LADAR %.1f %.1f",LADARdistance[84],LADARdistance[225]);
                LCDPrintfLine(2, "%.1f %.1f %.1f",LADARdistance[172],LADARdistance[54],LADARdistance[113]);
                break;
            case 2:
                LCDPrintfLine(1,"x:%.2f,y:%.2f",ROBOTps.x,ROBOTps.y);
                LCDPrintfLine(2,"t:%.1f,f:%.1f",ROBOTps.theta,previous_frame);
                break;

            case 3:
                LCDPrintfLine(1,"latency %d",RW_timer);
                LCDPrintfLine(2,"t:%.1f,f:%.1f",ROBOTps.theta,previous_frame);
                break;

            case 4:
                LCDPrintfLine(1,"D_b %.2f D_p %.2f",color_distance_b,color_distance_p);
                LCDPrintfLine(2,"B %d P %d",numpels_b, numpels_p);
                break;

            case 5:
                LCDPrintfLine(1,"xB %.1f yB %.1f",x_blue[0],y_blue[0]);
                LCDPrintfLine(2,"B %d P %d",blue_count, pink_count);
                break;

            case 6:
                LCDPrintfLine(1,"Distance_p %.2f",color_distance_p);
                LCDPrintfLine(2,"B %d T %d",numpels_b, pink_tally);
                break;
            case 8:
                LCDPrintfLine(1,"Distance_b %.2f",color_distance_b);
                LCDPrintfLine(2,"B %d P %d",numpels_b, numpels_p);
                break;

            }
        }


        if (statePos == 12) speed_factor = 0;
        SetRobotOutputs(((float)speed_factor)*vref,turn,0,0,0,0,0,0,0,0);

        timecount++;
    }
}


pose UpdateOptitrackStates(pose localROBOTps, int * flag) {

    pose localOPTITRACKps;

    // Check for frame errors / packet loss
    if (previous_frame == Optitrackdata[OPTITRACKDATASIZE-1]) {
        frame_error++;
    }
    previous_frame = Optitrackdata[OPTITRACKDATASIZE-1];

    // Set local trackableID if first receive data
    if (firstdata){
        //trackableID = (int)Optitrackdata[OPTITRACKDATASIZE-1]; // removed to add new trackableID in shared memory
        trackableID = Optitrackdata[OPTITRACKDATASIZE-2];
        firstdata = 0;
    }

    // Check if local trackableID has changed - should never happen
    if (trackableID != Optitrackdata[OPTITRACKDATASIZE-2]) {
        trackableIDerror++;
        // do some sort of reset(?)
    }

    // Save position and yaw data
    if (isnan(Optitrackdata[0]) != 1) {  // this checks if the position data being received contains NaNs
        // check if x,y,yaw all equal 0.0 (almost certainly means the robot is untracked)
        if ((Optitrackdata[0] != 0.0) && (Optitrackdata[1] != 0.0) && (Optitrackdata[2] != 0.0)) {
            // save x,y
            // adding 2.5 so everything is shifted such that optitrack's origin is the center of the arena (while keeping all coordinates positive)
            // This was the old way for Optitrack coordinates
            //localOPTITRACKps.x = Optitrackdata[0]*FEETINONEMETER; // was 2.5 for size = 5
            //localOPTITRACKps.y = -1.0*Optitrackdata[1]*FEETINONEMETER+4.0;

            // This is the new coordinates for Motive
            localOPTITRACKps.x = -1.0*Optitrackdata[0]*FEETINONEMETER;
            localOPTITRACKps.y = Optitrackdata[1]*FEETINONEMETER+4.0;

            // make this a function
            temp_theta = fmodf(localROBOTps.theta,(float)(2*PI));//(theta[trackableID]%(2*PI));
            tempOPTITRACK_theta = Optitrackdata[2];
            if (temp_theta > 0) {
                if (temp_theta < PI) {
                    if (tempOPTITRACK_theta >= 0.0) {
                        // THETA > 0, kal in QI/II, OT in QI/II
                        localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI + tempOPTITRACK_theta*2*PI/360.0;
                    } else {
                        if (temp_theta > (PI/2)) {
                            // THETA > 0, kal in QII, OT in QIII
                            localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI + PI + (PI + tempOPTITRACK_theta*2*PI/360.0);
                        } else {
                            // THETA > 0, kal in QI, OT in QIV
                            localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI + tempOPTITRACK_theta*2*PI/360.0;
                        }
                    }
                } else {
                    if (tempOPTITRACK_theta <= 0.0) {
                        // THETA > 0, kal in QIII, OT in QIII
                        localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI + PI + (PI + tempOPTITRACK_theta*2*PI/360.0);
                    } else {
                        if (temp_theta > (3*PI/2)) {
                            // THETA > 0, kal in QIV, OT in QI
                            localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI + 2*PI + tempOPTITRACK_theta*2*PI/360.0;
                        } else {
                            // THETA > 0, kal in QIII, OT in QII
                            localOPTITRACKps.theta = (floorf((localROBOTps.theta)/((float)(2.0*PI))))*2.0*PI + tempOPTITRACK_theta*2*PI/360.0;
                        }
                    }
                }
            } else {
                if (temp_theta > -PI) {
                    if (tempOPTITRACK_theta <= 0.0) {
                        // THETA < 0, kal in QIII/IV, OT in QIII/IV
                        localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI + tempOPTITRACK_theta*2*PI/360.0;
                    } else {
                        if (temp_theta < (-PI/2)) {
                            // THETA < 0, kal in QIII, OT in QII
                            localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI - PI + (-PI + tempOPTITRACK_theta*2*PI/360.0);
                        } else {
                            // THETA < 0, kal in QIV, OT in QI
                            localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI + tempOPTITRACK_theta*2*PI/360.0;
                        }
                    }
                } else {
                    if (tempOPTITRACK_theta >= 0.0) {
                        // THETA < 0, kal in QI/II, OT in QI/II
                        localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI - PI + (-PI + tempOPTITRACK_theta*2*PI/360.0);
                    } else {
                        if (temp_theta < (-3*PI/2)) {
                            // THETA < 0, kal in QI, OT in QIV
                            localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI - 2*PI + tempOPTITRACK_theta*2*PI/360.0;
                        } else {
                            // THETA < 0, kal in QII, OT in QIII
                            localOPTITRACKps.theta = ((int)((localROBOTps.theta)/(2*PI)))*2.0*PI + tempOPTITRACK_theta*2*PI/360.0;
                        }
                    }
                }
            }
            *flag = 1;
        }
    }
    return localOPTITRACKps;
}

