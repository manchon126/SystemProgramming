#include <stdio.h>
#include <stdlib.h>


typedef enum {false, true} bool;

typedef int Data;

typedef struct _node{
    Data item;
    struct _node* next;
} Node;

typedef struct _queue{
    Node* front;
    Node* rear;
} Queue;


float mySqrt(float n);
float getAvrg(int* array, int len, int portion);
float getStdev(int* array, int len, float avrg, int portion);

int primeNumFilter(Queue* pq, int A, int B);
void printLarge10Info(int* array, int len);
void printFreq10Info(int* array, int len);

void initQ(Queue* pq);
bool isEmptyQ(Queue* pq);
void enQ(Queue* pq, Data item);
Data deQ(Queue* pq);

void quicksort_3way(int* array, int left, int right);


int main(void)
{
    Queue myQ;

    int A, B;
    int N;
    int numOfPN = 0;
    int idx;
    int previousPN;

    int* arrayOfGap;
    float avrgOfgap;

    initQ(&myQ);

    printf("Start & End : ");
    scanf("%d %d", &A, &B);
    printf("Cutting portion N(%%) : get info of higer N%% prime number : ");
    scanf("%d", &N);

    numOfPN = primeNumFilter(&myQ, A, B);

    if( numOfPN >= 1){
        previousPN = deQ(&myQ);
    }
    if( numOfPN >= 2 ){
        arrayOfGap = (int *)malloc(sizeof(int) * (numOfPN-1) );
    }
        

    idx = 0;

    while( !(myQ.front == NULL) ){
        Data temp = deQ(&myQ);
        arrayOfGap[idx] = temp-previousPN;
        previousPN = temp;
        idx++;
    }

    
    printf("\n");

    printf("# of prime number between %d & %d : %d\n", A, B, numOfPN);
    printf("Average of gap : %f\n", numOfPN<2 ? 0.0 : (avrgOfgap=getAvrg(arrayOfGap, numOfPN-1, 100)) );
    printf("Standard deviation of gap : %f\n", numOfPN<2 ? 0.0 : getStdev(arrayOfGap, numOfPN-1, avrgOfgap, 100) );
    if(N<=100 && N>0){
        printf("Average of gap of higher %d%% P.N.: %f\n", N, numOfPN<2 ? 0.0 : (avrgOfgap=getAvrg(arrayOfGap, numOfPN-1, N)) );
        printf("Standard deviation of gap of higher %d%% P.N. : %f\n", N, numOfPN<2 ? 0.0 : getStdev(arrayOfGap, numOfPN-1, avrgOfgap, N) );
    }

    
    quicksort_3way(arrayOfGap, 0, numOfPN-2);

    if( numOfPN >= 2){
        printf("Size and frequency of gap ; Top 10 in size (size : frequency)\n ");
        printLarge10Info(arrayOfGap, numOfPN-1);
    }
    if( numOfPN >= 2){
        printf("Size and frequency of gap ; Top 10 in frequency (size : frequency)\n ");
        printFreq10Info(arrayOfGap, numOfPN-1);
    }

    if(arrayOfGap != NULL){
        free(arrayOfGap);
    }

    return 0;

}



float mySqrt(float n){
    float result = n/100;

    if( n<0 )
        exit(1);

    if( n==0 )
        return 0.0;


    for(int i=0; i<12; i++){
        result = 0.5*( result + n/result );
    }

    return result;

}

float getAvrg(int* array, int len, int portion){
    int firstIdx = len*(1.0-(float)portion/100);
    int sum = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5= 0;
    int i = len-1, limit = firstIdx+4;

    for( ; i>=limit; i-=5){
        sum += array[i];
        sum2 += array[i-1];
        sum3 += array[i-2];
        sum4 += array[i-3];
        sum5 += array[i-4];
    }

    for( ; i>=firstIdx; i--){
        sum += array[i];
    }

    sum = sum + sum2 + sum3 + sum4 + sum5;
    
    return (float)sum / (len-firstIdx);
}

float getStdev(int* array, int len, float avrg, int portion){
    int firstIdx = len*(1.0-(float)portion/100);
    float sum = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0, sum5 = 0.0;
    float deviation, deviation2, deviation3, deviation4, deviation5 ;
    int i = len-1, limit = firstIdx+4;

    for( ; i>=limit; i-=5){
        deviation = array[i] - avrg;
        sum += deviation*deviation;
        
        deviation2 = array[i+1] - avrg;
        sum2 += deviation*deviation;
        
        deviation3 = array[i+2] - avrg;
        sum3 += deviation*deviation;
        
        deviation4 = array[i+3] - avrg;
        sum4 += deviation*deviation;
        
        deviation5 = array[i+4] - avrg;
        sum5 += deviation*deviation;
    }

    for( ; i>=firstIdx; i--){
        deviation = array[i] - avrg;
        sum += deviation*deviation;
    }

    sum = sum + sum2 + sum3 + sum4 + sum5;

    return mySqrt(sum / (len-firstIdx));
}


int primeNumFilter(Queue* pq, int A, int B){
    int countPN = 0;
    int i0, limit = ((int) mySqrt(B))-3;
    
    bool* isPrime = (bool *)malloc(sizeof(bool) * (B+1));

    for(int i = 2; i<=B; i++){
        isPrime[i] = true;
    }

    
    for(int j = 4; j<= B; j+=2)
        isPrime[j] = false;

    for( i0 = 3 ; i0<=limit; i0+=4){
        int i1 = i0+1;
        if (isPrime[i0] == true) {
            int i2 = i0+2;
            for(int j = i0*2; j<= B; j+=i0)
                isPrime[j] = false;
            if (isPrime[i2] == true) {
                for(int j = i2*2; j<= B; j+=i2)
                    isPrime[j] = false;
            }
        }
        else{
            if (isPrime[i1] == true) {
                int i3 = i0+3;
                for(int j = i1*2; j<= B; j+=i1)
                    isPrime[j] = false;
                if (isPrime[i3] == true){
                    for(int j = i3*2; j<= B; j+=i3)
                        isPrime[j] = false;
                }
            }
            else{
                int i2 = i0+2;
                if (isPrime[i2] == true) {
                    for(int j = i2*2; j<= B; j+=i2)
                        isPrime[j] = false;
                }
                else{
                    int i3 = i0+3;
                    if (isPrime[i3] == true){
                        for(int j = i3*2; j<= B; j+=i3)
                            isPrime[j] = false;
                    }
                }
            }
        }
    }
    
    for( ; i0<= limit+3; i0++){
        if (isPrime[i0]) {
            for(int j = i0*2; j<= B; j+=i0)
                isPrime[j] = false;
        }
    }

    
    for(int i = A; i<= B; i++){
        if ( isPrime[i] == true){
            enQ(pq, i);
            countPN++;
        }
    }

    free(isPrime);

    return countPN;
}

void printLarge10Info(int* array, int len){
    int countTen = 0;
    int idx = 0;
    int priviousGap = 0; //array의 첫째 원소는 반드시 1 이상임을 이용
    int countEach;

    while( countTen<=10 && idx<len ){
        if (array[idx] != priviousGap){
            if (countTen!=0){
                printf("%d : %d\t", priviousGap, countEach);
            }
            priviousGap = array[idx];
            countTen++;
            countEach = 1;
        }
        else{
            countEach++;
        }
        idx++;
    }

    if( countTen<10 ){
        printf("%d : %d\t", priviousGap, countEach);
    }

    printf("\n");
} 

void printFreq10Info(int* array, int len){
    typedef struct _PN{
        int size;
        int freq;
    } PN;

    int countTen = 0;
    int idx = 0;
    int priviousGap = 0; //array의 첫째 원소는 반드시 1 이상임을 이용
    int countFreq;
    int tempLen = (array[0] - array[len-1]) / 2 + 2;
    
    PN* tempArray = (PN *)malloc(sizeof(PN) * tempLen);

    for(int i=0; i<len; i++){
        if( array[i] != priviousGap ){
            if( i != 0 ) {
                tempArray[idx].size = priviousGap;
                tempArray[idx].freq = countFreq;
                idx++;
            }
            priviousGap = array[i];
            countFreq = 1;
        }
        else{
            countFreq++;
        }
    }
    tempArray[idx].size = priviousGap;
    tempArray[idx].freq = countFreq;

    while( countTen<10 && countTen<=idx){
        int targetIdx = 0;
        int mostFreq = 0;
        for(int j=0; j<=idx; j++){
            if(tempArray[j].freq > mostFreq){
                mostFreq = tempArray[j].freq;
                targetIdx = j;
            }
        }
        printf("%d : %d\t", tempArray[targetIdx].size, tempArray[targetIdx].freq);
        tempArray[targetIdx].freq = -1;
        countTen++;
    }

    printf("\n");

    free(tempArray);
}


void initQ(Queue* pq){
    pq->front = pq->rear = NULL;
}

bool isEmptyQ(Queue* pq){
    return (pq->front == NULL);
}

void enQ(Queue* pq, Data item){
    Node* newNode = (Node* )malloc(sizeof(Node));
    newNode->item = item;
    newNode->next = NULL;

    if( pq->front == NULL ){
        pq->front = pq->rear = newNode;
    }

    else{
        pq->rear->next = newNode;
        pq->rear = newNode;
    }
}

Data deQ(Queue* pq){
    Node* rNode;
    Data rData;

    if( pq->front == NULL) {
        exit(1);
    }

    rNode = pq->front;
    rData = pq->front->item;
    
    pq->front = pq->front->next;

    free(rNode);
    return rData;    
}


void quicksort_3way(int* array, int left, int right){
    if(left>=right)
        return;

    else{
        int pivot = array[right];
        int lIdx = left-1, rIdx = right;
        int idx = left;
        int temp;

        while( idx < rIdx){
            if( array[idx] > pivot ){
                lIdx++;
                temp = array[lIdx];
                array[lIdx] = array[idx];
                array[idx] = temp;
                idx++;
            }
            else if( array[idx] < pivot){
                rIdx--;
                temp = array[rIdx];
                array[rIdx] = array[idx];
                array[idx] = temp;
            }
            else
                idx++;
        }

        array[right] = array[lIdx+1];
        array[lIdx+1] = pivot;

        quicksort_3way(array, left, lIdx);
        quicksort_3way(array, rIdx, right);
    }
    
}