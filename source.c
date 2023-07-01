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

void mergeSort(int* array, int left, int right);

int main(void)
{
    Queue myQ;

    int A, B;
    int N;
    int numOfPN = 0;
    int idx;

    int* arrayOfGap = NULL;
    float avrgOfgap;

    initQ(&myQ);

    printf("Start & End : ");
    scanf("%d %d", &A, &B);
    printf("Cutting portion N(%%) : get info of higer N%% prime number : ");
    scanf("%d", &N);

    numOfPN = primeNumFilter(&myQ, A, B);

    if( numOfPN >= 1){
        deQ(&myQ);
    }
    if( numOfPN >= 2 ){
        arrayOfGap = (int *)malloc(sizeof(int) * (numOfPN-1) );
    }
        

    idx = 0;

    while( !isEmptyQ(&myQ) ){
        Data temp = deQ(&myQ);
        arrayOfGap[idx] = temp;
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

    
    mergeSort(arrayOfGap, 0, numOfPN-2);

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
    int sum = 0;

    for(int i=(len-1); i>=firstIdx; i--){
        sum += array[i];
    }
    
    return (float)sum / (len-firstIdx);
}

float getStdev(int* array, int len, float avrg, int portion){
    int firstIdx = len*(1.0-(float)portion/100);
    float sum = 0.0;
    float deviation = 0.0 ;

    for(int i=(len-1); i>=firstIdx; i--){
        deviation = array[i] - avrg;
        sum += deviation*deviation;
    }

    return mySqrt(sum / (len-firstIdx));
}


int primeNumFilter(Queue* pq, int A, int B){
    int countPN = 0;
    int previousPN = 0;
    
    for(int i = A; i<=B; i++){
        bool isPrime = true;
        for(int j = 2; j<= mySqrt(i); j++){
            if(i%j == 0){
                isPrime = false;
                break;
            }
        }
        if(isPrime){
            enQ(pq, i-previousPN);
            previousPN = i;
            countPN++;
        }
    }

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

    if(isEmptyQ(pq)){
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

    if(isEmptyQ(pq)){
        exit(1);
    }

    rNode = pq->front;
    rData = pq->front->item;
    
    pq->front = pq->front->next;

    free(rNode);
    return rData;    
}


void mergeSort(int* array, int left, int right){
    int mid;
    int* leftArray, * rightArray;
    int idx, lIdx, rIdx;
  
    if( left >= right){
        return;
    }
  
    mid = (left + right) / 2;

    mergeSort(array, left, mid);
    mergeSort(array, mid+1, right);



    leftArray = (int *)malloc(sizeof(int) * (mid-left+1));
    rightArray = (int *)malloc(sizeof(int) * (right-mid));


    for(int i = left; i <= mid; i++)
        leftArray[(i-left)] = array[i];

    for(int i = mid+1; i <= right; i++)
        rightArray[(i-(mid+1))] = array[i];
    

    idx = left, lIdx = 0, rIdx = 0;

    while( (lIdx <= (mid-left) ) && (rIdx <= (right-(mid+1)) ) ){
        if( leftArray[lIdx] > rightArray[rIdx] ){
            array[idx] = leftArray[lIdx];
            lIdx++;
        }
        else{
            array[idx] = rightArray[rIdx];
            rIdx++;
        }
        idx++;
    }

    while( lIdx <= (mid-left) ){
        array[idx] = leftArray[lIdx];
        idx++;
        lIdx++;
    }

    while( rIdx <= (right-(mid+1)) ){
        array[idx] = rightArray[rIdx];
        idx++;
        rIdx++;
    }

    free(leftArray);
    free(rightArray);
}