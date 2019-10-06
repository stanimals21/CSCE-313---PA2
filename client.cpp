/*
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date  : 2/8/19
 */
#include "common.h"
#include "FIFOreqchannel.h"
#include <stdio.h>

using namespace std;


int main(int argc, char *argv[]){
    int n = 100;    // default number of requests per "patient"
	int p = 15;		// number of patients
    srand(time_t(NULL));

    FIFORequestChannel chan ("control", FIFORequestChannel::CLIENT_SIDE);

    // sending a non-sense message, you need to change this
    // char x = 55;
    // chan.cwrite (&x, sizeof (x));
    // char* buf = chan.cread ();


    // // ----------------- part 1 -----------------

    // double time = 0;
    // double ecgVal;
    // double unitTime = 0.004;

    // freopen("./received/x1.csv", "w", stdout);

    // for(double i = 0; time < 59.996; i++){
    // // write the time to a file
    //     time = unitTime*i;
    //     cout << time << ",";

    // // creates new data request
    //     datamsg* ecg1 = new datamsg(1, time, 1);
    //     datamsg* ecg2 = new datamsg(1, time, 2);

    // // writes data request to channel chan
    //     chan.cwrite(ecg1, sizeof(datamsg));

    // // stores data received from server in char*
    //     char* ecg1_response = chan.cread();
    //     ecgVal = *(double*) ecg1_response;
    //     cout << ecgVal << ",";

    // // same steps for eg2
    //     chan.cwrite(ecg2, sizeof(datamsg));
    //     char* ecg2_response = chan.cread();
    //     ecgVal = *(double*) ecg2_response;

    //     cout << ecgVal << endl;
    // }
    // fclose(stdout);

    // COMPARE FILES & DO TIME OF DAY THING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



    // ----------------- part 2 ----------------- // DO TRUNCATING AND DIFF!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // string fileName = "1.csv";
    // const char* fileString = fileName.c_str();

    // // prepare request array to be sent to server for FileSize
    // char* requestArr = new char[sizeof(filemsg) + fileName.length() + 1];

    // filemsg* msg = new filemsg(0, 0);

    // memcpy(requestArr, msg, sizeof(filemsg));
    // strcpy(requestArr + sizeof(filemsg), fileString);

    // chan.cwrite(requestArr, sizeof(filemsg) + fileName.length() + 1);

    // char* response = chan.cread();

    // // Get packets of size 256MB and print to .csv file

    // freopen("./received/y1.csv", "w", stdout); //  prompt to write to new .csv file

    // __int64_t size = *(__int64_t*)response;


    // // find number of requests to make
    // int packetSize = 255;
    // int iters = size / packetSize;

    // // request multiple packets
    // int i;
    // for(i = 0; i <= iters; i++)
    // {
    //     if(size % packetSize !=0 && i == iters)
    //     {
    //         msg = new filemsg(i*packetSize, size % packetSize);
    //     }
    //     else
    //     {
    //         msg = new filemsg(i*packetSize, packetSize);
    //     }
    //     memcpy(requestArr, msg, sizeof(filemsg)); // replaces msg with new one
    //     chan.cwrite(requestArr, sizeof(filemsg) + fileName.length() + 1);
    //     response = chan.cread();
    //     cout << response;
    // }

    // fclose(stdout);

    // -------------- part 3 --------------------

    // MESSAGE_TYPE channelReq = NEWCHANNEL_MSG;
    // chan.cwrite(&channelReq, sizeof(MESSAGE_TYPE));
    // char* message_response = chan.cread();
    // FIFORequestChannel chan2 (message_response, FIFORequestChannel::CLIENT_SIDE);


    // double ecgValDemo;

    // // creates new data request
    //     cout << 0 << ", ";
    //     datamsg* ecg1 = new datamsg(1, 0, 1);
    //     datamsg* ecg2 = new datamsg(1, 0, 2);

    // // writes data request to channel chan
    //     chan2.cwrite(ecg1, sizeof(datamsg));

    // // stores data received from server in char*
    //     char* ecg1_response = chan2.cread();
    //     ecgValDemo = *(double*) ecg1_response;
    //     cout << ecgValDemo << ",";

    // // same steps for eg2
    //     chan2.cwrite(ecg2, sizeof(datamsg));
    //     char* ecg2_response = chan2.cread();
    //     ecgValDemo = *(double*) ecg2_response;

    //     cout << ecgValDemo << endl;


    // // Second Data point

    // cout << 0.08 << ", ";
    // ecg1 = new datamsg(1, 0.08, 1);
    // ecg2 = new datamsg(1, 0.08, 2);
    // chan2.cwrite(ecg1, sizeof(datamsg));

    // ecg1_response = chan2.cread();
    // ecgValDemo = *(double*) ecg1_response;
    // cout << ecgValDemo << ",";

    // chan2.cwrite(ecg2, sizeof(datamsg));
    // ecg2_response = chan2.cread();
    // ecgValDemo = *(double*) ecg2_response;

    // cout << ecgValDemo << endl;

    //---------------- part 4 ------------------

    // // for calling ./dataserver
    // char* args[2];
    // args[0] = "./dataserver";
    // args[1] = NULL;

    // // child process
    // if(fork()==0)
    // {
    //     execv(args[0], args);
    // }



    // closing the channel    
    MESSAGE_TYPE m = QUIT_MSG;
    chan2.cwrite (&m, sizeof (MESSAGE_TYPE));
    chan.cwrite (&m, sizeof (MESSAGE_TYPE));

   
}
