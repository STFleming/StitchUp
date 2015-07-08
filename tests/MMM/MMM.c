#define N 100
void MMM(float A[N][N], float B[N][N], float C[N][N])
{
    int i=0, j=0, k=0;                                                                                           
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            C[i][j] = 0;
            for(k=0;k<N;k++)
            {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    return;
}

