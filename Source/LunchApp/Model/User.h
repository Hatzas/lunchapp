#pragma once

#include <QString>


class User
{
public:
	enum EUserRole
	{
		eRegular,
		eAdmin
	};

				User( QString userName, EUserRole role );

	QString		getUsername() { return username; }
	void		setUsername( QString username ) { this->username = username; };

	QString		getPassword() { return password; }
	void		setPassword( QString password ) { this->password = password; }

	EUserRole	getRole() { return role; }
	void		setRole( EUserRole role ) { this->role = role; }

private:
	QString		username;
	QString		password;
	EUserRole	role;

};