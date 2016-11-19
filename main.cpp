#include "fib_heap.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <unordered_map>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char * argv[])
{
   /*
 	*   Stream classes for input and output files
 	*/
	fstream input;
	if (argc < 2)
	{
		cout <<"INPUT FILE NAME NOT PRESENT"<<endl;
		exit(0);
	}
	input.open (argv[1]);
	ofstream output;
	output.open("output_file.txt");

   /*
 	*   Associative container to store the hash table
 	*/ 
	std::unordered_map<std::string,  heap_node* > HashTable;

	// Temporary variables used for string manipulation and as flags
	int k = 0;
	int m = 0;
	int query = 0;
	int temp_pos = 0;
	bool first_flag = true;
	int temp = 0;
	int hashtag_value = 0;

	string line, str;
	string string_number, just_name, just_number;

   /*
 	*   Object of class fib_class
 	*/
	fib_class fheap;

	// node pointer to store the return value of 'insert' function
	heap_node * node = NULL;

	while(true)
	{
	   /*
 		*   Read input file line by line until "STOP" is encountered
 		*/
		getline(input, line);
		if (line.compare("STOP") == 0 | line.compare("stop") == 0)
			break;

	   /*
 		*   If input line is a hashtag
 		*/
		if (line[0] == '#')
		{
			// Seperate the string and frequency
			temp = line.find_first_of(' ');
			str = line.substr(1, temp-1);
			hashtag_value = stoi(line.substr(temp+1));

			/*
			 *	Insert to fib heap and update hashtable
		   	 */
			if (HashTable.count(str))
			{
				// If key already present in hashtable, increase the key
				fheap.increase_key(fheap.heap, HashTable[str], hashtag_value);
			}
			else
			{
				// New hashtag : Insert to heap and hashtable
				node = fheap.insert(fheap.heap , hashtag_value, str);
				HashTable.insert({ str, node });
			}
		}
	   /*
 		*   Input line is a query
 		*/
		else
		{
			first_flag = true;
			query = stoi(line);
			// Buffer array to store removed nodes
			string max_nodes[query];

	   	   /*
 			*   Remove maximum node and store the returned string in the buffer "query" times
 			*/
			for (k = 0; k < query; ++k)
			{
				string_number = fheap.remove_max(fheap.heap);
				max_nodes[k] = string_number;
			}
			for (k = 0; k < query; ++k)
			{
				//Seperate the hashtag and frequency from the string
				temp_pos = max_nodes[k].find_first_of(',');
				just_name = max_nodes[k].substr(0, temp_pos);
				just_number = max_nodes[k].substr(temp_pos+1);
				m = stoi(just_number);
				
				// Re-insert the node into heap and update the hashtable
				node = fheap.insert(fheap.heap , m, just_name);
				HashTable[just_name] = node;
				
				/* 
				 * first_flag is set to false after first write to the outupt.
				 * This helps in generating output in the required format
				 */
				if (first_flag)
				{
					output << just_name;
					first_flag = false;
				}
				else
				{
					//write hashtags to output file seperated by commas
					output << "," << just_name;
				}
			}
			output << endl;
		}

	//fheap.printHeap(fheap.heap); 					 This printHeap function can be used to visualize root nodes

	}
	// Free all the nodes using hashtable pointer values
	for (auto& x: HashTable) 
	{
    	//cout << x.second << endl;
    	delete (x.second);
	}

  	//cout<<fheap.count<<endl;						heap.count can be used to get the total number of nodes in the heap.
	// close the i/o files
	input.close();
	output.close();

	return 0;
}