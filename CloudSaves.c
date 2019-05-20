/*
##### CloudSaves #####
### Author: Varauk ###
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <Windows.h> //provides GetPrivateProfileString/Int

//will be loaded from ini file
char pathGameSave[MAX_PATH];
char pathCloudSave[MAX_PATH];
char pathToExecutable[MAX_PATH];

int printPaths;
int checkInet;
int copySubfoldersToo;
char hostaddress[60];
char iniName[30] = "CloudSaves.ini"; //EDIT THIS, for example "Game.ini" to keep better track which game this ini file is configured for

//commands get created when used
char localUpdateCmd[255];
char cloudUpdateCmd[255];
char internetCheckCmd[80];
char launchGameCmd[255];

//looong string to create an example ini file
char defaultIniCreator[460] = "[config]\n;0 = true, 1 = false\ninternetcheck=0\nprintPaths=1\ncopySubfoldersToo=1\ninternetaddress=www.google.com\n;You could enter your Cloud Services address here for example\n\n[paths]\n;Example: C:\\Users\\Name\\Documents\\\"My Games\"\\GameName -> note that it will copy ALL files in that directory, so only select the actual location of the saves\npathGameSave=\npathCloudSave=\npathToExecutable=\n;Example: C:\\Games\\\"Game Name\"\\game.exe -> can also be a .lnk to that .exe";


char currentDir[MAX_PATH];
char pathToIni[255];



int main(void) {

    //get the full path to the .ini file (necessary for GetPrivateProfileString)
    getcwd(currentDir, sizeof(currentDir));
    strcat(pathToIni, currentDir);
    strcat(pathToIni, "\\");
    strcat(pathToIni, iniName);


    //ini parsing, checks if it exists, creates it if not, otherwise loads the data
    FILE *ini_pointer;
    if (ini_pointer = fopen(pathToIni, "r")) {
        printf("[LOG] configuration file found, reading data...\n");
        checkInet = GetPrivateProfileInt("config", "internetcheck", 0, pathToIni);
        printPaths = GetPrivateProfileInt("config", "printPaths", 1, pathToIni);
        copySubfoldersToo = GetPrivateProfileInt("config", "copySubfoldersToo", 1, pathToIni);
        GetPrivateProfileString("config", "internetaddress", 0, hostaddress, 100, pathToIni);
        GetPrivateProfileString("paths", "pathGameSave", 0, pathGameSave, MAX_PATH, pathToIni);
        GetPrivateProfileString("paths", "pathCloudSave", 0, pathCloudSave, MAX_PATH, pathToIni);
        GetPrivateProfileString("paths", "pathToExecutable", 0, pathToExecutable, MAX_PATH, pathToIni);

        //empty paths
        if (strlen(pathGameSave) == 0 || strlen(pathCloudSave) == 0 || strlen(pathToExecutable) == 0) {
            printf("[ERROR] At least one of the paths is empty. Edit it first.\n");
            fclose(ini_pointer);
            system("pause");
            return 1;
        }
        fclose(ini_pointer);


        if (printPaths == 0) {
            printf("[LOG] Paths:\nLocal Saves: %s\nCloud Saves: %s\nExecutable: %s\n", pathGameSave, pathCloudSave, pathToExecutable);
            system("timeout 3");
        }

    } else { //no ini file
        printf("[ERROR] Couldn't find the .ini configuration file, first time running this app? Creating one now...\n\n###INI FILE###\n");
        fclose(ini_pointer); //close read pointer
        ini_pointer = fopen(pathToIni, "w");
        printf("%s\n##############\n\n[LOG] App will now exit, edit the paths first.\n", defaultIniCreator);
        fprintf(ini_pointer, "%s", defaultIniCreator); //writes .ini file
        fclose(ini_pointer);
        system("pause");
        return 0;
    }



    //check internet connection to specified host?
    if (checkInet == 0) {
        printf("[LOG] Checking internet status...\n");
        sprintf(internetCheckCmd, "ping -n 2 %s > nul", hostaddress); //create the command

        if (system(internetCheckCmd)) {
            printf("[ERROR] %s seems to be unavailable, are you online? Proceeding may result in loss of save progress!\n", hostaddress);
            system("timeout 99");
        } else {
            printf("[LOG] %s responded, connection looks fine...\n", hostaddress);
            system("timeout 5");
        }
    }

    //finally done with the setup, lets update stuff

    //update local saves (from cloud location)
    printf("[LOG] Updating local saves...\n");
    if (copySubfoldersToo == 0) { //with subfolders
        sprintf(localUpdateCmd, "xcopy %s %s /Y /E", pathCloudSave, pathGameSave); //create the command
    } else {
        sprintf(localUpdateCmd, "xcopy %s %s /Y", pathCloudSave, pathGameSave); //create the command
    }
    system(localUpdateCmd);
    system("timeout 4");

    //launch game
    printf("[LOG] Launching %s...\n\nWaiting for termination...\n\n", pathToExecutable);
    sprintf(launchGameCmd, "%s /WAIT", pathToExecutable); //create the command
    system(launchGameCmd);
    system("timeout 1"); //allow the game to save or whatever, can be removed (program should wait long enough by default, since it only continues when the .exe terminates)

    //update cloud saves (from local location)
    printf("[LOG] Updating cloud saves...\n");
    if (copySubfoldersToo == 0) { //with subfolders
        sprintf(cloudUpdateCmd, "xcopy %s %s /Y /E", pathGameSave, pathCloudSave); //create the command
    } else {
        sprintf(cloudUpdateCmd, "xcopy %s %s /Y", pathGameSave, pathCloudSave); //create the command
    }
    system(cloudUpdateCmd);
    system("timeout 6");
}
