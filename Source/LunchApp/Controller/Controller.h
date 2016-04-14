#pragma once

#include <QThread>
#include <QDate>

#include "../Model/Dish.h"
#include "../Model/Week.h"
#include "../Model/User.h"

class DataTransfer;

class Controller : public QThread
{
	Q_OBJECT

public:
								Controller( QObject *parent = 0 );
								~Controller();

	static	User*				getUser()							{ return user; }

			void				sendDummyWeek( QDate startDate, QDate endDate );

signals:
			void				weekArrived( const Week& week );
			void				allDishesArrived( Day containerDay );
			void				notify( const QString& msg, const QPixmap& pixmap );

public slots:
			void				requestWeekBefore( const Week& week );
			void				requestWeekAfter( const Week& week );
			void				requestWeek( QDate startDate, QDate endDate );
			void				requestAllDishes();

			void				selectionChangedOn( const Dish& dish );

			void				publishWeek( const Week& week );

			void				uploadPicture( QPixmap pixmap );

			void				onTimeCheck();

protected:
			void				run();

private slots:
			void				dataFinished( Week& week );

private:
	static	User*						user;

	static	std::map<QString, int>		weekDays;

			DataTransfer*				dataTransfer;

			QTimer*						timeChecker;
			QDate						lastShownMenuDate;

			Week						currentWeek;

			std::vector<Dish>	getAllDishes();

	static	bool				compareDays( Day first, Day second );

			void				notifyDayMenu();

			void				randomizeRatings( std::vector<Dish> &dishesVect );
};
