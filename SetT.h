#pragma once
#include<string>
#include<iostream>
#include<forward_list>
#include<unordered_map>
#include <fstream>
using namespace std;

const int DEFAULT_BUCKETS = 5;

template<class T>
class SetT
{
public:
	SetT();
	SetT(int numBucks);


	~SetT();

	void Add(T elem);												// mutator/transformer
	void Remove(T elem);											// mutator/ transformer
	bool Contains(T elem) const;									// observor 

	int Size() const { return numElems; }							// observer 


	double LoadFactor() const;
	// void ReHash(int numBucks);  // Extra credit

	void operator+(T elem);				// Add
	void operator-(T elem);				// Remove
	SetT operator+(const SetT& other);  // Union (Combination of the set )
	SetT operator*(const SetT& other);  // Intersection
	SetT operator-(const SetT& other);  // Difference
	SetT operator=(const SetT& other);

	void ResetIterator();	// Reset iterator
	bool HasNext();

	T GetNextItem();

private:
	forward_list<T>** buckets;	// An array of forward_list's 
								// (ie, each index is a forward_list pointer)
	int numBuckets;
	int getHashIndex(const T& elem);
	int numElems = 0;						// This is for every individual buckets within the list 

	// Iterator variables
	int currBucket;											// highlights the current buckets
	mutable typename forward_list<T>::iterator bucketIter;	// the iterator of the current bucket
	// Any other private functions and variables you want/need
};

template<class T>
int SetT<T>::getHashIndex(const T& key)
{
	// This is done... No touching!
	unordered_map<int, T> mapper;
	typename unordered_map<int, T>::hasher hashFunction = mapper.hash_function();
	return static_cast<int>(hashFunction(key) % numBuckets);
}

// This automatically sets the bucket in order to start chaining
template<class T>
SetT<T>::SetT()															// defult constructor
{
	buckets = new forward_list<T> * [DEFAULT_BUCKETS];					// creates the amount of buckets
	for (int i = 0; i < DEFAULT_BUCKETS; i++) {
		buckets[i] = new forward_list<T>;
	}
	numBuckets = DEFAULT_BUCKETS;										// change the current buckets
	numElems++;
	//cout << numBuckets << endl;

}

		// helps us fix the amout of buckets that are beign used 
template<class T>
SetT<T>::SetT(int numBucks)
{
	buckets = new forward_list<T> * [numBucks];					// Does the same thing as the defult constructor
																// but uses a custom number
	for (int i = 0; i < numBucks; i++)
	{
		buckets[i] = new forward_list<T>;						// Creating another list 
	}
	numBuckets = numBucks;										// The number of buckets 
	//cout << numBuckets;
}

template<class T>
SetT<T>::~SetT()											// Using a function similar to chunklist
															// and using the next bucket to overshadow the
															// the first array
{
	for (int count = 0; count < numBuckets; count++)
	{
		buckets[count] = buckets[count+ 1];
	}

}

template<class T>
void SetT<T>::Add(T elem)
{
	currBucket = getHashIndex(elem);						// get the has index and set it to currBucket
	if (Contains(elem))										// See if the element already exist within the set
	{
		// cout << "\n\n" <<  elem  << " already exist in the list " << endl;
		return;
	}
														// If it manages to go through the entire list without																	// duplicating, then it's a "new" element
	buckets[currBucket]->push_front(elem);				// finding the elem, then the element is added 
	// cout << "\n\n" << elem << " added to the set " << endl;
	return;
}

template<class T>
void SetT<T>::Remove(T elem)
{
	ResetIterator();
	currBucket = getHashIndex(elem);					//  get the index of the element					
	if (Contains(elem))									// see if the element arleady exist within the set
	{
		buckets[currBucket]->remove(elem);				// remove the element within the bucket
		numElems--;										// updata the element 
		//cout <<  "\n\n" << " removing " << elem << endl;
		return;
	}
	// cout << "\nThe element does not exist " << endl;
	return;												// If the element does not exist, then return
}

template<class T>
bool SetT<T>::Contains(T elem) const
{
														// The hash function already found the current bucket
	for (int count = 0; count < numBuckets; count++)
	{
		for (bucketIter = buckets[count]->begin(); bucketIter != buckets[count]->end(); bucketIter++)
		{
			if (*bucketIter == elem)					// If the bucketIter is equal to the element, then 					
			{											// The element exist within the list 
				return true;
			}
		}
	}
	return false;										// bucketIter* is able to go through the entire list
}														// Then the element that is being passed in does not exist


template<class T>
void SetT<T>::operator+(T elem)
{
	this->Add(elem);								// Using the 'this' operator to add values onto the list
	return;
	
}

template<class T>
void SetT<T>::operator-(T elem)
{
	this->Remove(elem);									// using the 'this' to remove the value
}


template<class T>
SetT<T> SetT<T>::operator+(const SetT& other)
{
	/*
		In this function, I pushed set_A onto the result, and then set_b.
		The Add function keeps the entire list from repeating
	*/
	SetT<T> result;

			// Push setA onto the result first, then set B ()
	auto setA = buckets;

				// ********** Printing out set A ****************** //
				// ********** pushing set A onto result first ********** //
	ofstream f;
	f.open("output.txt");

		f << "\n\n first set is: " << endl 
			<< " ----------------- " << endl;

	cout << "\n\n first set is: " << endl 
		<< " ----------------- " << endl;
	for (int count = 0; count < numBuckets; count++) {

		for (auto SET_A = buckets[count]->begin(); SET_A != buckets[count]->end(); SET_A++) {

			f << *SET_A << " ";
			cout << *SET_A << " ";
			result.Add(*SET_A);
		}
	}
			// ************ Once pushing A onto the result ************** //
			// ************ Using Add(*SET_B), it will take care of duplicate values ********** //
			// ************ Thus, giving us the Union ******************* //
	f << "\n\nsecond set: " << endl
		<< " ------------- " << endl;

	cout << "\n\nsecond set: " << endl
		<< " ------------- " << endl;
	for (int count = 0; count < other.numBuckets; count++) {

		for (auto SET_B = other.buckets[count]->begin(); SET_B != other.buckets[count]->end(); SET_B++) {

			f << *SET_B << " ";
			cout << *SET_B << " ";										
			result.Add(*SET_B);
		}
	}

				// *********** Print out the result ************************** //
	f << "\n\nThe union of both sets is: " << endl;
	cout << "\n\nThe union of both sets is: " << endl;

	for (int count = 0; count < numBuckets; count++)
	{
		for (auto res = result.buckets[count]->begin(); res != result.buckets[count]->end(); res++)
		{
			f << *res << " ";
			cout << *res << " ";
		}
	}
	f.close();
	return result;
}

template<class T>
SetT<T> SetT<T>::operator*(const SetT& other)
{
	SetT<T> result;
	ofstream f;
	f.open("output.txt", ios::app);
																							// Similar to Union, except seeing if the 
	for (int count = 0; count < numBuckets; count++)										// element within the SetA is equal to the element 
	{																						// an element is within setb
		for (auto SET_A = buckets[count]->begin(); SET_A != buckets[count]->end(); SET_A++)
		{
			for (int temp = 0; temp < numBuckets; temp++)
			{
				for (auto SET_b = other.buckets[temp]->begin(); SET_b != other.buckets[temp]->end(); SET_b++)
				{
					if (*SET_A == *SET_b)												// if one of the element exist within both set, 
					{																	// then push the element in the list 
						result.Add(*SET_A);
					}
					else
						continue;									
				}
			}
		}
	}
	// This function should return the Intersection between "this" and otherSet.
	// It should NOT change "this" or otherSet

	cout << "\n\nThe intercection of both of the sets is: " << endl;
	f << "\n\nThe intercection of both of the sets is: " << endl;
	for (int count = 0; count < numBuckets; count++)
	{
		for (auto res = result.buckets[count]->begin(); res != result.buckets[count]->end(); res++)
		{
			cout << *res << " ";
			f << *res << " ";
		}
	}
	f.close();
	return result;
}

template<class T>
SetT<T> SetT<T>::operator-(const SetT& other)
{
	SetT<T> result;
	ofstream f;

	f.open("output.txt", ios::app);
	// This function should return the Difference between "this" and otherSet.
	// It should NOT change "this" or otherSet
	cout << "\n\nFirst set is: " << endl
		<< "------------" << endl<< endl;

	f << "\n\nFirst set is: " << endl
		<< "------------" << endl << endl;
	for (int count = 0; count < numBuckets; count++) {					// We are going to push the first set, set_A onto the list
																		// and use set b to  remove all the elements that are present in
																		// set_A;
		for (auto SET_A = buckets[count]->begin(); SET_A != buckets[count]->end(); SET_A++) {
			cout << *SET_A << " ";
			f << *SET_A << " ";
			result.Add(*SET_A);
		}
	}

	cout << "\n\nSecond set is: " << endl
		<< "------------" << endl << endl;

	f << "\n\nSecond set is: " << endl
		<< "------------" << endl << endl;
	for (int count = 0; count < numBuckets; count++)							// Printing out the second list to see the difference
	{					

		for (auto SET_b = other.buckets[count]->begin(); SET_b != other.buckets[count]->end(); SET_b++) {
			cout << *SET_b << " ";
			f << *SET_b << " ";
		}
	}

			for (int temp = 0; temp < numBuckets; temp++)						// removing all the values that are present in the second 
			{																	// set, assuming it exists
				for (auto SET_b = other.buckets[temp]->begin(); SET_b != other.buckets[temp]->end(); SET_b++)
				{
						result.Remove(*SET_b);

				}
			}
			cout << "\n\nThe difference beteen the first set to the second set is: " << endl;
			f << "\n\nThe difference between the first set to the second set is " << endl;
	for (int count = 0; count < numBuckets; count++)
	{
		for (auto res = result.buckets[count]->begin(); res != result.buckets[count]->end(); res++)
		{
			cout << *res << " ";
			f << *res << " ";
		}
	}
	f.close();
	return result;
}

template<class T>
void SetT<T>::ResetIterator()
{
	// Reset the iterator for the set
	currBucket = 0;
	bucketIter = buckets[currBucket]->begin();
	return;
}

template<class T>
bool SetT<T>::HasNext()
{
	// This function should check if anything is
	// left in the current bucket iterator.

	// If nothing is left in the current bucket iterator,
	// move to a bucket that does have elements in the bucket.

	// If you can't find a bucket with elements, return false.
	while (currBucket < numBuckets) {

		if (bucketIter != buckets[currBucket]->end()) {

			return true;
		}
		else {
			currBucket++;

			bucketIter = buckets[currBucket]->begin();
			continue;

		}
	}

	return false;
}

template<class T>
T SetT<T>::GetNextItem()
{
	// Returns the next item.  You should go to each bucket and
	// iterate through each item in the bucket.  After iterating
	// through the bucket, move onto the next bucket.
	T item;

		for (bucketIter; bucketIter != buckets[currBucket]->end(); ++bucketIter)
		{
			item = *bucketIter;
			++bucketIter;
			if (bucketIter == buckets[currBucket]->end())
			{
				HasNext();
			}
			return item;
		}
}
