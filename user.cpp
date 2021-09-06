#include <string>
#include "user.h"

User::User() {
	id = username = email = password = "";
}

User::User(string id, string name, string email, string password) : id(id), username(name), email(email), password(password) {}

User::User(const User& other) {
	this->id = other.id;
	this->username = other.username;
	this->email = other.email;
	this->password = other.password;
}

string User::getId() const {
	return this->id;
}

void User::setId(string new_id) {
	this->id = new_id;
}

string User::getName() const {
	return this->username;
}

void User::setName(string new_name) {
	this->username = new_name;
}
string User::getEmail() const {
	return this->email;
}

void User::setEmail(string new_email) {
	this->email = new_email;
}

string User::getPassword() const {
	return this->password;
}

void User::setPassword(string new_password) {
	this->password = new_password;
}

User::~User() {}