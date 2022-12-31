#include<iostream>
#include<vector>
#include<cstdlib>
#include<cstring>
#include<fstream>
#include<sstream>
#include<cmath>
#include<string>



struct Record {
	long long int id;
	long long int mid;
	char* name;
	char* bio;

};


class Block{

public: 
	std::vector<Record> records; 
	int record_size;
	int record_page_size;
	Block* overflow;  

	Block(int size) {	
		record_size = size; 
		record_page_size = floor(4096 / size);
		overflow = NULL;
		records.clear();
	}



	bool findrecord(long int id, Record* rec) {
		Block* find = this;

		while (find) {
			for (int i = 0; i < find->records.size(); i++) {
				
				if (find->records[i].id == id) {
					*rec = find->records[i];
					return true;

				}
			}
			find = find->overflow;

		}

		rec = NULL;
		return false;

	}


	void SaveMoreRecord(std::vector<Record>& rec) {
		for (int i = 0; i < records.size(); i++) 
			rec.push_back(records[i]);

		records.clear();

		if (overflow) {
			overflow->SaveMoreRecord(rec);
			delete overflow;
			overflow = NULL;

		}
	}

	void examine_block(Record rec) {
		if (records.size() < record_page_size)
			records.push_back(rec);

		else {

			if (overflow == NULL) 
				overflow = new Block(record_size);

			overflow->examine_block(rec);
		}
	}
};

class HashTable{
public:
	std::vector<Block*> record_blocks; 
	int record_size;
	int num_records; 
	int num_bits; 
	int block_max_capacity; 
	
	
	HashTable(int record_size) {
		num_records = 0;
		num_bits = 1; 
		block_max_capacity = floor(4096 / record_size);
		this->record_size = record_size;
		record_blocks.push_back(new Block(record_size));
		record_blocks.push_back(new Block(record_size));

	}

	
	unsigned long Hashfunction(unsigned long id) {
		unsigned long key = (1 << num_bits);
		return (unsigned long)(id % key + key) % key;

	}
	
	
	void Insert(Record* emp) {
		unsigned long key = Hashfunction(emp->id);
		
		if (record_blocks.size() <= key) {
			key -= (1 << (num_bits - 1));

		}

		record_blocks[key]->examine_block(*emp);
		num_records++;

		while (80 <= capacity()) {
			record_blocks.push_back(new Block(record_size));
			num_bits = ceil(log2((double)record_blocks.size()));

			key = record_blocks.size() - 1 - (1 << (num_bits - 1));
			std::vector<Record> rec;
			record_blocks[key]->SaveMoreRecord(rec);
			for (int i = 0; i < rec.size(); i++) {
				record_blocks[Hashfunction(rec[i].id)]->examine_block(rec[i]);

			}
		}
	}
	

	int capacity() {
		double ratio = (double)num_records / record_blocks.size();
		int percent = (100 * ratio / block_max_capacity);
		return percent;
	}	

	void Lookup(int id) {
		Record rec;
		unsigned long key = Hashfunction(id);
		if (key >= record_blocks.size()) 
			key -= (1 << (num_bits - 1));

		if (record_blocks[key]->findrecord(id, &rec)) {
			std::cout << "ID: " << rec.id << '\n'
				<< "Name: " << rec.name << '\n'
				<< "Bio: " << rec.bio << '\n'
				<< "Manager ID: " <<rec.mid << '\n';
		}
		else 
			std::cout << "Can not found" << std::endl;

	}
	

	void SaveAllRecords(std::vector<Record> &rec){
		for (int i = 0; i < record_blocks.size();i++){
			record_blocks[i]->SaveMoreRecord(rec);
		}
	}


	int GetRecordNum(){
		return num_records;
	}

	

};


class Fileprocess {
public:
	std::fstream fin;
	std::fstream fout;
	std::string file;

	void StringToChar(std::string& str, char* c, int size) {
		for (int i = 0; i < str.length(); i++) {
			if (str[i] != '\0')
				c[i] = str[i];
			else
				break;
			if (i == size - 1)
				break;
		}
	}

	Fileprocess(std::string file_name) {
		file.assign(file_name);
	}

	void ReadFile(std::vector<Record*>& emp) {
		
		std::vector<std::string> row;
		std::string line, word, temp;

		fin.open(file.c_str(), std::ios::in);

		Record* rc;
		while (getline(fin, line)) {
			row.clear();
			std::stringstream d(line);
			while (getline(d, word, ',')) {
				row.push_back(word);
				//std::cout << word << std::endl;
			}

			rc = create_emp(atoi(row[0].c_str()), row[1], row[2], atoi(row[3].c_str()));
			emp.push_back(rc);

		}

		fin.close();
	}


	Record* create_emp(long int id, std::string name, std::string bio, long long int mid) {
		const int size = floor(500 / sizeof(char));
		Record* emp = new Record;
		emp->id = id;
		emp->mid = mid;
		emp->name = new char[size + 1];
		emp->bio = new char[size + 1];
		

		memset(emp->name, '\0', size + 1);
		memset(emp->bio, '\0', size + 1);

		StringToChar(name, emp->name, size);
		StringToChar(bio, emp->bio, size);
		return emp;
	}

	int GetSizefRecord(Record* rec) {
		return sizeof(rec) + 2 * (floor(500 / sizeof(char)));

	}


	void Save(HashTable* table){
		fout.open("EmployeeIndex", std::ios::out);
		std::vector<Record> rec;

		table->SaveAllRecords(rec);
		for (int i = 0; i < rec.size();i++){
			fout << rec[i].id << ","
			<< rec[i].name << ","
			<< rec[i].bio << ","
			<< rec[i].mid << "\n";	
		}
		fout.close();
	}

};

void SearchCommand(HashTable *table){
	unsigned long int id = -1;
	
	std::cout << "Enter the ID: ";
	std::cin >> id;
	std::cin.ignore();

	if (id >= 0){
		std::cout << "The record for id " << id << " is: " << std::endl;
		table->Lookup(id);
	}
	else
		std::cout << "Error: Invalid ID" << std::endl;
}

void CreateCommand(HashTable *table, Fileprocess *file){
	Record *rec;
	long int id = 1;
	long int mid = 2;
	std::string name;
	std::string bio ="";
	std::cout << "Enter ID: ";
	std::cin >> id;
	std::cin.ignore();

	std::cout << "Enter name: ";
	getline(std::cin, name);

	std::cout << "Enter bio: ";
	getline(std::cin, bio);

	std::cout << "Enter manager ID: ";
	std::cin >> mid;
	std::cin.ignore();

	if (id >= 0 && mid >= 0){
		rec = file->create_emp(id, name, bio, mid);
		table->Insert(rec);
	}
	else
		std::cout << "Error: Invalid ID" << std::endl;
}

int main(int argc, char* argv[])
{
	// For Testing
	std::string file_name = "Employee1.csv";
	std::vector<Record*> emp;
	std::string action = "";
	int record_size;

	Fileprocess file(file_name);
	file.ReadFile(emp);

	record_size = file.GetSizefRecord(emp[0]);

	HashTable table(record_size);

	for (int i = 0; i < emp.size(); i++)
		table.Insert(emp[i]);

	do{
		std::cout << "\n\nTotal record count is: "<< table.GetRecordNum() << std::endl;
		std::cout << "\nEnter \"C\" Insert record" << std::endl;
		std::cout << "Enter \"L\" Search record" << std::endl;
		std::cout << "Enter \"E\" Save and Exit" << std::endl;
		std::cout << "Note: you cannot overwrite old records." <<std::endl;
		std::cout << ": ";
		getline(std::cin, action);
		
		if (action.compare("L") == 0)
			SearchCommand(&table);
		else if (action.compare("C") == 0)
			CreateCommand(&table, &file);
		else
			std::cout << "Oops! unknown command." <<std::endl;

	}while (action.compare("E") != 0);
	file.Save(&table);
	return 0;
}
