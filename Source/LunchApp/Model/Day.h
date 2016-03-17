#pragma once

#include "Course.h"

class Day
{

private:
	QList<Course>	courses;

public:
	QList<Course>	getCourses() { return courses; }

};
