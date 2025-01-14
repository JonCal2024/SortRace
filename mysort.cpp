#include <fstream>
#include <iostream>
#include <thread>
#include <string>
using namespace std;

/**********************************************
* NAME: 		Jonny Olswang
* DATE:			April 5, 2023
* CLASS:		CECS 325-01, MW 6:00p-7:15p
* PROGRAM 5: 	Sorting Contest (threading with threads)
*
* I certify that this program is my own original work. I did not copy any part of this program from
* any other source. I further certify that I typed each and every line of code in this program.
* ---------------------------------------------
* This program takes in the names of an input
* and output file from the command line, takes
* all of the numbers from the input file and
* sorts them in groups using threaded
* sorts, merges the whole array, and outputs
* the sorted array to stdout. 
***********************************************/

const int THREAD_PARTITIONS = 8;

/**********************************************
* PROTOTYPES - SEE BELOW MAIN()
***********************************************/
void radixSort(int* unsortedArr, int arrSize);
void countingSort(int* arr, int arrSize, int digitPosition);
int getMaxValue(int* arr, int arrSize);
void mergeAll(int arr[], int arrSize, int partitions);
void merge(int* arr1, int* arr2, int arrSize);

int main(int argc, char *argv[])
{
	if(argc == 2) //if given args are valid
	{
		
		ifstream iFile;			//input file stream object
		int readerIndex;		//will act as the psuedo size of array
		int sortMe[1000000];	//int array to store up to a million values
		
		iFile.open(argv[1]);	//initialize input file stream with given arg

		for(readerIndex = 0; !iFile.eof(); readerIndex++)	//while not at the end of file, read into array
		{
			iFile >> sortMe[readerIndex];
		}

		int subarraySize = readerIndex / THREAD_PARTITIONS;	//represents size of subarrays
		
 		//call threads: pass in name of sort, addresses where subarrays should start, and the sizes of those subarrays
 		thread t1(radixSort, &sortMe[0 * subarraySize], subarraySize);
 		thread t2(radixSort, &sortMe[1 * subarraySize], subarraySize);
 		thread t3(radixSort, &sortMe[2 * subarraySize], subarraySize);
 		thread t4(radixSort, &sortMe[3 * subarraySize], subarraySize);
 		thread t5(radixSort, &sortMe[4 * subarraySize], subarraySize);
 		thread t6(radixSort, &sortMe[5 * subarraySize], subarraySize);
 		thread t7(radixSort, &sortMe[6 * subarraySize], subarraySize);
 		thread t8(radixSort, &sortMe[7 * subarraySize], subarraySize);

		//join threads back to main program
 		t1.join();
 		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();

		mergeAll(sortMe, readerIndex, THREAD_PARTITIONS);	//merges all sorted subarrays into one sorted array in memory

		
		for(int i = 0; i < readerIndex; i++)	//output sorted array into file
		{
			printf("%d\n", sortMe[i]);			//use printf to increase output stream efficiency
		}

		iFile.close();	//close file reader
	}
	else //error message
	{
		cout << "mysort:";
		for(int i = 1; i < argc; i++)
		{
			cout << " " << argv[i];
		}
		cout << ": Please enter the name for the input file only" << endl;
	}
	return 0;
}


/****************************************************
* radixSort(int*, int)
* return: void
*
* Recieves an integer pointer that points to an
* integer array and the size of that
* array, finds the max value in that array, then calls
* countingSort for each digit that max value has. 
*****************************************************/
void radixSort(int* unsortedArr, int arrSize)
{
	int maxValue = getMaxValue(unsortedArr, arrSize);	//find max value in array

	for(int digitPosition = 1; maxValue / digitPosition > 0; digitPosition *= 10)	//loops for each individual digit max value has
	{
		countingSort(unsortedArr, arrSize, digitPosition);
	}
}

/****************************************************
* getMaxValue(int*, int)
* return: int
*
* Recieves an integer pointer that points to an
* integer array and the size of that
* array, and returns the largest integer value in that
* array.
*****************************************************/
int getMaxValue(int* arr, int arrSize)
{
	int max = arr[0];
	for(int i = 1; i < arrSize; i++)
	{
		if(arr[i] > max)
		{
			max = arr[i];
		}
	}
	return max;
}

/****************************************************
* countingSort(int*, int, int)
* return: void
*
* Recieves an integer pointer that points to an
* integer array, the size of that array, and the 
* digit position it should search through (ones, tens,
* thousands, etc). Using one array to keep track of
* instances of ints and another to copy actual values
* into, countingSort sorts the passed array by the
* digits at the digitPosition.
*****************************************************/
void countingSort(int* arr, int arrSize, int digitPosition)
{
	const int MAX_DIGIT = 10;			//non-inclusive upper bound of what value a single digit int can have
	int output[arrSize];				//temp array to shift arr elements
	int digitCount[MAX_DIGIT] = { 0 };	//array to count instances of a value in a set position
										//across all arr elements, initialize elements to 0

	//count the number of occurrences of each digit at the given position in arr elements						
	for(int i = 0; i < arrSize; i++)	
	{
		digitCount[(arr[i] / digitPosition) % 10] += 1;
	}

	//compute the running sum of digit counts to determine the correct position of each element in the output array
	for(int i = 1; i < MAX_DIGIT; i++)
	{
		digitCount[i] += digitCount[i - 1];
	}

	//place each element in its correct position in the output array using the digit counts
	for(int i = arrSize - 1; i >= 0; i--)
	{
		output[digitCount[(arr[i] / digitPosition) % 10] - 1] = arr[i];
		digitCount[(arr[i] / digitPosition) % 10] -= 1;
	}

	//copy temp array output to original array
	for(int i = 0; i < arrSize; i++)
	{
		arr[i] = output[i];
	}
}

/****************************************************
* mergeAll(int*, int, int)
* return: void
*
* Recieves an integer pointer that points to an
* integer array with sections that are independently
* sorted, the size of that array, and the
* number of partitions in the array, then calls merge 
* until subarrays are completely merged into one
* sorted array.
*****************************************************/
void mergeAll(int* arr, int arrSize, int partitions)
{
	while(partitions > 1)	//while there are more than one partitions
	{
		for(int i = 0; i < partitions; i += 2)	//go through the partitions in groups of 2 and call merge 
		{
			merge(&arr[i * (arrSize / partitions)], &arr[(i + 1) * (arrSize / partitions)], arrSize / partitions);
			//&arr[i * (arrSize / partitions)] is the address of the start of the first subarray,
			//&arr[(i + 1) * (arrSize / partitions)] is the address of the start of the second subarray,
			//and arrSize / partitions is the size of the sorted subarrays
		}
		
		partitions /= 2;	//since we merged the subarrays in pairs of 2, divide partitions by 2
	}
}

/****************************************************
* merge(int*, int*, int)
* return: void
*
* Recieves two integer pointers that point to the
* start of two sorted arrays, and the size of each
* array. The elements of the two arrays are compared
* and the smaller value is put into a temp array.
* Once both arrays have been run through, assign
* all the values of the temp array into the
* memory where the two arrays are (they are neighbors
* in memory, so the result is one big sorted array).
*****************************************************/
void merge(int* arr1, int* arr2, int arrSize)
{
	int tempArr[arrSize * 2];	//temporary int array twice the size of the passed arrays
	int arr1Index = 0;			//index for first array
	int arr2Index = 0;			//index for second array
	int tempIndex = 0;			//index for temp array

	while(arr1Index < arrSize && arr2Index < arrSize)	//while both array indeces are less than their size
	{
		if(arr1[arr1Index] <= arr2[arr2Index])	//if the next element in arr1 is less than or equal to the next element in arr2
		{
			tempArr[tempIndex++] = arr1[arr1Index++];	//assign the next element of arr1 to tempArr, increment indeces for arr1 and tempArr
		}
		else if(arr2[arr2Index] < arr1[arr1Index])	//if the next element in arr1 is greather than the next element in arr2
		{
			tempArr[tempIndex++] = arr2[arr2Index++]; //assign the next element of arr2 to tempArr, increment indeces for arr2 and tempArr
		}
	}
	
	while(arr1Index < arrSize) //if arr1Index is less than array size, assign remaining values of arr1 to tempArr
	{
		tempArr[tempIndex++] = arr1[arr1Index++];
	}

	while(arr2Index < arrSize) //if arr2Index is less than array size, assign remaining values of arr2 to tempArr
	{
		tempArr[tempIndex++] = arr2[arr2Index++];
	}

	for(int i = 0; i < tempIndex; i++)	//copy elements of tempArr into memory, starting at first address of arr1
										//arr1 and arr2 are next to each other in memory, so it will overwrite both arrays
	{
 		arr1[i] = tempArr[i];
	}
}
