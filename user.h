#ifndef __USER_H  
#define __USER_H

#include <iostream>
#include <string>

using namespace std;

class User {
private:
	string id, username, email, password;

public:
	User();
	User(string id, string name, string email, string password);
	User(const User& other);

	string getId() const;
	void setId(string new_id);

	string getName() const;
	void setName(string new_name);

	string getEmail() const;
	void setEmail(string new_email);

	string getPassword() const;
	void setPassword(string new_password);

	~User();
};

#endif