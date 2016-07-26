


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef vector <string> record_t;
typedef vector <record_t> data_t;
typedef vector  <long> time_gap;
typedef vector  <long> volume_gap;
typedef vector  <long> price_gap;
// Here is the data we produce.
data_t outData;

int max_gap(long a, long b) {
	return (a<b) ? b : a;     // or: return comp(a,b)?b:a; for version 
}

int symbole_gap(string record) {

	int k = -1;

	for (size_t i = 0; i < outData.size(); i++) {

		if (record == outData[i][0]) {
			k = i;
			break;
		}


	}

	return k;     // or: return k 
}

void quantity_gap(int k, record_t& record) {

	int MaxPrice = max_gap(atoi(outData[k][4].c_str()), atoi(record[3].c_str()));
	int Volume = atoi(outData[k][2].c_str()) + atoi(record[2].c_str());

	outData[k][2] = to_string(Volume);
	outData[k][4] = to_string(MaxPrice);
}

void output_gap(record_t& record) {
    
    
int match = symbole_gap(record[1]);
	record_t tradeRec;
	tradeRec = { record[1], to_string(0), record[2], record[3], record[3] };
	if (outData.size() == 0) {
		outData.push_back(tradeRec);

	}
	else
	{
		//
	
		if (match == -1) {
		    
			outData.push_back(tradeRec);

		}
		else 
		{	

			quantity_gap(match, record);


		}
	}

}

void weightedTime_gap(int i, data_t& inData) {

	time_gap time_g;
	volume_gap volume_g;
	price_gap price_g;
	int WeightedAveragePrice = 0;
	long MaxTime = 0, t = 0, sum_price = 0, sum_volume = 0;

	for (size_t j = 0; j < inData.size(); j++)
	{

		if (inData[j][1] == outData[i][0]) {

			time_g.push_back(atol(inData[j][0].c_str()));
			volume_g.push_back(atol(inData[j][2].c_str()));;
			price_g.push_back(atol(inData[j][3].c_str()));;


		}


	}


	for (size_t j = 1; j < time_g.size(); j++)
	{

		sum_volume += volume_g[j];
		sum_price += volume_g[j] * price_g[j];
		MaxTime = max_gap(MaxTime, time_g[j] - time_g[j - 1]);
		t=time_g[j] - time_g[j - 1];

	}


	outData[i][1] = to_string(MaxTime);
	if(sum_volume != 0)  outData[i][3] = sum_price / sum_volume;
	MaxTime = sum_volume = sum_price = 0;
	volume_g.clear();
	price_g.clear();
	time_g.clear();




}


istream& operator >> (istream& ins, record_t& record)
{
	// make sure that the returned record contains only the stuff we read now
	record.clear();


	string line;
	getline(ins, line);

	// now we'll use a stringstream to separate the fields out of the line
	stringstream ss(line);
	string field;
	while (getline(ss, field, ','))
	{

		stringstream fs(field);
		string f = "";  // (default value is "")
		fs >> f;

		// add the newly-converted field to the end of the record
		record.push_back(f);
	}

	return ins;
}


istream& operator >> (istream& ins, data_t& inData)
{
	// make sure that the returned data only contains the CSV data we read here
	inData.clear();
	outData.clear();
	// For every record we can read from the file, append it to our resulting data
	record_t record;

	while (ins >> record)
	{
		inData.push_back(record);
		//
		output_gap(record);



	}

	// Again, return the argument stream as required for this kind of input stream overload.
	return ins;
}

void MaxTimeGap_AveragePrice(data_t& inData)
{


	for (size_t i = 0; i < outData.size(); i++)
	{

		weightedTime_gap(i, inData);

	}




}


void  sortedSymbol_asc()
{

	for (size_t j = 0; j < outData.size(); j++) {
		for (size_t i = 0; i < outData.size() - j - 1; i++)
		{

			if (outData[i][0] > outData[i + 1][0])
			{
				record_t temp = outData[i];             // swap elements
				outData[i] = outData[i + 1];
				outData[i + 1] = temp;
			}

		}
	}

}



//-----------------------------------------------------------------------------
// Now to put it all to use.
int main()
{
	// Here is the data we want.
	data_t inData;


	// Here is the file containing the data. Read it into data.
	ifstream infile("input.csv");
infile >> inData;

	// Complain if something went wrong.
	if (!infile.eof())
	{
		cout << "ooey!\n";
		return 1;
	}

	infile.close();

	// Otherwise, list some basic information about the file.
	cout << "Your CSV file contains " << inData.size() << " records.\n";

	unsigned max_record_size = 0;
	for (unsigned n = 0; n < inData.size(); n++)
		if (max_record_size < inData[n].size())
			max_record_size = inData[n].size();
	cout << "The record has " << max_record_size << " fields.\n";




	MaxTimeGap_AveragePrice(inData);
	sortedSymbol_asc();
	// Here is the file containing the data. Read it into data.
	ofstream outfile("output.csv");

	for (size_t i = 0; i < outData.size(); i++)
	{
		// print first column's element
		outfile << outData[i][0];

		// print remaining columns
		for (size_t j = 1; j < outData[i].size(); j++)
		{
			outfile << ", " << outData[i][j];
		}

		// print newline between rows
		outfile << endl;
	}
	outfile.close();

	cout << "Good bye!\n";
	return 0;
}