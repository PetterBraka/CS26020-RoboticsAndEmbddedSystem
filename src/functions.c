/*
 * File:   functions.c
 * Author: Petter Vang Brakalsvalet
 *
 * Created on 21 March 2019
 */

#include "allcode_api.h"
#include "functions.h"

/*
 * this is an array to see all the cells in the maze.
 * 0 represents a unexplored cell and 1 is an explored.
 */
int navArray[5][5] = {
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
};
/*
 * move is variable regenerating the move the robot dose.
 * Forwards = 0
 * Left = 1
 * Right = 2
 * Backwards = 3
 * 4 is a default value to prevent errors.
 */
int move = 4;

/*
 * nav is variable regenerating the orientation of the robot dose.
 * North = 0
 * West = 1
 * East = 2
 * South = 3
 * 4 is a default value to prevent errors.
 */
int nav = 0;

/*
 * x and y is the position of the robot, and
 * nestX and nestY this represent where the nest is located.
 * the default value for the nest is outside the maze so that
 * the robot will explore the maze forever if the nest is not found.
 */
int x = 1;
int y = 0;
int nestX = 10;
int nestY = 10;

/**
 This function will make the robot follow the left wall.
 */
void findWall() {
    int leftIR = FA_ReadIR(0);
    int frontIR = FA_ReadIR(2);
    int rightIR = FA_ReadIR(4);

    if ((leftIR < 20)) {
        /*
         * This will check if their is a hole in the wall left wall.
         * if there is a hole in the left wall it will go in that space.
         */
        FA_SetMotors(0, 0);
        FA_Left(80);
        while ((stopInCenter()) == 0) {
            FA_SetMotors(31, 25);
        }
        FA_BTSendString("X,Y is ", 8);
        FA_BTSendNumber(x);
        FA_BTSendString(",", 2);
        FA_BTSendNumber(y);
        FA_BTSendString("\n", 2);
        move = 1;
        changeNav();
        return;
    } else if ((frontIR < 100) && (leftIR > 200)) {
        /*
         * This will check if it's a wall to the left and and opening forward.
         * if there is a opening forwards and a wall to the left it will move forwards.
         */
        keepCenter();
        while (stopInCenter() == 0) {
            FA_SetMotors(31, 25);
            keepCenter();
        }
        FA_BTSendString("X,Y is ", 8);
        FA_BTSendNumber(x);
        FA_BTSendString(",", 2);
        FA_BTSendNumber(y);
        FA_BTSendString("\n", 2);
        move = 0;
        changeNav();
        return;
    } else if ((frontIR > 700) && (leftIR < 20)) {
        /*
         * This will check if there is a wall in front and a opening to the left.
         * if there is a wall in front and a opening to the left it will move to the left.
         */
        FA_SetMotors(0, 0);
        FA_Left(80);
        while (stopInCenter() == 0) {
            FA_SetMotors(31, 25);
        }
        FA_BTSendString("X,Y is ", 8);
        FA_BTSendNumber(x);
        FA_BTSendString(",", 2);
        FA_BTSendNumber(y);
        FA_BTSendString("\n", 2);
        move = 1;
        changeNav();
        return;
    } else if ((frontIR > 700) && (leftIR > 200) && (rightIR < 20)) {
        /*
         * This will check if there is a wall to the front and left but not to the right.
         * if there is a wall in front and to the left it will move to the right.
         */
        keepCenter();
        FA_SetMotors(0, 0);
        FA_Right(85);
        while (stopInCenter() == 0) {
            FA_SetMotors(31, 25);
        }
        FA_BTSendString("X,Y is ", 8);
        FA_BTSendNumber(x);
        FA_BTSendString(",", 2);
        FA_BTSendNumber(y);
        FA_BTSendString("\n", 2);
        move = 2;
        changeNav();
        return;
    } else if ((frontIR > 700) && (leftIR > 200) && (rightIR > 200)) {
        /*
         * This will check if the there is walls on all sides except of the back.
         * if it is surrounded it wil turn around and go backwards.
         */
        FA_SetMotors(0, 0);
        FA_Left(85);
        FA_Left(85);
        while (stopInCenter() == 0) {
            FA_SetMotors(31, 25);
        }
        FA_BTSendString("X,Y is ", 8);
        FA_BTSendNumber(x);
        FA_BTSendString(",", 2);
        FA_BTSendNumber(y);
        FA_BTSendString("\n", 2);
        move = 3;
        changeNav();
        return;
    }
}

/**
 * This will check if the robot is in a dark area or not.
 * @return 1 if it is too dark.
 */
int detectDark() {
    if (FA_ReadLight() < 700) {
        FA_BTSendString("Found dark", 11);
        FA_BTSendString("\n", 2);
        return 1;
    }
    return 0;
}

/**
 * This will check the distant to the left wall to check the possition of the robot in a cell.
 * If the left wall is more then 5 cm it is too faraway form the wall.
 * If the left wall is between 3 and 4 cm it is in the center of the cell.
 * If the left wall is lest then 3 cm it is too close to the wall.
 * @return 0 is too faraway from the wall.
 * @return 1 is in center of cell.
 * @return 2 is too close to the wall.
 */
int whereIsLeftWall() {
    int irL = FA_ReadIR(0);
    int cm3 = 1300;
    int cm4 = 800;
    int cm5 = 350;
    if ((irL < cm4 - 200) && irL > (cm5)) {
        return 0;
    } else if ((irL < cm3) && (irL > cm4 + 100)) {
        return 1;
    } else if (irL > (cm3 + 50)) {
        return 2;
    }
    return 3;
}

/**
 * This will adjust the robot according to the left wall and check
 * if the robot have space behinde the robot to do the manoeuvre.
 */
void keepCenter() {
    int back = FA_ReadIR(6);
    if (back < 100) {
        switch (whereIsLeftWall()) {
            case 0:
                FA_SetMotors(0, 0);
                FA_Right(5);
                FA_Backwards(20);
                FA_Left(10);
                FA_Forwards(20);
                FA_LCDBacklight(20);
                break;
            case 1:
                FA_LCDBacklight(100);
                break;
            case 2:
                FA_SetMotors(0, 0);
                FA_Left(5);
                FA_Backwards(20);
                FA_Right(10);
                FA_Forwards(20);
                FA_LCDBacklight(20);
                break;
        }
    } else {
        switch (whereIsLeftWall()) {
            case 0:
                FA_SetMotors(0, 0);
                FA_Left(15);
                FA_Forwards(20);
                FA_LCDBacklight(10);
                break;
            case 1:
                FA_LCDBacklight(100);
                break;
            case 2:
                FA_SetMotors(0, 0);
                FA_Right(5);
                FA_Forwards(20);
                FA_LCDBacklight(10);
                break;
        }
    }
}

/**
 * This is a function to finde a line in the floor of the maze.
 * @return 1 if the ground is dark.
 * @return 0 if the ground is light.
 */
int findLine() {
    short left = FA_ReadLine(0);
    if (left < 80) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * This will make the robot go too the center of a cell.
 * @return 1 after it found a line and gone to the center.
 * @return 0 if line is not found.
 */
int stopInCenter() {
    if (findLine() == 1) {
        FA_Forwards(100);
        FA_SetMotors(0, 0);
        FA_DelayMillis(500);
        return 1;
    } else {
        return 0;
    }
}

/**
 * This will change nav the orientation of the robot depending on the move the robot made.
 */
void changeNav() {
    if (nav == 0) {
        switch (move) {
            case 0:
                nav = 0;
                break;
            case 1:
                nav = 1;
                break;
            case 2:
                nav = 2;
                break;
            case 3:
                nav = 3;
                break;
        }
        print();
    } else if (nav == 1) {
        switch (move) {
            case 0:
                nav = 1;
                break;
            case 1:
                nav = 3;
                break;
            case 2:
                nav = 0;
                break;
            case 3:
                nav = 2;
                break;
        }
        print();
    } else if (nav == 2) {
        switch (move) {
            case 0:
                nav = 2;
                break;
            case 1:
                nav = 0;
                break;
            case 2:
                nav = 3;
                break;
            case 3:
                nav = 1;
                break;
        }
        print();
    } else if (nav == 3) {
        switch (move) {
            case 0:
                nav = 3;
                break;
            case 1:
                nav = 2;
                break;
            case 2:
                nav = 1;
                break;
            case 3:
                nav = 0;
                break;
        }
        print();
    }
}

/**
 * This will send the orientation of the robot to a bluetooth devise connected to the robot.
 */
void print() {
    switch (nav) {
        case 0:
            FA_BTSendString("north", 10);
            FA_BTSendString("\n", 2);
            break;
        case 1:
            FA_BTSendString("west", 10);
            FA_BTSendString("\n", 2);
            break;
        case 2:
            FA_BTSendString("east", 10);
            FA_BTSendString("\n", 2);
            break;
        case 3:
            FA_BTSendString("south", 10);
            FA_BTSendString("\n", 2);
            break;
    }
    moveArray();
}

/**
 * This will set the position to explored if the robot is in that position.
 * If the robot is outside of the maze it will send a massage to a bluetooth devise.
 */
void moveArray() {
    if (nav == 0) {
        //north
        y = y + 1;
        if (navArray[x][y] == 0) {
            navArray[x][y] = 1;
        }
    } else if (nav == 1) {
        //west
        x = x - 1;
        if (navArray[x][y] == 0) {
            navArray[x][y] = 1;
        }
    } else if (nav == 2) {
        //east
        x = x + 1;
        if (navArray[x][y] == 0) {
            navArray[x][y] = 1;
        }
    } else if (nav == 3) {
        //south
        y = y - 1;
        if (navArray[x][y] == 0) {
            navArray[x][y] = 1;
        }
    }
    if ((y < 0) || (y > 5) || (x < 0) || (x > 5)) {
        FA_BTSendString("I'm not in the maze\n", 21);
        FA_BTSendString("Help\n", 6);
    }
    printArray();
}

/**
 * This will send the array of the explored cells and unexplored cells to a bluetooth device connected to the robot.
 */
void printArray() {
    while (checkArray() == 0) {
        int xPrint = 0;
        FA_BTSendString("{", 2);
        FA_BTSendString("\n", 2);
        while (xPrint < 5) {
            int yPrint = 0;
            FA_BTSendString("  ", 3);
            FA_BTSendString("{", 2);
            while (yPrint < 5) {
                FA_BTSendNumber(navArray[yPrint][xPrint]);
                FA_BTSendString(", ", 3);
                yPrint++;
            }
            FA_BTSendString("}", 3);
            FA_BTSendString("  ", 3);
            FA_BTSendString("\n", 2);
            xPrint++;
        }
        FA_BTSendString("}", 2);
        FA_BTSendString("\n", 2);
        return;
    }
}

/**
 * This will check if the maze is fully explored or not.
 * @return 1 if the maze is fully explored.
 * @return 0 if the maze is not fully explored.
 */
int checkArray() {
    int xCehck = 0;
    while (xCehck < 5) {
        int yCheck = 0;
        while (yCheck < 5) {
            if (navArray[xCehck][yCheck] == 1) {
                yCheck++;
            } else {
                return 0;
            }
        }
        xCehck++;
    }
    FA_BTSendString("All cells explored\n", 20);
    return 1;
}

/**
 * This will play a note.
 */
void finishedMaze() {
    FA_PlayNote(300, 500);
    goToNest();
}

/**
 * This will send the coordinates of the nest to a bluetooth device connected to the robot.
 */
void foundNest() {
    FA_LCDBacklight(0);
    nestX = x;
    nestY = y;
    FA_BTSendString("nest is at X,Y ", 5);
    FA_BTSendNumber(nestX);
    FA_BTSendString(",", 5);
    FA_BTSendNumber(nestY);
    FA_BTSendString("\n", 2);
    findWall();
}

/**
 * This will make the robot go back to the nest and send the coordinates
 * of the robot and the nest to a bluetooth device connected.
 * When the robot gets to the nest it will stop and play an note.
 */
void goToNest() {
    while (stopInCenter() == 0) {
        FA_BTSendString("X,Y is ", 8);
        FA_BTSendNumber(x);
        FA_BTSendString(",", 2);
        FA_BTSendNumber(y);
        FA_BTSendString("\n", 2);
        FA_BTSendString("nest is at X,Y ", 5);
        FA_BTSendNumber(nestX);
        FA_BTSendString(",", 5);
        FA_BTSendNumber(nestY);
        FA_BTSendString("\n\n", 3);
        FA_ResetEncoders();
        FA_SetDriveSpeed(20);
        FA_SetMotors(20, 20);
        findWall();
        if (x == nestX && y == nestY) {
            FA_SetMotors(0, 0);
            FA_PlayNote(300, 2000);
            return;
        }
    }
}