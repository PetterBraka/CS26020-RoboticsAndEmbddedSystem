/*
 * File:   main.c
 * Author: pev2
 *
 * Created on 07 March 2019, 11:59
 */


#include "allcode_api.h"
#include "functions.h"

int main(void) {
    FA_RobotInit();
    int left = 20;
    int right = 20;
    FA_ResetEncoders();
    FA_SetDriveSpeed(20);
    printArray();
    FA_DelaySecs(1);
    while (1) {
        while (stopInCenter() == 0) {
            if (detectDark() == 1) {
                foundNest();
                FA_SetMotors(left, right);
            } else {
                FA_SetMotors(left, right);
                findWall();
            }
            while (checkArray() == 1){
                finishedMaze();
                return 0;
            }
        }
    }
    return 0;
}