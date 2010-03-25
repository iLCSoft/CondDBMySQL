/*
*  This file contains some basic operations with Date & Time,
* since JS accepts wrong dates and changes the to valid
* dates without giving any warning of what was wrong.
*  These functions come in hand when we need to warn the
* user that the date is in a wrong format.
*
* Make comments to tiago@cern.ch
* Author: Gama Franco
*/

/*returns true if date is valid, else false*/
function checkDate(year, month, day)
{
var leap = 0;

	/*check for integer values*/
	if (isNaN(parseInt(year, 10)) ||
			isNaN(parseInt(month, 10)) ||
			isNaN(parseInt(day,10))) {
		return false;
	}

   /* Validation of day*/
   if (day < 1) {
      return false;
   }

   /* Validation of month*/
   if ((month < 1) || (month > 12)) {
      return false;
   }

   /* Validation of year*/
   if (year < 1) {
      return false;
   }

   /* Validation leap-year / february / day */
   if ((year % 4 == 0) || (year % 100 == 0) || (year % 400 == 0)) {
      leap = 1;
   }
   if ((month == 2) && (leap == 1) && (day > 29)) {
      return false;
   }
   if ((month == 2) && (leap != 1) && (day > 28)) {
      return false;
   }

   /* Validation of other months */
   if ((day > 31) && ((month == "1") || (month == "3") || (month == "5") ||
   (month == "7") || (month == "8") || (month == "10") || (month == "12")))
   {
	return false;
   }
   if ((day > 30) && ((month == "1") || (month == "6") || (month == "9") ||
   (month == "11"))) {
	  return false;
   }

   return true;
}

/*returns true if the time is valid, else false*/
/*Note: 0 <= hours <= 23*/
function checkTime(hours, minutes, seconds)
{

	/*check for integer values*/
	if (isNaN(parseInt(hours, 10)) ||
			isNaN(parseInt(minutes, 10)) ||
			isNaN(parseInt(seconds,10))) {
		return false;
	}

    /*validate hours*/
	if ( hours < 0 && hours > 23 )
		 return false;

    /*validate minutes*/
	if ( minutes < 0 && minutes > 59 )
		 return false;

      /*validate seconds*/
	if ( seconds < 0 && seconds > 59 )
		 return false;

	return true;
}