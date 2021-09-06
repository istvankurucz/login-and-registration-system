#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "user.h"

using namespace std;

// Menu
void show_menu(const User &user);
void menu_line(bool top = false);

string generate_id();
string* slice_row(string row);
void update_user(const User& user);
bool email_checker(string email);
bool empty_file(ifstream& fin);

// Menu options
void login(User &user);
void add_user(User &user);
void edit(User &user);
void logout(User& user);

int main() {
	cout << "-----------------------------" << endl;
	cout << "LOGIN AND REGISTRATION SYSTEM" << endl;
	cout << "-----------------------------" << endl;
	srand(time(NULL));

	bool exit = false;
	User user;

	while (!exit)
	{
		show_menu(user);

		char option;
		cout << "Which option do you choose? "; cin >> option;

		switch (option)
		{
		case '1':
			login(user);
			break;

		case '2':
			add_user(user);
			break;

		case '3':
			if (user.getId() != "") edit(user);
			else cout << "The user is not logged in" << endl;
			break;

		case '4':
			logout(user);
			break;

		case '0':
			logout(user);
			cout << endl;
			cout << "------------ EXIT ------------" << endl;
			exit = true;
			break;

		default:
			cout << "Invalid input" << endl;
		}

	}
}

void show_menu(const User& user) {
	cout << endl << endl;
	menu_line(true);
	menu_line();
	if (user.getId() == "") {
		cout << "#          1: Login          #" << endl;
		menu_line();
	}
	cout << "#    2: Create an account    #" << endl;
	menu_line();
	if (user.getId() != "") {
		cout << "#     3: Edit information    #" << endl;
		menu_line();
		cout << "#         4: Logout          #" << endl;
		menu_line();
	}
	cout << "#          0: Exit           #" << endl;
	menu_line();
	menu_line(true);
	cout << endl << endl;
}

void menu_line(bool top) {
	for (int i = 0; i < 30; i++) {
		if (!top and i != 0 and i != 29) cout << " ";
		else cout << "#";
	}
	cout << endl;
}

string generate_id() {
	char chars[36] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	string id = "";

	for (int i = 0; i < 8; i++) {
		int index = rand() % 36;
		bool uppercase = rand() % 2 == 0;

		if (uppercase) id += toupper(chars[index]);
		else id += chars[index];
	}
	return id;
}

string* slice_row(string row) {
	int index = 0, from = 0, i = 0;
	string* user_data = new string[4];

	do {
		index = row.find(";", from);

		user_data[i] = row.substr(from, index - from);
		i++;

		from = index + 1;
	} while (index != -1);

	return user_data;
}

void update_user(const User& user) {
	string row;
	ofstream fout("temp.txt");
	ifstream fin("users.txt");

	while (!fin.eof()) {
		fin >> row;
		string* data = slice_row(row);
		if (data[0] == user.getId()) fout << user.getId() << ";" << user.getName() << ";" << user.getEmail() << ";" << user.getPassword() << endl;
		else fout << row << endl;
		delete[] data;
	}
	fin.close();
	fout.close();

	remove("users.txt");
	int result = rename("temp.txt", "users.txt");
	if (!result) cout << endl << "File successfully renamed!" << endl;
	else cout << endl << "Error renaming the file." << endl;
}

bool email_checker(string email) {
	int index_at = email.find("@");
	if (index_at == -1) return false;
	else {
		char allowed[39] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '.', '-', '_' };
		string prefix = email.substr(0, index_at);
		string domain = email.substr(index_at + 1, email.length() - index_at);
		
		// Check prefix
		if (prefix[0] == '.' or prefix[0] == '_' or prefix[0] == '-' or prefix[prefix.length() - 1] == '.' or prefix[prefix.length() - 1] == '_' or prefix[prefix.length() - 1] == '-') return false;
		bool correct;
		for (int i = 0; i < prefix.length(); i++) {
			correct = false;

			if (prefix[i] == '.' or prefix[i] == '_' or prefix[i] == '-') {
				if (prefix[i + 1] == '.' or prefix[i + 1] == '_' or prefix[i + 1] == '-') return false;
			}

			for (int j = 0; j < 39; j++) {
				if (prefix[i] == allowed[j]) {
					correct = true;
					break;
				}
			}
			if (!correct) return false;
		}

		// Check domain
		if (domain.find(".") == -1 or domain.substr(domain.find(".") + 1, domain.length() - domain.find(".")).length() < 2) return false;
		if (domain[0] == '.' or domain[0] == '-' or domain[domain.length() - 1] == '.' or domain[domain.length() - 1] == '-') return false;
		for (int i = 0; i < domain.length(); i++) {
			correct = false;

			if (domain[i] == '.' or domain[i] == '-') {
				if (domain[i + 1] == '.' or domain[i + 1] == '-') return false;
			}

			for (int j = 0; j < 38; j++) {
				if (domain[i] == allowed[j]) {
					correct = true;
					break;
				}
			}
			if (!correct) return false;
		}

		return true;
	}
}

bool empty_file(ifstream& fin) {
	return fin.peek() == ifstream::traits_type::eof();
}

void login(User& user) {
	ifstream fin("users.txt");
	if (user.getId() == "" and !empty_file(fin)) {
		string name, password, row;
		bool correct = false;

		cout << endl;
		cout << "----------- LOGIN -----------" << endl;
		cout << endl;
		cout << "Username: "; cin >> name;
		cout << "Password: "; cin >> password;

		while (!fin.eof() and !correct) {
			fin >> row;
			string* data = slice_row(row);
			if (data[1] == name) {
				correct = true;
				if (data[3] == password) {
					user.setId(data[0]);
					user.setName(data[1]);
					user.setEmail(data[2]);
					user.setPassword(data[3]);
				}
				else cout << "Wrong password!" << endl;
			}
			delete[] data;
		}
		if (!correct) cout << "There is no user with this username: " << name << endl;
	}
	else cout << "You've already logged in or the DB is empty." << endl;
	fin.close();
}

void add_user(User& user) {
	cout << endl;
	cout << "----- CREATE NEW ACCOUNT -----" << endl;
	cout << endl;

	// Input fields
	string id = generate_id(), name="", email="", password="";
	cout << "Please enter your username: "; cin >> name;
	cout << endl;
	cout << "Please enter your email address: "; cin >> email;
	cout << endl;
	if (email_checker(email)) {
		cout << "Please enter your password: "; cin >> password;

		// Login the user
		user.setId(id);
		user.setName(name);
		user.setEmail(email);
		user.setPassword(password);

		// Update the DB
		if (name != "" and email != "" and password != "") {
			ofstream fout("users.txt", ios_base::app);
			fout << generate_id() << ";" << name << ";" << email << ";" << password << endl;
			fout.close();

			cout << endl << "The user successfully addded." << endl;
		}
		else cout << endl << "Error adding the new user." << endl;
	}
	else cout << endl << "Invalid email address!" << endl;
}

void edit(User& user) {
	cout << endl;
	cout << "------------ EDIT ------------" << endl;
	cout << endl;

	string new_data;
	char option = 0;
	while (tolower(option) != 'q') {
		cout << endl;
		cout << "If you want to finish the editing press 'q'!" << endl;
		cout << "What do you want to edit? (n: username / e: email / p: password) "; cin >> option;

		switch (tolower(option)) {
			case 'n':
				cout << "Please enter your new username: "; cin >> new_data;
				user.setName(new_data);
				cout << "Username successfully updated to " << user.getName() << "." << endl;
				break;

			case 'e':
				cout << "Please enter your new email address: "; cin >> new_data;
				if (email_checker(new_data)) {
					user.setEmail(new_data);
					cout << "Email address successfully updated to " << user.getEmail() << "." << endl;
				}
				else cout << endl << "Invalid email address!" << endl;
				break;

			case 'p':
				cout << "Please enter your new password: "; cin >> new_data;
				user.setPassword(new_data);
				cout << "Password successfully changed to " << user.getPassword() << "." << endl;
				break;

			case 'q':
				break;

			default:
				cout << "Invalid input!" << endl;
		}
	}
	update_user(user);
}

void logout(User& user) {
	user.setId("");
	user.setName("");
	user.setEmail("");
	user.setPassword("");

	cout << endl;
	cout << "The user successfully logged out." << endl;
}