#pragma once

#include <QString>

enum userRole
{
	REGULAR,
	ADMIN
};

class User
{

private:
	QString		username;
	QString		password;
	userRole	role;

public:
	QString		getUsername() { return username; }
	void		setUsername( QString username ) { this->username = username; };

	QString		getPassword() { return password; }
	void		setPassword( QString password ) { this->password = password; }

	userRole	getRole() { return role; }
	void		setRole( userRole role ) { this->role = role; }

};