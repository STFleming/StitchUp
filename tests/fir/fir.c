/*
    FIR with 32 inputs, 32bit, 16 taps
    NOTE: To change INPUTSIZE and TAPS, uncomment the other FIRFilterStreaming function
    and comment out the current one. Currently, a loop has been unrolled for better performance.
*/
#define TAPS 16
int FIRFilterStreaming ( int in, int coefficients[], int previous[]){
    int temp = 0, j;    

    // UNROLLING THIS IMPROVES PERFROMANCE
     previous[15] = previous[14];
     previous[14] = previous[13];
     previous[13] = previous[12];
     previous[12] = previous[11];
     previous[11] = previous[10];
     previous[10] = previous[9];
     previous[9] = previous[8];
     previous[8] = previous[7];
     previous[7] = previous[6];
     previous[6] = previous[5];
     previous[5] = previous[4];
     previous[4] = previous[3];
     previous[3] = previous[2];
     previous[2] = previous[1];
     previous[1] = previous[0];
    previous[0] = in;

    if (previous[TAPS-1] == 0)
        return 0;
    else {
        for (j = 0; j < TAPS; j++){
            temp += previous[TAPS - j - 1]*coefficients[j];
        }

        return temp;
    }

}
