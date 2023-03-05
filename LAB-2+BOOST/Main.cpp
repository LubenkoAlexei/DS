#include <boost/thread.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <iostream>
const int sizeOfVector = 10000;
const int timeToSleep = 7;
const int timeToSleep2 = 12;
using std::cout;
using std::cin;
using std::vector;
using std::endl;

struct forThreads {
	boost::container::small_vector<int, sizeOfVector>arr;
	double average;
	int min;
	int max;
	int indexMin;
	int indexMax;
};
void MinMax(forThreads* ft) {
	boost::container::small_vector<int, sizeOfVector> arr = ft->arr;
	int min, max, imin, imax;
	min = max = arr[0];
	imin = imax = 0;
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] > max) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(timeToSleep));
			max = arr[i];
			imax = i;
		}
		if (arr[i] <min ) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(timeToSleep));
			min = arr[i];
			imin = i;
		}
	}
	ft->indexMax = imax;
	ft->indexMin = imin;
	ft->max = max;
	ft->min = min;
	cout << "Minimum is: " << min << endl;
	cout << "Maximum is: " << max << endl;
	cout << "Thread is finished" << endl;
}

void Average(forThreads* ft) {
	boost::container::small_vector<int, sizeOfVector>arr = ft->arr;
	double sum = 0.;
	for (int i = 0; i < arr.size(); i++) {
		sum += arr[i];
		boost::this_thread::sleep(boost::posix_time::milliseconds(timeToSleep2));
	}
	ft->average = sum / arr.size();
	cout << "average is: " << ft->average << endl;
	cout << "Thread is finished" << endl;
}

int main() {
	cout << "Input size of array: ";
	int size;
	cin >> size;
	boost::container::small_vector<int, sizeOfVector> vect;
	int elem;
	cout << "Input elements of array: ";
	for (int i = 0; i < size; i++) {
		cin >> elem;
		vect.push_back(elem);
	}
	forThreads* forThread = new forThreads();
	forThread->arr = vect;

	boost::thread min_max(MinMax, forThread);
	boost::thread average(Average, forThread);


	min_max.join();
	average.join();
	vect[forThread->indexMax] = vect[forThread->indexMin] = forThread->average;
	
	cout << "New array: ";
	int i = 0;
	BOOST_FOREACH(i, vect) {
		cout << i <<" ";
	}
	delete[]forThread;
	return 0;
}
