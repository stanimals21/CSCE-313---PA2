#include "common.h"
#include "FIFOreqchannel.h"
#include <stdio.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){

    // int n = 100;    // default number of requests per "patient"
	// int p = 15;

    int p = -1;
    double t = -1;
    int e = -1;
    int c = -1;
    string f = "";
    srand(time_t(NULL));

    // source: https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt
    int opt;
    while((opt = getopt(argc, argv, "p:t:e:cf:")) != -1)  
    {  
        switch(opt)
        {
            case 'p':
                if(optarg){
                    if(stoi(optarg) >= 1  && stoi(optarg) <= 15){
                        p = stoi(optarg);
                    }
                }
                break;
            case 't':
                if(optarg){
                    if (stoi(optarg) <= 59.99 && stoi(optarg) >= 0 && (int)(stoi(optarg)*1000) % 4 == 0)
                    {
                        t = stod(optarg);
                    }
                }
                break;
            case 'e':
                if(optarg){
                    if(stoi(optarg) == 1 || stoi(optarg) == 2)
                    {
                        e = stoi(optarg);
                    }
                }
                break;
            case 'c':
                c = 1;
                break;
            case 'f':
                if(optarg) {
                    // check to see if file is in directory?? (fstream might handle it)
                    if(optarg)
                    {
                        f = optarg;
                    }
                }
                break;
            default:
                abort();
        }
    }

    char* args[2];
    args[0] = "./dataserver";
    args[1] = NULL;

    // child process
    if(fork()==0)
    {
        // execute ./dataserver
        execv(args[0], args);
    }

    FIFORequestChannel chan ("control", FIFORequestChannel::CLIENT_SIDE);

    // ----------------- part 1 ----------------- comparing files diff -s <path to first file> <path to second file> (works)
    if(p!= -1 && (t==-1 || e==-1))
    {
        // to calculate runtime
        struct timeval start, end;

        double time = 0;
        double ecgVal;
        double unitTime = 0.004;

        string writeData = "./received/x" + to_string(p) + ".csv";
        ofstream oFile(writeData);
        gettimeofday(&start, NULL);

        for(double i = 0; time < 59.996; i++){
        // write the time to a file
            time = unitTime*i;
            oFile << time << ",";

        // creates new data request
            datamsg* ecg1 = new datamsg(1, time, 1);
            datamsg* ecg2 = new datamsg(1, time, 2);

        // writes data request to channel chan
            chan.cwrite(ecg1, sizeof(datamsg));

        // stores data received from server in char*
            char* ecg1_response = chan.cread();
            ecgVal = *(double*) ecg1_response;
            oFile << ecgVal << ",";

        // same steps for eg2
            chan.cwrite(ecg2, sizeof(datamsg));
            char* ecg2_response = chan.cread();
            ecgVal = *(double*) ecg2_response;

            oFile << ecgVal << endl;
        }

        gettimeofday(&end, NULL);
        oFile.close();

        double elapsedTime = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)*1e-6);
        cout << "The time elapsed is: " << elapsedTime << "s" <<endl << endl;
    }

    if(p!=-1 && t !=-1 && e !=-1)
    {
        struct timeval start, end;

        datamsg* pointMsg = new datamsg(p, t, e);
        chan.cwrite(pointMsg, sizeof(datamsg));
        char* dataPoint = chan.cread();

        cout << "Data Point: " << endl;
        cout << *(double*) dataPoint<< endl;
        
        double elapsedTime = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)*1e-6);
        cout << "The time elapsed is: " << elapsedTime << "s" <<endl << endl;
    }

    // ----------------- part 2 ----------------- 
    if(f != "")
    {
        struct timeval start, end;
        string fileName = f; 
        const char* fileString = fileName.c_str();

        // prepare request array to be sent to server for FileSize
        char* requestArr = new char[sizeof(filemsg) + fileName.length() + 1];

        filemsg* msg = new filemsg(0, 0);

        memcpy(requestArr, msg, sizeof(filemsg));
        strcpy(requestArr + sizeof(filemsg), fileString);

        chan.cwrite(requestArr, sizeof(filemsg) + fileName.length() + 1);

        char* response = chan.cread();

        // Get packets of size 256MB and print to .csv file
        string writeFile = "./received/y" + f;
        ofstream oFile(writeFile);

        __int64_t size = *(__int64_t*)response;


        // find number of requests to make
        int packetSize = 256;
        int iters = size / packetSize;

        // request multiple packets
        int i;
        for(i = 0; i <= iters; i++)
        {
            gettimeofday(&start, NULL);
            if(size % packetSize !=0 && i == iters)
            {
                msg = new filemsg(i*packetSize, size % packetSize);
            }
            else
            {
                msg = new filemsg(i*packetSize, packetSize);
            }
            memcpy(requestArr, msg, sizeof(filemsg)); // replaces msg with new one
            chan.cwrite(requestArr, sizeof(filemsg) + fileName.length() + 1);
            response = chan.cread();
            
            // for mak
            if(size % packetSize !=0 && i == iters)
            {
                oFile.write(response, size % packetSize);
            }
            else
            {
                oFile.write(response, packetSize);
            }
        }
        gettimeofday(&end, NULL);
        double elapsedTime2 = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)*1e-6);
        cout << "The time elapsed is: " << elapsedTime2 << "s" <<endl << endl;
        oFile.close();
    }

    // -------------- part 3 --------------------
    if(c != -1)
    {
        MESSAGE_TYPE channelReq = NEWCHANNEL_MSG;
        chan.cwrite(&channelReq, sizeof(MESSAGE_TYPE));
        char* message_response = chan.cread();
        FIFORequestChannel chan2 (message_response, FIFORequestChannel::CLIENT_SIDE);

        // Demonstration
        cout << "Test values for new channel: " << endl;
        double ecgValDemo;

        // creates new data request
            cout << 0 << ", ";
            datamsg* ecg1 = new datamsg(1, 0, 1);
            datamsg* ecg2 = new datamsg(1, 0, 2);

        // writes data request to channel chan
            chan2.cwrite(ecg1, sizeof(datamsg));

        // stores data received from server in char*
            char* ecg1_response = chan2.cread();
            ecgValDemo = *(double*) ecg1_response;
            cout << ecgValDemo << ",";

        // same steps for eg2
            chan2.cwrite(ecg2, sizeof(datamsg));
            char* ecg2_response = chan2.cread();
            ecgValDemo = *(double*) ecg2_response;

            cout << ecgValDemo << endl;

        // Second Data point
        cout << 0.08 << ", ";
        ecg1 = new datamsg(1, 0.08, 1);
        ecg2 = new datamsg(1, 0.08, 2);
        chan2.cwrite(ecg1, sizeof(datamsg));

        ecg1_response = chan2.cread();
        ecgValDemo = *(double*) ecg1_response;
        cout << ecgValDemo << ",";

        chan2.cwrite(ecg2, sizeof(datamsg));
        ecg2_response = chan2.cread();
        ecgValDemo = *(double*) ecg2_response;

        cout << ecgValDemo << endl;
        MESSAGE_TYPE m = QUIT_MSG;
        chan2.cwrite (&m, sizeof (MESSAGE_TYPE));
    }


    //---------------- part 4 ------------------

    //CODE FOR RUNNING ./dataserver IS AT TOP OF FILE (we need to run dataserver before we run client)

    // datamsg* msg = new datamsg(1, 0, 1);
    // chan.cwrite(msg, sizeof(datamsg));
    // char* resp = chan.cread();

    // double string = *(double*) resp;
    // cout << string;

    // -------------- part 5 -------------------


    // closing the channel
    MESSAGE_TYPE m = QUIT_MSG;
    chan.cwrite (&m, sizeof (MESSAGE_TYPE));
    //end of program
    wait(NULL);
}
