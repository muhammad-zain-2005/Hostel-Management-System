#include<iostream>
#include<fstream>
using namespace std;
// const variables
const int t_floor = 2;
const int rooms_per_floor = 3;
// Functons
void room_data(int**, int**, int**, int**);
void create_room(int**, int**, int**, int**);
void admit_resident(int**, int**, int**, int**, int*);
void display_hostel_status(int**, int**, int**, int**);
void search_by_id(int*);
void update_resident_by_id(int*);
void change_room(int** room_num, int** type, int** capacity, int** occupancy, int* trgt);
void checkout_resident(int**, int**, int**, int**, int*, int*);
void reports(int**, int**, int**, int**);
int main() {
	// dynamic memory for rows
	int** room_num = new int* [t_floor];
	int** type = new int* [t_floor];
	int** occupancy = new int* [t_floor];
	int** capacity = new int* [t_floor];

	// dynamic memory for columns in each row
	for (int i = 0; i < t_floor; i++) {
		room_num[i] = new int[rooms_per_floor];
		type[i] = new int[rooms_per_floor];
		occupancy[i] = new int[rooms_per_floor];
		capacity[i] = new int[rooms_per_floor];
	}
	int total_residents = 0;
	int choice = -1;

	ifstream check("room.txt");

	if (check)
	{
		room_data(room_num, type, capacity, occupancy);
	}

	check.close();

	ifstream fin("residents.txt");
	int id, sem, room, fee;
	char name[30], dep[30], mobile[20];
	while (fin >> id >> name >> dep >> sem >> mobile >> room >> fee)
	{
		total_residents++;
	}
	fin.close();

	while (choice != 0) {
		cout << endl;
		// Menu Driven
		cout << "=================================================" << endl;
		cout << "           HOSTEL MANAGEMENT SYSTEM              " << endl;
		cout << "=================================================" << endl;
		cout << " Current Registered Residents: " << total_residents << endl;
		cout << "-------------------------------------------------" << endl;
		cout << " 1. Initialize / Reset Room Data" << endl;
		cout << " 2. Admit New Resident" << endl;
		cout << " 3. Display Complete Hostel Status" << endl;
		cout << " 4. Update Resident Record " << endl;
		cout << " 5. Transfer Resident to Another Room" << endl;
		cout << " 6. Checkout Resident (Remove Record)" << endl;
		cout << " 7. Search Residents / Listings" << endl;
		cout << " 8. Generate Reports / Summaries" << endl;
		cout << " 0. Exit System" << endl;
		cout << "-------------------------------------------------" << endl;
		cout << "Enter your choice : ";
		cin >> choice;
		switch (choice) {
		case 1:
			cout << endl;
			cout << "[Initializing Rooms...]" << endl;
			char confirm;
			cout << "WARNING: This will delete all resident records. Continue (Y/N)? ";
			cin >> confirm;

			if (confirm == 'Y' || confirm == 'y')
			{
				create_room(room_num, type, capacity, occupancy);
				total_residents = 0;
				cout << "Success: Rooms initialized and saved to room.txt!" << endl;
			}
			else
			{
				cout << "Operation cancelled." << endl;
			}
			break;
		case 2:
		{
			cout << endl;
			cout << "[Loading Admission Portal...]" << endl;
			admit_resident(room_num, type, capacity, occupancy, &total_residents);
		}
		break;
		case 3:
		{
			cout << endl;
			cout << "[Gathering Hostel Data...]" << endl;
			display_hostel_status(room_num, type, capacity, occupancy);
		}
		break;
		case 4: {
			int id;
			cout << "Enter The ID To Be Searched : ";
			cin >> id;
			update_resident_by_id(&id);
			break;
		}
		case 5: {
			int id;
			cout << "Enter Resident ID to Transfer: ";
			cin >> id;
			change_room(room_num, type, capacity, occupancy, &id);
			break;
		}
		case 6: {
			int id;
			cout << "Enter Resident ID to Checkout: ";
			cin >> id;
			checkout_resident(room_num, type, capacity, occupancy, &total_residents, &id);
			break;
		}
		case 7: {
			int target;
			cout << "Enter The ID To Be Searched : ";
			cin >> target;
			cout << endl << endl;
			search_by_id(&target);
			break;
		}
		case 8: {
			cout << endl;
			cout << "[Computing Reports from Stored Data...]" << endl;
			reports(room_num, type, capacity, occupancy);
			break;
		}
		case 0:
			cout << endl;
			cout << "Saving data and shutting down. Goodbye!" << endl;
			break;
		default:
			cout << endl;
			cout << "Error: Invalid choice!!!" << endl;
		}

	}
	// deallocating memory from heap to prevent memory leaks
	for (int i = 0; i < t_floor; i++) {
		delete[] room_num[i];
		delete[] type[i];
		delete[] occupancy[i];
		delete[] capacity[i];
	}
	delete[] room_num;
	delete[] type;
	delete[] occupancy;
	delete[] capacity;
	return 0;
}
void room_data(int** room_num, int** type, int** capacity, int** occupancy)
{
	ifstream fin("room.txt");
	if (!fin)
	{
		cout << "room.txt not found";
		return;
	}
	for (int i = 0; i < t_floor; i++)
	{
		for (int j = 0; j < rooms_per_floor; j++)
		{
			fin >> room_num[i][j] >> type[i][j] >> capacity[i][j] >> occupancy[i][j];
		}
	}
	fin.close();
}
void create_room(int** room_num, int** type, int** capacity, int** occupancy) {
	ofstream fout("room.txt");
	for (int i = 0; i < t_floor; i++) {
		int floor = i + 1;
		for (int j = 0; j < rooms_per_floor; j++) {
			int room_position = j + 1;
			room_num[i][j] = (floor * 100) + room_position;
			if (floor == 1) {
				type[i][j] = 1;
			}
			else if (floor == 2) {
				type[i][j] = 2;
			}
			capacity[i][j] = type[i][j];
			occupancy[i][j] = 0;
			fout << room_num[i][j] << " " << type[i][j] << " " << capacity[i][j] << " " << occupancy[i][j] << endl;
		}
	}
	fout.close();
	ofstream new_fout("residents.txt");
	new_fout.close();
}

void admit_resident(int** room_num, int** type, int** capacity, int** occupancy, int* total_rsdnt)
{
	ofstream fout("residents.txt", ios::app);
	ifstream fin;
	int id;
	int req_room = 0;
	char name[30];
	char dep[30];
	int sem = 0;
	char mobile[20];
	bool room_found;
	int fee = 0;

	ifstream check("room.txt");

	if (!check)
	{
		cout << "Please initialize rooms first!" << endl;
		return;
	}

	cout << "====================================================" << endl;
	cout << "             HOSTELITES ADMISSION PORTAL               " << endl;
	cout << "====================================================" << endl;
	cout << "Welcome! Please enter the hostelite's details below." << endl;
	cout << "The system will automatically verify room availability" << endl;
	cout << "and ID uniqueness before confirming the admission." << endl;
	cout << "----------------------------------------------------" << endl << endl;

	bool valid_id = false;
	while (!valid_id) {
		while (true)
		{
			cout << "Enter Resident ID (4 digits): ";
			cin >> id;

			if (id >= 1000 && id <= 9999)
			{
				break;
			}

			cout << "ID must be exactly 4 digits!" << endl;
		}
		bool duplicate = false;
		fin.open("residents.txt");
		int id_check, sem_check, room_check, fee_check;
		char name_check[30], dep_check[30], mob_check[20];
		while (fin >> id_check >> name_check >> dep_check >> sem_check >> mob_check >> room_check >> fee_check) {
			if (id == id_check) {
				duplicate = true;
				break;
			}
		}
		fin.close();
		if (duplicate) {
			cout << endl;
			cout << "!!! Resident With ID " << id << " Already Exists !!!" << endl;
		}
		else {
			valid_id = true;
		}
	}

	cout << "Enter Resident's Name (No Spaces) : ";
	cin.ignore();
	cin >> name;
	cout << "Enter Department Name (No Spaces) : ";
	cin >> dep;
	while (sem < 1 || sem > 8)
	{
		cout << "Enter semester (1-8): ";
		cin >> sem;
	}
	bool valid = false;
	while (!valid)
	{
		int count = 0;
		cout << "Enter Your 11 digits Mobile # :";
		cin >> mobile;
		while (mobile[count] != '\0')
		{
			count++;
		}
		if (count == 11)
		{
			valid = true;
			for (int i = 0; i < 11; i++)
			{
				if (mobile[i] < '0' || mobile[i]>'9')
				{
					valid = false;
					break;
				}
			}
		}
		if (!valid)
		{
			cout << "Error : Mobile No. Must Be of 11 Digits" << endl;
			cout << "Note : Mobile No. Should Only Contain Digits" << endl;
		}
	}
	cout << endl;

	bool valid_room = false;
	while (!valid_room)
	{
		room_found = false;
		cout << "---Rooms List---" << endl;
		for (int i = 1; i <= t_floor; i++)
		{
			for (int j = 1; j <= rooms_per_floor; j++)
			{
				cout << i << "0" << j << endl;
			}
		}
		cout << "Enter Requested Room # : " << endl;
		cin >> req_room;
		for (int i = 0; i < t_floor; i++)
		{
			for (int j = 0; j < rooms_per_floor; j++)
			{
				if (room_num[i][j] == req_room)
				{
					room_found = true;
					if (occupancy[i][j] < capacity[i][j])
					{
						occupancy[i][j]++;
						cout << "Success: Resident allocated to room!" << endl;
						valid_room = true;
					}
					else {
						cout << "Room Is At Maximum Capacity!! :(" << endl;
					}
					break;
				}
			}
			if (room_found)
				break;
		}
		if (!room_found)
			cout << "Error : Room Does not Exist!!" << endl;
	}

	bool status = false;
	while (!status)
	{
		cout << "Enter Fee Status (0 or 1) : ";
		cin >> fee;
		if (fee >= 0 && fee <= 1)
		{
			status = true;
		}
		if (!status)
		{
			cout << "Fee Status Can Only Be 0 or 1" << endl;
		}
	}

	fout << id << " " << name << " " << dep << " " << sem << " " << mobile << " " << req_room << " " << fee << endl;
	fout.close();
	(*total_rsdnt)++;

	cout << endl;
	cout << "Resident successfully added to the system!" << endl;

	ofstream f_room("room.txt");
	for (int i = 0; i < t_floor; i++) {
		for (int j = 0; j < rooms_per_floor; j++) {
			f_room << room_num[i][j] << " " << type[i][j] << " " << capacity[i][j] << " " << occupancy[i][j] << endl;
		}
	}
	f_room.close();
}

void display_hostel_status(int** room_num, int** type, int** capacity, int** occupancy) {

	cout << "====================================================" << endl;
	cout << "              CURRENT HOSTEL STATUS                  " << endl;
	cout << "====================================================" << endl << endl;
	cout << "==================== ROOM LIST ====================" << endl;
	for (int i = 0; i < t_floor; i++) {
		for (int j = 0; j < rooms_per_floor; j++) {
			int available = capacity[i][j] - occupancy[i][j];
			cout << "Room :" << room_num[i][j] << "    |   ";
			if (type[i][j] == 1) {
				cout << "Type : single    |   ";
			}
			else {
				cout << "Type : double    |   ";
			}
			cout << "Capacity: " << capacity[i][j] << " | ";
			cout << "Occupancy: " << occupancy[i][j] << " | ";
			cout << "Availablity: " << available << " | ";
			if (occupancy[i][j] == capacity[i][j])
				cout << "Status: Full";
			else
				cout << "Status: Available";
			cout << endl;

		}
	}
	cout << endl;
	cout << "============== RESIDENT LIST ==============" << endl;

	ifstream fin("residents.txt");
	if (!fin)
	{
		cout << "Error opening residents.txt" << endl;
		return;
	}

	int id, sem, room, fee;
	char name[30], dep[30], mobile[20];

	while (fin >> id >> name >> dep >> sem >> mobile >> room >> fee)
	{
		cout << "ID: " << id
			<< "  Name: " << name
			<< "  Room: " << room << endl;
	}

	fin.close();
}

void update_resident_by_id(int* target)
{
	ifstream fin("residents.txt");
	if (!fin)
	{
		cout << "Error opening residents.txt" << endl;
		return;
	}
	ofstream temp("temp.txt");

	int id;
	char name[30];
	char dep[30];
	int sem;
	char mobile[20];
	int room;
	int fee;
	bool found = false;
	while (fin >> id >> name >> dep >> sem >> mobile >> room >> fee)
	{
		if (id == *target)
		{
			found = true;
			char new_choice;
			do
			{
				int choice = 0;
				cout << "\nResident Found" << endl;
				cout << "1. Update Id" << endl;
				cout << "2. Update Name" << endl;
				cout << "3. Update Department" << endl;
				cout << "4. Update Semester" << endl;
				cout << "5. Update Mobile Number" << endl;
				cout << "6. Update Fee Status" << endl;
				cout << "Enter choice: ";
				cin >> choice;
				bool valid = false;
				switch (choice)
				{
				case 1:
				{
					int new_id;
					bool duplicate;
					do
					{
						duplicate = false;
						cout << "Enter New 4-Digit ID: ";
						cin >> new_id;
						if (new_id < 1000 || new_id > 9999)
						{
							cout << "ID must be exactly 4 digits!" << endl;
							continue;
						}
						ifstream fin_check("residents.txt");
						int id2, sem2, room2, fee2;
						char name2[30], dep2[30], mobile2[20];
						while (fin_check >> id2 >> name2 >> dep2 >> sem2 >> mobile2 >> room2 >> fee2)
						{
							if (id2 == new_id && id2 != id)
							{
								duplicate = true;
								break;
							}
						}

						fin_check.close();

						if (duplicate)
						{
							cout << "This ID already exists!" << endl;
						}

					} while (duplicate || new_id < 1000 || new_id > 9999);

					id = new_id;

					cout << "Resident ID updated successfully." << endl;
					break;
				}
				case 2:
				{
					cout << "Enter new name: ";
					cin.ignore();
					cin.getline(name, 30);
					break;
				}

				case 3:
				{
					cout << "Enter new department: ";
					cin.ignore();
					cin.getline(dep, 30);
					break;
				}
				case 4:
				{
					int new_sem;
					do
					{
						cout << "Enter New semester (1-8): ";
						cin >> new_sem;
					} while (new_sem < 1 || new_sem > 8);

					sem = new_sem;
					break;
				}
				case 5:
				{
					valid = false;
					while (!valid)
					{
						int count = 0;
						cout << "Enter Your New 11 digits Mobile # :";
						cin >> mobile;
						while (mobile[count] != '\0')
						{
							count++;
						}
						if (count == 11)
						{
							valid = true;
							for (int i = 0; i < 11; i++)
							{
								if (mobile[i] < '0' || mobile[i]>'9')
								{
									valid = false;
									break;
								}
							}
						}
						if (!valid)
						{
							cout << "Error : Mobile No. Must Be of 11 Digits" << endl << endl;
							cout << "Note : Mobile No. Should Only Contain Digits" << endl << endl;
						}
					}
					break;
				}
				case 6:
				{
					do {
						cout << "Enter fee status (0 or 1): ";
						cin >> fee;
					} while (fee != 0 && fee != 1);
					break;
				}
				default:
					cout << "Invalid choice!" << endl;
				}
				while (true)
				{
					cout << endl;
					cout << "Do You Want To Update Another Field ? (Y / N) : ";
					cin >> new_choice;
					if (new_choice == 'Y' || new_choice == 'y' ||
						new_choice == 'N' || new_choice == 'n')
					{
						break;
					}
					cout << "Please Enter Y or N Only." << endl;
				}
			} while (new_choice == 'Y' || new_choice == 'y');
			temp << id << " " << name << " " << dep << " " << sem << " " << mobile << " " << room << " " << fee << endl;
		}
		else
		{
			temp << id << " " << name << " " << dep << " " << sem << " " << mobile << " " << room << " " << fee << endl;
		}
	}

	fin.close();
	temp.close();

	if (found)
	{
		ifstream temp_read("temp.txt");
		ofstream fout("residents.txt");

		while (temp_read >> id >> name >> dep >> sem >> mobile >> room >> fee)
		{
			fout << id << " "
				<< name << " "
				<< dep << " "
				<< sem << " "
				<< mobile << " "
				<< room << " "
				<< fee << endl;
		}

		temp_read.close();
		fout.close();

		cout << "Resident record updated successfully." << endl;
	}
	else
	{
		cout << "Resident ID not found." << endl;
	}
}
void change_room(int** room_num, int** type, int** capacity, int** occupancy, int* trgt)
{
	ifstream check("room.txt");
	if (!check)
	{
		cout << "Please initialize rooms first!" << endl;
		return;
	}
	ifstream fin("residents.txt");
	if (!fin)
	{
		cout << "Error opening residents.txt" << endl;
		return;
	}
	int id, sem, room, fee;
	char name[30], dep[30], mobile[20];

	int old_room = -1;
	bool res_found = false;

	for (int i = 0; i < t_floor; i++)
	{
		for (int j = 0; j < rooms_per_floor; j++)
		{
			cout << "Room: " << room_num[i][j] << " Capacity: " << capacity[i][j] << " Occupancy: " << occupancy[i][j] << endl;
		}
	}

	while (fin >> id >> name >> dep >> sem >> mobile >> room >> fee) {
		if (id == *trgt) {
			old_room = room;
			res_found = true;
			break;
		}
	}
	fin.close();

	if (!res_found) {
		cout << "Resident ID not found inside database!" << endl;
		return;
	}

	bool room_found;
	int req_room = 0;
	bool valid_room = false;

	while (!valid_room)
	{
		room_found = false;
		cout << "---Rooms List---" << endl;
		for (int i = 1; i <= t_floor; i++)
		{
			for (int j = 1; j <= rooms_per_floor; j++)
			{
				cout << i << "0" << j << endl;
			}
		}
		cout << "Enter New Requested Room # : " << endl;
		cin >> req_room;
		if (req_room == old_room)
		{
			cout << "Resident Is Already Allocated To This Room....!! " << endl;
			continue;
		}

		for (int i = 0; i < t_floor; i++)
		{
			for (int j = 0; j < rooms_per_floor; j++)
			{
				if (room_num[i][j] == req_room)
				{
					room_found = true;
					if (occupancy[i][j] < capacity[i][j])
					{
						occupancy[i][j]++;

						for (int r = 0; r < t_floor; r++) {
							for (int c = 0; c < rooms_per_floor; c++) {
								if (room_num[r][c] == old_room) {
									occupancy[r][c]--;
								}
							}
						}

						cout << "Success: Resident allocated to New room!" << endl;
						valid_room = true;
					}
					else {
						cout << "Room Is At Maximum Capacity!! :(" << endl;
					}
					break;
				}
			}
			if (room_found)
				break;
		}
		if (!room_found)
			cout << "Error : Room Does not Exist!!" << endl;
	}

	ifstream fiin("residents.txt");
	ofstream temp("temp.txt");
	while (fiin >> id >> name >> dep >> sem >> mobile >> room >> fee) {
		if (id == *trgt) {
			room = req_room;
		}
		temp << id << " " << name << " " << dep << " " << sem << " " << mobile << " " << room << " " << fee << endl;
	}
	fiin.close();
	temp.close();

	ifstream temp_read("temp.txt");
	ofstream fout("residents.txt");
	while (temp_read >> id >> name >> dep >> sem >> mobile >> room >> fee) {
		fout << id << " " << name << " " << dep << " " << sem << " " << mobile << " " << room << " " << fee << endl;
	}
	temp_read.close();
	fout.close();

	ofstream f_room("room.txt");
	for (int i = 0; i < t_floor; i++) {
		for (int j = 0; j < rooms_per_floor; j++) {
			f_room << room_num[i][j] << " " << type[i][j] << " " << capacity[i][j] << " " << occupancy[i][j] << endl;
		}
	}
	f_room.close();
}

void checkout_resident(int** room_num, int** type, int** capacity, int** occupancy, int* total_rsdnt, int* trgt) {
	ifstream check("room.txt");
	if (!check)
	{
		cout << "Please initialize rooms first!" << endl;
		return;
	}
	ifstream fin("residents.txt");
	if (!fin)
	{
		cout << "Error opening residents.txt" << endl;
		return;
	}
	ofstream temp("temp.txt");
	int id, sem, room, fee;
	char name[30], dep[30], mobile[20];
	bool found = false;
	int target_room = -1;

	while (fin >> id >> name >> dep >> sem >> mobile >> room >> fee) {
		if (id == *trgt) {
			found = true;
			target_room = room;
			continue;
		}
		temp << id << " " << name << " " << dep << " " << sem << " " << mobile << " " << room << " " << fee << endl;
	}
	fin.close();
	temp.close();

	if (found) {
		for (int i = 0; i < t_floor; i++) {
			for (int j = 0; j < rooms_per_floor; j++) {
				if (room_num[i][j] == target_room) {
					if (occupancy[i][j] > 0)
					{
						occupancy[i][j]--;
					}
				}
			}
		}
		(*total_rsdnt)--;

		ifstream temp_read("temp.txt");
		ofstream fout("residents.txt");
		while (temp_read >> id >> name >> dep >> sem >> mobile >> room >> fee) {
			fout << id << " " << name << " " << dep << " " << sem << " " << mobile << " " << room << " " << fee << endl;
		}
		temp_read.close();
		fout.close();

		ofstream f_room("room.txt");
		for (int i = 0; i < t_floor; i++) {
			for (int j = 0; j < rooms_per_floor; j++) {
				f_room << room_num[i][j] << " " << type[i][j] << " " << capacity[i][j] << " " << occupancy[i][j] << endl;
			}
		}
		f_room.close();
		cout << "Resident Checked out cleanly!" << endl;
	}
	else {
		cout << "ID not found!" << endl;
	}
}

void search_by_id(int* target) {
	ifstream fin("residents.txt");
	if (!fin)
	{
		cout << "Error opening residents.txt" << endl;
		return;
	}
	bool found = false;
	int id;
	char name[30]; char dep[30]; char num[20]; int sem; int fee; int room_num;

	while (fin >> id >> name >> dep >> sem >> num >> room_num >> fee) {
		if (*target == id) {
			found = true;
			break;
		}
	}
	if (found) {
		cout << "ID     Name    Department   Semester   Mobile #     Room No.   Fee Status " << endl;
		cout << id << "    " << name << "     " << dep << "         " << sem << "        " << num << "     " << room_num << "          " << fee << endl;
	}
	else {
		cout << "ID Not Found!!!" << endl;
	}
	fin.close();
}
void reports(int** room_num, int** type, int** capacity, int** occupancy) {
	ifstream check("room.txt");
	if (!check)
	{
		cout << "Please initialize rooms first!" << endl;
		return;
	}
	int total_rooms = t_floor * rooms_per_floor;
	int total_residents = 0;
	int total_capacity_seats = 0;
	int single_res = 0;
	int double_res = 0;
	int max_occupancy = -1;

	for (int i = 0; i < t_floor; i++) {
		for (int j = 0; j < rooms_per_floor; j++) {
			total_capacity_seats += capacity[i][j];
			total_residents += occupancy[i][j];
			if (type[i][j] == 1) {
				single_res += occupancy[i][j];
			}
			else if (type[i][j] == 2) {
				double_res += occupancy[i][j];
			}
			if (occupancy[i][j] > max_occupancy) {
				max_occupancy = occupancy[i][j];
			}
		}
	}
	int available_seats = total_capacity_seats - total_residents;
	ifstream fin("residents.txt");
	if (!fin)
	{
		cout << "Error opening residents.txt" << endl;
		return;
	}
	int paid_count = 0;
	int unpaid_count = 0;
	int r_id, r_sem, r_room, r_fee;
	char r_name[30], r_dep[30], r_mobile[20];

	if (fin) {
		while (fin >> r_id >> r_name >> r_dep >> r_sem >> r_mobile >> r_room >> r_fee) {
			if (r_fee == 1) {
				paid_count++;
			}
			else {
				unpaid_count++;
			}
		}
		fin.close();
	}
	cout << "====================================================" << endl;
	cout << "             SYSTEM SUMMARY PERFORMANCE REPORT      " << endl;
	cout << "====================================================" << endl;
	cout << " Total Created Rooms : " << total_rooms << endl;
	cout << " Total Active Residents : " << total_residents << endl;
	cout << " Total Available Seats : " << available_seats << endl;
	cout << "----------------------------------------------------" << endl;
	cout << " Residents in Single Rooms : " << single_res << endl;
	cout << " Residents in Double Rooms : " << double_res << endl;
	cout << "----------------------------------------------------" << endl;
	cout << " Paid Status Account Balances : " << paid_count << " Active" << endl;
	cout << " Unpaid Status Balance Issues : " << unpaid_count << " Pending" << endl;
	cout << "----------------------------------------------------" << endl;

	cout << " Most Occupied Room(s) (Maximum load: " << max_occupancy << "): " << endl;
	if (max_occupancy == 0) {
		cout << " None (All rooms are currently empty...)" << endl;
	}
	else {
		for (int i = 0; i < t_floor; i++) {
			for (int j = 0; j < rooms_per_floor; j++) {
				if (occupancy[i][j] == max_occupancy) {
					cout << " Room Code " << room_num[i][j] << " [" << occupancy[i][j] << "/" << capacity[i][j] << " Filled]" << endl;
				}
			}
		}
	}
	cout << "====================================================" << endl;
}